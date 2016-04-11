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

// Buttons for the move test menu
typedef enum{
    BTN_MOVE_FORWARD = 0,
    BTN_MOVE_BACKWARD,
    BTN_MOVE_LEFT,
    BTN_MOVE_RIGHT,
    BTN_MOVE_STOP,
    BTN_MOVE_BACK,
    BTN_NUM_MOVE,

    BTN_NONE_MOVE
}BUTTON_MOVE_ID;

// Buttons for the blocks test menu
typedef enum{
    BTN_BLOCKS_OPEN = 0,
    BTN_BLOCKS_CLOSE,
    BTN_BLOCKS_STOP,
    BTN_BLOCKS_BACK,
    BTN_NUM_BLOCKS,

    BTN_NONE_BLOCKS
}BUTTON_BLOCKS_ID;

// Buttons for the fish test menu
typedef enum{
    BTN_FISH_LIFT = 0,
    BTN_FISH_DROP,
    BTN_FISH_STOP,
    BTN_FISH_BACK,
    BTN_NUM_FISH,

    BTN_NONE_FISH
}BUTTON_FISH_ID;

// Buttons for the cabins test menu
typedef enum{
    BTN_CABINS_GO = 0,
    BTN_CABINS_STOP,
    BTN_CABINS_BACK,
    BTN_NUM_CABINS,

    BTN_NONE_CABINS
}BUTTON_CABINS_ID;

// Buttons for the parasol test menu
typedef enum{
    BTN_PARASOL_TRIGGER = 0,
    BTN_PARASOL_STOP,
    BTN_PARASOL_BACK,
    BTN_NUM_PARASOL,

    BTN_NONE_PARASOL
}BUTTON_PARASOL_ID;

// Definition of a button with a rectangle and a color that define it
typedef struct{
    RECT rc;
    int color;
}BUTTON_INFO;

// Definition of a structure referring to all buttons
typedef struct{
    RECT rcDisp;
    BUTTON_INFO buttons[BTN_NUM];
    BUTTON_INFO buttons_tests[BTN_NUM_TESTS];
    BUTTON_INFO buttons_move[BTN_NUM_MOVE];
    BUTTON_INFO buttons_blocks[BTN_NUM_BLOCKS];
    BUTTON_INFO buttons_fish[BTN_NUM_FISH];
    BUTTON_INFO buttons_cabins[BTN_NUM_CABINS];
    BUTTON_INFO buttons_parasol[BTN_NUM_PARASOL];
    BUTTON_INFO buttons_competitionchoice1[10];
    BUTTON_INFO buttons_competitionchoice2[6];
    BUTTON_INFO buttons_competitionsummary[8];
}DESK_INFO;

// Checks if we touched a button
int GUI_ROBINSUN_CheckButton(DESK_INFO *pDeskInfo, POINT *pt, BUTTON_INFO *button, int num, int none){
	int ButtonId = none;
	int i;

	for(i=0;i<num && (ButtonId == none);i++){
		if (IsPtInRect(pt, &button[i].rc))
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

void GUI_ROBINSUN_Init(alt_video_display *pDisplay, DESK_INFO *pDeskInfo, BUTTON_INFO *button, int num){
	int i;
	RECT rc;
	const int BoxH = 40;
	const int DistBorder = 2;
	const int DistW = 25;
	int GapH;

	// Space to put between the buttons in order to be equally spaced
	GapH = (pDisplay->height - ((num)*BoxH))/(num+1);

	rc.top = DistBorder + GapH;
	rc.bottom = rc.top + BoxH;
	rc.left = DistW;
	rc.right = pDisplay->width - DistW;

	// Desk border
	RectSet(&pDeskInfo->rcDisp, DistBorder, pDisplay->width-DistBorder, DistBorder, pDisplay->height-DistBorder);

	// Initialization of the rectangles for the buttons
	for(i=0;i<num;i++){
		RectCopy(&button[i].rc, &rc);
		RectOffset(&rc, 0, BoxH+GapH);
	}
}

void GUI_ROBINSUN_Draw(alt_video_display *pDisplay, DESK_INFO *pDeskInfo, BUTTON_INFO *button, int num, char *text1, char *text2, char *text3, char *text4, char *text5, char *text6){
    int i;
    RECT rc;

    // draw border
    RectCopy(&rc, &pDeskInfo->rcDisp);
    vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_NOT_FILL, pDisplay);

    // draw buttons
    for(i=0;i<num;i++){
        RectCopy(&rc, &button[i].rc);
        if (i == 0){
        	vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, BLACK_24, DO_FILL, pDisplay);
            vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
            vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, text1);
        }
        else if (i == 1){
        	vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, BLACK_24, DO_FILL, pDisplay);
        	vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
        	vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, text2);
        }
        else if (i == 2){
        	vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, BLACK_24, DO_FILL, pDisplay);
        	vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
        	vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, text3);
        }
        else if (i == 3){
        	vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, BLACK_24, DO_FILL, pDisplay);
        	vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
        	vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, text4);
        }
        else if (i == 4){
        	vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, BLACK_24, DO_FILL, pDisplay);
        	vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
        	vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, text5);
        }
        else if (i == 5){
        	vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, BLACK_24, DO_FILL, pDisplay);
        	vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
        	vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, text6);
        }
        else
        {

        }
    }
}

