#include <JEO:JEO.h>
//#include "arexx.h"
#include<clib/alib_protos.h>
#include<clib/exec_protos.h>
#include<clib/rexxsyslib_protos.h>


int  createARexxPort (char* portname);
void freeARexxPort (void);
void replyARexxMsg(struct RexxMsg* msg, LONG rc, char* result);

ULONG getARexxSig (VOID);
struct RexxMsg* getARexxMsg (VOID);

static struct MsgPort* arexxport = NULL;

int createARexxPort (char *portname)
{
  int success = FALSE;
	char* error = NULL;

	Forbid();
	if(FindPort (portname) == NULL)
	{
		if(arexxport = CreatePort(portname, 1))
			success = TRUE;
		else
			error = "Error: cannot create ARexx port\n";
	}
	else
		error = "Error: cannot create ARexx port (the name already exists)\n";
	Permit();

  if(error)
    printf(error);

	return (success);
}

VOID freeARexxPort (VOID)
{
	if (arexxport)
	{
		struct RexxMsg* msg;

		RemPort(arexxport);
		while (msg = getARexxMsg ())
			replyARexxMsg (msg, 20, NULL);
		DeletePort (arexxport);
		arexxport = NULL;
	}
}

ULONG getARexxSig()
{
	return (ULONG)(arexxport ? 1L << arexxport->mp_SigBit : 0L);
}

struct RexxMsg* getARexxMsg ()
{
	return (struct RexxMsg*)GetMsg(arexxport);
}

void replyARexxMsg (struct RexxMsg* msg, LONG rc, char *result)
{
	msg->rm_Result1 = rc;
	msg->rm_Result2 = NULL;
	/* If the reply expects a result string, create one */
	if((msg->rm_Action & RXFF_RESULT) AND rc == 0 AND result != NULL)
		msg->rm_Result2 = (LONG)CreateArgstring(result, strlen(result));
	ReplyMsg((struct Message*)msg);
}


main ()
{
	if (createARexxPort ("VirusExecutor"))
	{
	}
}

VOID handleIDCMP (VOID)
{
	char* text = "Hello World!";
	int going = TRUE;
	int drawing = FALSE;
	ULONG drawsig, toolsig, arexxsig, gotsig;
	struct Window* drawwin = getDrawWin();

	arexxsig = getARexxSig();

	while(going)
	{
		struct IntuiMessage* intuimsg;
		toolsig = getToolSig();
		gotsig = Wait (toolsig | arexxsig);

		if (gotsig)
		{
			while(intuimsg = GT_GetIMsg(drawwin->UserPort))
			{
				ULONG class = intuimsg->Class;
				UWORD code = intuimsg->Code;
				WORD mousex = intuimsg->MouseX;
				/* Reply when finished copying bits from message */
				GT_ReplyIMsg(intuimsg);
				switch(class)
				{
				case IDCMP_MOUSEBUTTONS:
					switch(code)
					{
					case SELECTDOWN:
						drawing = TRUE;
						break;
					case SELECTUP:
						drawing = FALSE;
						break;
					}
					/* break; omitted so we draw on click, too */
				case IDCMP_MOUSEMOVE:
					if(drawing)
					{
						Move(drawwin->RPort, mousex, mousey);
						Text(drawwin->RPort, text, strlen(text));
					}
					break;
				case IDCMP_MENUPICK:
					going = doMenuPick(drawwin, code);
					drawwin = getDrawWin();
					drawsig = 1 << drawwin->UserPort->mp_SigBit;
					break;
				}
			}
		}
		if (going AND (gotsig & arexxsig))
		{
			struct RexxMsg* msg;
			while(going && (msg = getARexxMsg()))
				going = doARexx(msg, drawwin);
		}
	}
}
