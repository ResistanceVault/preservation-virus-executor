#include <JEO:JEO.h>

char C_mode[][2] = { "b", "w", "l" };

main ()
{
	char Hold[10];

	sprintf (Hold, "move.%s test\n", C_mode[0]);
	printf ("%s", Hold);
}