void GUI_ROBINSUN_CompetitionChoice1_Init(alt_video_display *pDisplay, DESK_INFO *pDeskInfo){
	int i;
	RECT rc;
	const int BoxH = 40;
	const int DistBorder = 2;
	const int DistW = 10;
	int GapH, BoxW;

	// Space to put between the buttons in order to be equally spaced
	GapH = (pDisplay->height - (6*BoxH) - 12 - 4)/(3+1);
	BoxW = (pDisplay->width - 2*DistW - 5)/2;

	// Desk border
	RectSet(&pDeskInfo->rcDisp, DistBorder, pDisplay->width-DistBorder, DistBorder, pDisplay->height-DistBorder);

	rc.top = DistBorder + GapH;
	rc.bottom = rc.top + BoxH;
	rc.left = DistW;
	rc.right = rc.left + BoxW;

	// Initialization of the rectangles for the buttons
	for(i=0;i<10;i++){
		if (i==0 || i==3){
			RectCopy(&pDeskInfo->buttons_competitionchoice1[i].rc, &rc);
			RectOffset(&rc, 0, BoxH+5);
		}
		else if(i==1 || i==4 || i==6 || i==8){
			RectCopy(&pDeskInfo->buttons_competitionchoice1[i].rc, &rc);
			RectOffset(&rc, BoxW+5, 0);
		}
		else if(i==5){
			RectCopy(&pDeskInfo->buttons_competitionchoice1[i].rc, &rc);
			RectOffset(&rc, -BoxW-5, BoxH+2);
		}
		else{
			RectCopy(&pDeskInfo->buttons_competitionchoice1[i].rc, &rc);
			RectOffset(&rc, -BoxW-5, BoxH+GapH);
		}
	}
}

void GUI_ROBINSUN_CompetitionChoice1_Draw(alt_video_display *pDisplay, DESK_INFO *pDeskInfo){
	int i;
	RECT rc;

	// draw border
	RectCopy(&rc, &pDeskInfo->rcDisp);
	vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_NOT_FILL, pDisplay);

	// draw buttons
	for(i=0;i<10;i++){
		RectCopy(&rc, &pDeskInfo->buttons_competitionchoice1[i].rc);
		if (i == 0){
			vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, BLACK_24, DO_FILL, pDisplay);
			vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
			vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "Team");
		}
		else if (i == 1){
			vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, GREEN_24, DO_FILL, pDisplay);
		}
		else if (i == 2){
			vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, PURPLE_24, DO_FILL, pDisplay);
		}
		else if (i == 3){
			vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, BLACK_24, DO_FILL, pDisplay);
			vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
			vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "Strategy");
		}
		else if (i == 4){
			vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, BLACK_24, DO_FILL, pDisplay);
			vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
			vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "1");
		}
		else if (i == 5){
			vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, BLACK_24, DO_FILL, pDisplay);
			vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
			vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "2");
		}
		else if (i == 6){
			vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, BLACK_24, DO_FILL, pDisplay);
			vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
			vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "3");
		}
		else if (i == 7){
			vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, BLACK_24, DO_FILL, pDisplay);
			vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
			vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "4");
		}
		else if (i == 8){
			vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, BLACK_24, DO_FILL, pDisplay);
			vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
			vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "Back");
		}
		else if (i == 9){
			vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, BLACK_24, DO_FILL, pDisplay);
			vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
			vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "Continue");
		}
		else
		{

		}
	}
}

void GUI_ROBINSUN_CompetitionChoice2_Init(alt_video_display *pDisplay, DESK_INFO *pDeskInfo){
	int i;
	RECT rc;
	const int BoxH = 40;
	const int DistBorder = 2;
	const int DistW = 10;
	int GapH, BoxW;

	// Space to put between the buttons in order to be equally spaced
	GapH = (pDisplay->height - (6*BoxH) - 12 - 4)/(3+1);
	BoxW = (pDisplay->width - 2*DistW - 5)/2;

	// Desk border
	RectSet(&pDeskInfo->rcDisp, DistBorder, pDisplay->width-DistBorder, DistBorder, pDisplay->height-DistBorder);

	rc.top = DistBorder + GapH;
	rc.bottom = rc.top + BoxH;
	rc.left = DistW;
	rc.right = rc.left + 2*BoxW + 5;

	// Initialization of the rectangles for the buttons
	for(i=0;i<6;i++){
		if (i==0){
			RectCopy(&pDeskInfo->buttons_competitionchoice2[i].rc, &rc);
			rc.right = rc.left + BoxW;
			RectOffset(&rc, 0, BoxH+5);
		}
		else if(i==1 || i==4){
			RectCopy(&pDeskInfo->buttons_competitionchoice2[i].rc, &rc);
			RectOffset(&rc, BoxW+5, 0);
		}
		else if(i==2){
			RectCopy(&pDeskInfo->buttons_competitionchoice2[i].rc, &rc);
			RectOffset(&rc, -BoxW-5, BoxH+GapH);
		}
		else{
			RectCopy(&pDeskInfo->buttons_competitionchoice2[i].rc, &rc);
			RectOffset(&rc, 0, 3*BoxH + 7 + GapH);
		}
	}
}

void GUI_ROBINSUN_CompetitionChoice2_Draw(alt_video_display *pDisplay, DESK_INFO *pDeskInfo){
	int i;
	RECT rc;

	// draw border
	RectCopy(&rc, &pDeskInfo->rcDisp);
	vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_NOT_FILL, pDisplay);

	// draw buttons
	for(i=0;i<6;i++){
		RectCopy(&rc, &pDeskInfo->buttons_competitionchoice2[i].rc);
		if (i == 0){
			vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, BLACK_24, DO_FILL, pDisplay);
			vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
			vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "Nb of opponents");
		}
		else if (i == 1){
			vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, BLACK_24, DO_FILL, pDisplay);
			vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
			vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "1");
		}
		else if (i == 2){
			vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, BLACK_24, DO_FILL, pDisplay);
			vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
			vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "2");
		}
		else if (i == 4){
			vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, BLACK_24, DO_FILL, pDisplay);
			vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
			vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "Back");
		}
		else if (i == 5){
			vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, BLACK_24, DO_FILL, pDisplay);
			vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
			vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "Continue");
		}
		else
		{

		}
	}
}

