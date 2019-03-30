#include <JEO:JEO.h>
#include <exec/memory.h>
#include <proto/dos.h>
#include <libraries/reqtools.h>
#include "VE_Prototypes.h"
#include <dos/doshunks.h>

/*
BOOL CheckBobek (UWORD *Buf, ULONG size)
{
	BOOL flag = FALSE;

	if (size > 460)	// Length of virus...
	{
		ULONG i = 0, start;
		ULONG size_off;
		ULONG virus_start, virus_end;
		ULONG virus_code;
		UWORD start_code[2];
		ULONG code_size = 0;
		ULONG code_start = 0;
		UWORD hunks;
		ULONG *Buffer;
		ULONG special;

		Buffer = (ULONG *)Buf;
		if (Buffer[0] == 0x3f3)	// A exe file?
		{
			hunks = Buffer[2];

			if ((Buffer[hunks + 5] & 0x1FFFFFFF) == 0x3e9)	// Check only 29 bits...
				i = (hunks + 6) * 2;
		}		

		size_off = i;

		if (((size / 2) > i) AND (i > 0))	// Hmmmmm
		{
			code_size += Buf[i] * 0x10000;
			code_size += Buf[i+1];

			special = 0;
			if (code_size & HUNKF_ADVISORY)
				special = 0x20000000;
			else if (code_size & HUNKF_CHIP)
				special = 0x40000000;
			else if (code_size & HUNKF_FAST)
				special = 0x80000000;

  	  code_size -= special;
			code_size *= 4;

			i += 2;
			if (Buf[i] == 0x6100)	// Jump
			{
				code_start = i;
				i++;
				start = i + Buf[i] / 2;
				if (start < (size / 2) - 19)
				{
					if (Buf[start] == 0x2a57)
					{
						virus_code = start;

						if (Buf[++start] == 0x2B7A)
						if (Buf[++start] == 0xFEB4)
						if (Buf[++start] == 0xFFFC)
						if (Buf[++start] == 0x5997)
						if (Buf[++start] == 0x2C78)
						if (Buf[++start] == 0x0004)
						if (Buf[++start] == 0x4EAE)
						if (Buf[++start] == 0xFD84)
						if (Buf[++start] == 0x2F17)
						if (Buf[++start] == 0x487A)
						if (Buf[++start] == 0xFE48)
						if (Buf[++start] == 0x2F57)
						if (Buf[++start] == 0x0008)
						if (Buf[++start] == 0x588F)
						if (Buf[++start] == 0x4E75)
						if (Buf[++start] == 0x5B42)
						if (Buf[++start] == 0x4F42)
						if (Buf[++start] == 0x454B)
						if (Buf[++start] == 0x215D)
						{
							virus_end = start + 2;
							start -= 183;
							start_code[0] = Buf[start];
							start_code[1] = Buf[start+1];
							start -= 45;
							virus_start = start;

							if (Buf[start]   == 0x2f00)
							if (Buf[++start] == 0x2c78)
							if (Buf[++start] == 0x0004)
							if (Buf[++start] == 0x4aae)
							if (Buf[++start] == 0xfffc)
							if (Buf[++start] == 0x673e)
								flag = TRUE;
						}
					}
				}
			}
			if (flag)
			{
				ULONG j;

				code_size -= 460;
				code_size /= 4;
				if (special)
	    		code_size += special;

				Buf[10] = code_size >> 16;	// First Hunk
				Buf[11] = (UWORD)code_size;
				Buf[size_off] = code_size >> 16;
				Buf[size_off+1] = (UWORD)code_size;

				Buf[code_start] = start_code[0];
				Buf[code_start+1] = start_code[1];

//				printf ("UWORD virus_end %ld\n", virus_end);

				for (i = virus_end, j = virus_start; i < size / 2; i++, j++)
					Buf[j] = Buf[i];	// Kopiere fremover
			}
		}
	}
	return (flag);
}

BOOL CheckBastardInstaller2 (UWORD *Buf, ULONG size)
{
	ULONG crc;

	if (size == 38260) // Hmmms?
	{
		crc = Calculate_CRC ((UBYTE *)Buf, size);
		if (crc == 0x1d2204ae)
			return (TRUE);
	}
	return (FALSE);
}

BOOL CheckBastardInstaller1 (UWORD *Buf, ULONG size)
{
	ULONG crc;

	if (size == 4748) // Hmmms?
	{
		crc = Calculate_CRC ((UBYTE *)Buf, size);
		if (crc == 0xd3b06e4a)
			return (TRUE);
	}
	return (FALSE);
}

BOOL CheckBastard (UWORD *Buf, ULONG size)
{
	ULONG i, start, j;
	BOOL flag = FALSE;
	ULONG size_off;
	ULONG virus_start;
	ULONG virus_code;
	ULONG filesize;
	UBYTE check = 0;

	filesize = size;

	if (size > 800)
	{
		ULONG code_size = 0;

		for (i = 0; i < 400; i++)
		{
			if (Buf[i] == 0x0000 AND Buf[i+1] == 0x03e9)	// Code hunk
			{
				i += 2;
				size_off = i;
				code_size += Buf[i] * 0x10000;
				code_size += Buf[i+1];
				i += 2;
				if (Buf[i] == 0x6100)	// Jump
				{
					virus_start = i;
					i++;
					start = i + Buf[i] / 2;
					if (start < (size  / 2) - 100)
					{
						if (Buf[start] == 0x48e7 AND Buf[start+1] == 0xfffe)
						{
							virus_code = start;
							check = 0;
							for (j = start + 2; j < start + 95; j++)
							{
								if (flag)
									break;

								switch (Buf[j])
								{
									case 0x41fa:	// LEA 1
									{
										if (check == 0)
											check++;
										else
											check = 0;
										break;
									}
									case 0x41e8:	// LEA 2
									{
										if (check == 1)
											check++;
										else
											check = 0;
										break;
									}
									case 0x2c78:
									{
										if (Buf[j+1] == 0x0004)			// MOVE.L 4,a6
										{
											if (check == 2)
												check++;
											else
												check = 0;
										}
										break;
									}
									case 0x4eae:
									{
										if (Buf[j+1] == 0xfd84)			// JSR
										{
											if (check == 3)
												flag = TRUE;
										}
										break;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	if (flag)
	{
		BPTR fh;
		UWORD nop = 0x4e71;

		if (fh = Open (FileName, MODE_OLDFILE))
		{
			Seek (fh, virus_start * 2, OFFSET_BEGINNING);
			Write (fh, &nop, 2);
			Write (fh, &nop, 2);
			Close (fh);
		}
	}
	return (flag);
}

BOOL CheckTeamMOS (UWORD *Buffer, LONG size)
{
	BOOL found = FALSE;
	UWORD data, data1, val;
	int i;

//	printf ("%ld\n", size = RemoveHunk (Buffer, size));
//	printf ("%ld\n", size = RemoveHunk (Buffer, size));

	if (size > 988)
	{
		for (i = 0; i < 400; i++)
		{
			if (Buffer[i] == 0x41fa)
			{
				data = i + (Buffer[i + 1] / 2) + 1;
				i++;
				if (Buffer[++i] == 0x43fa)
				{
					data1 = Buffer[++i];
					if (Buffer[++i] == 0x2c78)
					{
						if (Buffer[++i] == 0x0004)
						{
							if (Buffer[++i] == 0x303c)
							{
								val = Buffer[++i];
								if (Buffer[++i] == 0xb158)	// EOR!								
								{
									found = TRUE;
									break;
								}
							}
						}
					}
				}
			}
		}
	}

	if (found)
	{
		UWORD new;
		char Hold[30] = "";


		printf ("data = %04lx\n", data);
		printf ("data1 = %04x\n", data1);
		printf ("val = %04lx\n", val);
		printf ("JAAAAA\n\n");

		printf ("Data = %04lx\n", Buffer[data]);
		printf ("check = %04lx\n", Buffer[data1]);

		for (i = data + 21; i < data + 30; i++)
		{
			new = Eor_w (val, Buffer[i]);
			SPrintF (Dummy, "%lc%lc", new>>8, (UBYTE)new);
			strcat (Hold, Dummy);
		}
		if (strcmp (Hold, " illegally ripped "))
			found = FALSE;
	}

	return (found);
}
*/

