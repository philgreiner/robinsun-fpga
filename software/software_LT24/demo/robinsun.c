/*
 * robinsun.c
 *
 *  Created on: 22 mars 2016
 *      Author: Sophie
 */

#include "../terasic_lib/terasic_includes.h"
#include "robinsun.h"
#include "../graphic_lib/alt_video_display.h"
#include "../terasic_lib/touch_spi.h"
#include "../graphic_lib/simple_graphics.h"
#include "../graphic_lib/geometry.h"
#include "system.h"

#define DOT_SIZE    5

alt_u32 color_team[] = {
	0x800080,	//PURPLE
	0x008000,	//GREEN
	0xFFFFFF	//WHITE
};

// Buttons for the main menu
typedef enum{
    BTN_TEST = 0,
    BTN_COMPET,
    BTN_NUM,

    BTN_NONE
}BUTTON_ID;

// Buttons for the tests menu
typedef enum{
    BTN_MOVE = 0,
    BTN_BLOCKS,
    BTN_FISH,
    BTN_CABINS,
    BTN_PARASOL,
    BTN_BACK_TESTS,
    BTN_NUM_TESTS,

    BTN_NONE_TESTS
}BUTTON_TESTS_ID;

// Buttons for the action test menu
typedef enum{
    BTN_ACTION_MSG = 0,
    BTN_ACTION_OK,
    BTN_ACTION_BACK,
    BTN_NUM_ACTION,

    BTN_NONE_ACTION
}BUTTON_ACTION_ID;

// Buttons for the competition menu
typedef enum{
    BTN_GREEN = 0,
    BTN_PURPLE,
    BTN_BACK_COMPET,
    BTN_NUM_COMPET,

    BTN_NONE_COMPET
}BUTTON_COMPET_ID;

// Buttons for the green team menu
typedef enum{
    BTN_GREEN_MSG = 0,
    BTN_GREEN_OK,
    BTN_GREEN_BACK,
    BTN_NUM_GREEN,

    BTN_NONE_GREEN
}BUTTON_GREEN_START;

// Buttons for the purple team menu
typedef enum{
    BTN_PURPLE_MSG = 0,
    BTN_PURPLE_OK,
    BTN_PURPLE_BACK,
    BTN_NUM_PURPLE,

    BTN_NONE_PURPLE
}BUTTON_PURPLE_START;

// Definition of a button with a rectangle and a color that define it
typedef struct{
    RECT rc;
    int color;
}BUTTON_INFO;

// Definition of a structure referring to all buttons
typedef struct{
	int Button_Index;
    RECT rcDisp;
    BUTTON_INFO buttons[BTN_NUM];
    BUTTON_INFO buttons_tests[BTN_NUM_TESTS];
    BUTTON_INFO buttons_action[BTN_NUM_ACTION];
    BUTTON_INFO buttons_compet[BTN_NUM_COMPET];
    BUTTON_INFO buttons_green[BTN_NUM_GREEN];
    BUTTON_INFO buttons_purple[BTN_NUM_PURPLE];
}DESK_INFO;

// Draws a rectangle with bold edges
int vid_draw_box_bold (int horiz_start, int vert_start, int horiz_end, int vert_end, int color, int fill, alt_video_display* display)
{

  // If we want to fill in our box
  if (fill) {
     vid_paint_block (horiz_start, vert_start, horiz_end, vert_end, color, display);
  // If we're not filling in the box, just draw four lines.
  } else {
    vid_draw_line(horiz_start, vert_start, horiz_start, vert_end-1, 4, color, display);
    vid_draw_line(horiz_end-1, vert_start, horiz_end-1, vert_end-1, 4, color, display);
    vid_draw_line(horiz_start, vert_start, horiz_end-1, vert_start, 4, color, display);
    vid_draw_line(horiz_start, vert_end-1, horiz_end-1, vert_end-1, 4, color, display);
  }

  return (0);
}

// Checks if we touched a button of the main menu
int GUI_ROBINSUN_CheckButton(DESK_INFO *pDeskInfo, POINT *pt){
    int ButtonId = BTN_NONE;
    int i;

    for(i=0;i<BTN_NUM && (ButtonId == BTN_NONE);i++){
        if (IsPtInRect(pt, &pDeskInfo->buttons[i].rc))
            ButtonId = i;
    }
    return ButtonId;
}

