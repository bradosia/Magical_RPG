#include "cMonster1.h"

cMonster1::cMonster1()
{
	SetBaseTilesHeight(1);
	SetBaseTilesWidth(1);
	SetHitHeight(TILE_SIZE+TILE_SIZE/2);
	SetHitWidth(TILE_SIZE);
	SetFrameTexTiles(4);
	SetMaxHealth(30);
	SetHealth(30);
	SetDamage(10);
}
cMonster1::~cMonster1(){}

bool cMonster1::Attack(cBicho* player)
{
	bool hit = false;
	if(GetFrame() == 3 && GetDelay() == 0)
	{
		if(GetState() == STATE_ATTACKUP)
		{
			cRect attack_area;
			attack_area.top = GetY()+GetBaseHeight()+TILE_SIZE;
			attack_area.bottom = GetY()+GetBaseHeight();
			attack_area.left = GetX();
			attack_area.right = GetX()+GetBaseWidth();

			if( player->GetState() != STATE_DYING && Intersection(attack_area,player->GetHitBox()) ) 
			{
				cPlayer1* player1 = dynamic_cast<cPlayer1*>(player);
				if(player1 != NULL && player1->IsDoingSkill()) player1->TestShieldProtection(STATE_ATTACKUP,GetDamage(),GetX(),GetY());
				else
				{
					int new_health = player->GetHealth() - GetDamage();
					if(new_health <= 0) player->SetState(STATE_DYING);
					else if(!dynamic_cast<cPlayer*>(player)->IsSuperAttacking()) 
					{
						player->SetState(STATE_DAMAGEDOWN);
						player->SetFrame(0);
						player->SetDelay(0);
					}
					if(new_health < 0) new_health = 0;
					player->SetHealth(new_health);
					hit = true;
				}
			}
		}
		else if(GetState() == STATE_ATTACKDOWN)
		{
			cRect attack_area;
			attack_area.top = GetY();
			attack_area.bottom = GetY()-TILE_SIZE;
			attack_area.left = GetX();
			attack_area.right = GetX()+GetBaseWidth();

			if( player->GetState() != STATE_DYING && Intersection(attack_area,player->GetHitBox()) ) 
			{
				cPlayer1* player1 = dynamic_cast<cPlayer1*>(player);
				if(player1 != NULL && player1->IsDoingSkill()) player1->TestShieldProtection(STATE_ATTACKDOWN,GetDamage(),GetX(),GetY());
				else
				{
					int new_health = player->GetHealth() - GetDamage();
					if(new_health <= 0) player->SetState(STATE_DYING);
					else if(!dynamic_cast<cPlayer*>(player)->IsSuperAttacking()) 
					{
						player->SetState(STATE_DAMAGEUP);
						player->SetFrame(0);
						player->SetDelay(0);
					}
					if(new_health < 0) new_health = 0;
					player->SetHealth(new_health);
					hit = true;
				}
			}
		}
		else if(GetState() == STATE_ATTACKLEFT)
		{
			cRect attack_area;
			attack_area.top = GetY()+GetBaseHeight();
			attack_area.bottom = GetY();
			attack_area.left = GetX()-TILE_SIZE;
			attack_area.right = GetX();

			if( player->GetState() != STATE_DYING && Intersection(attack_area,player->GetHitBox()) ) 
			{
				cPlayer1* player1 = dynamic_cast<cPlayer1*>(player);
				if(player1 != NULL && player1->IsDoingSkill()) player1->TestShieldProtection(STATE_ATTACKLEFT,GetDamage(),GetX(),GetY());
				else
				{
					int new_health = player->GetHealth() - GetDamage();
					if(new_health <= 0) player->SetState(STATE_DYING);
					else if(!dynamic_cast<cPlayer*>(player)->IsSuperAttacking()) 
					{
						player->SetState(STATE_DAMAGERIGHT);
						player->SetFrame(0);
						player->SetDelay(0);
					}
					if(new_health < 0) new_health = 0;
					player->SetHealth(new_health);
					hit = true;
				}
			}
		}
		else if(GetState() == STATE_ATTACKRIGHT)
		{
			cRect attack_area;
			attack_area.top = GetY()+GetBaseHeight();
			attack_area.bottom = GetY();
			attack_area.left = GetX()+GetBaseWidth();
			attack_area.right = GetX()+GetBaseWidth()+TILE_SIZE;

			if( player->GetState() != STATE_DYING && Intersection(attack_area,player->GetHitBox()) ) 
			{
				cPlayer1* player1 = dynamic_cast<cPlayer1*>(player);
				if(player1 != NULL && player1->IsDoingSkill()) player1->TestShieldProtection(STATE_ATTACKRIGHT,GetDamage(),GetX(),GetY());
				else
				{
					int new_health = player->GetHealth() - GetDamage();
					if(new_health <= 0) player->SetState(STATE_DYING);
					else if(!dynamic_cast<cPlayer*>(player)->IsSuperAttacking()) 
					{
						player->SetState(STATE_DAMAGELEFT);
						player->SetFrame(0);
						player->SetDelay(0);
					}
					if(new_health < 0) new_health = 0;
					player->SetHealth(new_health);
					hit = true;
				}
			}
		}
	}
	return hit;
}

