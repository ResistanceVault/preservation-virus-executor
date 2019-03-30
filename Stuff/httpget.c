#include <JEO:JEO.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/socket.h>
#include <proto/usergroup.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <exec/memory.h>

struct Library * SocketBase;
#define BUFFSIZE 256

#define	IOCPARM_MASK	0x1fff		/* parameter length, at most 13 bits */
#define	IOCPARM_LEN(x)	(((x) >> 16) & IOCPARM_MASK)
#define	IOCBASECMD(x)	((x) & ~IOCPARM_MASK)
#define	IOCGROUP(x)	(((x) >> 8) & 0xff)

#define	IOCPARM_MAX	NBPG		/* max size of ioctl, mult. of NBPG */
#define	IOC_VOID	0x20000000	/* no parameters */
#define	IOC_OUT		0x40000000	/* copy out parameters */
#define	IOC_IN		0x80000000	/* copy in parameters */
#define	IOC_INOUT	(IOC_IN|IOC_OUT)
#define	IOC_DIRMASK	0xe0000000	/* mask for IN/OUT/VOID */

#define _IOC(inout,group,num,len) \
	(inout | ((len & IOCPARM_MASK) << 16) | ((group) << 8) | (num))
#define	_IO(g,n)	_IOC(IOC_VOID,	(g), (n), 0)
#define	_IOR(g,n,t)	_IOC(IOC_OUT,	(g), (n), sizeof(t))

#define	FIONREAD	_IOR('f', 127, int)	/* get # bytes to read */

UWORD inflag;

char * strchr1(char * string, char c)
{
  while (*string)
    if (*string == c)
      return string;
    else
      string++;

  return NULL;
}

char * strstr1(char * string, char * s)
{
  while (*string)
    if (strchr(s, *string) != NULL)
      return string;
    else
      string++;
  
  return NULL;
}

static int printUrlFault(void)
{
  Printf("Url has to have format: [http://]host.domain.net[:port]/path\n");
  return 20;
}

static int	do_connect(char * name, int port);
static void	netperror(char * banner);

VOID FindVersion (char *Name, char *Buf, char *CurVersion)
{
	int i, j;
	char Hold[200];
	BOOL start = OFF, start1 = OFF;
	BOOL found = FALSE;

	for (i = 0, j = 0; Buf[i] != 0; i++)
	{
		if (start)
		{
			if (Buf[i] != ' ')
				start1 = ON;
		}
		if (start1)
		{
			Hold[j] = Buf[i];
			if (Buf[i] == '\n')
			{
				Hold[j] = 0;
				found = TRUE;
				break;
			}
			j++;
		}

		if (i < (200 - 20))
		{
			if (Buf[i] == 'V')
			if (Buf[i+1] == 'e')
			if (Buf[i+2] == 'r')
			if (Buf[i+3] == 's')
			if (Buf[i+4] == 'i')
			if (Buf[i+5] == 'o')
			if (Buf[i+6] == 'n')
			if (Buf[i+7] == ':')
			{
				i += 7;
				start = ON;
			}
		}
	}
	if (found)
	{
		if (strcmp (CurVersion, Hold) < 0)
			printf ("%s v%s is available...\n", Name, Hold);
		else
			printf ("No new version available\n");
	}
}

long instrip(unsigned char *buff, long length)
{
	register long i = 0, j = 0;
	unsigned char *tb = AllocMem(length+2, MEMF_PUBLIC);
	if(tb)
	{
		while(i < length)
		{
			if(buff[i]==255 && !inflag)
				inflag = TRUE;
			else {
				tb[j] = buff[i];
				j++;
				inflag = FALSE;
			}
			i++;
		}
		CopyMem(tb, buff, j);
		FreeMem(tb, length+2);
		return(j);
	}
	return(length);
}

