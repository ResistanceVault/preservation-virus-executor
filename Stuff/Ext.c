#include <JEO:JEO.h>

char Dummy[1000];

char ExtNames[][5] =
{
	"iff", "jpg", "jpeg", "gif", "mpg", "mpeg", "htm", "html"
};

#define MAXEXT (sizeof (ExtNames) / 5)

BOOL CheckExt (char *FileName)
{
	int i, len;
	BOOL ext = FALSE;

	len = strlen (FileName);

	printf ("%ld\n", sizeof (ExtNames));
	if (len >= 3)
	{
		for (i = len - 2; i > 0; i--)
		{
			if (FileName[i] == '.')
			{
				ext = TRUE;
				break;
			}
		}
		if (ext)
		{
			i++;
			len = len - i;
			strncpy (Dummy, &FileName[i], len);
			Dummy[len] = 0;
			for (i = 0; i < MAXEXT; i++)
			{
				if (!(stricmp (ExtNames[i], Dummy)))
					return (TRUE);
			}
		}
	}
	return (FALSE);
}

main (int argc, char **argv)
{
	char Name[100];

	if (argc == 2)
	{
		strcpy (Name, argv[1]);
		if (CheckExt (Name))
			printf ("\nExtension found...\n\n");
		else
			printf ("\nExtension NOT found...\n\n");
	}
}
