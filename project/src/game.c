#include "stm32f7xx_hal.h"
#include "gfx.h"
#include "src/gwin/gwin_class.h"
#include "stdlib.h"
#include "string.h"
#include "invaders.h"
#include "game.h"

Entity enemy[GAME_INV_MAP_W][GAME_INV_MAP_H];
Entity bullet;
Entity explosion;
Entity player;

const char * enemiesSprites0[GAME_INV_MAP_H]={"en00.bmp","en10.bmp","en20.bmp"};
const char * enemiesSprites1[GAME_INV_MAP_H]={"en01.bmp","en11.bmp","en21.bmp"};
const char playerSprite[] = "player.bmp";

bool_t fTime = TRUE;
bool_t sp0 = TRUE;
bool_t rightD = TRUE;
bool_t act_y = FALSE;
bool_t gameOver = FALSE;
bool_t youWin = FALSE;
int abs_y = 0;
int abs_xL = 0;
int abs_xR = 0;
uint16_t screenW = 0;
uint16_t screenH = 0;
uint16_t enemyNum = 0;
uint16_t score = 0;
uint16_t invaderStepTime = GAME_INV_STEP_TIME;

static GEventMouse ev;
GTimer  invaderTimer;
GTimer  bulletTimer;

void invadersGame(UI_State * game_state, uint8_t * orient)
{
  if(fTime)
  {
    gdispClear(Black);
    initGame(orient);
    initPlayer();
    fTime = FALSE;
  }

  playerRutine();

  if(gameOver)
  {
    *game_state = GAME_OVER;
    fTime = TRUE;
  }
}

void initPlayer(void)
{
  player.active = TRUE;
  player.type = TYPE_PLAYER;
  player.x = screenW/2-GAME_PLAYER_W/2;
  player.y = screenH-GAME_PLAYER_H;

  drawPlayer();
}

void drawPlayer(void)
{
  gdispFillArea(player.x+12, player.y, GAME_PLAYER_W-24, 2, Green);
  gdispFillArea(player.x+10, player.y+2, GAME_PLAYER_W-20, 4, Green);
  gdispFillArea(player.x+2, player.y+6, GAME_PLAYER_W-4, 2, Green);
  gdispFillArea(player.x, player.y+8, GAME_PLAYER_W, 8, Green);
}

void playerRutine(void)
{
  ginputGetMouseStatus(0, &ev);
  if(ev.buttons & GINPUT_MOUSE_BTN_LEFT)
  {
    ginputGetMouseStatus(0, &ev);
    if(ev.y > screenH/2)
    {
      gdispFillArea(player.x, player.y, GAME_PLAYER_W, GAME_PLAYER_H, Black);
      if(ev.x<screenW/2)
        player.x=(player.x<0)?screenW-GAME_PLAYER_W:--player.x;
      else
        player.x=(player.x>screenW-GAME_PLAYER_W)?0:++player.x;
      drawPlayer();
    }
    else
    {
      shotBullet(player.x+GAME_PLAYER_W/2);
    }
  }

}

void initGame(uint8_t * orient)
{
  gameOver = FALSE;
  rightD = TRUE;
  enemyNum = 0;
  explosion.active = FALSE;
  screenW = gdispGetWidth();
  screenH = gdispGetHeight();

  int i,j;
  for(j=GAME_INV_MAP_H-1;j>=0;j--)
    for(i=0;i<GAME_INV_MAP_W;i++)
    {
      enemyNum++;
      enemy[i][j].active = TRUE;
      enemy[i][j].type = TYPE_ENEMY;
      enemy[i][j].sprite[0] = enemiesSprites0[(GAME_INV_MAP_H-1-j)/2];
      enemy[i][j].sprite[1] = enemiesSprites1[(GAME_INV_MAP_H-1-j)/2];
      enemy[i][j].x = (GAME_INV_W + GAME_INV_SBTW)*i;
      enemy[i][j].y = (GAME_INV_H + GAME_INV_SBTW)*j+GAME_SCORE_TEXT_H+1;

      gdispImageOpenFile(&(enemy[i][j].gdi), enemy[i][j].sprite[0]);
      gdispImageDraw(&(enemy[i][j].gdi), enemy[i][j].x, enemy[i][j].y, GAME_INV_W, GAME_INV_H, 0, 0);
      gdispImageClose(&(enemy[i][j].gdi));
      gfxSleepMilliseconds (50);
    }
  sp0 = !sp0;
  initScore();
  invaderStepTime = enemyNum * GAME_INV_STEP_TIME;
  gtimerStart(&invaderTimer, invaderStep, 0, TRUE, invaderStepTime);

}

