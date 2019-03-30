/*
 *  Source machine generated by GadToolsBox V2.0b
 *  which is (c) Copyright 1991-1993 Jaba Development
 *
 *  GUI Designed by : Jan Erik Olausen
 */

#define GetString( g )      ((( struct StringInfo * )g->SpecialInfo )->Buffer  )
#define GetNumber( g )      ((( struct StringInfo * )g->SpecialInfo )->LongInt )

#define GD_MM_DUMP                             0
#define GD_MM_LINE_MINUS                       1
#define GD_MM_LINE_PLUS                        2
#define GD_MM_PAGE_MINUS                       3
#define GD_MM_PAGE_PLUS                        4
#define GD_MM_ADDRESS                          5
#define GD_MM_EXIT                             6

#define GDX_MM_DUMP                            0
#define GDX_MM_LINE_MINUS                      1
#define GDX_MM_LINE_PLUS                       2
#define GDX_MM_PAGE_MINUS                      3
#define GDX_MM_PAGE_PLUS                       4
#define GDX_MM_ADDRESS                         5
#define GDX_MM_EXIT                            6

#define Monitor_CNT 7

extern struct IntuitionBase *IntuitionBase;
extern struct Library       *GadToolsBase;

extern struct Screen        *Scr;
extern UBYTE                 *PubScreenName;
extern APTR                  VisualInfo;
extern struct Window        *MonitorWnd;
extern struct Gadget        *MonitorGList;
extern struct Gadget        *MonitorGadgets[7];
extern UWORD                 MonitorLeft;
extern UWORD                 MonitorTop;
extern UWORD                 MonitorWidth;
extern UWORD                 MonitorHeight;
extern UBYTE                *MonitorWdt;
extern UBYTE                *MM_DUMP0Labels[];
extern struct TextAttr       topaz8;
extern UWORD                 MonitorGTypes[];
extern struct NewGadget      MonitorNGad[];
extern ULONG                 MonitorGTags[];


extern int SetupScreen( void );
extern void CloseDownScreen( void );
extern void MonitorRender( void );
extern int OpenMonitorWindow( void );
extern void CloseMonitorWindow( void );