void GUI_ROBINSUN_CompetitionSummary_Init(alt_video_display *pDisplay, DESK_INFO *pDeskInfo){
	int i;
	RECT rc;
	const int BoxH = 30;
	const int DistBorder = 2;
	const int DistW = 10;
	int GapH, BoxW;

	// Space to put between the buttons in order to be equally spaced
	GapH = (pDisplay->height - (8*BoxH) - 6 - 4)/(5+1);
	BoxW = (pDisplay->width - 2*DistW - 5)/2;

	// Desk border
	RectSet(&pDeskInfo->rcDisp, DistBorder, pDisplay->width-DistBorder, DistBorder, pDisplay->height-DistBorder);

	rc.top = DistBorder + GapH;
	rc.bottom = rc.top + BoxH;
	rc.left = DistW + BoxW + 5;
	rc.right = rc.left + BoxW;

	// Initialization of the rectangles for the buttons
	for(i=0;i<8;i++){
		if (i==0){
			RectCopy(&pDeskInfo->buttons_competitionsummary[i].rc, &rc);
			RectOffset(&rc, -BoxW-5, BoxH+GapH);
		}
		else if(i==1 || i==3){
			RectCopy(&pDeskInfo->buttons_competitionsummary[i].rc, &rc);
			RectOffset(&rc, 0, BoxH+2);
		}
		else if(i==2){
			RectCopy(&pDeskInfo->buttons_competitionsummary[i].rc, &rc);
			RectOffset(&rc, 0, BoxH+GapH);
		}
		else if(i==4 || i==6){
			RectCopy(&pDeskInfo->buttons_competitionsummary[i].rc, &rc);
			rc.right = rc.right + 5 + BoxW;
			RectOffset(&rc, 0, BoxH+GapH);
		}
		else{
			RectCopy(&pDeskInfo->buttons_competitionsummary[i].rc, &rc);
			rc.right = rc.right - 5 - BoxW;
			RectOffset(&rc, 0, BoxH+2);
		}
	}
}

