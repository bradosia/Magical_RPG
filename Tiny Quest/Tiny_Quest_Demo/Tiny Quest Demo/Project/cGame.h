#pragma once

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

//dimensiones de la window inicial
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

//controls
#define P1_UP		'i'
#define P1_DOWN		'k'
#define P1_LEFT		'j'
#define P1_RIGHT	'l'
#define P1_ATTACK	'z'
#define P1_SKILL	'x'

#define P2_UP		GLUT_KEY_UP
#define P2_DOWN		GLUT_KEY_DOWN
#define P2_LEFT		GLUT_KEY_LEFT
#define P2_RIGHT	GLUT_KEY_RIGHT
#define P2_ATTACK	'*'
#define P2_SKILL	'-'

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