// Checks if we touched a button of the competition menu
int GUI_ROBINSUN_CheckButtonCompet(DESK_INFO *pDeskInfo, POINT *pt){
    int ButtonId = BTN_NONE_COMPET;
    int i;

    for(i=0;i<BTN_NUM_COMPET && (ButtonId == BTN_NONE_COMPET);i++){
        if (IsPtInRect(pt, &pDeskInfo->buttons_compet[i].rc))
            ButtonId = i;
    }
    return ButtonId;
}

// Checks if we touched a button of the green team menu
int GUI_ROBINSUN_CheckButtonGreen(DESK_INFO *pDeskInfo, POINT *pt){
    int ButtonId = BTN_NONE_GREEN;
    int i;

    for(i=0;i<BTN_NUM_GREEN && (ButtonId == BTN_NONE_GREEN);i++){
        if (IsPtInRect(pt, &pDeskInfo->buttons_green[i].rc))
            ButtonId = i;
    }
    return ButtonId;
}

// Checks if we touched a button of the purple team menu
int GUI_ROBINSUN_CheckButtonPurple(DESK_INFO *pDeskInfo, POINT *pt){
    int ButtonId = BTN_NONE_PURPLE;
    int i;

    for(i=0;i<BTN_NUM_PURPLE && (ButtonId == BTN_NONE_PURPLE);i++){
        if (IsPtInRect(pt, &pDeskInfo->buttons_purple[i].rc))
            ButtonId = i;
    }
    return ButtonId;
}

int GUI_ROBINSUN_CheckButtonTests(DESK_INFO *pDeskInfo, POINT *pt){
    int ButtonId = BTN_NONE_TESTS;
    int i;

    for(i=0;i<BTN_NUM_TESTS && (ButtonId == BTN_NONE_TESTS);i++){
        if (IsPtInRect(pt, &pDeskInfo->buttons_tests[i].rc))
            ButtonId = i;
    }
    return ButtonId;
}

int GUI_ROBINSUN_CheckButtonAction(DESK_INFO *pDeskInfo, POINT *pt){
    int ButtonId = BTN_NONE_ACTION;
    int i;

    for(i=0;i<BTN_NUM_ACTION && (ButtonId == BTN_NONE_ACTION);i++){
        if (IsPtInRect(pt, &pDeskInfo->buttons_action[i].rc))
            ButtonId = i;
    }
    return ButtonId;
}


// ------------------------------------------------------------------------------ //
// ----------------------- BEGIN OF THE DISPLAY FUNCTIONS ----------------------- //
// ------------------------------------------------------------------------------ //


// Displays the starting message
void GUI_ROBINSUN_ShowWelcome(alt_video_display *pDisplay){
	int x, y;

	// SCREEN_WIDTH = 240
	// SCREEN_HEIGHT = 320
	x = pDisplay->width / 2;	// 60 = 9mm   SCREEN = 36.5mm
	y = pDisplay->height / 2 - 50;	// 20 = 3mm

	vid_print_string_alpha(x - 77, y, WHITE_24, BLACK_24, tahomabold_32, pDisplay, "Welcome"); // Welcome = 23mm
	vid_print_string_alpha(x - 57, y+40, BLUE_24, BLACK_24, tahomabold_20, pDisplay, "ROBINSUN"); // ROBINSUN = 17mm
	vid_print_string_alpha(x - 83, y+62, BLUE_24, BLACK_24, tahomabold_20, pDisplay, "Team's Member !"); // 25mm
}


// -------------------------- MAIN MENU -------------------------- //

// Initialization of the main menu
void GUI_ROBINSUN_DeskInit(alt_video_display *pDisplay, DESK_INFO *pDeskInfo){

	int i;
    RECT rc;
    const int BoxH = 40;
    const int DistBorder = 2;
    const int DistH = 30;
    int GapH;

    //BTN_NUM = 2
    // Space to put between the buttons in order to be equally spaced
    GapH = (pDisplay->height - ((BTN_NUM)*BoxH))/(BTN_NUM+1);

    rc.top = DistBorder + GapH;
    rc.bottom = rc.top + BoxH;
    rc.left = DistH;
    rc.right = pDisplay->width - DistH;

    // Desk border
    RectSet(&pDeskInfo->rcDisp, DistBorder, pDisplay->width-DistBorder, DistBorder, pDisplay->height-DistBorder);

    // Initialization of the rectangles for the buttons
    for(i=0;i<BTN_NUM;i++){
        RectCopy(&pDeskInfo->buttons[i].rc, &rc);
        RectOffset(&rc, 0, BoxH+GapH);
    }
    pDeskInfo->Button_Index = 100;
}