bool cMonster1::IA(bool *collision_map,std::list<cBicho*> &enemies,cBicho* p1,cBicho* p2)
{
	unsigned int i;
	//si algun jugador es alcanzable, asigno el estado de ataque correspondiente
	cRect hitbox_p1 = p1->GetHitBox();
	cRect hitbox_p2 = p2->GetHitBox();

	cRect attack_areaV,attack_areaH;

	attack_areaV.top = GetY()+GetBaseHeight()+TILE_SIZE;
	attack_areaV.bottom = GetY()-TILE_SIZE;
	attack_areaV.left = GetX();
	attack_areaV.right = GetX()+GetBaseWidth();

	attack_areaH.top = GetY()+GetBaseHeight();
	attack_areaH.bottom = GetY();
	attack_areaH.left = GetX()-TILE_SIZE;
	attack_areaH.right = GetX()+GetBaseWidth()+TILE_SIZE;

	if(Intersection(attack_areaV,hitbox_p1))
	{
		if(hitbox_p1.bottom < GetY())   SetState(STATE_ATTACKDOWN);
		else SetState(STATE_ATTACKUP);
		SetFrame(0);
		SetDelay(0);
		return true;
	}
	else if(Intersection(attack_areaH,hitbox_p1))
	{
		if(hitbox_p1.left < GetX())   SetState(STATE_ATTACKLEFT);
		else SetState(STATE_ATTACKRIGHT);
		SetFrame(0);
		SetDelay(0);
		return true;
	}
	else if(Intersection(attack_areaV,hitbox_p2))
	{
		if(hitbox_p2.bottom < GetY())   SetState(STATE_ATTACKDOWN);
		else SetState(STATE_ATTACKUP);
		SetFrame(0);
		SetDelay(0);
		return true;
	}
	else if(Intersection(attack_areaH,hitbox_p2))
	{
		if(hitbox_p2.left < GetX())   SetState(STATE_ATTACKLEFT);
		else SetState(STATE_ATTACKRIGHT);
		SetFrame(0); 
		SetDelay(0);
		return true;
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

		min_dist = sqrt(pow(GetX() - p1->GetX(),2.0f) + pow((GetY()+TILE_SIZE) - p1->GetY(),2.0f));
		best_state = STATE_WALKUP;
		dist = sqrt(pow(GetX() - p2->GetX(),2.0f) + pow((GetY()+TILE_SIZE) - p2->GetY(),2.0f));
		if(dist < min_dist) {min_dist = dist; best_state = STATE_WALKUP;}
		dist = sqrt(pow(GetX() - p1->GetX(),2.0f) + pow((GetY()-TILE_SIZE) - p1->GetY(),2.0f));
		if(dist < min_dist) {min_dist = dist; best_state = STATE_WALKDOWN;}
		dist = sqrt(pow(GetX() - p2->GetX(),2.0f) + pow((GetY()-TILE_SIZE) - p2->GetY(),2.0f));
		if(dist < min_dist) {min_dist = dist; best_state = STATE_WALKDOWN;}
		dist = sqrt(pow((GetX()+TILE_SIZE) - p1->GetX(),2.0f) + pow(GetY() - p1->GetY(),2.0f));
		if(dist < min_dist) {min_dist = dist; best_state = STATE_WALKRIGHT;}
		dist = sqrt(pow((GetX()+TILE_SIZE) - p2->GetX(),2.0f) + pow(GetY() - p2->GetY(),2.0f));
		if(dist < min_dist) {min_dist = dist; best_state = STATE_WALKRIGHT;}
		dist = sqrt(pow((GetX()-TILE_SIZE) - p1->GetX(),2.0f) + pow(GetY() - p1->GetY(),2.0f));
		if(dist < min_dist) {min_dist = dist; best_state = STATE_WALKLEFT;}
		dist = sqrt(pow((GetX()-TILE_SIZE) - p2->GetX(),2.0f) + pow(GetY() - p2->GetY(),2.0f));
		if(dist < min_dist) {min_dist = dist; best_state = STATE_WALKLEFT;}

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