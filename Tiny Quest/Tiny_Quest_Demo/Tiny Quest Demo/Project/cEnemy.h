#pragma once

#include "cBicho.h"
#include "cPlayer1.h"

class cEnemy: public cBicho
{
public:
	cEnemy();
	virtual ~cEnemy(); //cuando llamemos al destructor de un cEnemy, se ejecutara el destructor de la subclase correspondiente

	void Draw(int tex_id,int tex_w,int tex_h,bool run);
	void DrawRect(int tex_id,float xo,float yo,float xf,float yf);
	void NextFrame(int max);

	int GetFrameTop();
	int GetFrameBottom();
	void SetFrameTexTiles(int ftt);

	void Move();

private:
	int frame_walk_delay,
		frame_attack_delay,
		frame_dying_delay,
		frame_texture_tiles, //el numero en tiles de textura que ocupa el lado de un frame del sprite
		step_length; //TILE_SIZE ha de ser multiplo de step_length
};