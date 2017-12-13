#include "cMonster2.h"

cMonster2::cMonster2()
{
	SetBaseTilesHeight(1);
	SetBaseTilesWidth(1);
	SetHitHeight(TILE_SIZE+TILE_SIZE/2);
	SetHitWidth(TILE_SIZE);
	SetFrameTexTiles(4);
	SetMaxHealth(30);
	SetHealth(30);
}
cMonster2::~cMonster2(){}

bool cMonster2::Attack(std::list<cBicho*> &players,std::list<cProjectile*> &projectiles,bool *proj_collision_map)
{
	
	if(GetFrame() == 6 && GetDelay() == 0)
	{
		int x,y;
		projectiles.push_back(new cArrow);

		if(GetState() == STATE_ATTACKUP)
		{
			x = GetX() + (GetBaseWidth() - projectiles.back()->GetHitWidth())/2;
			y = GetY() + GetBaseHeight();
			projectiles.back()->SetState(STATE_LEVITATEUP);
			projectiles.back()->SetPosition(x,y);
			//evaluo si el proyectil aparece encima de un enemigo o objeto
			if( projectiles.back()->ComputeEnemyCollisions(players) || projectiles.back()->CollidesObjUD(proj_collision_map,false) || projectiles.back()->CollidesObjUD(proj_collision_map,true))
				projectiles.back()->SetState(STATE_ENDINGUP);
		}
		else if(GetState() == STATE_ATTACKDOWN)
		{
			x = GetX() + (GetBaseWidth() - projectiles.back()->GetHitWidth())/2;
			y = GetY();
			projectiles.back()->SetState(STATE_LEVITATEDOWN);
			projectiles.back()->SetPosition(x,y);
			//evaluo si el proyectil aparece encima de un enemigo o objeto
			if( projectiles.back()->ComputeEnemyCollisions(players) || projectiles.back()->CollidesObjUD(proj_collision_map,false) )
				projectiles.back()->SetState(STATE_ENDINGDOWN);
		}
		else if(GetState() == STATE_ATTACKLEFT)
		{
			x = GetX();
			y = GetY() + (GetBaseHeight() - projectiles.back()->GetHitHeight())/2;
			projectiles.back()->SetState(STATE_LEVITATELEFT);
			projectiles.back()->SetPosition(x,y);
			//evaluo si el proyectil aparece encima de un enemigo o objeto
			if( projectiles.back()->ComputeEnemyCollisions(players) || projectiles.back()->CollidesObjLR(proj_collision_map,false) )
				projectiles.back()->SetState(STATE_ENDINGLEFT);
		}
		else if(GetState() == STATE_ATTACKRIGHT)
		{
			x = GetX() + GetBaseWidth();
			y = GetY() + (GetBaseHeight() - projectiles.back()->GetHitHeight())/2;
			projectiles.back()->SetState(STATE_LEVITATERIGHT);
			projectiles.back()->SetPosition(x,y);
			//evaluo si el proyectil aparece encima de un enemigo o objeto
			if( projectiles.back()->ComputeEnemyCollisions(players) || projectiles.back()->CollidesObjLR(proj_collision_map,false) || projectiles.back()->CollidesObjLR(proj_collision_map,true))
				projectiles.back()->SetState(STATE_ENDINGRIGHT);
		}
		return true;
	}
	return false;
}

