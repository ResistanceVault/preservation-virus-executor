#include <JEO:JEO.h>
#include <proto/dos.h>
#include <dos/dostags.h>
#include <rexx/rxslib.h>
#include <rexx/errors.h>
#include <exec/memory.h>
#include <exec/execbase.h>

#define SIG_KILL	SIGBREAKF_CTRL_C
#define SIG_HANDSHAKE	SIGF_SINGLE

#define PORTMASK(P)      (1L << (((struct MsgPort *)(P))->mp_SigBit))

struct MsgPort *RexxPort;
char RexxPortName[] = "VIRUSEXECUTOR";
struct Process		*RexxProcess;
struct RxsLib		*RexxSysBase;

extern struct ExecBase *SysBase;

VOID ReplyRexxCommand (struct RexxMsg *RexxMessage, LONG Primary, LONG Secondary)
{
	if(RexxMessage)
	{
		RexxMessage->rm_Result1 = Primary;
		RexxMessage->rm_Result2 = Secondary;

		ReplyMsg ((struct Message *)RexxMessage);
	}
}

VOID RexxServer (VOID)
{
	struct Process *ThisProcess = (struct Process *)SysBase->ThisTask;

	if(RexxPort = CreateMsgPort())
	{
		struct RexxMsg	*RexxMsg;
		ULONG		 SignalSet;
		BYTE		 Done = FALSE;

		RexxPort->mp_Node.ln_Name	= RexxPortName;
		RexxPort->mp_Node.ln_Pri	= 1;

		AddPort(RexxPort);

		Signal (ThisProcess, SIG_HANDSHAKE);

		do
		{
			SignalSet = Wait (SIG_KILL | PORTMASK(RexxPort));

			if(SignalSet & SIG_KILL)
				Done = TRUE;

			if(SignalSet & PORTMASK(RexxPort))
			{
				while(RexxMsg = (struct RexxMsg *)GetMsg(RexxPort))
				{
					if(RexxMsg -> rm_Node . mn_Node . ln_Type == NT_REPLYMSG)
						FreeMem (RexxMsg, sizeof (struct RexxMsg));
					else
					{
/*
						if(!ParseRexxCommand(RexxMsg))
						{
							UBYTE Buffer[10];

							sprintf(Buffer,"%ld",LastRexxError = TERMERROR_UNKNOWN_COMMAND);
							SetRexxVar (RexxMsg,"TERM.LASTERROR", Buffer, strlen(Buffer));
							ReplyRexxCommand (RexxMsg, RC_ERROR, TERMERROR_UNKNOWN_COMMAND);
						}
*/
					}
				}
			}
		}
		while(!Done);

		Done = FALSE;

		do
		{
			while(RexxMsg = (struct RexxMsg *)GetMsg(RexxPort))
			{
				if(RexxMsg -> rm_Node.mn_Node.ln_Type == NT_REPLYMSG)
					FreeMem (RexxMsg, sizeof (struct RexxMsg));
				else
					ReplyRexxCommand(RexxMsg,-1,0);
			}

			WaitPort(RexxPort);
		}
			while(!Done);

		DeleteMsgPort(RexxPort);
		RexxPort = NULL;
	}

	Forbid();
	RexxProcess = NULL;
	Signal (ThisProcess, SIG_HANDSHAKE);
}

main ()
{
	if(RexxSysBase = (struct RxsLib *)OpenLibrary(RXSNAME,0))
	{
		Forbid();
		if (RexxProcess = (struct Process *)CreateNewProcTags(
			NP_Entry,	RexxServer,
			NP_Name,	"term Rexx Process",
			NP_Priority,	5,
			NP_StackSize,	8192,
			NP_WindowPtr,	-1,
		TAG_END))
		{
			ClrSignal (SIG_HANDSHAKE);

			Wait (SIG_HANDSHAKE);
		}

		Permit();

		if(!RexxProcess)
			return(LocaleString(MSG_TERMINIT_UNABLE_TO_CREATE_AREXX_PROCESS_TXT));
	}

	OpenRexx ();


	if(RexxSysBase)
	{
		CloseLibrary(RexxSysBase);

		RexxSysBase = NULL;
	}

}
