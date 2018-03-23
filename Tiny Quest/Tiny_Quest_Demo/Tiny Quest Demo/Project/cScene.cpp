#include "cScene.h"

cScene::cScene(void)
{
	seq = 0;
	delay = 0;
	frame_delay = 6;
}

cScene::~cScene(void)
{
}

bool cScene::LoadLevel(int level,int tex_w, int tex_h)
{
	bool res;
	FILE *fd;
	char file[32];
	int i,j,px,py,id,tiles_w;
	char c;
	float coordx_tile, coordy_tile;
	float tex_offset_x,tex_offset_y;

	//limpio la informacion del anterior nivel
	animated_tiles.clear();
	animated_tiles_up.clear();

	tex_offset_x = 1.0f/float(tex_w/TEXTURE_TILE_SIZE);
	tex_offset_y = 1.0f/float(tex_h/TEXTURE_TILE_SIZE);
	tiles_w = (tex_w/TEXTURE_TILE_SIZE);
	res=true;

	if(level<10) sprintf(file,"Levels/%s0%d%s",(char *)LEVEL_FILENAME,level,(char *)FILENAME_EXT);
	else		 sprintf(file,"Levels/%s%d%s",(char *)LEVEL_FILENAME,level,(char *)FILENAME_EXT);

	fd=fopen(file,"r");
	if(fd==NULL) return false;

	id_TerrainFlat=glGenLists(1);
	glNewList(id_TerrainFlat,GL_COMPILE);
		glBegin(GL_QUADS);
	
			for(j=SCENE_HEIGHT-1;j>=0;j--)
			{
				px=SCENE_Xo;
				py=SCENE_Yo+(j*TILE_SIZE);

				for(i=0;i<SCENE_WIDTH;i++)
				{
					fscanf(fd,"%i",&id);

					terrain_flat[(j*SCENE_WIDTH)+i] = id;
					coordx_tile = tex_offset_x*((id-1)%tiles_w); coordy_tile = tex_offset_y*((id-1)/tiles_w);

					if(id)
					{
						//coords de textura que ha de coger el vertice; donde se dibujara el vertice;
						glTexCoord2f(coordx_tile             ,coordy_tile+tex_offset_y);	glVertex3i(px          ,py          ,(SCENE_HEIGHT*TILE_SIZE)+2);
						glTexCoord2f(coordx_tile+tex_offset_x,coordy_tile+tex_offset_y);	glVertex3i(px+TILE_SIZE,py          ,(SCENE_HEIGHT*TILE_SIZE)+2);
						glTexCoord2f(coordx_tile+tex_offset_x,coordy_tile			  );	glVertex3i(px+TILE_SIZE,py+TILE_SIZE,(SCENE_HEIGHT*TILE_SIZE)+2);
						glTexCoord2f(coordx_tile             ,coordy_tile			  );	glVertex3i(px          ,py+TILE_SIZE,(SCENE_HEIGHT*TILE_SIZE)+2);
					}
					px+=TILE_SIZE;
				
					fscanf(fd,"%c",&c); //pass comma or enter
				}
			}

		glEnd();
	glEndList();

	id_TerrainSolid=glGenLists(1);
	glNewList(id_TerrainSolid,GL_COMPILE);
		glBegin(GL_QUADS);
	
			for(j=SCENE_HEIGHT-1;j>=0;j--)
			{
				px=SCENE_Xo;
				py=SCENE_Yo+(j*TILE_SIZE);

				for(i=0;i<SCENE_WIDTH;i++)
				{
					fscanf(fd,"%i",&id);

					terrain_solid[(j*SCENE_WIDTH)+i] = id;
					coordx_tile = tex_offset_x*((id-1)%tiles_w); coordy_tile = tex_offset_y*((id-1)/tiles_w);

					if(id)
					{
						//coords de textura que ha de coger el vertice; donde se dibujara el vertice;
						glTexCoord2f(coordx_tile             ,coordy_tile+tex_offset_y);	glVertex3i(px          ,py          ,(SCENE_HEIGHT*TILE_SIZE)+2);
						glTexCoord2f(coordx_tile+tex_offset_x,coordy_tile+tex_offset_y);	glVertex3i(px+TILE_SIZE,py          ,(SCENE_HEIGHT*TILE_SIZE)+2);
						glTexCoord2f(coordx_tile+tex_offset_x,coordy_tile			  );	glVertex3i(px+TILE_SIZE,py+TILE_SIZE,(SCENE_HEIGHT*TILE_SIZE)+2);
						glTexCoord2f(coordx_tile             ,coordy_tile			  );	glVertex3i(px          ,py+TILE_SIZE,(SCENE_HEIGHT*TILE_SIZE)+2);
					}
					px+=TILE_SIZE;
				
					fscanf(fd,"%c",&c); //pass comma or enter
				}
			}

		glEnd();
	glEndList();

	id_SolidAlpha=glGenLists(1);
	glNewList(id_SolidAlpha,GL_COMPILE);
		glBegin(GL_QUADS);
	
			for(j=SCENE_HEIGHT-1;j>=0;j--)
			{
				px=SCENE_Xo;
				py=SCENE_Yo+(j*TILE_SIZE);

				for(i=0;i<SCENE_WIDTH;i++)
				{
					fscanf(fd,"%i",&id);

					solid_alpha[(j*SCENE_WIDTH)+i] = id;
					coordx_tile = tex_offset_x*((id-1)%tiles_w); coordy_tile = tex_offset_y*((id-1)/tiles_w);

					if(id)
					{
						//coords de textura que ha de coger el vertice; donde se dibujara el vertice;
						glTexCoord2f(coordx_tile             ,coordy_tile+tex_offset_y);	glVertex3i(px          ,py          ,(SCENE_HEIGHT*TILE_SIZE)+2);
						glTexCoord2f(coordx_tile+tex_offset_x,coordy_tile+tex_offset_y);	glVertex3i(px+TILE_SIZE,py          ,(SCENE_HEIGHT*TILE_SIZE)+2);
						glTexCoord2f(coordx_tile+tex_offset_x,coordy_tile			  );	glVertex3i(px+TILE_SIZE,py+TILE_SIZE,(SCENE_HEIGHT*TILE_SIZE)+2);
						glTexCoord2f(coordx_tile             ,coordy_tile			  );	glVertex3i(px          ,py+TILE_SIZE,(SCENE_HEIGHT*TILE_SIZE)+2);
					}
					px+=TILE_SIZE;
				
					fscanf(fd,"%c",&c); //pass comma or enter
				}
			}

		glEnd();
	glEndList();

	//en este bucle almaceno las tiles animadas del nivel inferior, si las hubiera
	for(j=SCENE_HEIGHT-1;j>=0;j--)
	{
		px=SCENE_Xo;
		py=SCENE_Yo+(j*TILE_SIZE);

		for(i=0;i<SCENE_WIDTH;i++)
		{
			fscanf(fd,"%i",&id);

			if(id)
			{
				cTile newtile;
				newtile.SetX(px);
				newtile.SetY(py);
				newtile.SetTexCoordX( tex_offset_x*((id-1)%tiles_w) );
				newtile.SetTexCoordY( tex_offset_y*((id-1)/tiles_w) );
				animated_tiles.push_back(newtile);
			}
			px+=TILE_SIZE;
				
			fscanf(fd,"%c",&c); //pass comma or enter
		}
	}

	id_Bridges=glGenLists(1);
	glNewList(id_Bridges,GL_COMPILE);
		glBegin(GL_QUADS);
	
			for(j=SCENE_HEIGHT-1;j>=0;j--)
			{
				px=SCENE_Xo;
				py=SCENE_Yo+(j*TILE_SIZE);

				for(i=0;i<SCENE_WIDTH;i++)
				{
					fscanf(fd,"%i",&id);

					bridges[(j*SCENE_WIDTH)+i] = id;
					coordx_tile = tex_offset_x*((id-1)%tiles_w); coordy_tile = tex_offset_y*((id-1)/tiles_w);

					if(id)
					{
						//coords de textura que ha de coger el vertice; donde se dibujara el vertice;
						glTexCoord2f(coordx_tile             ,coordy_tile+tex_offset_y);	glVertex3i(px          ,py          ,(SCENE_HEIGHT*TILE_SIZE)+1);
						glTexCoord2f(coordx_tile+tex_offset_x,coordy_tile+tex_offset_y);	glVertex3i(px+TILE_SIZE,py          ,(SCENE_HEIGHT*TILE_SIZE)+1);
						glTexCoord2f(coordx_tile+tex_offset_x,coordy_tile			  );	glVertex3i(px+TILE_SIZE,py+TILE_SIZE,(SCENE_HEIGHT*TILE_SIZE)+1);
						glTexCoord2f(coordx_tile             ,coordy_tile			  );	glVertex3i(px          ,py+TILE_SIZE,(SCENE_HEIGHT*TILE_SIZE)+1);
					}
					px+=TILE_SIZE;
				
					fscanf(fd,"%c",&c); //pass comma or enter
				}
			}

		glEnd();
	glEndList();

	id_AlwaysDown=glGenLists(1);
	glNewList(id_AlwaysDown,GL_COMPILE);
		glBegin(GL_QUADS);
	
			for(j=SCENE_HEIGHT-1;j>=0;j--)
			{
				px=SCENE_Xo;
				py=SCENE_Yo+(j*TILE_SIZE);

				for(i=0;i<SCENE_WIDTH;i++)
				{
					fscanf(fd,"%i",&id);

					always_down[(j*SCENE_WIDTH)+i] = id;
					coordx_tile = tex_offset_x*((id-1)%tiles_w); coordy_tile = tex_offset_y*((id-1)/tiles_w);

					if(id)
					{
						//coords de textura que ha de coger el vertice; donde se dibujara el vertice;
						glTexCoord2f(coordx_tile             ,coordy_tile+tex_offset_y);	glVertex3i(px          ,py          ,(SCENE_HEIGHT*TILE_SIZE)+2);
						glTexCoord2f(coordx_tile+tex_offset_x,coordy_tile+tex_offset_y);	glVertex3i(px+TILE_SIZE,py          ,(SCENE_HEIGHT*TILE_SIZE)+2);
						glTexCoord2f(coordx_tile+tex_offset_x,coordy_tile			  );	glVertex3i(px+TILE_SIZE,py+TILE_SIZE,(SCENE_HEIGHT*TILE_SIZE)+2);
						glTexCoord2f(coordx_tile             ,coordy_tile			  );	glVertex3i(px          ,py+TILE_SIZE,(SCENE_HEIGHT*TILE_SIZE)+2);
					}
					px+=TILE_SIZE;
				
					fscanf(fd,"%c",&c); //pass comma or enter
				}
			}

		glEnd();
	glEndList();

	id_ZtileUp=glGenLists(1);
	glNewList(id_ZtileUp,GL_COMPILE);
		glBegin(GL_QUADS);
	
			for(j=SCENE_HEIGHT-1;j>=0;j--)
			{
				px=SCENE_Xo;
				py=SCENE_Yo+(j*TILE_SIZE);

				for(i=0;i<SCENE_WIDTH;i++)
				{
					fscanf(fd,"%i",&id);

					z_tileup[(j*SCENE_WIDTH)+i] = id;
					coordx_tile = tex_offset_x*((id-1)%tiles_w); coordy_tile = tex_offset_y*((id-1)/tiles_w);

					if(id)
					{
						//coords de textura que ha de coger el vertice; donde se dibujara el vertice;
						glTexCoord2f(coordx_tile             ,coordy_tile+tex_offset_y);	glVertex3i(px          ,py          ,py+TILE_SIZE);
						glTexCoord2f(coordx_tile+tex_offset_x,coordy_tile+tex_offset_y);	glVertex3i(px+TILE_SIZE,py          ,py+TILE_SIZE);
						glTexCoord2f(coordx_tile+tex_offset_x,coordy_tile			  );	glVertex3i(px+TILE_SIZE,py+TILE_SIZE,py+TILE_SIZE);
						glTexCoord2f(coordx_tile             ,coordy_tile			  );	glVertex3i(px          ,py+TILE_SIZE,py+TILE_SIZE);
					}
					px+=TILE_SIZE;
				
					fscanf(fd,"%c",&c); //pass comma or enter
				}
			}

		glEnd();
	glEndList();

	id_ZtileDown=glGenLists(1);
	glNewList(id_ZtileDown,GL_COMPILE);
		glBegin(GL_QUADS);
	
			for(j=SCENE_HEIGHT-1;j>=0;j--)
			{
				px=SCENE_Xo;
				py=SCENE_Yo+(j*TILE_SIZE);

				for(i=0;i<SCENE_WIDTH;i++)
				{
					fscanf(fd,"%i",&id);

					z_tiledown[(j*SCENE_WIDTH)+i] = id;
					coordx_tile = tex_offset_x*((id-1)%tiles_w); coordy_tile = tex_offset_y*((id-1)/tiles_w);

					if(id)
					{
						//coords de textura que ha de coger el vertice; donde se dibujara el vertice;
						glTexCoord2f(coordx_tile             ,coordy_tile+tex_offset_y);	glVertex3i(px          ,py          ,py);
						glTexCoord2f(coordx_tile+tex_offset_x,coordy_tile+tex_offset_y);	glVertex3i(px+TILE_SIZE,py          ,py);
						glTexCoord2f(coordx_tile+tex_offset_x,coordy_tile			  );	glVertex3i(px+TILE_SIZE,py+TILE_SIZE,py);
						glTexCoord2f(coordx_tile             ,coordy_tile			  );	glVertex3i(px          ,py+TILE_SIZE,py);
					}
					px+=TILE_SIZE;
				
					fscanf(fd,"%c",&c); //pass comma or enter
				}
			}

		glEnd();
	glEndList();

	id_AlwaysUp=glGenLists(1);
	glNewList(id_AlwaysUp,GL_COMPILE);
		glBegin(GL_QUADS);
	
			//always_up
			for(j=SCENE_HEIGHT-1;j>=0;j--)
			{
				px=SCENE_Xo;
				py=SCENE_Yo+(j*TILE_SIZE);

				for(i=0;i<SCENE_WIDTH;i++)
				{
					fscanf(fd,"%i",&id);

					always_up[(j*SCENE_WIDTH)+i] = id;
					coordx_tile = tex_offset_x*((id-1)%tiles_w); coordy_tile = tex_offset_y*((id-1)/tiles_w);

					if(id)
					{
						//coords de textura que ha de coger el vertice; donde se dibujara el vertice;
						glTexCoord2f(coordx_tile             ,coordy_tile+tex_offset_y);	glVertex3i(px          ,py          ,0);
						glTexCoord2f(coordx_tile+tex_offset_x,coordy_tile+tex_offset_y);	glVertex3i(px+TILE_SIZE,py          ,0);
						glTexCoord2f(coordx_tile+tex_offset_x,coordy_tile			  );	glVertex3i(px+TILE_SIZE,py+TILE_SIZE,0);
						glTexCoord2f(coordx_tile             ,coordy_tile			  );	glVertex3i(px          ,py+TILE_SIZE,0);
					}
					px+=TILE_SIZE;
				
					fscanf(fd,"%c",&c); //pass comma or enter
				}
			}

		glEnd();
	glEndList();

	id_AlwaysUpAlpha=glGenLists(1);
	glNewList(id_AlwaysUpAlpha,GL_COMPILE);
		glBegin(GL_QUADS);
	
			//always_up
			for(j=SCENE_HEIGHT-1;j>=0;j--)
			{
				px=SCENE_Xo;
				py=SCENE_Yo+(j*TILE_SIZE);

				for(i=0;i<SCENE_WIDTH;i++)
				{
					fscanf(fd,"%i",&id);

					always_up_alpha[(j*SCENE_WIDTH)+i] = id;
					coordx_tile = tex_offset_x*((id-1)%tiles_w); coordy_tile = tex_offset_y*((id-1)/tiles_w);

					if(id)
					{
						//coords de textura que ha de coger el vertice; donde se dibujara el vertice;
						glTexCoord2f(coordx_tile             ,coordy_tile+tex_offset_y);	glVertex3i(px          ,py          ,0);
						glTexCoord2f(coordx_tile+tex_offset_x,coordy_tile+tex_offset_y);	glVertex3i(px+TILE_SIZE,py          ,0);
						glTexCoord2f(coordx_tile+tex_offset_x,coordy_tile			  );	glVertex3i(px+TILE_SIZE,py+TILE_SIZE,0);
						glTexCoord2f(coordx_tile             ,coordy_tile			  );	glVertex3i(px          ,py+TILE_SIZE,0);
					}
					px+=TILE_SIZE;
				
					fscanf(fd,"%c",&c); //pass comma or enter
				}
			}

		glEnd();
	glEndList();

	//en este bucle almaceno las tiles animadas del nivel superior, si las hubiera
	for(j=SCENE_HEIGHT-1;j>=0;j--)
	{
		px=SCENE_Xo;
		py=SCENE_Yo+(j*TILE_SIZE);

		for(i=0;i<SCENE_WIDTH;i++)
		{
			fscanf(fd,"%i",&id);

			if(id)
			{
				cTile newtile;
				newtile.SetX(px);
				newtile.SetY(py);
				newtile.SetTexCoordX( tex_offset_x*((id-1)%tiles_w) );
				newtile.SetTexCoordY( tex_offset_y*((id-1)/tiles_w) );
				animated_tiles_up.push_back(newtile);
			}
			px+=TILE_SIZE;
				
			fscanf(fd,"%c",&c); //pass comma or enter
		}
	}

	fclose(fd);

	return res;
}

