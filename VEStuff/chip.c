#include <JEO:JEO.h>

typedef struct
{
	UBYTE id;
	char Name[11];
} ChipSet[] =
{
		"Std Agnus ",
		"ECS Agnus ",
		"AGA Alice ",
		"Std Denise",
		"ECS Denise",
		"AGA Lisa  ",
};


struct
{
  WORD err_no;
  BYTE *msg;
} os_errlist[] = 
{      
  103, "Insufficient free store!!!",
  105, "Task table full!!!",
  120, "Argument line invalid or too long!!!",
  121, "File is not an object module!!!",
  122, "Invalid resident library during load!!!",
  201, "No default directory!!!",
  202, "Object in use!!!",

