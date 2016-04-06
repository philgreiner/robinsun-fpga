// ============================================================================
// Copyright (c) 2014 by Terasic Technologies Inc.
// ============================================================================
//
// Permission:
//
//   Terasic grants permission to use and modify this code for use
//   in synthesis for all Terasic Development Boards and Altera Development
//   Kits made by Terasic.  Other use of this code, including the selling
//   ,duplication, or modification of any portion is strictly prohibited.
//
// Disclaimer:
//
//   This VHDL/Verilog or C/C++ source code is intended as a design reference
//   which illustrates how these types of functions can be implemented.
//   It is the user's responsibility to verify their design for
//   consistency and functionality through the use of formal
//   verification methods.  Terasic provides no warranty regarding the use
//   or functionality of this code.
//
// ============================================================================
//
//  Terasic Technologies Inc
//  9F., No.176, Sec.2, Gongdao 5th Rd, East Dist, Hsinchu City, 30070. Taiwan
//
//
//                     web: http://www.terasic.com/
//                     email: support@terasic.com
//
// ============================================================================

#include "../terasic_lib/terasic_includes.h"
#include "gui.h"
#include "../graphic_lib/alt_video_display.h"
#include "../terasic_lib/touch_spi.h"
#include "../graphic_lib/simple_graphics.h"
#include "../graphic_lib/geometry.h"


#define DOT_SIZE    5

void GUI_ShowInfo(alt_video_display *pDisplay, char *pText);
void GUI_ShowWelcome(alt_video_display *pDisplay);
void GUI_ShowTouchPoint(alt_video_display *pDisplay, int X1, int Y1, int color);


alt_u32 szPallete[] = {
    WHITE_24,
    0xFFFFFF,
    0x0000FF,
    0x00FF00,
    0xFF0000
};

typedef enum{
    BTN_CLEAR = 0,
    BTN_WHITE,
    BTN_BLUE,
    BTN_GREEN,
    BTN_RED,
    BTN_NUM,
    
    BTN_NONE
}BUTTON_ID;

typedef struct{
    RECT rc;
    int  color;
}PALETTE_INFO;

typedef struct{
    int Paint_Index;
    RECT rcPaint;
    PALETTE_INFO szPalette[BTN_NUM];
}DESK_INFO;


void GUI_ShowWelcome(alt_video_display *pDisplay){
	int x, y;


	x = pDisplay->width / 2 - 60;
	y = pDisplay->height / 2 - 10;

	vid_print_string_alpha(x, y, BLUE_24, BLACK_24, tahomabold_20, pDisplay, "Terasic");
	vid_print_string_alpha(x, y+22, BLUE_24, BLACK_24, tahomabold_20, pDisplay, "Touch Demo");
}

void GUI_ShowInfo(alt_video_display *pDisplay, char *pText){
    static int x=0,y=100;
   // vid_clean_screen(pReader, BLACK_24);
    vid_print_string_alpha(x, y, BLUE_24, BLACK_24, tahomabold_20, pDisplay, pText);
 //   VIPFR_ActiveDrawFrame(pReader);
}

void GUI_ShowTouchPoint(alt_video_display *pDisplay, int X, int Y, int color){
    vid_draw_circle(X, Y, 10, color, DO_FILL, pDisplay);
   // VIPFR_ActiveDrawFrame(pReader);
}


//----------------------------------------------------------------------------------------------------

void GUI_DeskInit(alt_video_display *pDisplay, DESK_INFO *pDeskInfo){
    int i;
    RECT rc;
    const int BoxH = 40;
    const int BoxW = 40;
    const int GapH = 5;
    const int DrawBorder = 2;
    const int TextW = 0;
    const int ClearExtraW = 30;
    int GapW;

    
    GapW = (pDisplay->width - TextW - ClearExtraW - (BTN_NUM*BoxW))/(BTN_NUM+1);
    //printf("GapW=%d\r\n", GapW);
    rc.top = pDisplay->height - BoxH - GapH;
    rc.bottom = rc.top + BoxH;
    rc.left = GapW + TextW + ClearExtraW;
    rc.right = rc.left + BoxW;
    
    // desk drawing area    
    RectSet(&pDeskInfo->rcPaint, DrawBorder, pDisplay->width-DrawBorder, DrawBorder, rc.top - DrawBorder);
    
    for(i=0;i<BTN_NUM;i++){
        RectCopy(&pDeskInfo->szPalette[i].rc, &rc);
        pDeskInfo->szPalette[i].color = szPallete[i];
        RectOffset(&rc, BoxW+GapW, 0);
    }
    pDeskInfo->szPalette[BTN_CLEAR].rc.left -= ClearExtraW;
    pDeskInfo->Paint_Index = BTN_BLUE;
}

//----------------------------------------------------------------------------------------------------

int GUI_CheckButton(DESK_INFO *pDeskInfo, POINT *pt){
    int ButtonId = BTN_NONE;
    int i;
    
    for(i=0;i<BTN_NUM && (ButtonId == BTN_NONE);i++){
        if (IsPtInRect(pt, &pDeskInfo->szPalette[i].rc))
            ButtonId = i;
    }
    return ButtonId;
}

//----------------------------------------------------------------------------------------------------

