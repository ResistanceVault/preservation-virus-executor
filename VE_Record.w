struct PropInfo Slide = {
	AUTOKNOB+FREEVERT,	/* PropInfo flags */
	0,0,	/* horizontal and vertical pot values */
	0,0xFFFF,	/* horizontal and vertical body values */
};

struct Image PropImage = {
	0,3,	/* XY origin relative to container TopLeft */
	22,4,	/* Image width and height in pixels */
	0,	/* number of bitplanes in Image */
	NULL,	/* pointer to ImageData */
	0x0000,0x0000,	/* PlanePick and PlaneOnOff */
	NULL	/* next Image structure */
};

struct Gadget PropGadget = {
	NULL,	/* next gadget */
	393,49,	/* origin XY of hit box relative to window TopLeft */
	30,200,	/* hit box width and height */
	NULL,	/* gadget flags */
	GADGIMMEDIATE+RELVERIFY+FOLLOWMOUSE,	/* activation flags */
	PROPGADGET,	/* gadget type flags */
	(APTR)&PropImage,	/* gadget border or image to be rendered */
	NULL,	/* alternate imagery for selection */
	NULL,	/* first IntuiText structure */
	NULL,	/* gadget mutual-exclude long word */
	(APTR)&Slide,	/* SpecialInfo structure */
	NULL,	/* user-definable data */
	NULL	/* pointer to user-definable data */
};

struct NewWindow NewWindowStructure2 = {
	0,0,	/* window XY origin relative to TopLeft of screen */
	640,256,	/* window width and height */
	0,1,	/* detail and block pens */
	INTUITICKS+MOUSEBUTTONS+MOUSEMOVE+GADGETUP+GADGETDOWN+CLOSEWINDOW+RAWKEY,	/* IDCMP flags */
	WINDOWCLOSE+REPORTMOUSE+ACTIVATE+RMBTRAP+NOCAREREFRESH,	/* other window flags */
	&PropGadget,	/* first gadget in gadget list */
	NULL,	/* custom CHECKMARK imagery */
	NULL,	/* window title */
	NULL,	/* custom screen pointer */
	NULL,	/* custom bitmap */
	0,0,	/* minimum width and height */
	0,0,	/* maximum width and height */
	CUSTOMSCREEN	/* destination screen type */
};


/* end of PowerWindows source generation */
