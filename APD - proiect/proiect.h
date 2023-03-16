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

#define  MAIN_PANEL                       1       /* callback function: OnMainPanel */
#define  MAIN_PANEL_NR_TRECERI_PRIN_ZERO  2       /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_MEDIE                 3       /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_MEDIANA               4       /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_DISPERSIE             5       /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_MIN_INDEX             6       /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_MAX_INDEX             7       /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_Maxim                 8       /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_Minim                 9       /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_GRAPH_IN_SECONDS      10      /* control type: graph, callback function: (none) */
#define  MAIN_PANEL_GRAPH                 11      /* control type: graph, callback function: (none) */
#define  MAIN_PANEL_BUTTON_LOAD           12      /* control type: command, callback function: OnButtonLoad */
#define  MAIN_PANEL_HISTOGRAM             13      /* control type: graph, callback function: (none) */
#define  MAIN_PANEL_NR_DE_INTERVALE       14      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_GRAPH_DERIVATA        15      /* control type: graph, callback function: (none) */
#define  MAIN_PANEL_GRAPH_FILTRAT         16      /* control type: graph, callback function: (none) */
#define  MAIN_PANEL_NUMERIC_ALPHA         17      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_COMMANDBUTTON_APLICA  18      /* control type: command, callback function: onApplyButton */
#define  MAIN_PANEL_COMMANDBUTTON_NEXT    19      /* control type: command, callback function: onChangeSeconds */
#define  MAIN_PANEL_COMMANDBUTTON_PREV    20      /* control type: command, callback function: onChangeSeconds */
#define  MAIN_PANEL_NUMERIC_STOP_SECONDS  21      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_NUMERIC_START_SECONDS 22      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_RING_FILTER           23      /* control type: ring, callback function: (none) */
#define  MAIN_PANEL_NUMERIC_DIM_FER       24      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_BUTTON_DERIVATA       25      /* control type: command, callback function: OnButtonDerivata */
#define  MAIN_PANEL_DECORATION_2          26      /* control type: deco, callback function: (none) */
#define  MAIN_PANEL_DECORATION_3          27      /* control type: deco, callback function: (none) */
#define  MAIN_PANEL_DECORATION            28      /* control type: deco, callback function: (none) */
#define  MAIN_PANEL_DECORATION_4          29      /* control type: deco, callback function: (none) */
#define  MAIN_PANEL_SWITCH_PANEL          30      /* control type: binary, callback function: OnSwitchPanel */

#define  PANEL                            2
#define  PANEL_GRAPH                      2       /* control type: graph, callback function: (none) */

#define  PANEL2                           3       /* callback function: OnFrequencyPanel */
#define  PANEL2_SWITCH_PANEL              2       /* control type: binary, callback function: OnSwitchPanel */
#define  PANEL2_GRAPH_SPECT_WIND_FILT     3       /* control type: graph, callback function: (none) */
#define  PANEL2_NUMERIC_POWER_PEAK        4       /* control type: numeric, callback function: (none) */
#define  PANEL2_NUMERIC_FREQ_PEAK         5       /* control type: numeric, callback function: (none) */
#define  PANEL2_NUMERIC_NR_PUNCTE         6       /* control type: numeric, callback function: (none) */
#define  PANEL2_TIMER                     7       /* control type: timer, callback function: OnClickTimer */
#define  PANEL2_GRAPH_WINDOWING_RAW       8       /* control type: graph, callback function: (none) */
#define  PANEL2_GRAPH_WINDOWING_FILT      9       /* control type: graph, callback function: (none) */
#define  PANEL2_GRAPH_FILT                10      /* control type: graph, callback function: (none) */
#define  PANEL2_GRAPH_SPECT_WIND_RAW      11      /* control type: graph, callback function: (none) */
#define  PANEL2_WINDOW                    12      /* control type: binary, callback function: (none) */
#define  PANEL2_GRAPH_RAW_SIGNAL          13      /* control type: graph, callback function: (none) */
#define  PANEL2_GRAPH_WINDOW              14      /* control type: graph, callback function: (none) */
#define  PANEL2_SWITCH_FILTER             15      /* control type: binary, callback function: (none) */
#define  PANEL2_DECORATION                16      /* control type: deco, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK onApplyButton(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnButtonDerivata(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnButtonLoad(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK onChangeSeconds(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnClickTimer(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFrequencyPanel(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnMainPanel(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnSwitchPanel(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif