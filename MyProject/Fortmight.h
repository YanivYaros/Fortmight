/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  ABOUTPANEL                       1       /* callback function: Aboutpanel */
#define  ABOUTPANEL_TEXTMSG               2       /* control type: textMsg, callback function: (none) */
#define  ABOUTPANEL_PICTURE               3       /* control type: picture, callback function: (none) */
#define  ABOUTPANEL_PICTURE_2             4       /* control type: picture, callback function: (none) */

#define  LBPANEL                          2       /* callback function: leaderBordpanel */
#define  LBPANEL_LEADERBORDTABLE          2       /* control type: table, callback function: (none) */

#define  MENUPANEL                        3       /* callback function: Menufunc */
#define  MENUPANEL_ABOUTBUTTON            2       /* control type: command, callback function: OpenAboutMe */
#define  MENUPANEL_LEADERBORDBUTTON       3       /* control type: command, callback function: OpenLeaderBord */
#define  MENUPANEL_STARTBUTTON            4       /* control type: command, callback function: StartGame */
#define  MENUPANEL_NAMESTRING             5       /* control type: string, callback function: (none) */
#define  MENUPANEL_TEXTMSG_2              6       /* control type: textMsg, callback function: (none) */
#define  MENUPANEL_PICTURE                7       /* control type: picture, callback function: (none) */
#define  MENUPANEL_TEXTMSG                8       /* control type: textMsg, callback function: (none) */

#define  PANEL                            4       /* callback function: exitfunc */
#define  PANEL_CANVAS                     2       /* control type: canvas, callback function: (none) */
#define  PANEL_KeysTextBox                3       /* control type: string, callback function: (none) */
#define  PANEL_SpaceshipTextBox           4       /* control type: string, callback function: (none) */
#define  PANEL_SpaceshipHit               5       /* control type: LED, callback function: (none) */
#define  PANEL_ScoreControl               6       /* control type: numeric, callback function: (none) */
#define  PANEL_LivesCountControl          7       /* control type: numeric, callback function: (none) */
#define  PANEL_TIMER                      8       /* control type: timer, callback function: Timerfunc */
#define  PANEL_HEARTSCANVAS               9       /* control type: canvas, callback function: (none) */
#define  PANEL_RETURNTOMENU               10      /* control type: command, callback function: ReturnToMenu */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

#define  MENUBAR                          1
#define  MENUBAR_MENUBAR                  2
#define  MENUBAR_MENUBAR_HOWTOPLAY        3       /* callback function: OpenTutorialDoc */
#define  MENUBAR_MENUBAR_DEMO             4       /* callback function: OpenDemoVideo */


     /* Callback Prototypes: */

int  CVICALLBACK Aboutpanel(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK exitfunc(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK leaderBordpanel(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Menufunc(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OpenAboutMe(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK OpenDemoVideo(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK OpenLeaderBord(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK OpenTutorialDoc(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK ReturnToMenu(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK StartGame(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Timerfunc(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