int httpget (char *Name, char *url, char *CurVersion)
{
  char buf[1024]; /*stack usage of program should fit in 4000 bytes of memory*/
  char * p, * u;  /* ^^^ in current program 1k local buffer is sufficient */
  int i, port, sd, flags;
	struct msghdr *msg;
	fd_set rd;
	LONG oldsize;

  /*
   * A URL may start w/ http:// or without it in this application.
   */ 
  if (memcmp(url, "http://", 7) == 0)
    url+= 7;
  
  if ((p = strstr1(url, ":/")) == NULL)
    return printUrlFault();

  if (*p == ':') {
    /*
     * user want's to access custom port/service.
     */
    u = strchr1(p, '/');

    if (u == NULL)
      return printUrlFault();

    if (u == p) {
      Printf("No port/service defined\n");
      return 20;
    }
    /*
     * But now, I'm too lazy to write full `getservbyname' query here,
     * so only numeric port information will do
     */
    if ((port = atoi(p+1)) == 0) {
      Printf("Given port not numeric\n");
      return 20;
    }
  }
  else {
    u = p;
    port = 80;	/* standard http -service port number */
  }

  *p = '\0';

  if ((sd = do_connect(url, port)) < 0)
    return 20;

  *p = '/';
  /*
   * I could have done a hack to write "GET " at position u - 4 and done
   * a single send() call to peer. I don't know a hostname that fits in 3
   * bytes and get positive answer from name server. Anyway, that would
   * have been too `kludgy' way to solve the problem in this example.
   */

  i = strlen(u);
  u[i++] = '\n'; /* here it is safe to replace terminating NUL w/ '\n';
  
  /*
   * unless fails, send will send all bytes, since in NONBLOCKing mode
   */


	FD_ZERO (&rd);
	FD_SET(sd, &rd);

  if ((send(sd, "GET ", 4, 0) != 4) || (send(sd, u, i, 0) != i))
	{
    netperror("send");
    return 20;
  }

  while ((i = recv(sd, buf, sizeof buf, 0)) > 0)
    Write(Output(), buf, i);

	printf ("\n%s%s", url, u);

	FindVersion (Name, buf, CurVersion);
  
  return 0;
}

/*
 * Stuff to make connection. Reuse freely.
 */
int
getSockAddr(const char *	 host,
	    int			 port,
	    struct sockaddr_in * addr)
{
  struct hostent * remote;

  if ((remote = gethostbyname(host)) != NULL)
    memcpy(&addr->sin_addr, remote->h_addr, sizeof addr->sin_addr);
  else if ((addr->sin_addr.s_addr = inet_addr(host)) == (unsigned long)-1) {
    return FALSE;
  }

  addr->sin_port = htons(port);
  addr->sin_family = AF_INET;

  return TRUE;
}

const int ONE = 1;

int inetconn (const struct sockaddr_in * addr)
{
  int sd;

  if ((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    netperror ("socket");
    return -1;
  }
  
  setsockopt(sd, SOL_SOCKET, SO_KEEPALIVE, &ONE, sizeof ONE);
    
  if (connect(sd, (struct sockaddr *)addr, sizeof (struct sockaddr_in)) < 0) {
    CloseSocket(sd);
    netperror("connect");
    return -1;
  }

  return sd;
}


static int do_connect(char * host, int port)
{
  struct sockaddr_in addr = { 0 }; /* implicit bzero() here when compiled w/ gcc */
  
  if (getSockAddr(host, port, &addr) == FALSE) {
    Printf("Cannot resolve host address\n");
    return -1;
  }

  return inetconn(&addr);
}
  

/*
 * This `perror' works only for sockets. Therefore hidden here at the end
 * of this program. There is a general perror function available in network
 * link library without any additional speed/size overhead. This is used to
 * demonstrate the power of `SocketBaseTagList()' function.
 */

#include <amitcp/socketbasetags.h>

static void netperror(char * banner)
{
  ULONG taglist[5];
  
  taglist[0] = SBTM_GETVAL(SBTC_ERRNO);
  /* taglist[1] set when tag 0 executed */
  taglist[2] = SBTM_GETREF(SBTC_ERRNOSTRPTR);
  taglist[3] = (LONG)&taglist[1]; /* reads and writes tag[1] */
  taglist[4] = NULL;	/* TAG_END if <utility/tagitem> is included */

  SocketBaseTagList((struct TagItem *)taglist);

  if (banner)
    Printf("%s:", banner);

  Printf("%s\n", (char *)taglist[1]);
}

// ***************************************************************

main ()
{
	int rv = 20;	/* default exit value */

	if ((SocketBase = OpenLibrary ("bsdsocket.library", 4)) != NULL)
	{
		rv = httpget ("VirusExecutor", "home.online.no/~jae-ola/VE/VirusExecutor.readme", "2.05");

		CloseLibrary(SocketBase);
	}
	else
		printf ("Cannot open bsdsocket.library\n");

  return rv;
}

