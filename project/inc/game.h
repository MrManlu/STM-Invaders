#ifndef _GAME_H_
#define _GAME_H_
#include "invaders.h"

#define GAME_INV_MAP_W 7 //Rows of enemies
#define GAME_INV_MAP_H 5  //Columns of enemies

#define GAME_INV_W  24
#define GAME_INV_H  16
#define GAME_INV_EXP_W  24
#define GAME_INV_EXP_H  20
#define GAME_INV_SBTW 10		//Space between Aliens
#define GAME_INV_STEP 10
#define GAME_INV_STEP_TIME 10

#define GAME_BULLET_H 10
#define GAME_BULLET_STEP 10
#define GAME_BULLET_STEP_TIME 50

#define GAME_PLAYER_W 26
#define GAME_PLAYER_H 16

#define GAME_SCORE_TEXT_H 22

enum
{
	TYPE_BULLET,
 	TYPE_ENEMY,
 	TYPE_BOMB,
	TYPE_PLAYER,
};

typedef struct Entity{
  bool_t active;
  int type;
  int x, y;
  int old_x, old_y;
  //int thinkTime;
  char *sprite[2];
  gdispImage gdi;
  //void (*action)(void);
  //void (*draw)(void);
  //int pick_x, pick_y,pick_w,pick_h;
  //int sprite_type;
} Entity;

void initPlayer(void);
void drawPlayer(void);
void playerRutine(void);
void invadersGame(UI_State * game_state, uint8_t * orient);
void initGame(uint8_t * orient);
void invaderStep(void* arg);
void shotBullet(uint16_t pos_x);
bool_t collision(void);
void bulletStep(void* arg);
void initScore(void);
void killInvader(uint16_t pos_x, uint16_t pos_y);
static int uitoa(unsigned int value, char * buf, int max);

#endif  /* _MENU_H_ */
