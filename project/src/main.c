#include "gfx.h"
#include "menu.h"
#include "invaders.h"
#include "game.h"

UI_State game_state = GAME_MENU;

int main(void)
{
  gfxInit();
  ginputGetMouse(0);

  uint8_t whichOrientation = 2;
  gdispSetOrientation(orients[whichOrientation]);

  #if INVADERS_SHOW_SPLASH
    invadersSplash();
    gfxSleepMilliseconds(2000);
  #endif

  while (TRUE) {
    switch (game_state) {
      case GAME_PLAY:
        invadersGame(&game_state,&whichOrientation);
        //game_state = GAME_OVER;
        break;

      case GAME_OVER:
        game_state = GAME_MENU;
        break;

      case GAME_MENU:
      default:
        invadersMenu(&game_state,&whichOrientation);
        break;
    }
  }
}
