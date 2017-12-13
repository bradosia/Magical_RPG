#pragma once

#include "cTexture.h"

//Image array size
#define NUM_TEX		 12

//Image identifiers
#define IMG_TILESET	 0
#define IMG_PLAYER1	 1
#define IMG_SUPERP1  2
#define IMG_PLAYER2	 3
#define	IMG_SUPERP2  4
#define IMG_OVERLAY1 5
#define IMG_OVERLAY2 6
#define IMG_MONSTER1 7
#define IMG_MONSTER2 8
#define IMG_FIREBALL 9
#define IMG_ARROW	 10
#define IMG_HEART	 11

class cData
{
public:
	cData(void);
	~cData(void);

	int  GetID(int img);
	void GetSize(int img, int *w, int *h);
	bool Load();

private:
	cTexture textures[NUM_TEX];
	bool LoadImage(int img,char *filename,int type = GL_RGBA);
};