bool cMonster2::IA(bool *collision_map,bool *proj_collision_map,std::list<cBicho*> &enemies,cBicho* p1,cBicho* p2)
{
	int i;
	bool collision=false;
	cArrow arrow;
	int arrow_x,arrow_y;
	cRect hitbox_p1 = p1->GetHitBox();
	cRect hitbox_p2 = p2->GetHitBox();

	arrow_x = GetX() + (GetBaseWidth() - arrow.GetHitWidth())/2;
	arrow_y = GetY() + (GetBaseHeight() - arrow.GetHitHeight())/2;

	//si algun jugador es alcanzable, asigno el estado de ataque correspondiente
	if(hitbox_p1.right > arrow_x && hitbox_p1.left < arrow_x + arrow.GetHitWidth())
	{
		if(hitbox_p1.bottom > GetY())
		{
			for(i=((GetY()+GetBaseHeight())/TILE_SIZE); i<=(hitbox_p1.bottom/TILE_SIZE) && !collision; i++)
			{
			 collision = proj_collision_map[ (GetX()/TILE_SIZE) + i*SCENE_WIDTH ];
			}
			if(!collision)
			{
				SetState(STATE_ATTACKUP);
				SetFrame(0);
				SetDelay(0);
				return true;
			}
		}
		else
		{
			for(i=(hitbox_p1.bottom/TILE_SIZE); i<=((GetY()-GetBaseHeight())/TILE_SIZE) && !collision; i++)
			{
			 collision = proj_collision_map[ (GetX()/TILE_SIZE) + i*SCENE_WIDTH ];
			}
			if(!collision)
			{
				SetState(STATE_ATTACKDOWN);
				SetFrame(0);
				SetDelay(0);
				return true;
			}
		}
	}
	else if(hitbox_p1.top > arrow_y && hitbox_p1.bottom < arrow_y + arrow.GetHitHeight())
	{
		if(hitbox_p1.left > GetX())
		{
			for(i=((GetX()+GetBaseWidth())/TILE_SIZE); i<=(hitbox_p1.left/TILE_SIZE) && !collision; i++)
			{
			 collision = proj_collision_map[ i + (GetY()/TILE_SIZE)*SCENE_WIDTH ];
			}
			if(!collision)
			{
				SetState(STATE_ATTACKRIGHT);
				SetFrame(0);
				SetDelay(0);
				return true;
			}
		}
		else
		{
			for(i=(hitbox_p1.left/TILE_SIZE); i<=((GetX()-GetBaseWidth())/TILE_SIZE) && !collision; i++)
			{
			 collision = proj_collision_map[ (GetX()/TILE_SIZE) + i*SCENE_WIDTH ];
			}
			if(!collision)
			{
				SetState(STATE_ATTACKLEFT);
				SetFrame(0);
				SetDelay(0);
				return true;
			}
		}
	}
	else if(hitbox_p2.right > arrow_x && hitbox_p2.left < arrow_x + arrow.GetHitWidth())
	{
		if(hitbox_p2.bottom > GetY())
		{
			for(i=((GetY()+GetBaseHeight())/TILE_SIZE); i<=(hitbox_p2.bottom/TILE_SIZE) && !collision; i++)
			{
			 collision = proj_collision_map[ (GetX()/TILE_SIZE) + i*SCENE_WIDTH ];
			}
			if(!collision)
			{
				SetState(STATE_ATTACKUP);
				SetFrame(0);
				SetDelay(0);
				return true;
			}
		}
		else
		{
			for(i=(hitbox_p2.bottom/TILE_SIZE); i<=((GetY()-GetBaseHeight())/TILE_SIZE) && !collision; i++)
			{
			 collision = proj_collision_map[ (GetX()/TILE_SIZE) + i*SCENE_WIDTH ];
			}
			if(!collision)
			{
				SetState(STATE_ATTACKDOWN);
				SetFrame(0);
				SetDelay(0);
				return true;
			}
		}
	}
	else if(hitbox_p2.top > arrow_y && hitbox_p2.bottom < arrow_y + arrow.GetHitHeight())
	{
		if(hitbox_p2.left > GetX())
		{
			for(i=((GetX()+GetBaseWidth())/TILE_SIZE); i<=(hitbox_p2.left/TILE_SIZE) && !collision; i++)
			{
			 collision = proj_collision_map[ i + (GetY()/TILE_SIZE)*SCENE_WIDTH ];
			}
			if(!collision)
			{
				SetState(STATE_ATTACKRIGHT);
				SetFrame(0);
				SetDelay(0);
				return true;
			}
		}
		else
		{
			for(i=(hitbox_p2.left/TILE_SIZE); i<=((GetX()-GetBaseWidth())/TILE_SIZE) && !collision; i++)
			{
			 collision = proj_collision_map[ (GetX()/TILE_SIZE) + i*SCENE_WIDTH ];
			}
			if(!collision)
			{
				SetState(STATE_ATTACKLEFT);
				SetFrame(0);
				SetDelay(0);
				return true;
			}
		}
	}

	//Si no puede atacar, calculo el path y asigno el estado de movimiento correspondiente
	//si le atacaron mientras se movia pudo haberse quedado desplazado de los margenes de la tile, es necesario recolocarlo
	if(GetX()%TILE_SIZE != 0)
	{
		if(GetX()%TILE_SIZE < TILE_SIZE/2) SetState(STATE_WALKLEFT);
		else SetState(STATE_WALKRIGHT);
	}
	else if(GetY()%TILE_SIZE != 0)
	{
		if(GetY()%TILE_SIZE < TILE_SIZE/2) SetState(STATE_WALKDOWN);
		else SetState(STATE_WALKUP);
	}
	else
	{
		int new_collision_map[SCENE_WIDTH * SCENE_HEIGHT]; //0 para vacio, 1 para obstaculo, 2 para players
		std::vector<int> tiles;
		std::list<cBicho*>::iterator it;

		//creo un nuevo mapa de colisiones en el que ademas aparecen reflejadas las tiles ocupadas por bichos
		for(i=0; i<SCENE_WIDTH * SCENE_HEIGHT; i++) new_collision_map[i] = collision_map[i];
		for(it=enemies.begin(); it!=enemies.end(); it++)
		{
			if((*it) != this)
			{
				tiles = (*it)->GetTiles();
				for(i=0; i<tiles.size(); i++)
				{
					new_collision_map[ tiles[i] ] = 1;
				}
			}
		}
		tiles = p1->GetTiles();
		for(i=0; i<tiles.size(); i++)
		{
			new_collision_map[ tiles[i] ] = 2;
		}
		tiles = p2->GetTiles();
		for(i=0; i<tiles.size(); i++)
		{
			new_collision_map[ tiles[i] ] = 2;
		}

		//determino por que direccion queda el camino mas corto para llegar hasta un jugador, si es que es alcanzable
		std::vector<int> occupied_tiles = GetTiles();
		float dist, min_dist;
		int best_state;
		bool illegal_move = false;
		std::vector<int> choices(0);

		min_dist = abs((GetX()+TILE_SIZE) - p1->GetX());
		best_state = STATE_WALKRIGHT;
		dist = abs((GetX()+TILE_SIZE) - p2->GetX());
		if(dist < min_dist) {min_dist = dist; best_state = STATE_WALKRIGHT;}
		dist = abs((GetX()-TILE_SIZE) - p1->GetX());
		if(dist < min_dist) {min_dist = dist; best_state = STATE_WALKLEFT;}
		dist = abs((GetX()-TILE_SIZE) - p2->GetX());
		if(dist < min_dist) {min_dist = dist; best_state = STATE_WALKLEFT;}
		dist = abs((GetY()+TILE_SIZE) - p1->GetY());
		if(dist < min_dist) {min_dist = dist; best_state = STATE_WALKUP;}
		dist = abs((GetY()+TILE_SIZE) - p2->GetY());
		if(dist < min_dist) {min_dist = dist; best_state = STATE_WALKUP;}
		dist = abs((GetY()-TILE_SIZE) - p1->GetY());
		if(dist < min_dist) {min_dist = dist; best_state = STATE_WALKDOWN;}
		dist = abs((GetY()-TILE_SIZE) - p2->GetY());
		if(dist < min_dist) {min_dist = dist; best_state = STATE_WALKDOWN;}

		for(i=0; i<occupied_tiles.size() && !illegal_move; i++) if(new_collision_map[occupied_tiles[i] + SCENE_WIDTH]) illegal_move=true;
		if(!illegal_move) choices.push_back(STATE_WALKUP);
		else illegal_move = false;
		for(i=0; i<occupied_tiles.size() && !illegal_move; i++) if(new_collision_map[occupied_tiles[i] - SCENE_WIDTH]) illegal_move=true;
		if(!illegal_move) choices.push_back(STATE_WALKDOWN);
		else illegal_move = false;
		for(i=0; i<occupied_tiles.size() && !illegal_move; i++) if(new_collision_map[occupied_tiles[i] + 1]) illegal_move=true;
		if(!illegal_move) choices.push_back(STATE_WALKRIGHT);
		else illegal_move = false;
		for(i=0; i<occupied_tiles.size() && !illegal_move; i++) if(new_collision_map[occupied_tiles[i] - 1]) illegal_move=true;
		if(!illegal_move) choices.push_back(STATE_WALKLEFT);
		else illegal_move = false;

		if(choices.size()) 
		{
			SetState(choices[rand()%choices.size()]); //asigna una direccion aleatoria de entre las posibles
			for(i=0; i<choices.size(); i++) if(choices[i] == best_state) SetState(best_state); //si una de esas direcciones posibles era la mejor, asigna esa
		    Move(); //empiezo a moverme en la direccion asignada, si la hubiera, para diferenciarme del caso en que llego a una nueva tile
		}
	}
	return false;
}