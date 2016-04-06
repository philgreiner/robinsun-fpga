#ifndef VIP_TEXT_H_
#define VIP_TEXT_H_

#include "alt_video_display.h"
#include "../fonts/fonts.h"


int vid_print_string_alpha(int horiz_offset, int vert_offset, int color, int background_color, struct abc_font_struct font[], alt_video_display * display, char string[]);
int vid_print_char_alpha (int horiz_offset, int vert_offset, int color, char character, int background_color, struct abc_font_struct font[], alt_video_display * display);

#endif /*VIP_TEXT_H_*/
