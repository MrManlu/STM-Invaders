#include "stm32f7xx_hal.h"
#include "gfx.h"
#include "src/gwin/gwin_class.h"
#include "stdlib.h"
#include "string.h"
#include "invaders.h"

#ifdef INVADERS_SHOW_SPLASH
GTimer                invadersSplashSlide;
int invadersSplashCurrentY =-INVADERS_SPLASH_HEIGHT;
gdispImage            invadersSplashImage;
#endif

#ifdef INVADERS_SHOW_SPLASH
void invadersSplashSlider(void* arg)
{
  (void)arg;
  gdispImageOpenFile(&invadersSplashImage, "splash.bmp");
  gdispImageDraw(&invadersSplashImage, gdispGetWidth()/2-INVADERS_SPLASH_WIDTH/2, invadersSplashCurrentY++, INVADERS_SPLASH_WIDTH, INVADERS_SPLASH_HEIGHT, 0, 0);
  gdispImageClose(&invadersSplashImage);
  if(invadersSplashCurrentY>gdispGetHeight()/4-INVADERS_SPLASH_HEIGHT/2)
  {
    gtimerStop(&invadersSplashSlide);
    invadersSplashCurrentY=-INVADERS_SPLASH_HEIGHT;
  }
}

void invadersSplash()
{
  gdispClear(Black);
  gtimerStart(&invadersSplashSlide, invadersSplashSlider, 0, TRUE, 10);
}
#endif
