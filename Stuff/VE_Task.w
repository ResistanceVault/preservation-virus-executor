
struct NewWindow TaskWindow = {
	10,180,	/* window XY origin relative to TopLeft of screen */
	70,10,	/* window width and height */
	0,1,	/* detail and block pens */
	INTUITICKS,	/* IDCMP flags */
	BORDERLESS+RMBTRAP+NOCAREREFRESH+ACTIVATE,	/* other window flags */
	NULL,	/* first gadget in gadget list */
	NULL,	/* custom CHECKMARK imagery */
	NULL,	/* window title */
	NULL,	/* custom screen pointer */
	NULL,	/* custom bitmap */
	5,5,	/* minimum width and height */
	0,0,	/* maximum width and height */
	CUSTOMSCREEN	/* destination screen type */
};


/* end of PowerWindows source generation */