void GUI_ROBINSUN_CompetitionSummary_Draw(alt_video_display *pDisplay, DESK_INFO *pDeskInfo, int color, int strategy, int opponent){
	int i;
	RECT rc;

	// draw border
	RectCopy(&rc, &pDeskInfo->rcDisp);
	vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_NOT_FILL, pDisplay);

	// draw buttons
	for(i=0;i<8;i++){
		RectCopy(&rc, &pDeskInfo->buttons_competitionsummary[i].rc);
		if (i == 0){
			vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, BLACK_24, DO_FILL, pDisplay);
			vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
			vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-26)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "Back");
		}
		else if (i == 1){
			vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, BLACK_24, DO_FILL, pDisplay);
			vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
			vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-26)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "Team");
		}
		else if (i == 2){
			if(color==1)
				vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, GREEN_24, DO_FILL, pDisplay);
			else
				vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, PURPLE_24, DO_FILL, pDisplay);
		}
		else if (i == 3){
			vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, BLACK_24, DO_FILL, pDisplay);
			vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
			vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-26)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "Strategy");
		}
		else if (i == 4){
			vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, BLACK_24, DO_FILL, pDisplay);
			vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
			if(strategy==1)
				vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-26)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "1");
			else if(strategy==2)
				vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-26)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "2");
			else if(strategy==3)
				vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-26)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "3");
			else
				vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-26)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "4");

		}
		else if (i == 5){
			vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, BLACK_24, DO_FILL, pDisplay);
			vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
			vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-26)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "Nb of opponents");
		}
		else if (i == 6){
			vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, BLACK_24, DO_FILL, pDisplay);
			vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
			if(opponent==1)
				vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-26)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "1");
			else
				vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-26)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "2");
		}
		else{
			vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, BLACK_24, DO_FILL, pDisplay);
			vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
			vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-26)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "Send to ROBINSUN");
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
	int tests_or_compet=1, tests_or_compet_loop=1;
	int compet2, compet2_loop, compet3, compet3_loop;
	int test=1;
	int color=0, strategy=0, opponent=0;

	int check_button_menu=0;
	int check_button_tests=0, check_button_compet=0, check_button_compet2=0, check_button_compet3=0;
	int check_button_move=0, check_button_blocks=0, check_button_fish=0, check_button_cabins=0, check_button_parasol=0;

	int * signal = (int*) SIGNAL_OUT_BASE;
	*signal = 0;

	POINT Pt;
	RECT rc;
	RECT rcTouchTest;
	RECT rcTouchMove, rcTouchBlocks, rcTouchFish, rcTouchCabins, rcTouchParasol, rcTouchTestBack;
	RECT rcTouchMoveForward, rcTouchMoveBackward, rcTouchMoveLeft, rcTouchMoveRight, rcTouchMoveStop, rcTouchMoveBack;
	RECT rcTouchBlocksOpen, rcTouchBlocksClose, rcTouchBlocksStop, rcTouchBlocksBack;
	RECT rcTouchFishLift, rcTouchFishDrop, rcTouchFishStop, rcTouchFishBack;
	RECT rcTouchCabinsGo, rcTouchCabinsStop;
	RECT rcTouchParasolTrigger, rcTouchParasolStop;
	RECT rcTouchCompet;

    // clean screen
    vid_clean_screen(pDisplay, BLACK_24);

    // Welcome message for Robinsun Team's Member
    GUI_ROBINSUN_ShowWelcome(pDisplay);
    usleep(2*1000*1000);
    vid_clean_screen(pDisplay, BLACK_24);

    while(1){
    	vid_clean_screen(pDisplay, BLACK_24);
    	GUI_ROBINSUN_Init(pDisplay, &DeskInfo, DeskInfo.buttons, BTN_NUM);
    	GUI_ROBINSUN_Draw(pDisplay, &DeskInfo, DeskInfo.buttons, BTN_NUM, "Tests", "Competition", "", "", "", "");

    	RectCopy(&rcTouchTest, &DeskInfo.buttons[BTN_TEST].rc);
    	RectCopy(&rcTouchCompet, &DeskInfo.buttons[BTN_COMPET].rc);

    	desk=1;
    	Touch_EmptyFifo(pTouch);

    	while(desk){
    		// touch
    		if (Touch_GetXY(pTouch, &X, &Y)){
    			//printf("x=%d, y=%d\r\n", X,Y);
    			PtSet(&Pt, X, Y);

    			check_button_menu = GUI_ROBINSUN_CheckButton(&DeskInfo, &Pt,DeskInfo.buttons,BTN_NUM,BTN_NONE);
    			if (check_button_menu == BTN_TEST){

    				tests_or_compet=1;
    				while(tests_or_compet){
    					vid_clean_screen(pDisplay, BLACK_24);
    					usleep(1000*1000);
    					Touch_EmptyFifo(pTouch);

    					GUI_ROBINSUN_Init(pDisplay, &DeskInfo, DeskInfo.buttons_tests, BTN_NUM_TESTS);
    					GUI_ROBINSUN_Draw(pDisplay, &DeskInfo, DeskInfo.buttons_tests, BTN_NUM_TESTS, "Move", "Blocks", "Fish", "Cabins", "Parasol", "Back");

    					tests_or_compet_loop=1;
    					while(tests_or_compet_loop){

    						RectCopy(&rcTouchMove, &DeskInfo.buttons_tests[BTN_MOVE].rc);
    						RectCopy(&rcTouchBlocks, &DeskInfo.buttons_tests[BTN_BLOCKS].rc);
    						RectCopy(&rcTouchFish, &DeskInfo.buttons_tests[BTN_FISH].rc);
    						RectCopy(&rcTouchCabins, &DeskInfo.buttons_tests[BTN_CABINS].rc);
    						RectCopy(&rcTouchParasol, &DeskInfo.buttons_tests[BTN_PARASOL].rc);
    						RectCopy(&rcTouchTestBack, &DeskInfo.buttons_tests[BTN_BACK_TESTS].rc);

    						//touch
    						if (Touch_GetXY(pTouch, &X, &Y)){
    							PtSet(&Pt, X, Y);
    							check_button_tests = GUI_ROBINSUN_CheckButton(&DeskInfo, &Pt,DeskInfo.buttons_tests,BTN_NUM_TESTS,BTN_NONE_TESTS);

    							if(check_button_tests == BTN_MOVE){
    								test=1;

    								vid_clean_screen(pDisplay, BLACK_24);
    								usleep(1000*1000);

    								GUI_ROBINSUN_Init(pDisplay, &DeskInfo, DeskInfo.buttons_move, BTN_NUM_MOVE);
    								GUI_ROBINSUN_Draw(pDisplay, &DeskInfo, DeskInfo.buttons_move, BTN_NUM_MOVE, "Go forward", "Go backward", "Turn to the left", "Turn to the right", "STOP", "Back");

    								RectCopy(&rcTouchMoveForward, &DeskInfo.buttons_move[BTN_MOVE_FORWARD].rc);
    								RectCopy(&rcTouchMoveBackward, &DeskInfo.buttons_move[BTN_MOVE_BACKWARD].rc);
    								RectCopy(&rcTouchMoveLeft, &DeskInfo.buttons_move[BTN_MOVE_LEFT].rc);
    								RectCopy(&rcTouchMoveRight, &DeskInfo.buttons_move[BTN_MOVE_RIGHT].rc);
    								RectCopy(&rcTouchMoveStop, &DeskInfo.buttons_move[BTN_MOVE_STOP].rc);
    								RectCopy(&rcTouchMoveBack, &DeskInfo.buttons_move[BTN_MOVE_BACK].rc);

    								Touch_EmptyFifo(pTouch);

    								while(test){
    									// touch
    									if (Touch_GetXY(pTouch, &X, &Y)){
    										PtSet(&Pt, X, Y);
    										check_button_move = GUI_ROBINSUN_CheckButton(&DeskInfo, &Pt,DeskInfo.buttons_move,BTN_NUM_MOVE,BTN_NONE_MOVE);
    										//printf("signal vaut: %d\n",*signal);
    										if (check_button_move == BTN_MOVE_FORWARD){

    											vid_draw_box (rcTouchMoveForward.left, rcTouchMoveForward.top, rcTouchMoveForward.right, rcTouchMoveForward.bottom, BLACK_24, DO_FILL, pDisplay);
    											vid_draw_box (rcTouchMoveForward.left, rcTouchMoveForward.top, rcTouchMoveForward.right, rcTouchMoveForward.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
    											vid_print_string_alpha(rcTouchMoveForward.left+8, rcTouchMoveForward.top+(RectHeight(&rcTouchMoveForward)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "Go forward");

    											if(*signal == 1){
    												*signal = 0;
    											}
    											else{
    												*signal = 1;
    											}

    											Touch_EmptyFifo(pTouch);
    										}
    										else if (check_button_move == BTN_MOVE_BACKWARD){

    											vid_draw_box (rcTouchMoveBackward.left, rcTouchMoveBackward.top, rcTouchMoveBackward.right, rcTouchMoveBackward.bottom, BLACK_24, DO_FILL, pDisplay);
    											vid_draw_box (rcTouchMoveBackward.left, rcTouchMoveBackward.top, rcTouchMoveBackward.right, rcTouchMoveBackward.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
    											vid_print_string_alpha(rcTouchMoveBackward.left+8, rcTouchMoveBackward.top+(RectHeight(&rcTouchMoveBackward)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "Go backward");

    											if(*signal == 2){
    												*signal = 0;
    											}
    											else{
    												*signal = 2;
    											}

    											Touch_EmptyFifo(pTouch);
    										}
    										else if (check_button_move == BTN_MOVE_LEFT){

    											vid_draw_box (rcTouchMoveLeft.left, rcTouchMoveLeft.top, rcTouchMoveLeft.right, rcTouchMoveLeft.bottom, BLACK_24, DO_FILL, pDisplay);
    											vid_draw_box (rcTouchMoveLeft.left, rcTouchMoveLeft.top, rcTouchMoveLeft.right, rcTouchMoveLeft.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
    											vid_print_string_alpha(rcTouchMoveLeft.left+8, rcTouchMoveLeft.top+(RectHeight(&rcTouchMoveLeft)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "Turn to the left");

    											if(*signal == 3){
    												*signal = 0;
    											}
    											else{
    												*signal = 3;
    											}

    											Touch_EmptyFifo(pTouch);
    										}
    										else if (check_button_move == BTN_MOVE_RIGHT){

    											vid_draw_box (rcTouchMoveRight.left, rcTouchMoveRight.top, rcTouchMoveRight.right, rcTouchMoveRight.bottom, BLACK_24, DO_FILL, pDisplay);
    											vid_draw_box (rcTouchMoveRight.left, rcTouchMoveRight.top, rcTouchMoveRight.right, rcTouchMoveRight.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
    											vid_print_string_alpha(rcTouchMoveRight.left+8, rcTouchMoveRight.top+(RectHeight(&rcTouchMoveRight)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "Turn to the right");

    											if(*signal == 4){
    												*signal = 0;
    											}
    											else{
    												*signal = 4;
    											}

    											Touch_EmptyFifo(pTouch);
    										}
    										else if (check_button_move == BTN_MOVE_STOP){

    											vid_draw_box (rcTouchMoveStop.left, rcTouchMoveStop.top, rcTouchMoveStop.right, rcTouchMoveStop.bottom, BLACK_24, DO_FILL, pDisplay);
    											vid_draw_box (rcTouchMoveStop.left, rcTouchMoveStop.top, rcTouchMoveStop.right, rcTouchMoveStop.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
    											vid_print_string_alpha(rcTouchMoveStop.left+8, rcTouchMoveStop.top+(RectHeight(&rcTouchMoveStop)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "STOP");

    											*signal = 0;

    											Touch_EmptyFifo(pTouch);
    										}
    										else if (check_button_move == BTN_MOVE_BACK){

    											*signal=0;
    											tests_or_compet_loop=0;
    											test=0;

    											Touch_EmptyFifo(pTouch);
    										}
    										else{
    											Touch_EmptyFifo(pTouch);
    										}
    										//printf("signal vaut: %d\n",*signal);
    									}
    								}
    							}//end if(check_button_tests == BTN_MOVE)
    							else if(check_button_tests == BTN_BLOCKS){
    								test=1;

    								vid_clean_screen(pDisplay, BLACK_24);
    								usleep(1000*1000);

    								GUI_ROBINSUN_Init(pDisplay, &DeskInfo, DeskInfo.buttons_blocks, BTN_NUM_BLOCKS);
    								GUI_ROBINSUN_Draw(pDisplay, &DeskInfo, DeskInfo.buttons_blocks, BTN_NUM_BLOCKS, "Open", "Close", "STOP", "Back", "", "");

    								RectCopy(&rcTouchBlocksOpen, &DeskInfo.buttons_blocks[BTN_BLOCKS_OPEN].rc);
    								RectCopy(&rcTouchBlocksClose, &DeskInfo.buttons_blocks[BTN_BLOCKS_CLOSE].rc);
    								RectCopy(&rcTouchBlocksStop, &DeskInfo.buttons_blocks[BTN_BLOCKS_STOP].rc);
    								RectCopy(&rcTouchBlocksBack, &DeskInfo.buttons_blocks[BTN_BLOCKS_BACK].rc);

    								Touch_EmptyFifo(pTouch);

    								while(test){
    									// touch
    									if (Touch_GetXY(pTouch, &X, &Y)){
    										PtSet(&Pt, X, Y);
    										check_button_blocks = GUI_ROBINSUN_CheckButton(&DeskInfo, &Pt,DeskInfo.buttons_blocks,BTN_NUM_BLOCKS,BTN_NONE_BLOCKS);
    										//printf("signal vaut: %d\n",*signal);
    										if (check_button_blocks == BTN_BLOCKS_OPEN){

    											vid_draw_box (rcTouchBlocksOpen.left, rcTouchBlocksOpen.top, rcTouchBlocksOpen.right, rcTouchBlocksOpen.bottom, BLACK_24, DO_FILL, pDisplay);
    											vid_draw_box (rcTouchBlocksOpen.left, rcTouchBlocksOpen.top, rcTouchBlocksOpen.right, rcTouchBlocksOpen.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
    											vid_print_string_alpha(rcTouchBlocksOpen.left+8, rcTouchBlocksOpen.top+(RectHeight(&rcTouchBlocksOpen)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "Open");

    											if(*signal == 11){
    												*signal = 0;
    											}
    											else{
    												*signal = 11;
    											}

    											Touch_EmptyFifo(pTouch);
    										}
    										else if (check_button_blocks == BTN_BLOCKS_CLOSE){

    											vid_draw_box (rcTouchBlocksClose.left, rcTouchBlocksClose.top, rcTouchBlocksClose.right, rcTouchBlocksClose.bottom, BLACK_24, DO_FILL, pDisplay);
    											vid_draw_box (rcTouchBlocksClose.left, rcTouchBlocksClose.top, rcTouchBlocksClose.right, rcTouchBlocksClose.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
    											vid_print_string_alpha(rcTouchBlocksClose.left+8, rcTouchBlocksClose.top+(RectHeight(&rcTouchBlocksClose)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "Close");

    											if(*signal == 12){
    												*signal = 0;
    											}
    											else{
    												*signal = 12;
    											}

    											Touch_EmptyFifo(pTouch);
    										}
    										else if (check_button_blocks == BTN_BLOCKS_STOP){

    											vid_draw_box (rcTouchBlocksStop.left, rcTouchBlocksStop.top, rcTouchBlocksStop.right, rcTouchBlocksStop.bottom, BLACK_24, DO_FILL, pDisplay);
    											vid_draw_box (rcTouchBlocksStop.left, rcTouchBlocksStop.top, rcTouchBlocksStop.right, rcTouchBlocksStop.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
    											vid_print_string_alpha(rcTouchBlocksStop.left+8, rcTouchBlocksStop.top+(RectHeight(&rcTouchBlocksStop)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "STOP");

    											*signal = 0;

    											Touch_EmptyFifo(pTouch);
    										}
    										else if (check_button_blocks == BTN_BLOCKS_BACK){

    											*signal=0;
    											tests_or_compet_loop=0;
    											test=0;

    											Touch_EmptyFifo(pTouch);
    										}
    										else{
    											Touch_EmptyFifo(pTouch);
    										}
    										//printf("signal vaut: %d\n",*signal);
    									}
    								}
    							}//end if(check_button_tests == BTN_BLOCKS)
    							else if(check_button_tests == BTN_FISH){
    								test=1;

    								vid_clean_screen(pDisplay, BLACK_24);
    								usleep(1000*1000);

    								GUI_ROBINSUN_Init(pDisplay, &DeskInfo, DeskInfo.buttons_fish, BTN_NUM_FISH);
    								GUI_ROBINSUN_Draw(pDisplay, &DeskInfo, DeskInfo.buttons_fish, BTN_NUM_FISH, "Lift", "Drop", "STOP", "Back", "", "");

    								RectCopy(&rcTouchFishLift, &DeskInfo.buttons_fish[BTN_FISH_LIFT].rc);
    								RectCopy(&rcTouchFishDrop, &DeskInfo.buttons_fish[BTN_FISH_DROP].rc);
    								RectCopy(&rcTouchFishStop, &DeskInfo.buttons_fish[BTN_FISH_STOP].rc);
    								RectCopy(&rcTouchFishBack, &DeskInfo.buttons_fish[BTN_FISH_BACK].rc);

    								Touch_EmptyFifo(pTouch);

    								while(test){
    									// touch
    									if (Touch_GetXY(pTouch, &X, &Y)){
    										PtSet(&Pt, X, Y);
    										check_button_fish = GUI_ROBINSUN_CheckButton(&DeskInfo, &Pt,DeskInfo.buttons_fish,BTN_NUM_FISH,BTN_NONE_FISH);
    										//printf("signal vaut: %d\n",*signal);
    										if (check_button_fish == BTN_FISH_LIFT){

    											vid_draw_box (rcTouchFishLift.left, rcTouchFishLift.top, rcTouchFishLift.right, rcTouchFishLift.bottom, BLACK_24, DO_FILL, pDisplay);
    											vid_draw_box (rcTouchFishLift.left, rcTouchFishLift.top, rcTouchFishLift.right, rcTouchFishLift.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
    											vid_print_string_alpha(rcTouchFishLift.left+8, rcTouchFishLift.top+(RectHeight(&rcTouchFishLift)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "Lift");

    											if(*signal == 21){
    												*signal = 0;
    											}
    											else{
    												*signal = 21;
    											}

    											Touch_EmptyFifo(pTouch);
    										}
    										else if (check_button_fish == BTN_FISH_DROP){

    											vid_draw_box (rcTouchFishDrop.left, rcTouchFishDrop.top, rcTouchFishDrop.right, rcTouchFishDrop.bottom, BLACK_24, DO_FILL, pDisplay);
    											vid_draw_box (rcTouchFishDrop.left, rcTouchFishDrop.top, rcTouchFishDrop.right, rcTouchFishDrop.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
    											vid_print_string_alpha(rcTouchFishDrop.left+8, rcTouchFishDrop.top+(RectHeight(&rcTouchFishDrop)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "Drop");

    											if(*signal == 22){
    												*signal = 0;
    											}
    											else{
    												*signal = 22;
    											}

												Touch_EmptyFifo(pTouch);
    										}
    										else if (check_button_fish == BTN_FISH_STOP){

    											vid_draw_box (rcTouchFishStop.left, rcTouchFishStop.top, rcTouchFishStop.right, rcTouchFishStop.bottom, BLACK_24, DO_FILL, pDisplay);
    											vid_draw_box (rcTouchFishStop.left, rcTouchFishStop.top, rcTouchFishStop.right, rcTouchFishStop.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
    											vid_print_string_alpha(rcTouchFishStop.left+8, rcTouchFishStop.top+(RectHeight(&rcTouchFishStop)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "STOP");

    											*signal = 0;

    											Touch_EmptyFifo(pTouch);

    										}
    										else if (check_button_fish == BTN_FISH_BACK){

    											*signal=0;
    											tests_or_compet_loop=0;
    											test=0;

    											Touch_EmptyFifo(pTouch);
    										}
    										else{
    											Touch_EmptyFifo(pTouch);
    										}
    										//printf("signal vaut: %d\n",*signal);
    									}
    								}
    							}//end if(check_button_tests == BTN_FISH)
    							else if(check_button_tests == BTN_CABINS){
    								test=1;

    								vid_clean_screen(pDisplay, BLACK_24);
    								usleep(1000*1000);

    								GUI_ROBINSUN_Init(pDisplay, &DeskInfo, DeskInfo.buttons_cabins, BTN_NUM_CABINS);
    								GUI_ROBINSUN_Draw(pDisplay, &DeskInfo, DeskInfo.buttons_cabins, BTN_NUM_CABINS, "Go to the cabins", "STOP", "Back", "", "", "");

    								RectCopy(&rcTouchCabinsGo, &DeskInfo.buttons_cabins[BTN_CABINS_GO].rc);
    								RectCopy(&rcTouchCabinsStop, &DeskInfo.buttons_cabins[BTN_CABINS_STOP].rc);

    								Touch_EmptyFifo(pTouch);

    								while(test){
    									// touch
    									if (Touch_GetXY(pTouch, &X, &Y)){
    										PtSet(&Pt, X, Y);
    										check_button_cabins = GUI_ROBINSUN_CheckButton(&DeskInfo, &Pt,DeskInfo.buttons_cabins,BTN_NUM_CABINS,BTN_NONE_CABINS);
    										//printf("signal vaut: %d\n",*signal);
    										if (check_button_cabins == BTN_CABINS_GO){

    											vid_draw_box (rcTouchCabinsGo.left, rcTouchCabinsGo.top, rcTouchCabinsGo.right, rcTouchCabinsGo.bottom, BLACK_24, DO_FILL, pDisplay);
    											vid_draw_box (rcTouchCabinsGo.left, rcTouchCabinsGo.top, rcTouchCabinsGo.right, rcTouchCabinsGo.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
    											vid_print_string_alpha(rcTouchCabinsGo.left+8, rcTouchCabinsGo.top+(RectHeight(&rcTouchCabinsGo)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "Go to the cabins");

    											if(*signal == 31){
    												*signal = 0;
    											}
    											else{
    												*signal = 31;
    											}

    											Touch_EmptyFifo(pTouch);
    										}
    										else if (check_button_cabins == BTN_CABINS_STOP){

    											vid_draw_box (rcTouchCabinsStop.left, rcTouchCabinsStop.top, rcTouchCabinsStop.right, rcTouchCabinsStop.bottom, BLACK_24, DO_FILL, pDisplay);
    											vid_draw_box (rcTouchCabinsStop.left, rcTouchCabinsStop.top, rcTouchCabinsStop.right, rcTouchCabinsStop.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
    											vid_print_string_alpha(rcTouchCabinsStop.left+8, rcTouchCabinsStop.top+(RectHeight(&rcTouchCabinsStop)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "STOP");

    											*signal = 0;

    											Touch_EmptyFifo(pTouch);

    										}
    										else if (check_button_cabins == BTN_CABINS_BACK){

    											*signal=0;
    											tests_or_compet_loop=0;
    											test=0;

    											Touch_EmptyFifo(pTouch);
    										}
    										else{
    											Touch_EmptyFifo(pTouch);
    										}
    										//printf("signal vaut: %d\n",*signal);
    									}
    								}
    							}//end if(check_button_tests == BTN_CABINS)
    							else if (check_button_tests == BTN_PARASOL){
    								test=1;

    								//vid_print_string_alpha(rcTouchPurple.left+8, rcTouchPurple.top+(RectHeight(&rcTouchPurple)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "Purple OK");
    								vid_clean_screen(pDisplay, BLACK_24);
    								usleep(1000*1000);

    								GUI_ROBINSUN_Init(pDisplay, &DeskInfo, DeskInfo.buttons_parasol, BTN_NUM_PARASOL);
    								GUI_ROBINSUN_Draw(pDisplay, &DeskInfo, DeskInfo.buttons_parasol, BTN_NUM_PARASOL, "Trigger", "STOP", "Back", "", "", "");

    								RectCopy(&rcTouchParasolTrigger, &DeskInfo.buttons_parasol[BTN_PARASOL_TRIGGER].rc);
    								RectCopy(&rcTouchParasolStop, &DeskInfo.buttons_parasol[BTN_PARASOL_STOP].rc);

    								Touch_EmptyFifo(pTouch);

    								while(test){
    									// touch
    									if (Touch_GetXY(pTouch, &X, &Y)){
    										PtSet(&Pt, X, Y);
    										check_button_parasol = GUI_ROBINSUN_CheckButton(&DeskInfo, &Pt,DeskInfo.buttons_parasol,BTN_NUM_PARASOL,BTN_NONE_PARASOL);
    										//printf("signal vaut: %d\n",*signal);
    										if (check_button_parasol == BTN_PARASOL_TRIGGER){

    											vid_draw_box (rcTouchParasolTrigger.left, rcTouchParasolTrigger.top, rcTouchParasolTrigger.right, rcTouchParasolTrigger.bottom, BLACK_24, DO_FILL, pDisplay);
    											vid_draw_box (rcTouchParasolTrigger.left, rcTouchParasolTrigger.top, rcTouchParasolTrigger.right, rcTouchParasolTrigger.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
    											vid_print_string_alpha(rcTouchParasolTrigger.left+8, rcTouchParasolTrigger.top+(RectHeight(&rcTouchParasolTrigger)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "Trigger");

    											if(*signal == 41){
    												*signal = 0;
    											}
    											else{
    												*signal = 41;
    											}

    											Touch_EmptyFifo(pTouch);
    										}
    										if (check_button_parasol == BTN_PARASOL_STOP){

    											vid_draw_box (rcTouchParasolStop.left, rcTouchParasolStop.top, rcTouchParasolStop.right, rcTouchParasolStop.bottom, BLACK_24, DO_FILL, pDisplay);
    											vid_draw_box (rcTouchParasolStop.left, rcTouchParasolStop.top, rcTouchParasolStop.right, rcTouchParasolStop.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
    											vid_print_string_alpha(rcTouchParasolStop.left+8, rcTouchParasolStop.top+(RectHeight(&rcTouchParasolStop)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "STOP");

    											*signal=0;

    											Touch_EmptyFifo(pTouch);
    										}
    										else if (check_button_parasol == BTN_PARASOL_BACK){
    											*signal=0;
    											tests_or_compet_loop=0;
    											test=0;

    											Touch_EmptyFifo(pTouch);
    										}
    										else{
    											Touch_EmptyFifo(pTouch);
    										}
    										//printf("signal vaut: %d\n",*signal);
    									}
    								}
    							}

    							else if (check_button_tests == BTN_BACK_TESTS){
    								desk=0;
    								tests_or_compet=0;
    								tests_or_compet_loop=0;
    							}
    							else{

    							}
    						}
    					}//end while(tests_loop)
    				}//end while(tests)
    			}//end if (check_button_menu == BTN_TEST)
    			else if (check_button_menu == BTN_COMPET){
    				//vid_print_string_alpha(rcTouchCompet.left+8, rcTouchCompet.top+(RectHeight(&rcTouchCompet)-22)/2, BLACK_24, WHITE_24, tahomabold_20, pDisplay, "Competition OK");
    				tests_or_compet=1;

    				while(tests_or_compet){
    					vid_clean_screen(pDisplay, BLACK_24);
    					usleep(1000*1000);
    					Touch_EmptyFifo(pTouch);

    					GUI_ROBINSUN_CompetitionChoice1_Init(pDisplay, &DeskInfo);
    					GUI_ROBINSUN_CompetitionChoice1_Draw(pDisplay, &DeskInfo);

    					color=0;
    					strategy=0;
    					opponent=0;

    					tests_or_compet_loop=1;
    					while(tests_or_compet_loop){
    						//touch
    						if (Touch_GetXY(pTouch, &X, &Y)){
    							PtSet(&Pt, X, Y);
    							check_button_compet = GUI_ROBINSUN_CheckButton(&DeskInfo, &Pt,DeskInfo.buttons_competitionchoice1,10,11);

    							if(check_button_compet == 1 || check_button_compet == 2){
    								color = check_button_compet;

    								usleep(100*1000);
    								Touch_EmptyFifo(pTouch);
    							}
    							else if(check_button_compet == 4 || check_button_compet == 5 || check_button_compet == 6 || check_button_compet == 7){
    								strategy = check_button_compet;

    								usleep(100*1000);
    								Touch_EmptyFifo(pTouch);
    							}
    							else if(check_button_compet == 8){
    								desk=0;
    								tests_or_compet=0;
    								tests_or_compet_loop=0;
    							}
    							else if(check_button_compet == 9){
    								if(color==0){
    									vid_clean_screen(pDisplay, BLACK_24);
    									usleep(1000*1000);
    									Touch_EmptyFifo(pTouch);

    									RectSet(&DeskInfo.rcDisp, 2, pDisplay->width-2, 2, pDisplay->height-2);

    									rc.top = 2 + (pDisplay->height-87)/2;
    									rc.bottom = rc.top + 85;
    									rc.left = 20;
    									rc.right = pDisplay->width-20;

    									vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, BLACK_24, DO_FILL, pDisplay);
    									vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
    									vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-44)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "Please, select");
    									vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-44)+5, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "a team before.");
    									usleep(2*1000*1000);

    									tests_or_compet_loop=0;
    									Touch_EmptyFifo(pTouch);
    								}
    								else if(strategy==0){
    									vid_clean_screen(pDisplay, BLACK_24);
    									usleep(1000*1000);
    									Touch_EmptyFifo(pTouch);

    									RectSet(&DeskInfo.rcDisp, 2, pDisplay->width-2, 2, pDisplay->height-2);

    									rc.top = 2 + (pDisplay->height-87)/2;
    									rc.bottom = rc.top + 85;
    									rc.left = 20;
    									rc.right = pDisplay->width-20;

    									vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, BLACK_24, DO_FILL, pDisplay);
    									vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
    									vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-44)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "Please, select");
    									vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-44)+5, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "a strategy before.");
    									usleep(2*1000*1000);

    									tests_or_compet_loop=0;
    									Touch_EmptyFifo(pTouch);
    								}
    								else{
    									compet2=1;

    									while(compet2){
    										vid_clean_screen(pDisplay, BLACK_24);
    										usleep(1000*1000);
											Touch_EmptyFifo(pTouch);

											GUI_ROBINSUN_CompetitionChoice2_Init(pDisplay, &DeskInfo);
											GUI_ROBINSUN_CompetitionChoice2_Draw(pDisplay, &DeskInfo);

											compet2_loop=1;
											while(compet2_loop){
												//touch
												if (Touch_GetXY(pTouch, &X, &Y)){
													PtSet(&Pt, X, Y);
													check_button_compet2 = GUI_ROBINSUN_CheckButton(&DeskInfo, &Pt,DeskInfo.buttons_competitionchoice2,6,7);

													if(check_button_compet2 == 1){
														opponent = 1;

														usleep(100*1000);
														Touch_EmptyFifo(pTouch);
													}
													else if(check_button_compet2 == 2){
														opponent = 2;

														usleep(100*1000);
														Touch_EmptyFifo(pTouch);
													}
													else if(check_button_compet2 == 4){
														*signal=0;

														tests_or_compet_loop=0;
														compet2=0;
														compet2_loop=0;
													}
													else if(check_button_compet2 == 5){

														if(opponent==0){
															vid_clean_screen(pDisplay, BLACK_24);
															usleep(1000*1000);
															Touch_EmptyFifo(pTouch);

															RectSet(&DeskInfo.rcDisp, 2, pDisplay->width-2, 2, pDisplay->height-2);

															rc.top = 2 + (pDisplay->height-87)/2;
															rc.bottom = rc.top + 85;
															rc.left = 20;
															rc.right = pDisplay->width-20;

															vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, BLACK_24, DO_FILL, pDisplay);
															vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
															vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-44)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "Please, select the");
															vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-44)+5, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "nb of opponents.");
															usleep(2*1000*1000);

															compet2_loop=0;
															Touch_EmptyFifo(pTouch);
														}
														else{
															*signal = 33 + color*10 + strategy + opponent*4;

															printf("signal vaut:%d \n",*signal);
															Touch_EmptyFifo(pTouch);

															compet3=1;

															while(compet3){
																vid_clean_screen(pDisplay, BLACK_24);
																usleep(1000*1000);
																Touch_EmptyFifo(pTouch);

																GUI_ROBINSUN_CompetitionSummary_Init(pDisplay, &DeskInfo);
																GUI_ROBINSUN_CompetitionSummary_Draw(pDisplay, &DeskInfo, color, strategy-3, opponent);

																compet3_loop=1;
																while(compet3_loop){
																	//touch
																	if (Touch_GetXY(pTouch, &X, &Y)){
																		PtSet(&Pt, X, Y);
																		check_button_compet3 = GUI_ROBINSUN_CheckButton(&DeskInfo, &Pt,DeskInfo.buttons_competitionsummary,8,9);

																		if(check_button_compet3 == 0){
																			*signal=0;

																			compet2_loop=0;
																			compet3=0;
																			compet3_loop=0;
																		}
																		else if(check_button_compet3 == 7){
																			vid_clean_screen(pDisplay, BLACK_24);
																			usleep(1000*1000);
																			Touch_EmptyFifo(pTouch);

																			RectSet(&DeskInfo.rcDisp, 2, pDisplay->width-2, 2, pDisplay->height-2);

																			rc.top = 2 + (pDisplay->height-40)/2;
																			rc.bottom = rc.top + 40;
																			rc.left = 80;
																			rc.right = pDisplay->width-80;

																			vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, BLACK_24, DO_FILL, pDisplay);
																			vid_draw_box (rc.left, rc.top, rc.right, rc.bottom, WHITE_24, DO_NOT_FILL, pDisplay);
																			vid_print_string_alpha(rc.left+8, rc.top+(RectHeight(&rc)-22)/2, WHITE_24, BLACK_24, tahomabold_20, pDisplay, "Done");

																			while(1){
																			}
																		}
																	}
																}
															}
														}
													}
												}
											}
    									}
    								}
    							}
    						}
    					}
    				}
   				}
   			}
  		}//end while(desk)
	}//end while(1)
}
