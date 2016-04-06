#ifndef _ALT_VIDEO_DISPLAY_H_
#define _ALT_VIDEO_DISPLAY_H_


#define SCREEN_WIDTH	240
#define SCREEN_HEIGHT	320

typedef struct{
    int color_depth;
    int width;
    int height;
    int bytes_per_pixel;
    int interlace;
} alt_video_display;


#endif /*_ALT_VIDEO_DISPLAY_H_*/