void GUI_DeskDraw(alt_video_display *pDisplay, DESK_INFO *pDeskInfo){
    int i, active;
    RECT rc;
    
    
    // show text
    //  vid_print_string_alpha(pDeskInfo->rcPaint.left, pDeskInfo->szPalette[0].rc.top, BLUE_24, BLACK_24, tahomabold_20, pDisplay, "Terasic");
    //  vid_print_string_alpha(pDeskInfo->rcPaint.left, pDeskInfo->szPalette[0].rc.top+22, BLUE_24, BLACK_24, tahomabold_20, pDisplay, "Touch");

    // draw border
    RectCopy(&rc, &pDeskInfo->rcPaint);
    vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
    

    // draw palette
	active = pDeskInfo->Paint_Index;
    for(i=0;i<BTN_NUM;i++){
        RectCopy(&rc, &(pDeskInfo->szPalette[i].rc));
        if (i == BTN_CLEAR){
            vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, pDeskInfo->szPalette[i].color, DO_NOT_FILL, pDisplay);
            vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-22)/2, pDeskInfo->szPalette[i].color, BLACK_24, tahomabold_20, pDisplay, "Clear");
        }else{
        	if (active != i){
          		vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, BLACK_24, DO_FILL, pDisplay);
        		RectInflate(&rc, -4, -4);
        	}
      		vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, pDeskInfo->szPalette[i].color, DO_FILL, pDisplay);
        }            
    }
  
    // draw activce color
    // active = pDeskInfo->Paint_Index;
    //  RectCopy(&rc, &pDeskInfo->szPalette[active].rc);
    //  vid_print_string_alpha(rc.left+2, rc.top+2, BLACK_24, pDeskInfo->szPalette[active].color, tahomabold_20, pDisplay, "*");
        
    //active = pDeskInfo->Paint1_Index;
    //RectCopy(&rc, &pDeskInfo->szPalette[active].rc);
    //vid_print_string_alpha(rc.left+2, rc.top+2, BLACK_24, pDeskInfo->szPalette[active].color, tahomabold_20, pDisplay, "2");
}

//----------------------------------------------------------------------------------------------------

bool IsContinuedPoint(POINT *ptPre, POINT *ptNew){
    bool bYes = TRUE;
    const int nMax = 50;
    if (abs(ptPre->x - ptNew->x) > nMax)
        bYes = FALSE;
    else if (abs(ptPre->y - ptNew->y) > nMax)
        bYes = FALSE;
        
    return bYes;
}

//----------------------------------------------------------------------------------------------------

void GUI_ClearPaintArea(alt_video_display *pDisplay, DESK_INFO *pDeskInfo){
    RECT rc;
    RectCopy(&rc, &pDeskInfo->rcPaint);
    RectInflate(&rc, -1, -1);
    vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, BLACK_24, DO_FILL, pDisplay);
    //
  //  RectCopy(&rc, &pDeskInfo->szPalette[BTN_GESTRUE].rc);
  //  vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, BLACK_24, DO_FILL, pDisplay);
}

//----------------------------------------------------------------------------------------------------

void GUI(alt_video_display *pDisplay, TOUCH_HANDLE *pTouch){
    // video
    DESK_INFO DeskInfo;
    int X, Y;
    POINT Pt;
    const int nDotSize = DOT_SIZE;
    RECT rcTouch;
    int ColorPen, ButtonId;
    bool UpdatePalette;

    // clean screen
    vid_clean_screen(pDisplay, BLACK_24);
    
    // Show Terasic Welcome
    GUI_ShowWelcome(pDisplay);
    usleep(4*1000*1000);
    vid_clean_screen(pDisplay, BLACK_24);

    GUI_DeskInit(pDisplay, &DeskInfo);
    GUI_DeskDraw(pDisplay, &DeskInfo);
    
    RectCopy(&rcTouch, &DeskInfo.rcPaint);
    RectInflate(&rcTouch, -nDotSize-2, -nDotSize-2);
    
    ColorPen = DeskInfo.szPalette[DeskInfo.Paint_Index].color;
    //ColorPen1 = DeskInfo.szPalette[DeskInfo.Paint1_Index].color;
    
   //  printf("ContinuedTime=%d\r\n", ContinuedTime);
   // PtSet(&ptGesture, DeskInfo.szPalette[BTN_GESTRUE].rc.left+5, DeskInfo.szPalette[BTN_GESTRUE].rc.top+5);

    while(1){
            // touch
            if (Touch_GetXY(pTouch, &X, &Y)){

                printf("x=%d, y=%d\r\n", X,Y);
                PtSet(&Pt, X, Y);


                //   printf("t:%d, %d\r\n", (int)alt_nticks(), LastTimePt1);
                if ( IsPtInRect(&Pt, &rcTouch)){

                    vid_draw_circle(Pt.x, Pt.y, nDotSize, ColorPen, DO_FILL, pDisplay);

                }else if (GUI_CheckButton(&DeskInfo, &Pt) == BTN_CLEAR){
                    GUI_ClearPaintArea(pDisplay, &DeskInfo);
                    Touch_EmptyFifo(pTouch);
                }else {
                    ButtonId = GUI_CheckButton(&DeskInfo, &Pt);
                    UpdatePalette = FALSE;
                    if (ButtonId != BTN_NONE && ButtonId != BTN_CLEAR){
                        // chang draw color
                        if ( ButtonId != DeskInfo.Paint_Index ){
                            DeskInfo.Paint_Index = ButtonId;
                            UpdatePalette = TRUE;
                        }

                        if (UpdatePalette){
                            ColorPen = DeskInfo.szPalette[DeskInfo.Paint_Index].color;
                            GUI_DeskDraw(pDisplay, &DeskInfo);
                            Touch_EmptyFifo(pTouch);
                        }
                    }
                }
            } // if touch
        } // while
    
}



