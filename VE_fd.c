// -30	BeginIO
// -36	AbortIO

#include <libraries/reqtools.h>
#include "VE_prototypes.h"

char far Audio_dev_fd[6][MAX_LVO_LEN] = 
{
	"LibOpen",
	"LibClose",
	"LibExpunge",
	"LibExtFunc",
	"DevBeginIO",
	"DevAbortIO"
};

char far Console_dev_fd[12][MAX_LVO_LEN] = 
{
	"LibOpen",
	"LibClose",
	"LibExpunge",
	"LibExtFunc",
	"DevBeginIO",
	"DevAbortIO",
	"CDInputHandler",
	"RawKeyConvert",
	"consolePrivate1",
	"consolePrivate2",
	"consolePrivate3",
	"consolePrivate4"
};

char far Gameport_dev_fd[6][MAX_LVO_LEN] = 
{
	"LibOpen",
	"LibClose",
	"LibExpunge",
	"LibExtFunc",
	"DevBeginIO",
	"DevAbortIO"
};

char far Input_dev_fd[7][MAX_LVO_LEN] = 
{
	"LibOpen",
	"LibClose",
	"LibExpunge",
	"LibExtFunc",
	"DevBeginIO",
	"DevAbortIO",
	"PeekQualifier"
};

char far Keyboard_dev_fd[6][MAX_LVO_LEN] = 
{
	"LibOpen",
	"LibClose",
	"LibExpunge",
	"LibExtFunc",
	"DevBeginIO",
	"DevAbortIO"
};

char far Ramdrive_dev_fd[8][MAX_LVO_LEN] = 
{
	"LibOpen",
	"LibClose",
	"LibExpunge",
	"LibExtFunc",
	"DevBeginIO",
	"DevAbortIO",
	"KillRAD0",
	"KillRAD"
};

char far Scsi_dev_fd[6][MAX_LVO_LEN] = 
{
	"LibOpen",
	"LibClose",
	"LibExpunge",
	"LibExtFunc",
	"DevBeginIO",
	"DevAbortIO"
};

char far Timer_dev_fd[11][MAX_LVO_LEN] = 
{
	"LibOpen",
	"LibClose",
	"LibExpunge",
	"LibExtFunc",
	"DevBeginIO",
	"DevAbortIO",
	"AddTime",
	"SubTime",
	"CmpTime",
	"ReadEClock",
	"GetSysTime"
};

char far Trackdisk_dev_fd[6][MAX_LVO_LEN] = 
{
	"LibOpen",
	"LibClose",
	"LibExpunge",
	"LibExtFunc",
	"DevBeginIO",
	"DevAbortIO"
};
