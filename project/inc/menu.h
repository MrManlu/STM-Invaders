#ifndef _MENU_H_
#define _MENU_H_

#include "invaders.h"

#define MENU_FONT "DejaVuSans16_aa"

#define MENU_BUTTON_WIDTH   150
#define MENU_BUTTON_HEIGHT   30
#define MENU_BUTTON_SPACER    0

void invadersMenu(UI_State * game_state, uint8_t * orient);
void testScreen(void);
void showMenu(void);
void hideAll(void);
void destroyAll(void);

#endif  /* _MENU_H_ */