void cScene::ComputeCollisionMaps()
{
	int i;
	for(i=0; i<SCENE_WIDTH * SCENE_HEIGHT; i++)
	{
		collision_map[i] = (!bridges[i] && (terrain_solid[i] || solid_alpha[i] || always_down[i] || z_tileup[i]));
		proj_collision_map[i] = (!bridges[i] && (always_down[i] || z_tileup[i]));
	}
}

void cScene::Draw(int tex_id,int tex_w,int tex_h,bool run)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,tex_id);
	
	DrawAnimatedTiles(tex_id,tex_w,tex_h);
	glCallList(id_AlwaysUpAlpha);
	glCallList(id_AlwaysUp);
	glCallList(id_ZtileDown);
	glCallList(id_ZtileUp);
	glCallList(id_AlwaysDown);
	glCallList(id_Bridges);
	glCallList(id_SolidAlpha);
	glCallList(id_TerrainSolid);
	glCallList(id_TerrainFlat);

	glDisable(GL_TEXTURE_2D);

	if(run)
	{
		//NextFrame() para las tiles animadas
		delay++;
		if(delay == TILE_DELAY)
		{
			seq++;
			seq%=TILE_FRAMES;
			delay=0;
		}
	}
}
void cScene::DrawAnimatedTiles(int tex_id,int tex_w,int tex_h)
{
	unsigned int i;
	int px,py;
	float coordx_tile,coordy_tile;
	float tex_offset_x,tex_offset_y;

	tex_offset_x = 1.0f/float(tex_w/TEXTURE_TILE_SIZE);
	tex_offset_y = 1.0f/float(tex_h/TEXTURE_TILE_SIZE);

	for(i=0;i<animated_tiles.size();i++)
	{
		px = animated_tiles[i].GetX();
		py = animated_tiles[i].GetY();
		coordx_tile = animated_tiles[i].GetTexCoordX();
		coordy_tile = animated_tiles[i].GetTexCoordY();

		glBegin(GL_QUADS);

		glTexCoord2f(coordx_tile+tex_offset_x*seq    ,coordy_tile+tex_offset_y);	glVertex3i(px          ,py          ,(SCENE_HEIGHT*TILE_SIZE)+2);
		glTexCoord2f(coordx_tile+tex_offset_x*(seq+1),coordy_tile+tex_offset_y);	glVertex3i(px+TILE_SIZE,py          ,(SCENE_HEIGHT*TILE_SIZE)+2);
		glTexCoord2f(coordx_tile+tex_offset_x*(seq+1),coordy_tile);					glVertex3i(px+TILE_SIZE,py+TILE_SIZE,(SCENE_HEIGHT*TILE_SIZE)+2);
		glTexCoord2f(coordx_tile+tex_offset_x*seq    ,coordy_tile);					glVertex3i(px          ,py+TILE_SIZE,(SCENE_HEIGHT*TILE_SIZE)+2);
		
		glEnd();
	}
	for(i=0;i<animated_tiles_up.size();i++)
	{
		px = animated_tiles_up[i].GetX();
		py = animated_tiles_up[i].GetY();
		coordx_tile = animated_tiles_up[i].GetTexCoordX();
		coordy_tile = animated_tiles_up[i].GetTexCoordY();

		glBegin(GL_QUADS);

		glTexCoord2f(coordx_tile+tex_offset_x*seq    ,coordy_tile+tex_offset_y);	glVertex3i(px          ,py          ,0);
		glTexCoord2f(coordx_tile+tex_offset_x*(seq+1),coordy_tile+tex_offset_y);	glVertex3i(px+TILE_SIZE,py          ,0);
		glTexCoord2f(coordx_tile+tex_offset_x*(seq+1),coordy_tile);					glVertex3i(px+TILE_SIZE,py+TILE_SIZE,0);
		glTexCoord2f(coordx_tile+tex_offset_x*seq    ,coordy_tile);					glVertex3i(px          ,py+TILE_SIZE,0);
		
		glEnd();
	}
}
bool* cScene::GetCollisionMap()
{
	return collision_map;
}
bool* cScene::GetProjCollisionMap()
{
	return proj_collision_map;
}