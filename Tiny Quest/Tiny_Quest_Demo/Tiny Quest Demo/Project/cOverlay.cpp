#include "cOverlay.h"

cOverlay::cOverlay()
{
	tiles_w = 6;
	tiles_h = 4;
}
cOverlay::~cOverlay(){}

void cOverlay::SetX(int posx)
{
	x = posx;
}
int cOverlay::GetX()
{
	return x;
}
void cOverlay::SetY(int posy)
{
	y = posy;
}
int cOverlay::GetY()
{
	return y;
}
int cOverlay::GetWidth()
{
	return tiles_w*TILE_SIZE;
}
void cOverlay::SetBarOffsetX(int ox)
{
	bar_offset_x = ox;
}

void cOverlay::Draw(int tex_id)
{	
	int screen_x,screen_y;
	
	screen_x = x + SCENE_Xo;
	screen_y = y + SCENE_Yo;

	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D,tex_id);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f,1.0f);	glVertex3i(screen_x					   ,screen_y					,-2);
		glTexCoord2f(1.0f,1.0f);	glVertex3i(screen_x + tiles_w*TILE_SIZE,screen_y					,-2);
		glTexCoord2f(1.0f,0.0f);	glVertex3i(screen_x + tiles_w*TILE_SIZE,screen_y + tiles_h*TILE_SIZE,-2);
		glTexCoord2f(0.0f,0.0f);	glVertex3i(screen_x					   ,screen_y + tiles_h*TILE_SIZE,-2);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void cOverlay::DrawBars(int max1,int act1,int max2,int act2)
{
	int screen_x,screen_y;
	float factor1,factor2;
	int bar1_length,bar2_length;

	screen_x = x + SCENE_Xo;
	screen_y = y + SCENE_Yo;
	factor1 = ((float)act1/(float)max1);
	factor2 = ((float)act2/(float)max2);
	bar1_length = (tiles_w*TILE_SIZE - 2*TILE_SIZE - TILE_SIZE/2)*factor1;
	bar2_length = (tiles_w*TILE_SIZE - 2*TILE_SIZE - TILE_SIZE/2)*factor2;

	if(factor1 >= 0.5f) glColor3f( (1.0f-factor1)*2, 1.0f, 0.0f); //de verde a amarillo
	else glColor3f(1.0f, factor1*2, 0.0f); //de amarillo a rojo
	glBegin(GL_QUADS);
		glVertex3i(screen_x + bar_offset_x			    ,screen_y + 5*(TILE_SIZE/8),-1);
		glVertex3i(screen_x + bar_offset_x + bar1_length,screen_y + 5*(TILE_SIZE/8),-1);
		glVertex3i(screen_x + bar_offset_x + bar1_length,screen_y + 7*(TILE_SIZE/8),-1);
		glVertex3i(screen_x + bar_offset_x			    ,screen_y + 7*(TILE_SIZE/8),-1);
	glEnd();

	glColor3f(0.2f, 0.5f, 1.0f);
	glBegin(GL_QUADS);
		glVertex3i(screen_x + bar_offset_x			    ,screen_y + 1*(TILE_SIZE/8),-1);
		glVertex3i(screen_x + bar_offset_x + bar2_length,screen_y + 1*(TILE_SIZE/8),-1);
		glVertex3i(screen_x + bar_offset_x + bar2_length,screen_y + 3*(TILE_SIZE/8),-1);
		glVertex3i(screen_x + bar_offset_x			    ,screen_y + 3*(TILE_SIZE/8),-1);
	glEnd();

	glColor4f(1, 1, 1, 1); //devolvemos el color a blanco despues de dibujar las barras (para no afectar a las texturas)
}