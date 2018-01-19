#ifndef C_GAME_H
#define C_GAME_H

#include "cPlayer1.h"
#include "cPlayer2.h"
#include "cMonster1.h"
#include "cMonster2.h"
#include "cProjectile.h"
#include "cToken.h"
#include "cOverlay.h"
#include "cData.h"
#include "cSound.h"
#include "cShader.h"

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
#define KEY_P1_UP_T  	 0
#define KEY_P1_UP		'w'
#define KEY_P1_DOWN_T    0
#define KEY_P1_DOWN		's'
#define KEY_P1_LEFT_T    0
#define KEY_P1_LEFT		'a'
#define KEY_P1_RIGHT_T   0
#define KEY_P1_RIGHT	'd'
#define KEY_P1_ATTACK_T  0
#define KEY_P1_ATTACK	'q'
#define KEY_P1_SKILL_T   0
#define KEY_P1_SKILL	'e'

#define KEY_P2_UP_T  	1
#define KEY_P2_UP		GLUT_KEY_UP
#define KEY_P2_DOWN_T  	1
#define KEY_P2_DOWN		GLUT_KEY_DOWN
#define KEY_P2_LEFT_T  	1
#define KEY_P2_LEFT		GLUT_KEY_LEFT
#define KEY_P2_RIGHT_T  1
#define KEY_P2_RIGHT	GLUT_KEY_RIGHT
#define KEY_P2_ATTACK_T 0
#define KEY_P2_ATTACK	','
#define KEY_P2_SKILL_T  0
#define KEY_P2_SKILL	'.'

// key alias
#define P1_UP		0
#define P1_DOWN		1
#define P1_LEFT		2
#define P1_RIGHT	3
#define P1_ATTACK	4
#define P1_SKILL	5

#define P2_UP		10
#define P2_DOWN		11
#define P2_LEFT		12
#define P2_RIGHT	13
#define P2_ATTACK	14
#define P2_SKILL	15

class cGame
{
public:
	cGame(void);
	virtual ~cGame(void);

	bool Init(int level);
	bool Loop();
	void Finalize();

	//Input
	void ReadKeyboard(unsigned char key, int x, int y, bool press);
	void ReadSpecialKeyboard(unsigned char key, int x, int y, bool press);
	void ReadMouse(int button, int state, int x, int y);
	//Process
	bool Process();
	void Reshape(int w,int h);
	//Output
	void Render();

private:
	unsigned char keysNormal[256];
	unsigned char keysSpecial[256];
	unsigned char keys[256];
	cRect visible_area;
	int state,level;
	bool p1_attacks,p2_attacks;
	cScene Scene;
	cPlayer1 Player1;
	cPlayer2 Player2;
	cOverlay Overlay1;
	cOverlay Overlay2;
	std::list<cBicho*> enemies;
	std::list<cProjectile*> projectiles;
	std::list<cToken*> tokens;
	cData Data;
	cSound Sound;
	cShader Shader;
	float time;
	bool ia, epilepsia_mode;

	void UpdateCamera(int h1,int h2);
	bool LoadDynamicLayer(int lvl);
};

/*
Debug: clic en barra izquierda derecha para añadir breakpoint, F11 para ejecutar linia a linia
control + alt + Q y en expresion poner un objeto o this para "escuchar" sus variables
*/

#endif