// Draws the main menu
void GUI_ROBINSUN_DeskDraw(alt_video_display *pDisplay, DESK_INFO *pDeskInfo){
    int i;
    RECT rc;

    // draw border
    RectCopy(&rc, &pDeskInfo->rcDisp);
    vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_NOT_FILL, pDisplay);

    // draw buttons
    for(i=0;i<BTN_NUM;i++){
        RectCopy(&rc, &(pDeskInfo->buttons[i].rc));
        if (i == BTN_TEST){
            vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_FILL, pDisplay);
            vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-22)/2, BLACK_24, WHITE_24, tahomabold_20, pDisplay, "Tests");
        }
        else if (i == BTN_COMPET){
        	vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_FILL, pDisplay);
        	vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-22)/2, BLACK_24, WHITE_24, tahomabold_20, pDisplay, "Competition");
        }
        else
        {
      		//vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_FILL, pDisplay);
        }
    }
}

// ---------------------- TESTS MENU ---------------------- //

// Initialization of the tests menu
void GUI_ROBINSUN_Tests_Init(alt_video_display *pDisplay, DESK_INFO *pDeskInfo){

	int i;
    RECT rc;
    const int BoxH = 40;
    const int DistBorder = 2;
    const int DistH = 30;
    int GapH;

    // Space to put between the buttons in order to be equally spaced
    GapH = (pDisplay->height - ((BTN_NUM_TESTS)*BoxH))/(BTN_NUM_TESTS+1);

    rc.top = DistBorder + GapH;
    rc.bottom = rc.top + BoxH;
    rc.left = DistH;
    rc.right = pDisplay->width - DistH;

    // Desk border
    RectSet(&pDeskInfo->rcDisp, DistBorder, pDisplay->width-DistBorder, DistBorder, pDisplay->height-DistBorder);

    // Initialization of the rectangles for the buttons
    for(i=0;i<BTN_NUM_TESTS;i++){
        RectCopy(&pDeskInfo->buttons_tests[i].rc, &rc);
        RectOffset(&rc, 0, BoxH+GapH);
    }
    pDeskInfo->Button_Index = 100;
}

// Draws the test menu
void GUI_ROBINSUN_Tests_Draw(alt_video_display *pDisplay, DESK_INFO *pDeskInfo){
    int i;
    RECT rc;

    // draw border
    RectCopy(&rc, &pDeskInfo->rcDisp);
    vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_NOT_FILL, pDisplay);

    // draw buttons
    for(i=0;i<BTN_NUM_TESTS;i++){
        RectCopy(&rc, &(pDeskInfo->buttons_tests[i].rc));
        if (i == BTN_MOVE){
            vid_draw_box_bold (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
            vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "Move");
        }
        else if (i == BTN_BLOCKS){
        	vid_draw_box_bold (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
        	vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "Blocks");
        }
        else if (i == BTN_FISH){
        	vid_draw_box_bold (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
            vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "Fish");
        }
        else if (i == BTN_CABINS){
        	vid_draw_box_bold (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
            vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "Cabins");
        }
        else if (i == BTN_PARASOL){
          	vid_draw_box_bold (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
            vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "Parasol");
        }
        else if (i == BTN_BACK_TESTS){
        	vid_draw_box_bold (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
            vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "Back");
        }
        else
        {
      		//vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_FILL, pDisplay);
        }
    }
}

// ------------ MOVE MENU ------------ //