/*
char AMOSJoshuaVirus[] = "AMOS Joshua Trojan";
char AMOSJoshuaCloneVirus[] = "AMOS Joshua Clone Trojan";
char MUIVirus[] = "MUI";

UWORD ZakapiorData[20] =
{
	0x610C,
	0x646F,
	0x732E,
	0x6C69,
	0x6272,
	0x6172,
	0x7900,
	0x225F,
	0x7025,
	0x4EAE,
	0xFDD8,
	0x2C00,
	0x6744,
	0x41FA,
	0x01DC,
	0x2086,
	0x283C,
	0x0000,
	0x0196,
	0x7200
};

BOOL CheckZakapior (UWORD *Buffer, LONG size)
{
	int i;
	UWORD val;

	if (size > 500)	// Zakapior
	{
		for (i = 0; i < (size - 500) / 2; i++)
		{
			if (i > 10000)	// Skal vi gidde mere?
				break;
			if (Buffer[i] == 0x48e7)
			if (Buffer[i+1] == 0xfffe)
			if (Buffer[i+2] == 0x2c78)
			if (Buffer[i+3] == 0x0004)
			if (Buffer[i+8] == 0x303c)
			{
				val = Buffer[i+9];
				if (Buffer[i+10] == 0xb158)
				if (Buffer[i+11] == 0xb3c8)
				if (Buffer[i+12] == 0x66fa)
				if (Buffer[i+13] == 0x4eae)
				if (Buffer[i+14] == 0xfd84)
				{
					int j, k;
					BOOL z_flag = FALSE;

					for (j = i+15, k = 0; j < i+15+20; j++, k++)
					{
						if (Buffer[j] == (val ^ ZakapiorData[k]))
							z_flag = TRUE;
						else
							break;
					}
					if (z_flag)
						return (TRUE);
				}
			}
		}
	}
	return (FALSE);
}

UBYTE CheckLoadWB (VOID)	// Joshua Trojan + clone
{
	int i;
	char FileName[] = "C:LoadWB";
	BPTR fh;
	UBYTE ret = FALSE;

	if (fh = Open (FileName, MODE_OLDFILE))
	{
		UBYTE *Buffer;
		int size;
		ULONG checksum = 0;

		size = FileSize (FileName);
		if (size == 84772)	// Pakked!
		{
			if (Buffer = AllocMem (size, MEMF_CLEAR|MEMF_PUBLIC))
			{
				Read (fh, Buffer, size);
				for (i = 0; i < size; i++) 
					checksum += Buffer[i];
				FreeMem (Buffer, size);
				if (checksum == 0x008F6D1C)
					ret = 1;
			}
		}
		else if (size == 122836)	// Clone, not packed
		{
			if (Buffer = AllocMem (size, MEMF_CLEAR|MEMF_PUBLIC))
			{
				Read (fh, Buffer, size);
				for (i = 0; i < size; i++) 
					checksum += Buffer[i];
				FreeMem (Buffer, size);
				if (checksum == 0x4B31CC55)
					ret = 2;
			}
		}
		Close (fh);
	}	
	return (ret);
}

BOOL CheckJoshua (UWORD *Buffer, LONG size)
{
	int i;
	BOOL ret = FALSE;

	if (size == 144320)	// msxR.exe file
	{
		ULONG checksum = 0;

		for (i = 0; i < size / 2; i++)
			checksum += Buffer[i];

		if (checksum == 0x6D03CCCE)
			ret = TRUE;
	}
	else if (size == 60612)	// Workbench file
	{
		ULONG checksum = 0;

		for (i = 0; i < size / 2; i++)
			checksum += Buffer[i];

		if (checksum == 0x25750870)
			ret = TRUE;
	}
	else if (size == 102788)	// LoadWB file
	{
		ULONG checksum = 0;

		for (i = 0; i < size / 2; i++)
			checksum += Buffer[i];

		if (checksum == 0x4676140E)
			ret = TRUE;
	}

	return (ret);
}

BOOL CheckJoshuaClone (UWORD *Buffer, LONG size)
{
	int i;
	BOOL ret = FALSE;

	if (size == 182452)	// SonicIIAmiga.exe file
	{
		ULONG checksum = 0;

		for (i = 0; i < size / 2; i++)
			checksum += Buffer[i];

		if (checksum == 0x700A0AD8)
			ret = TRUE;
	}
	else if (size == 60728)	// miami.config file
	{
		ULONG checksum = 0;

		for (i = 0; i < size / 2; i++)
			checksum += Buffer[i];

		if (checksum == 0x254DC858)
			ret = TRUE;
	}
	else if (size == 122836)	// LoadWB file
	{
		ULONG checksum = 0;

		for (i = 0; i < size / 2; i++)
			checksum += Buffer[i];

		if (checksum == 0x4B31CC55)
			ret = TRUE;
	}

	return (ret);
}

BOOL CheckMUI (UWORD *Buffer, LONG size)
{
	int i;

	if (size > 100)	// MUI
	{
		for (i = 0; i < (size - 250) / 2; i++)
		{
			if (i > 10000)	// Skal vi gidde mere?
				break;
			if (Buffer[i]   == 0x4E55)
			if (Buffer[i+1] == 0)
			if (Buffer[i+2] == 0x4FEF)
			if (Buffer[i+3] == 0xFFF8)
			if (Buffer[i+4] == 0x41FA)
			if (Buffer[i+5] == 0x27C)
			if (Buffer[i+6] == 0x2008)
			if (Buffer[i+7] == 0x2F40)
			if (Buffer[i+8] == 4)
			if (Buffer[i+9] == 0x7000)
			if (Buffer[i+10] == 0x2F40)
			if (Buffer[i+11] == 0)
			if (Buffer[i+12] == 0x2C6C)
			if (Buffer[i+13] == 0xFFD8)
			if (Buffer[i+14] == 0x4CDF)
			if (Buffer[i+15] == 0x201)
			if (Buffer[i+16] == 0x4EAE)
			if (Buffer[i+17] == 0xFDD8)
			if (Buffer[i+18] == 0x2940)
			if (Buffer[i+19] == 0xFDFC)
			if (Buffer[i+20] == 0x4A80)
			if (Buffer[i+21] == 0x6700)
			if (Buffer[i+22] == 0x196)
			if (Buffer[i+23] == 0x4FEF)
			if (Buffer[i+24] == 0xFFFC)
			if (Buffer[i+25] == 0x41FA)
			if (Buffer[i+26] == 0x23E)
			if (Buffer[i+27] == 0x2008)
			if (Buffer[i+28] == 0x2F40)
				return (TRUE);
		}
	}
	return (FALSE);
}

BOOL CheckGBVirus (UBYTE *Buffer, LONG size)
{
	int i;

	if (size > 100)	// MUI
	{
		for (i = 0; i < (size - 250); i++)
		{
			if (Buffer[i]    == 0x70)
			if (Buffer[i+1]  == 0x00)
			if (Buffer[i+2]  == 0x22)
			if (Buffer[i+3]  == 0x70)
			if (Buffer[i+4]  == 0x00)
			if (Buffer[i+5]  == 0x00)
			if (Buffer[i+6]  == 0x43)
			if (Buffer[i+7]  == 0xF1)
			if (Buffer[i+8]  == 0x38)
			if (Buffer[i+9]  == 0x00)
			if (Buffer[i+10] == 0x7A)
			if (Buffer[i+11] == 0x00)
			if (Buffer[i+12] == 0x1A)
			if (Buffer[i+13] == 0x23)
			if (Buffer[i+14] == 0xE2)
			if (Buffer[i+15] == 0x9D)
			if (Buffer[i+16] == 0x16)
			if (Buffer[i+17] == 0x85)
			if (Buffer[i+18] == 0x1A)
			if (Buffer[i+19] == 0x23)
			if (Buffer[i+20] == 0xE2)
			if (Buffer[i+21] == 0x9D)
			if (Buffer[i+22] == 0x16)
			if (Buffer[i+23] == 0x85)
			if (Buffer[i+24] == 0x1A)
			if (Buffer[i+25] == 0x23)
			if (Buffer[i+26] == 0xE2)
			if (Buffer[i+27] == 0x9D)
			if (Buffer[i+28] == 0x16)
			if (Buffer[i+29] == 0x85)
			if (Buffer[i+30] == 0x1A)
			if (Buffer[i+31] == 0x23)
			if (Buffer[i+32] == 0xE2)
			if (Buffer[i+33] == 0x9D)
			if (Buffer[i+34] == 0x16)
			if (Buffer[i+35] == 0x85)
			if (Buffer[i+36] == 0x1A)
			if (Buffer[i+37] == 0x23)
				return (TRUE);
		}
	}
	return (FALSE);
}

ULONG far off[2000];
UWORD poly_count, poly_max;

VOID CheckOff (UWORD nr)
{
	UWORD diff;

	nr--;
	if (nr)	// større enn 0?
	{
		diff = off[nr] - off[nr-1];
		if (diff <= 10)
		{
			poly_count++;
			if (poly_count > poly_max)
				poly_max = poly_count;
		}
		else
			poly_count = 0;
	}
	else
	{
		poly_count++;
		poly_max = 1;
	}
}

VOID ScanPoly (UWORD *Buffer, LONG size)
{
	ULONG i;
	UBYTE x;
	UWORD cmd;
	UWORD asr, asl, lsr, lsl, roxr, roxl;
	UWORD tot;
	char *Poly;
	char Hold[30];
	BOOL go_flag;
	UWORD nr = 0;

	asr = asl = lsr = lsl = roxr = roxl = 0;
	poly_count = 0;

	Poly = malloc (5000);
	for (i = 0; i < size / 2; i++)
	{
		go_flag = TRUE;

		if (go_flag)
		{
			for (cmd = 0xE040; cmd <= 0xEE40; cmd += 0x0200)		// asr.w #1-8,d0-7
			{
				for (x = 0; x <= 7; x++)
				{
					if (Buffer[i] == cmd + x)
					{
						asr++;
						off[nr++] = i;
						go_flag = FALSE;
						CheckOff (nr);
						break;
					}
				}
				if (!go_flag)
					break;
			}
		}

		if (go_flag)
		{
			for (cmd = 0xE140; cmd <= 0xEF40; cmd += 0x0200)		// asl.w #1-8,d0-7
			{
				for (x = 0; x <= 7; x++)
				{
					if (Buffer[i] == cmd + x)
					{
						asl++;
						off[nr++] = i;
						go_flag = FALSE;
						CheckOff (nr);
						break;
					}
				}
				if (!go_flag)
					break;
			}
		}
		if (go_flag)
		{
			for (cmd = 0xE048; cmd <= 0xEE48; cmd += 0x0200)		// lsr.w #1-8,d0-7
			{
				for (x = 0; x <= 7; x++)
				{
					if (Buffer[i] == cmd + x)
					{
						lsr++;
						off[nr++] = i;
						CheckOff (nr);
						go_flag = FALSE;
						break;
					}
				}
				if (!go_flag)
					break;
			}
		}
		if (go_flag)
		{
			for (cmd = 0xE148; cmd <= 0xEF48; cmd += 0x0200)		// lsl.w #1-8,d0-7
			{
				for (x = 0; x <= 7; x++)
				{
					if (Buffer[i] == cmd + x)
					{
						lsl++;
						off[nr++] = i;
						CheckOff (nr);
						go_flag = FALSE;
						break;
					}
				}
				if (!go_flag)
					break;
			}
		}
		if (go_flag)
		{
			for (cmd = 0xE050; cmd <= 0xEE50; cmd += 0x0200)		// roxr.w #1-8,d0-7
			{
				for (x = 0; x <= 7; x++)
				{
					if (Buffer[i] == cmd + x)
					{
						roxr++;
						off[nr++] = i;
						CheckOff (nr);
						go_flag = FALSE;
						break;
					}
				}
				if (!go_flag)
					break;
			}
		}
		if (go_flag)
		{
			for (cmd = 0xE150; cmd <= 0xEF50; cmd += 0x0200)		// roxl.w #1-8,d0-7
			{
				for (x = 0; x <= 7; x++)
				{
					if (Buffer[i] == cmd + x)
					{
						roxl++;
						off[nr++] = i;
						CheckOff (nr);
						go_flag = FALSE;
						break;
					}
				}
				if (!go_flag)
					break;
			}
		}
	}

	mystrcpy (Poly, "Found: \n\n");

	if (asr AND asl AND lsr AND lsl AND roxr AND roxl)
	{
		tot = asr + asl + lsr + lsl + roxr + roxl;

		if (asr)
		{
			SPrintF (Hold, "ASR.W  %5ld\n", asr);
			strcat (Poly, Hold);
		}
		if (asl)
		{
			SPrintF (Hold, "ASL.W  %5ld\n", asl);
			strcat (Poly, Hold);
		}
		if (lsr)
		{
			SPrintF (Hold, "LSR.W  %5ld\n", lsr);
			strcat (Poly, Hold);
		}
		if (lsl)
		{
			SPrintF (Hold, "LSL.W  %5ld\n", lsl);
			strcat (Poly, Hold);
		}
		if (roxr)
		{
			SPrintF (Hold, "ROXR.W %5ld\n", roxr);
			strcat (Poly, Hold);
		}
		if (roxl)
		{
			SPrintF (Hold, "ROXL.W %5ld\n", roxl);
			strcat (Poly, Hold);
		}
		SPrintF (Hold, "\nTOTAL  %5ld\n\n", tot);
		SPrintF (Hold, "\nMAX    %5ld\n\n", poly_max);
		strcat (Poly, Hold);

//		for (i = 0; i < tot; i++)
//			printf ("off: %6ld\n", off[i]);
		JEOSoundRequest (GLS (&L_MESSAGE), Poly, GLS (&L_OK), S_ERROR);
	}

	free (Poly);
}
*/
//int FUCK = 0x231ad5;
