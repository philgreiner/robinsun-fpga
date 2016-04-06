// --------------------------------------------------------------------
// Copyright (c) 2011 by Terasic Technologies Inc. 
// --------------------------------------------------------------------
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
// --------------------------------------------------------------------
//           
//                     Terasic Technologies Inc
//                     356 Fu-Shin E. Rd Sec. 1. JhuBei City,
//                     HsinChu County, Taiwan
//                     302
//
//                     web: http://www.terasic.com/
//                     email: support@terasic.com
//
// --------------------------------------------------------------------

#include "../terasic_lib/terasic_includes.h"
#include "gui.h"
#include "../terasic_lib/touch_spi.h"
#include "../graphic_lib/simple_graphics.h"
#include "../graphic_lib/geometry.h"



void ShowInfo(alt_video_display *pDisplay, char *pText){
    vid_print_string_alpha(1, 1, WHITE_24, BLACK_24, tahomabold_20, pDisplay, pText);
}


void VPG_XLine(alt_video_display *pDisplay){
return ;
    // erase background
     vid_clean_screen(pDisplay, BLACK_24);

    // draw line
    vid_draw_line(0, 0, pDisplay->width-1, pDisplay->height-1, 1, WHITE_24, pDisplay);
    vid_draw_line(pDisplay->width-1, 0, 0, pDisplay->height-1, 1, WHITE_24, pDisplay);

    // show text
    ShowInfo(pDisplay, "X Line");


}


void VPG_ColorPanel(alt_video_display *pDisplay, alt_u32 Color){
    int x, y;
    char szText[32];

    for(y=0;y<pDisplay->height;y++){
        for(x=0;x<pDisplay->width;x++){
        	vid_set_pixel(x,y,Color, pDisplay);
        }
    }

    // show text
    sprintf(szText,"Panel: %08xh", (unsigned int) Color);
    ShowInfo(pDisplay, szText);



}

void VPG_Grid(alt_video_display *pDisplay, alt_u8 GridSize){
    int x,y;
    alt_u32 Value32;
    char szText[32];


    for(y=0;y<pDisplay->height;y++){
        for(x=0;x<pDisplay->width;x++){
            if (x%GridSize == 0 || y%GridSize == 0)
                Value32 = WHITE_24;
            else
                Value32 = BLACK_24;
            vid_set_pixel(x,y,Value32, pDisplay);
        }

    }
    // show text
    sprintf(szText,"Grid%d", GridSize);
    ShowInfo(pDisplay, szText);

}

void VPG_VGrid(alt_video_display *pDisplay, alt_u8 GridSize){
    int x,y;
    alt_u32 Value32;
    char szText[32];


    for(y=0;y<pDisplay->height;y++){
        for(x=0;x<pDisplay->width;x++){
            if (x%GridSize == 0)
                Value32 = WHITE_24;
            else
                Value32 = BLACK_24;
            vid_set_pixel(x,y,Value32, pDisplay);
        }

    }
    // show text
    sprintf(szText,"VGrid%d", GridSize);
    ShowInfo(pDisplay, szText);

}

void VPG_HGrid(alt_video_display *pDisplay, alt_u8 GridSize){
    int x,y;
    alt_u32 Value32;
    char szText[32];

    for(y=0;y<pDisplay->height;y++){
        for(x=0;x<pDisplay->width;x++){
            if (y%GridSize == 0)
                Value32 = WHITE_24;
            else
                Value32 = BLACK_24;
            vid_set_pixel(x,y,Value32, pDisplay);
        }

    }
    // show text
    sprintf(szText,"HGrid%d", GridSize);
    ShowInfo(pDisplay, szText);

}




void VPG_ColorBar(alt_video_display *pDisplay){
    int x,y;
    alt_u32 Value32;

    for(y=0;y<pDisplay->height;y++){
        for(x=0;x<pDisplay->width;x++){
            if (x == 0 || y == 0 || x == pDisplay->width-1 || y == pDisplay->height-1){
                Value32 = WHITE_24;
            }else if (y < pDisplay->height/4){
                // red
                Value32 = x & 0xFF;
            }else if (y < pDisplay->height/2){
                Value32 = (x & 0xFF) << 8;
            }else if (y < pDisplay->height*3/4){
                Value32 = (x & 0xFF) << 16;
            }else{
                Value32 = x & 0xFF;
                Value32 |= (x & 0xFF) << 8;
                Value32 |= (x & 0xFF) << 16;
            }
            vid_set_pixel(x,y,Value32, pDisplay);

        }

    }

}


void GUI_ShowPAT(alt_video_display *pDisplay, int PatId){

                switch(PatId){
                    case 0: VPG_ColorBar(pDisplay); break;
                    case 1: VPG_ColorPanel(pDisplay, WHITE_24); break;
                    case 2: VPG_ColorPanel(pDisplay, RED_24); break;
                    case 3: VPG_ColorPanel(pDisplay, GREEN_24); break;
                    case 4: VPG_ColorPanel(pDisplay, BLUE_24); break;
                    case 5: VPG_XLine(pDisplay); break;
                    case 6: VPG_VGrid(pDisplay, 2); break;
                    case 7: VPG_VGrid(pDisplay, 4); break;
                    case 8: VPG_VGrid(pDisplay, 8); break;
                    case 9: VPG_HGrid(pDisplay, 2); break;
                    case 10: VPG_HGrid(pDisplay, 4); break;
                    case 11: VPG_HGrid(pDisplay, 8); break;
                    case 12: VPG_Grid(pDisplay, 2); break;
                    case 13: VPG_Grid(pDisplay, 4); break;
                    case 14: VPG_Grid(pDisplay, 8); break;
                }

}

void GUI_VPG(alt_video_display *pDisplay, TOUCH_HANDLE *pTouch){
	int X, Y;
	int PatId = 0;

    // clean screen
    vid_clean_screen(pDisplay, BLACK_24);


    VPG_ColorBar(pDisplay);

    GUI_ShowPAT(pDisplay, PatId);

    while(1){
        if (Touch_GetXY(pTouch, &X, &Y)){
            //printf(".");

            // change pattern
            PatId++;
            if (PatId > 14)
                PatId = 0;

            printf("PatId=%d\r\n", PatId);
            GUI_ShowPAT(pDisplay, PatId);
            usleep(300*1000); // debounce
            Touch_EmptyFifo( pTouch);
        }
    }

}

