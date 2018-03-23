#include "cData.h"

cData::cData(void) {}
cData::~cData(void){}

int cData::GetID(int img)
{
	return textures[img].GetID();
}

void cData::GetSize(int img, int *w, int *h)
{
	textures[img].GetSize(w,h);
}

bool cData::LoadImage(int img, char *filename, int type)
{
	int res;

	res = textures[img].Load(filename,type);
	if(!res) return false;

	return true;
}
bool cData::Load()
{
	int res;

	res = LoadImage(IMG_TILESET,"Textures/tileset.png",GL_RGBA);
	if(!res) return false;
	res = LoadImage(IMG_PLAYER1,"Textures/player1.png",GL_RGBA);
	if(!res) return false;
	res = LoadImage(IMG_SUPERP1,"Textures/superp1.png",GL_RGBA);
	if(!res) return false;
	res = LoadImage(IMG_PLAYER2,"Textures/player2.png",GL_RGBA);
	if(!res) return false;
	res = LoadImage(IMG_SUPERP2,"Textures/superp2.png",GL_RGBA);
	if(!res) return false;
	res = LoadImage(IMG_OVERLAY1,"Textures/overlay1.png",GL_RGBA);
	if(!res) return false;
	res = LoadImage(IMG_OVERLAY2,"Textures/overlay2.png",GL_RGBA);
	if(!res) return false;
	res = LoadImage(IMG_MONSTER1,"Textures/monster1.png",GL_RGBA);
	if(!res) return false;
	res = LoadImage(IMG_MONSTER2,"Textures/monster2.png",GL_RGBA);
	if(!res) return false;
	res = LoadImage(IMG_FIREBALL,"Textures/fireball.png",GL_RGBA);
	if(!res) return false;
	res = LoadImage(IMG_ARROW,"Textures/arrow.png",GL_RGBA);
	if(!res) return false;
	res = LoadImage(IMG_HEART,"Textures/heart.png",GL_RGBA);
	if(!res) return false;
	
	return true;
}