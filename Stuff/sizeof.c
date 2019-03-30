#include <exec/types.h>
#include <limits.h>
 
main ()
{
  printf ("\nchar           (BYTE)                           %d  %d  %d\n", sizeof (BYTE), SCHAR_MIN, SCHAR_MAX);
  printf ("unsigned char  (UBYTE, BYTEBITS, *STRPTR, TEXT) %d  %d\n", sizeof (UBYTE), UCHAR_MAX);
  printf ("short          (WORD, BOOL, RPTR)               %d  %d  %d\n", sizeof (WORD), SHRT_MIN, SHRT_MAX);
  printf ("unsigned short (UWORD, WORDBITS)                %d  %d\n", sizeof (UWORD),USHRT_MAX);  
  printf ("long           (LONG)                           %d  %d  %d\n", sizeof (LONG), LONG_MIN, LONG_MAX);  
  printf ("unsigned long  (ULONG, LONGBITS)                %d  4294967295\n\n", sizeof (ULONG));
  printf ("float          (FLOAT)                          %d\n", sizeof (FLOAT));
  printf ("double         (DOUBLE)                         %d\n\n", sizeof (DOUBLE));
}