void invaderStep(void* arg)
{
  (void)arg;
  int i,j;
  abs_y=0;
  abs_xL=screenW;
  abs_xR=0;
  enemyNum = 0;

  if(explosion.active)
  {
    gdispFillArea(explosion.x, explosion.y, GAME_INV_EXP_W, GAME_INV_EXP_H, Black);
    explosion.active = FALSE;
  }

  for(i=0;i<GAME_INV_MAP_W;i++)
    for(j=0;j<GAME_INV_MAP_H;j++)
      if(enemy[i][j].active)
      {
        enemyNum++;
        if(rightD)
          gdispFillArea(enemy[i][j].x,enemy[i][j].y, GAME_INV_STEP, GAME_INV_H, Black);
        else
          gdispFillArea(enemy[i][j].x+GAME_INV_W-GAME_INV_STEP,enemy[i][j].y, GAME_INV_STEP, GAME_INV_H, Black);

        if(act_y)
        {
          gdispFillArea(enemy[i][j].x,enemy[i][j].y, GAME_INV_W, GAME_INV_STEP, Black);
          enemy[i][j].y+=GAME_INV_STEP;
          //gfxSleepMilliseconds (100);
        }
        enemy[i][j].x=(rightD)?(enemy[i][j].x+GAME_INV_STEP):(enemy[i][j].x-GAME_INV_STEP);
        abs_y = (enemy[i][j].y>(abs_y-GAME_INV_H))?(enemy[i][j].y+GAME_INV_H):abs_y;
        abs_xL = (enemy[i][j].x<abs_xL)?enemy[i][j].x:abs_xL;
        abs_xR = (enemy[i][j].x>(abs_xR-GAME_INV_W))?(enemy[i][j].x+GAME_INV_W):abs_xR;

        gdispImageOpenFile(&(enemy[i][j].gdi), (sp0)?enemy[i][j].sprite[0]:enemy[i][j].sprite[1]);
        gdispImageDraw(&(enemy[i][j].gdi), enemy[i][j].x, enemy[i][j].y, GAME_INV_W, GAME_INV_H, 0, 0);
        gdispImageClose(&(enemy[i][j].gdi));
      }

  if(act_y)
    act_y = FALSE;

  if(abs_xL < GAME_INV_STEP || abs_xR > (screenW-GAME_INV_STEP))
  {
    act_y = TRUE;
    rightD = !rightD;
  }

  if(enemyNum<=0)
  {
    gameOver = TRUE;
    youWin = TRUE;
  }

  if((abs_y+GAME_INV_STEP)>=screenH)
  {
    gameOver = TRUE;
    youWin = FALSE;
    gtimerStop(&invaderTimer);
  }

  sp0 = !sp0;
}

void shotBullet(uint16_t pos_x)
{
  if(!bullet.active)
  {
    bullet.active = TRUE;
    bullet.x = pos_x;
    bullet.y = screenH-GAME_BULLET_H-GAME_PLAYER_H;
    gtimerStart(&bulletTimer, bulletStep, 0, TRUE, GAME_BULLET_STEP_TIME);
  }
}

