char AHIName[] = "AHI Device";
char PlaySidLib[] = "playsid.library";

VOID CheckIntRAM (BYTE mode)
{
	    if (i == 7)	// Audio 0
 		  {
	  		p = (ULONG *)*exec;
				if ((*p == 0x302900b8) OR (*p == 0x302900c0) OR (*p ==0x302900bc))
				{
  				sprintf (Hold, "%s\n", AHIName);
  				strcat (Dummy, Hold);
  				valid = TRUE;
	  		}
	    }
	    if (i >= 8 OR i <= 10)	// Audio 1,2,3
 		  {
	  		p = (ULONG *)*exec;
				if (*p == 0x317c0780)
				{
  				sprintf (Hold, "%s\n", AHIName);
  				strcat (Dummy, Hold);
  				valid = TRUE;
	  		}
	    }

	    if (i >= 7 OR i <= 10)	// Audio 0,1,2,3
 		  {
	  		p = (ULONG *)*exec;
  			p += 12 / 4;
				if (*p == 0x64382200)
				{
  				sprintf (Hold, "%s\n", "AMOS.library");
  				strcat (Dummy, Hold);
  				valid = TRUE;
	  		}
	    }
	    if (i >= 7 OR i <= 9)	// Audio 0,1,2
 		  {
	  		p = (ULONG *)*exec;
  			p += 12 / 4;
				if (*p == 0x4bfa0024)
				{
  				sprintf (Hold, "%s\n", PlaySidLib);
  				strcat (Dummy, Hold);
  				valid = TRUE;
	  		}
	    }
	    if (i == 10)	// Audio 3
 		  {
	  		p = (ULONG *)*exec; 	
  			p += 16 / 4;
				if (*p == 0x009c4bfa)
				{
  				sprintf (Hold, "%s\n", PlaySidLib);
  				strcat (Dummy, Hold);
  				valid = TRUE;
	  		}
	    }

	    else if (i == 10)	// Audio Ch. 3
 		  {
   			ULONG *p;
	
	  		p = (ULONG *)*exec; 	
				if (*p == 0x01111111)
				{
  				sprintf (Hold, "%s\n", PatchNames[VE_AudDevice]);
  				strcat (Dummy, Hold);
  				valid = TRUE;
	  		}
	    }

