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

#define  PANEL                            1       /* callback function: exitfunc */
#define  PANEL_CANVAS                     2       /* control type: canvas, callback function: (none) */
#define  PANEL_KeysTextBox                3       /* control type: string, callback function: (none) */
#define  PANEL_SpaceshipTextBox           4       /* control type: string, callback function: (none) */
#define  PANEL_SpaceshipHit               5       /* control type: LED, callback function: (none) */
#define  PANEL_ScoreControl               6       /* control type: numeric, callback function: (none) */
#define  PANEL_LivesCountControl          7       /* control type: numeric, callback function: (none) */
#define  PANEL_TIMER                      8       /* control type: timer, callback function: Timerfunc */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK exitfunc(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Timerfunc(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