// Initialization of the green team menu
void GUI_ROBINSUN_Action_Init(alt_video_display *pDisplay, DESK_INFO *pDeskInfo){
	int i;
	RECT rc;
	const int BoxH = 40;
	const int DistBorder = 2;
	const int DistH = 30;
	int GapH;

	// Space to put between the buttons in order to be equally spaced
	GapH = (pDisplay->height - ((BTN_NUM_ACTION)*BoxH))/(BTN_NUM_ACTION+1);


	rc.top = DistBorder + GapH;
	rc.bottom = rc.top + BoxH;
	rc.left = DistH;
	rc.right = pDisplay->width - DistH;

	// Desk border
	RectSet(&pDeskInfo->rcDisp, DistBorder, pDisplay->width-DistBorder, DistBorder, pDisplay->height-DistBorder);

	// Initialization of the rectangles for the buttons
	for(i=0;i<BTN_NUM_ACTION;i++){
		RectCopy(&pDeskInfo->buttons_action[i].rc, &rc);
	    if(i == BTN_ACTION_MSG){
	    	RectOffset(&rc, 0, BoxH);
	    }
	    else{
	    	RectOffset(&rc, 0, BoxH+GapH);
	    }
	}
	pDeskInfo->Button_Index = 100;
}

// Draws the action menu
void GUI_ROBINSUN_Action_Draw(alt_video_display *pDisplay, DESK_INFO *pDeskInfo){
    int i;
    RECT rc;

    // draw border
    RectCopy(&rc, &pDeskInfo->rcDisp);
    vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_NOT_FILL, pDisplay);

    // draw buttons
    for(i=0;i<BTN_NUM_ACTION;i++){
        RectCopy(&rc, &(pDeskInfo->buttons_action[i].rc));
        if (i == BTN_ACTION_MSG){
            vid_draw_box_bold (rc.left, rc.top, rc.right, rc.bottom, BLACK_24, DO_NOT_FILL, pDisplay);
            vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "Start ?");
        }
        else if (i == BTN_ACTION_OK){
        	vid_draw_box_bold (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
        	vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "OK");
        }
        else if (i == BTN_ACTION_BACK)
        {
      		vid_draw_box_bold (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
        	vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "Back");
        }
    }
}

// -------------------- COMPETITION MENU -------------------- //

// Initialization of the competition menu
void GUI_ROBINSUN_Competition_Init(alt_video_display *pDisplay, DESK_INFO *pDeskInfo){

	int i;
    RECT rc;
    const int BoxH = 40;
    const int DistBorder = 2;
    const int DistH = 30;
    int GapH;

    // Space to put between the buttons in order to be equally spaced
    GapH = (pDisplay->height - ((BTN_NUM_COMPET)*BoxH))/(BTN_NUM_COMPET+1);

    rc.top = DistBorder + GapH;
    rc.bottom = rc.top + BoxH;
    rc.left = DistH;
    rc.right = pDisplay->width - DistH;

    // Desk border
    RectSet(&pDeskInfo->rcDisp, DistBorder, pDisplay->width-DistBorder, DistBorder, pDisplay->height-DistBorder);

    // Initialization of the rectangles for the buttons
    for(i=0;i<BTN_NUM_COMPET;i++){
        RectCopy(&pDeskInfo->buttons_compet[i].rc, &rc);
        RectOffset(&rc, 0, BoxH+GapH);
        pDeskInfo->buttons_compet[i].color = color_team[i];
    }
    pDeskInfo->Button_Index = 100;
}

// Draws the competition menu
void GUI_ROBINSUN_Competition_Draw(alt_video_display *pDisplay, DESK_INFO *pDeskInfo){
    int i;
    RECT rc;

    // draw border
    RectCopy(&rc, &pDeskInfo->rcDisp);
    vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_NOT_FILL, pDisplay);

    // draw buttons
    for(i=0;i<BTN_NUM_COMPET;i++){
        RectCopy(&rc, &(pDeskInfo->buttons_compet[i].rc));
        if (i == BTN_GREEN){
            vid_draw_box_bold (rc.left, rc.top, rc.right, rc.bottom, GREEN_24, DO_NOT_FILL, pDisplay);
            vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "Green");
        }
        else if (i == BTN_PURPLE){
        	vid_draw_box_bold (rc.left, rc.top, rc.right, rc.bottom, PURPLE_24, DO_NOT_FILL, pDisplay);
        	vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "Purple");
        }
        else if (i == BTN_BACK_COMPET){
        	vid_draw_box_bold (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
            vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "Back");
        }
        else
        {
      		//vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_FILL, pDisplay);
        }
    }
}

// ---------- GREEN TEAM MENU ---------- //

