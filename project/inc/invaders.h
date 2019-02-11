#ifndef _INVADERS_H_
#define _INVADERS_H_

//#define ROTATION GDISP_ROTATE_0
//#define ROTATION GDISP_ROTATE_90
#define ROTATION GDISP_ROTATE_180
//#define ROTATION GDISP_ROTATE_270

#define INVADERS_SPLASH_WIDTH 200
#define INVADERS_SPLASH_HEIGHT 84

typedef enum {
  GAME_MENU = 0,
  GAME_PLAY,
  GAME_OVER,
}UI_State;

static const orientation_t	orients[] = { GDISP_ROTATE_0, GDISP_ROTATE_90, GDISP_ROTATE_180, GDISP_ROTATE_270 };

#ifdef INVADERS_SHOW_SPLASH
    void INVADERSShowSplash(void);
    void invadersSplash(void);
#endif

#endif  /* _INVADERS_H_ */
