#include "stm32f7xx_hal.h"
#include "gfx.h"
#include "src/gwin/gwin_class.h"
#include "stdlib.h"
#include "string.h"
#include "invaders.h"
#include "menu.h"

const char * mainLabels[]={"Start game","Options >>"};
const char * optLabels[]={"Test Screen","Orientation","<< Back"};

gdispImage            invadersSplashImage;
static GListener gl;
static GHandle   ghStart;
static GHandle   ghOrientation;
static GHandle   ghOptions;
static GHandle   ghTestScreen;
//static GHandle   ghTestSound;
static GHandle   ghBackMain;
bool_t mainMenu = TRUE;
bool_t firstTime = TRUE;

static void mainWidgets(void) {
  GWidgetInit	wi[2];
  GWidgetInit *pw = NULL;

  pw=wi;

  for (size_t i = 0; i < 2; i++) {

	   // Apply some default values for GWIN
	    gwinWidgetClearInit(pw);
      pw->g.show = mainMenu;
	    pw->g.width = MENU_BUTTON_WIDTH;
	    pw->g.height = MENU_BUTTON_HEIGHT;
	    pw->g.y = gdispGetHeight()/2+MENU_BUTTON_HEIGHT*i+MENU_BUTTON_SPACER;
	    pw->g.x = gdispGetWidth()/2-pw->g.width/2;
	    pw->text = mainLabels[i];
      pw++;
  }
	ghStart = gwinButtonCreate(0, &wi[0]);
  ghOptions = gwinButtonCreate(0, &wi[1]);
}

static void optWidgets(void) {
  GWidgetInit	wi[3];
  GWidgetInit *pw = NULL;

  pw=wi;

  for (size_t i = 0; i < 3; i++) {
    gwinWidgetClearInit(pw);
    pw->g.show = !mainMenu;
    pw->g.width = MENU_BUTTON_WIDTH;
    pw->g.height = MENU_BUTTON_HEIGHT;
    pw->g.y = gdispGetHeight()/2+MENU_BUTTON_HEIGHT*i+MENU_BUTTON_SPACER;
    pw->g.x = gdispGetWidth()/2-pw->g.width/2;
    pw->text = optLabels[i];
    pw++;
  }
  ghTestScreen = gwinButtonCreate(0, &wi[0]);
  ghOrientation = gwinButtonCreate(0, &wi[1]);
  ghBackMain = gwinButtonCreate(0, &wi[2]);
}

void invadersMenu(UI_State * game_state, uint8_t * orient)
{

  gdispClear(Black);
  gdispImageOpenFile(&invadersSplashImage, "splash.bmp");
  gdispImageDraw(&invadersSplashImage, gdispGetWidth()/2-INVADERS_SPLASH_WIDTH/2, gdispGetHeight()/4-INVADERS_SPLASH_HEIGHT/2, INVADERS_SPLASH_WIDTH, INVADERS_SPLASH_HEIGHT, 0, 0);
  gdispImageClose(&invadersSplashImage);
  gwinSetDefaultFont(gdispOpenFont(MENU_FONT));

  if(firstTime)
  {
    mainWidgets();
    optWidgets();
    firstTime = FALSE;
    geventListenerInit(&gl);
    gwinAttachListener(&gl);
  }

  showMenu();

  GEvent* pe;

  pe = geventEventWait(&gl, TIME_INFINITE);

		switch(pe->type) {
			case GEVENT_GWIN_BUTTON:
				if (((GEventGWinButton*)pe)->gwin == ghStart)
        {
          mainMenu = TRUE;
          hideAll();
          *(game_state) = GAME_PLAY;
          return;
        }
        if (((GEventGWinButton*)pe)->gwin == ghOptions)
        {
          mainMenu = FALSE;
        }
        if (((GEventGWinButton*)pe)->gwin == ghBackMain)
        {
          mainMenu = TRUE;
        }
        if (((GEventGWinButton*)pe)->gwin == ghTestScreen)
        {
          hideAll();
          testScreen();
        }
        if (((GEventGWinButton*)pe)->gwin == ghOrientation)
        {
          if (++*(orient) >= sizeof(orients)/sizeof(orients[0]))
          *orient = 0;
          gdispSetOrientation(orients[*orient]);
          destroyAll();
          mainWidgets();
          optWidgets();
        }
			default:
				break;
		}
}

void testScreen(void)
{
  gdispClear(Red);
  gfxSleepMilliseconds (1000);
  gdispClear(Blue);
  gfxSleepMilliseconds (1000);
  gdispClear(Green);
  gfxSleepMilliseconds (1000);
}

void showMenu(void)
{
  hideAll();
  if(mainMenu)
  {
    gwinShow(ghStart);
    gwinShow(ghOptions);
  }
  else
  {
    gwinShow(ghTestScreen);
    gwinShow(ghOrientation);
    gwinShow(ghBackMain);
  }
}

void hideAll(void)
{
  gwinHide(ghStart);
  gwinHide(ghOptions);
  gwinHide(ghTestScreen);
  gwinHide(ghOrientation);
  gwinHide(ghBackMain);
}

void destroyAll(void)
{
  gwinDestroy(ghStart);
  gwinDestroy(ghOptions);
  gwinDestroy(ghTestScreen);
  gwinDestroy(ghOrientation);
  gwinDestroy(ghBackMain);
}