void bulletStep(void* arg)
{
  (void)arg;
  bullet.y = (bullet.y<GAME_SCORE_TEXT_H+1)?GAME_SCORE_TEXT_H+1:bullet.y;
  gdispDrawLine(bullet.x, bullet.y, bullet.x, bullet.y+GAME_BULLET_H, Black);
  if(bullet.y>GAME_SCORE_TEXT_H+1 && !collision() && !gameOver)
  {
    bullet.y = ((bullet.y-GAME_BULLET_STEP)<(GAME_SCORE_TEXT_H+1))?(GAME_SCORE_TEXT_H+1):(bullet.y-GAME_BULLET_STEP);
    gdispDrawLine(bullet.x, bullet.y, bullet.x, bullet.y+GAME_BULLET_H, White);
  }
  else
  {
    gtimerStop(&bulletTimer);
    bullet.active = FALSE;
  }
}

bool_t collision(void)
{
  bool_t itsCollision = FALSE;
  int i,j;
  for(i=0;i<GAME_INV_MAP_W;i++)
    for(j=0;j<GAME_INV_MAP_H;j++)
      if(enemy[i][j].active)
      {
        if(enemy[i][j].x+GAME_INV_W >= bullet.x && \
          enemy[i][j].x <= bullet.x && \
          enemy[i][j].y+GAME_INV_H >= bullet.y && \
          enemy[i][j].y <= bullet.y)
          {
            itsCollision = TRUE;
            killInvader(i,j);
            break;
          }
      }
  return itsCollision;
}

void initScore(void)
{
  score = 0;
  gdispDrawLine(0, GAME_SCORE_TEXT_H, screenW, GAME_SCORE_TEXT_H, White);

  gdispFillStringBox(0, 0, 60, GAME_SCORE_TEXT_H, "Score:", gdispOpenFont("DejaVuSans16_aa"), White, Black, justifyLeft);
  gdispFillStringBox(60, 0, 50, GAME_SCORE_TEXT_H, "0", gdispOpenFont("DejaVuSans16_aa"), White, Black, justifyLeft);

}

void refreshScore(uint16_t _score)
{
  score += _score;
  char pps_str[6];
  uitoa(score, pps_str, sizeof(pps_str));
  gdispFillStringBox(60, 0, 50, GAME_SCORE_TEXT_H, pps_str, gdispOpenFont("DejaVuSans16_aa"), White, Black, justifyRight);
}

void killInvader(uint16_t pos_x, uint16_t pos_y)
{
  enemy[pos_x][pos_y].active = FALSE;
  refreshScore(50);
  explosion.x = enemy[pos_x][pos_y].x;
  explosion.y = enemy[pos_x][pos_y].y-(GAME_INV_EXP_H-GAME_INV_H);

  gdispImageOpenFile(&(enemy[pos_x][pos_y].gdi), "exp.bmp");
  gdispImageDraw(&(enemy[pos_x][pos_y].gdi), \
  explosion.x, explosion.y, GAME_INV_EXP_W, GAME_INV_EXP_H, 0, 0);
  gdispImageClose(&(enemy[pos_x][pos_y]).gdi);

  invaderStepTime = enemyNum*GAME_INV_STEP_TIME;
  gtimerStop(&invaderTimer);
  gtimerStart(&invaderTimer, invaderStep, 0, TRUE, invaderStepTime);

  explosion.active = TRUE;
}

static int uitoa(unsigned int value, char * buf, int max) {
  int n = 0;
  int i = 0;
  int tmp = 0;

  if (!buf) return -3;
  if (2 > max) return -4;
  i=1;
  tmp = value;
  if (0 > tmp) {
    tmp *= -1;
    i++;
  }
  for (;;) {
    tmp /= 10;
    if (0 >= tmp) break;
    i++;
  }
  if (i >= max) {
    buf[0] = '?';
    buf[1] = 0x0;
    return 2;
  }
  n = i;
  tmp = value;
  if (0 > tmp) {
    tmp *= -1;
  }
  buf[i--] = 0x0;
  for (;;) {
    buf[i--] = (tmp % 10) + '0';
    tmp /= 10;
    if (0 >= tmp) break;
  }
  if (-1 != i) {
    buf[i--] = '-';
  }
  return n;
}
