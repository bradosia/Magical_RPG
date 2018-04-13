#ifndef C_GAME_H
#define C_GAME_H

#include <ctime>
#include <chrono>
#include <unordered_map>
#include <functional>

class cGraphic;

#include "../MR_logic/cCallbackTimer.h"
#include "../MR_logic/cActor.h" // Character, Enemies, Projectiles, Tokens
#include "../MR_logic/cModel.h"
#include "../MR_graphic/cTexture.h"
#include "../MR_logic/cLogicExt.h"

/* front end graphics, sound, key inputs */
#include "../MR_graphic/cActorFront.h"
#include "../MR_graphic/cProjectileFront.h"
#include "../MR_graphic/cTokenFront.h"

#include "../MR_logic/cProjectile.h"
#include "../MR_logic/cToken.h"
#include "../MR_graphic/cOverlay.h"
#include "../MR_logic/cData.h"
#include "../MR_graphic/cSound.h"
#include "../MR_graphic/cShader.h"

class cLogic;

//initial window dimensions
#define GAME_WIDTH	((SCENE_WIDTH-2)*TILE_SIZE + SCENE_Xo*2) //valores compuestos de #define siempre encerrados en parentesis
#define GAME_HEIGHT int((SCENE_WIDTH-2)*TILE_SIZE*1.5f + SCENE_Yo*2)

#define STATE_GAMEOVER    0
#define STATE_RUN         1
#define STATE_PAUSE       2
#define STATE_MAINMENU    3
#define STATE_LEVELCHANGE 4
#define STATE_ENDGAME	  5

#define TOTAL_LEVELS	  2
#define DYNAMIC_FILENAME  "dynamic_layer"

// keys
#define KEY_ACTOR_UNUSED    0
#define KEY_ACTOR_UP        1
#define KEY_ACTOR_DOWN      2
#define KEY_ACTOR_LEFT      3
#define KEY_ACTOR_RIGHT     4
#define KEY_ACTOR_ATTACK    5
#define KEY_ACTOR_SKILL     6

#define KEY_GAME_RENDER     20

class cGraphic {
private:
	std::vector<cActor*> actorList;
	std::vector<cModel*> modelList;
	cActor* actorMain;
	cActorFront* actorFrontPtr;
	cProjectileFront* projectileFrontPtr;
	cTokenFront* tokenFrontPtr;

	//unsigned char keysNormal[256];
	//unsigned char keysSpecial[256];
	//unsigned char keys[256];
	/* maps the key to the press
	 * keyBind[key] = 1
	 */
	int keyPress[1000];
	/* maps the key to the bound action
	 * keyBind[key] = KEY_ACTOR_UP
	 */
	int keyBind[1000];
	/* maps the actor action to the press
	 * keyAction[KEY_ACTOR_UP] = 1
	 */
	int keyAction[1000];
	int keyUpLast;
	int keyDownLast;

	cRect visible_area;
	int state, level;
	bool p1_attacks, nextFrame;
	cScene Scene;
	cOverlay Overlay1;
	//cOverlay Overlay2;
	std::vector<cBicho*> enemies;
	std::vector<cProjectile*> projectiles;
	std::vector<cToken*> tokens;
	cData Data;
	cSound Sound;
	cShader Shader;
	cLogic* logicPtr;
	std::chrono::steady_clock::time_point lastTime;
	std::chrono::microseconds frameTime;
	double fadeTime, limitFPS;
	bool ia, epilepsia_mode;
	std::unordered_map<std::string, std::function<void(cGraphic*)>> CBmap;

	void UpdateCamera(int h1, int h2);
	void UpdateCamera(int h1);
	bool LoadDynamicLayer(int lvl);
public:
	cGraphic(cLogic* logicPtr);
	virtual ~cGraphic(void);

	bool Init(int level);
	bool Loop();
	void Finalize();

	//Input
	void ReadKeyboard(int key, int, int action, int mods);
	void ReadSpecialKeyboard(unsigned char key, int x, int y, bool press);
	void ReadMouse(int button, int state, int x, int y);
	//Process
	bool Process();
	void Reshape(int w, int h);
	//Output
	void Render();

	cActor* getActorMain();
	void on(std::string event, std::function<void(cGraphic*)> CB);
};

/*
 Debug: click on left bar to add breakpoint, F11 to execute line to line
   control + alt + Q and in expression put an object or this to "listen" to its variables
 */

#endif