// Initialization of the green team menu
void GUI_ROBINSUN_Green_Start_Init(alt_video_display *pDisplay, DESK_INFO *pDeskInfo){
	int i;
	    RECT rc;
	    const int BoxH = 40;
	    const int DistBorder = 2;
	    const int DistH = 30;
	    int GapH;

	    // Space to put between the buttons in order to be equally spaced
	    GapH = (pDisplay->height - ((BTN_NUM_GREEN)*BoxH))/(BTN_NUM_GREEN+1);

	    rc.top = DistBorder + GapH;
	    rc.bottom = rc.top + BoxH;
	    rc.left = DistH;
	    rc.right = pDisplay->width - DistH;

	    // Desk border
	    RectSet(&pDeskInfo->rcDisp, DistBorder, pDisplay->width-DistBorder, DistBorder, pDisplay->height-DistBorder);

	    // Initialization of the rectangles for the buttons
	    for(i=0;i<BTN_NUM_GREEN;i++){
	        RectCopy(&pDeskInfo->buttons_green[i].rc, &rc);
	        if(i == BTN_GREEN_MSG){
	        	RectOffset(&rc, 0, BoxH);
	        }
	        else{
	        	RectOffset(&rc, 0, BoxH+GapH);
	        }
	        pDeskInfo->buttons_green[i].color = color_team[i];
	    }
	    pDeskInfo->Button_Index = 100;
}

// Draws the green team menu
void GUI_ROBINSUN_Green_Start_Draw(alt_video_display *pDisplay, DESK_INFO *pDeskInfo){
    int i;
    RECT rc;

    // draw border
    RectCopy(&rc, &pDeskInfo->rcDisp);
    vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_NOT_FILL, pDisplay);


	//BTN_GREEN_MSG = 0,
    //BTN_GREEN_OK,
    //BTN_GREEN_BACK,

    // draw buttons
    for(i=0;i<BTN_NUM_GREEN;i++){
        RectCopy(&rc, &(pDeskInfo->buttons_green[i].rc));
        if (i == BTN_GREEN_MSG){
            vid_draw_box_bold (rc.left, rc.top, rc.right, rc.bottom, BLACK_24, DO_NOT_FILL, pDisplay);
            vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "Start ?");
        }
        else if (i == BTN_GREEN_OK){
        	vid_draw_box_bold (rc.left, rc.top, rc.right, rc.bottom, GREEN_24, DO_NOT_FILL, pDisplay);
        	vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "OK");
        }
        else if (i == BTN_GREEN_BACK)
        {
      		vid_draw_box_bold (rc.left, rc.top, rc.right, rc.bottom, GREEN_24, DO_NOT_FILL, pDisplay);
        	vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "Back");
        }
    }
}

// ---------- PURPLE TEAM MENU ---------- //

// Initialization of the purple team menu
void GUI_ROBINSUN_Purple_Start_Init(alt_video_display *pDisplay, DESK_INFO *pDeskInfo){
	int i;
	    RECT rc;
	    const int BoxH = 40;
	    const int DistBorder = 2;
	    const int DistH = 30;
	    int GapH;

	    // Space to put between the buttons in order to be equally spaced
	    GapH = (pDisplay->height - ((BTN_NUM_PURPLE)*BoxH))/(BTN_NUM_PURPLE+1);

	    rc.top = DistBorder + GapH;
	    rc.bottom = rc.top + BoxH;
	    rc.left = DistH;
	    rc.right = pDisplay->width - DistH;

	    // Desk border
	    RectSet(&pDeskInfo->rcDisp, DistBorder, pDisplay->width-DistBorder, DistBorder, pDisplay->height-DistBorder);

	    // Initialization of the rectangles for the buttons
	    for(i=0;i<BTN_NUM_PURPLE;i++){
	        RectCopy(&pDeskInfo->buttons_purple[i].rc, &rc);
	        if(i == BTN_PURPLE_MSG){
	        	RectOffset(&rc, 0, BoxH);
	        }
	        else{
	        	RectOffset(&rc, 0, BoxH+GapH);
	        }
	        pDeskInfo->buttons_purple[i].color = color_team[i];
	    }
	    pDeskInfo->Button_Index = 100;
}

