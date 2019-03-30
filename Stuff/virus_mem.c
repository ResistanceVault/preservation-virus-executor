/*
	if (1)	// Bobek
	{
		ULONG *p;
		char *p_tmp;
		struct Library *lib;
		ULONG val;
		UWORD jmp;

		lib = (struct Library *)ReqToolsBase->DOSBase;
		p = (ULONG *)GetFunction (lib, -108);
		p_tmp = (char *)p;
		p_tmp += 18;
		CopyMem ((APTR)p_tmp, &jmp, 2);
		if (jmp == 0x4eb9)
		{
			sprintf (Dummy, "'Bobek link virus' found in memory!");
			sound = S_VIRUS;
		  if (JEOSoundRequest (GLS (&L_MEMORY_CHECK), Dummy, "Remove|Leave", sound))
		  {
				p_tmp += 2;
				CopyMem ((APTR)p_tmp, &val, 4);
				SetFunction (lib, -108, (FuncPtr)val);
			}
		}
	}
*/
/*
	if (1)
	{
		struct Process *pr;

		pr = (struct Process *)FindTask ("zakahackandpatch");
		if (pr)
		{
			sprintf (Dummy, "'Zakahackandpatch TCP Trojan' found in memory!");
			sound = S_VIRUS;
		  if (JEOSoundRequest (GLS (&L_MEMORY_CHECK), Dummy, "Remove|Leave", sound))
		  {
				RemTask ((struct Task *)pr);
				pr = 0;
			}
		}
	}
*/