// Draws the purple team menu
void GUI_ROBINSUN_Purple_Start_Draw(alt_video_display *pDisplay, DESK_INFO *pDeskInfo){
    int i;
    RECT rc;

    // draw border
    RectCopy(&rc, &pDeskInfo->rcDisp);
    vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_NOT_FILL, pDisplay);

    // draw buttons
    for(i=0;i<BTN_NUM_PURPLE;i++){
        RectCopy(&rc, &(pDeskInfo->buttons_purple[i].rc));
        if (i == BTN_PURPLE_MSG){
            vid_draw_box_bold (rc.left, rc.top, rc.right, rc.bottom, BLACK_24, DO_NOT_FILL, pDisplay);
            vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "Start ?");
        }
        else if (i == BTN_PURPLE_OK){
        	vid_draw_box_bold (rc.left, rc.top, rc.right, rc.bottom, PURPLE_24, DO_NOT_FILL, pDisplay);
        	vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "OK");
        }
        else if (i == BTN_PURPLE_BACK)
        {
      		vid_draw_box_bold (rc.left, rc.top, rc.right, rc.bottom, PURPLE_24, DO_NOT_FILL, pDisplay);
        	vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "Back");
        }
    }
}


// ------------------------------------------------------------------------------ //
// ------------------------------- MAIN FUNCTION -------------------------------- //
// ------------------------------------------------------------------------------ //

void GUI_ROBINSUN(alt_video_display *pDisplay, TOUCH_HANDLE *pTouch){
	DESK_INFO DeskInfo;
	int X, Y;
	int desk=1;
	int tests=1, tests_loop=1;
	int compet=1, compet_loop, green=1, purple=1;
	int action=1, action_type=0;

	volatile int * signal = (int*) SIGNAL_OUT_BASE;

	POINT Pt;
	RECT rcTouchTest;
	RECT rcTouchMove, rcTouchBlocks, rcTouchFish, rcTouchCabins, rcTouchParasol, rcTouchTestBack;
	RECT rcTouchActionOK, rcTouchActionBack;
	RECT rcTouchCompet;
	RECT rcTouchGreen, rcTouchGreenOK, rcTouchGreenBack;
	RECT rcTouchPurple, rcTouchPurpleOK, rcTouchPurpleBack;

    // clean screen
    vid_clean_screen(pDisplay, BLACK_24);

    // Welcome message for Robinsun Team's Member
    GUI_ROBINSUN_ShowWelcome(pDisplay);
    usleep(2*1000*1000);
    vid_clean_screen(pDisplay, BLACK_24);

    while(1){
    	vid_clean_screen(pDisplay, BLACK_24);
    	GUI_ROBINSUN_DeskInit(pDisplay, &DeskInfo);
    	GUI_ROBINSUN_DeskDraw(pDisplay, &DeskInfo);

    	//RectCopy(&rcTouch, &DeskInfo.rcDisp);
    	RectCopy(&rcTouchTest, &DeskInfo.buttons[BTN_TEST]);
    	RectCopy(&rcTouchCompet, &DeskInfo.buttons[BTN_COMPET]);

    	desk=1;
    	Touch_EmptyFifo(pTouch);

    	while(desk){
    		// touch
    		if (Touch_GetXY(pTouch, &X, &Y)){
    			printf("x=%d, y=%d\r\n", X,Y);
    			PtSet(&Pt, X, Y);

    			if (GUI_ROBINSUN_CheckButton(&DeskInfo, &Pt) == BTN_TEST){
    				tests=1;

    				while(tests){
    					//vid_print_string_alpha(rcTouchTest.left+8, rcTouchTest.top+(RectHeight(&rcTouchTest)-22)/2, BLACK_24, WHITE_24, tahomabold_20, pDisplay, "Tests OK");
    					vid_clean_screen(pDisplay, BLACK_24);
    					usleep(1000*1000);
    					Touch_EmptyFifo(pTouch);
    					tests_loop=1;

    					while(tests_loop){
    						GUI_ROBINSUN_Tests_Init(pDisplay, &DeskInfo);
    						GUI_ROBINSUN_Tests_Draw(pDisplay, &DeskInfo);

    						RectCopy(&rcTouchMove, &DeskInfo.buttons_tests[BTN_MOVE]);
    						RectCopy(&rcTouchBlocks, &DeskInfo.buttons_tests[BTN_BLOCKS]);
    						RectCopy(&rcTouchFish, &DeskInfo.buttons_tests[BTN_FISH]);
    						RectCopy(&rcTouchCabins, &DeskInfo.buttons_tests[BTN_CABINS]);
    						RectCopy(&rcTouchParasol, &DeskInfo.buttons_tests[BTN_PARASOL]);
    						RectCopy(&rcTouchTestBack, &DeskInfo.buttons_tests[BTN_BACK_TESTS]);

    						//touch
    						if (Touch_GetXY(pTouch, &X, &Y)){
    							PtSet(&Pt, X, Y);

    							if (GUI_ROBINSUN_CheckButtonTests(&DeskInfo, &Pt) == BTN_MOVE || GUI_ROBINSUN_CheckButtonTests(&DeskInfo, &Pt) == BTN_BLOCKS || GUI_ROBINSUN_CheckButtonTests(&DeskInfo, &Pt) == BTN_FISH || GUI_ROBINSUN_CheckButtonTests(&DeskInfo, &Pt) == BTN_CABINS || GUI_ROBINSUN_CheckButtonTests(&DeskInfo, &Pt) == BTN_PARASOL){
    								action=1;
    								action_type=GUI_ROBINSUN_CheckButtonTests(&DeskInfo, &Pt);

    								//vid_print_string_alpha(rcTouchPurple.left+8, rcTouchPurple.top+(RectHeight(&rcTouchPurple)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "Purple OK");
    								vid_clean_screen(pDisplay, BLACK_24);
    								usleep(1000*1000);

    								GUI_ROBINSUN_Action_Init(pDisplay, &DeskInfo);
    								GUI_ROBINSUN_Action_Draw(pDisplay, &DeskInfo);

    								RectCopy(&rcTouchActionOK, &DeskInfo.buttons_action[BTN_ACTION_OK]);
    								RectCopy(&rcTouchActionBack, &DeskInfo.buttons_action[BTN_ACTION_BACK]);

    								Touch_EmptyFifo(pTouch);

    								while(action){
    									// touch
    									if (Touch_GetXY(pTouch, &X, &Y)){
    										PtSet(&Pt, X, Y);

    										if (GUI_ROBINSUN_CheckButtonAction(&DeskInfo, &Pt) == BTN_ACTION_OK){
    											Touch_EmptyFifo(pTouch);
    											if(action_type == BTN_MOVE){
    												if(*signal == 1){
    													*signal = 0;
    													vid_draw_box_bold (rcTouchActionOK.left, rcTouchActionOK.top, rcTouchActionOK.right, rcTouchActionOK.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
    													vid_print_string_alpha(rcTouchActionOK.left+8, rcTouchActionOK.top+(RectHeight(&rcTouchActionOK)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "START !");
    													usleep(1*1000*1000);
    												}
    												else{
    													*signal = 1;
    													vid_draw_box_bold (rcTouchActionOK.left, rcTouchActionOK.top, rcTouchActionOK.right, rcTouchActionOK.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
    													vid_print_string_alpha(rcTouchActionOK.left+8, rcTouchActionOK.top+(RectHeight(&rcTouchActionOK)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "OK ?");
    													usleep(1*1000*1000);
    												}
    											}
    											else
    												vid_print_string_alpha(rcTouchActionOK.left+8, rcTouchActionOK.top+(RectHeight(&rcTouchActionOK)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "START !");
    										}
    										else if (GUI_ROBINSUN_CheckButtonAction(&DeskInfo, &Pt) == BTN_ACTION_BACK){
    											tests_loop=0;
    											action=0;
    										}
    									}
    								}
    							}
    							else if (GUI_ROBINSUN_CheckButtonTests(&DeskInfo, &Pt) == BTN_BACK_TESTS){
    								desk=0;
    								tests=0;
    								tests_loop=0;
    							}
    						}
    					}
    				}//end while(tests)
    			}
    			else if (GUI_ROBINSUN_CheckButton(&DeskInfo, &Pt) == BTN_COMPET){
    				compet=1;

    				while(compet){
    					//vid_print_string_alpha(rcTouchCompet.left+8, rcTouchCompet.top+(RectHeight(&rcTouchCompet)-22)/2, BLACK_24, WHITE_24, tahomabold_20, pDisplay, "Competition OK");
    					vid_clean_screen(pDisplay, BLACK_24);
    					usleep(1000*1000);

    					Touch_EmptyFifo(pTouch);
        				compet_loop=1;

    					while(compet_loop){
    						GUI_ROBINSUN_Competition_Init(pDisplay, &DeskInfo);
    						GUI_ROBINSUN_Competition_Draw(pDisplay, &DeskInfo);

    						RectCopy(&rcTouchGreen, &DeskInfo.buttons_compet[BTN_GREEN]);
    						RectCopy(&rcTouchPurple, &DeskInfo.buttons_compet[BTN_PURPLE]);

    						// touch
    						if (Touch_GetXY(pTouch, &X, &Y)){
    							PtSet(&Pt, X, Y);

    							if (GUI_ROBINSUN_CheckButtonCompet(&DeskInfo, &Pt) == BTN_GREEN){
    								green=1;

    								//vid_print_string_alpha(rcTouchGreen.left+8, rcTouchGreen.top+(RectHeight(&rcTouchGreen)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "Green OK");
    								vid_clean_screen(pDisplay, BLACK_24);
    								usleep(1000*1000);

    								GUI_ROBINSUN_Green_Start_Init(pDisplay, &DeskInfo);
    								GUI_ROBINSUN_Green_Start_Draw(pDisplay, &DeskInfo);

    								RectCopy(&rcTouchGreenOK, &DeskInfo.buttons_green[BTN_GREEN_OK]);
    								RectCopy(&rcTouchGreenBack, &DeskInfo.buttons_green[BTN_GREEN_BACK]);

    								Touch_EmptyFifo(pTouch);

    								while(green){
    									// touch
    									if (Touch_GetXY(pTouch, &X, &Y)){
    										PtSet(&Pt, X, Y);

    										if (GUI_ROBINSUN_CheckButtonGreen(&DeskInfo, &Pt) == BTN_GREEN_OK){
    											Touch_EmptyFifo(pTouch);
    											vid_print_string_alpha(rcTouchGreenOK.left+8, rcTouchGreenOK.top+(RectHeight(&rcTouchGreenOK)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "START !");
    										}
    										else if (GUI_ROBINSUN_CheckButtonGreen(&DeskInfo, &Pt) == BTN_GREEN_BACK){
    											compet_loop=0;
    											green=0;
    										}

    									}
    								}
    							}
    							else if (GUI_ROBINSUN_CheckButtonCompet(&DeskInfo, &Pt) == BTN_PURPLE){
    								purple=1;

    								//vid_print_string_alpha(rcTouchPurple.left+8, rcTouchPurple.top+(RectHeight(&rcTouchPurple)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "Purple OK");
    								vid_clean_screen(pDisplay, BLACK_24);
    								usleep(1000*1000);

    								GUI_ROBINSUN_Purple_Start_Init(pDisplay, &DeskInfo);
    								GUI_ROBINSUN_Purple_Start_Draw(pDisplay, &DeskInfo);

    								RectCopy(&rcTouchPurpleOK, &DeskInfo.buttons_purple[BTN_PURPLE_OK]);
    								RectCopy(&rcTouchPurpleBack, &DeskInfo.buttons_purple[BTN_PURPLE_BACK]);

    								Touch_EmptyFifo(pTouch);

    								while(purple){
    									// touch
    									if (Touch_GetXY(pTouch, &X, &Y)){
    										PtSet(&Pt, X, Y);

    										if (GUI_ROBINSUN_CheckButtonPurple(&DeskInfo, &Pt) == BTN_PURPLE_OK){
    											Touch_EmptyFifo(pTouch);
    											vid_print_string_alpha(rcTouchPurpleOK.left+8, rcTouchPurpleOK.top+(RectHeight(&rcTouchPurpleOK)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "START !");
    										}
    										else if (GUI_ROBINSUN_CheckButtonPurple(&DeskInfo, &Pt) == BTN_PURPLE_BACK){
    											compet_loop=0;
    											purple=0;
    										}
    									}
    								}
    							}
    							else if (GUI_ROBINSUN_CheckButtonCompet(&DeskInfo, &Pt) == BTN_BACK_COMPET){
    								desk=0;
    								compet=0;
    								compet_loop=0;
    							}
    						}
    					}
    				}
   				}
   			}
  		}//end while(desk)
	}//end while(1)
}
