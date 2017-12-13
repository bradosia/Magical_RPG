#include "cPlayer2.h"

cPlayer2::cPlayer2()
{
	SetMaxHealth(100);
	SetHealth(100);
	SetMaxSP(150);
	SetSP(150);
	SetCharge(0);
}
cPlayer2::~cPlayer2(){}

void cPlayer2::Draw(int tex_id,int tex_w,int tex_h,bool run)
{	
	float xo,yo,xf,yf;
	bool reverse=false;
	float tex_offset_x,tex_offset_y;

	tex_offset_x = 1.0f/float(tex_w/(GetFrameTexTiles()*TEXTURE_TILE_SIZE));
	tex_offset_y = 1.0f/float(tex_h/(GetFrameTexTiles()*TEXTURE_TILE_SIZE));

	switch(GetState())
	{
		case STATE_LOOKUP:		xo = tex_offset_x*1;   yo = tex_offset_y + tex_offset_y*0;
								break;
		case STATE_LOOKRIGHT:	xo = tex_offset_x*2;   yo = tex_offset_y + tex_offset_y*0;
								break;
		case STATE_LOOKDOWN:	xo = tex_offset_x*0;   yo = tex_offset_y + tex_offset_y*0;
								break;
		case STATE_LOOKLEFT:	xo = tex_offset_x*2;   yo = tex_offset_y + tex_offset_y*0;
								reverse = true;
								break;

		case STATE_SKILLUP:		xo = tex_offset_x*1;   yo = tex_offset_y + tex_offset_y*9 + (GetFrame()*tex_offset_y);
								if(run) NextFrame(2);
								break;
		case STATE_SKILLRIGHT:	xo = tex_offset_x*2;   yo = tex_offset_y + tex_offset_y*9 + (GetFrame()*tex_offset_y);
								if(run) NextFrame(2);
								break;
		case STATE_SKILLDOWN:	xo = tex_offset_x*0;   yo = tex_offset_y + tex_offset_y*9 + (GetFrame()*tex_offset_y);
								if(run) NextFrame(2);
								break;
		case STATE_SKILLLEFT:	xo = tex_offset_x*2;   yo = tex_offset_y + tex_offset_y*9 + (GetFrame()*tex_offset_y);
								if(run) NextFrame(2);
								reverse = true;
								break;

		case STATE_WALKUP:		xo = tex_offset_x*1;   yo = tex_offset_y + tex_offset_y*1 + (GetFrame()*tex_offset_y);
								if(run) NextFrame(4);
								break;
		case STATE_WALKRIGHT:	xo = tex_offset_x*2;   yo = tex_offset_y + tex_offset_y*1 + (GetFrame()*tex_offset_y);
								if(run) NextFrame(4);
								break;
		case STATE_WALKDOWN:	xo = tex_offset_x*0;   yo = tex_offset_y + tex_offset_y*1 + (GetFrame()*tex_offset_y);
								if(run) NextFrame(4);
								break;
		case STATE_WALKLEFT:	xo = tex_offset_x*2;   yo = tex_offset_y + tex_offset_y*1 + (GetFrame()*tex_offset_y);
								if(run) NextFrame(4);
								reverse = true;
								break;

		case STATE_SKILLWALKUP:		xo = tex_offset_x*1;   yo = tex_offset_y + tex_offset_y*10 + (GetFrame()*tex_offset_y);
									if(run) NextFrame(2);
									break;
		case STATE_SKILLWALKRIGHT:	xo = tex_offset_x*2;   yo = tex_offset_y + tex_offset_y*10 + (GetFrame()*tex_offset_y);
									if(run) NextFrame(2);
									break;
		case STATE_SKILLWALKDOWN:	xo = tex_offset_x*0;   yo = tex_offset_y + tex_offset_y*10 + (GetFrame()*tex_offset_y);
									if(run) NextFrame(2);
									break;
		case STATE_SKILLWALKLEFT:	xo = tex_offset_x*2;   yo = tex_offset_y + tex_offset_y*10 + (GetFrame()*tex_offset_y);
									if(run) NextFrame(2);
									reverse = true;
									break;

		case STATE_DAMAGEUP:	xo = tex_offset_x*1;   yo = tex_offset_y + tex_offset_y*12 + (GetFrame()*tex_offset_y);
								if(run) NextFrame(2);
								break;
		case STATE_DAMAGERIGHT:	xo = tex_offset_x*2;   yo = tex_offset_y + tex_offset_y*12 + (GetFrame()*tex_offset_y);
								if(run) NextFrame(2);
								break;
		case STATE_DAMAGEDOWN:	xo = tex_offset_x*0;   yo = tex_offset_y + tex_offset_y*12 + (GetFrame()*tex_offset_y);
								if(run) NextFrame(2);
								break;
		case STATE_DAMAGELEFT:	xo = tex_offset_x*2;   yo = tex_offset_y + tex_offset_y*12 + (GetFrame()*tex_offset_y);
								if(run) NextFrame(2);
								reverse = true;
								break;

		case STATE_ATTACKUP:	xo = tex_offset_x*1;   yo = tex_offset_y + tex_offset_y*5 + (GetFrame()*tex_offset_y);
								if(run) NextFrame(4);
								break;
		case STATE_ATTACKRIGHT:	xo = tex_offset_x*2;   yo = tex_offset_y + tex_offset_y*5 + (GetFrame()*tex_offset_y);
								if(run) NextFrame(4);
								break;
		case STATE_ATTACKDOWN:	xo = tex_offset_x*0;   yo = tex_offset_y + tex_offset_y*5 + (GetFrame()*tex_offset_y);
								if(run) NextFrame(4);
								break;
		case STATE_ATTACKLEFT:	xo = tex_offset_x*2;   yo = tex_offset_y + tex_offset_y*5 + (GetFrame()*tex_offset_y);
								reverse = true;
								if(run) NextFrame(4);
								break;

		case STATE_SUPERATTACK:	xo = tex_offset_x*3;   yo = tex_offset_y + tex_offset_y*1 + (GetFrame()*tex_offset_y);
								if(run) NextFrame(12);
								break;

		case STATE_DYING:		xo = tex_offset_x*3;   yo = tex_offset_y + tex_offset_y*0;
								break;
	}
	xf = xo + tex_offset_x;
	yf = yo - tex_offset_y;
	if(reverse) 
	{
		float aux = xo;
		xo = xf;  xf = aux;
	}
	DrawRect(tex_id,xo,yo,xf,yf);
}

bool cPlayer2::Attack(std::list<cBicho*> &enemies,std::list<cProjectile*> &projectiles,bool *proj_collision_map)
{
	int x,y;
	//los offset sirven para recolocar el proyectil de forma que solo tengamos que comprobar colision en la interseccion entre tiles (sea cual fuera la pos del jugador y el step del proyectil)
	int offset_x, offset_y;

	cFireball aux;
	int proyectile_step = aux.GetStepLength();

	offset_x = GetX()%proyectile_step;
	offset_y = GetY()%proyectile_step;

	if(GetState() != STATE_SUPERATTACK && GetFrame() == 2 && GetDelay() == 0)
	{
		if(GetState() == STATE_ATTACKUP)
		{
			projectiles.push_back(new cFireball);
			x = GetX() + (GetBaseWidth() - projectiles.back()->GetHitWidth())/2;
			y = GetY() + GetBaseHeight() - offset_y;
			projectiles.back()->SetState(STATE_LEVITATEUP);
			projectiles.back()->SetPosition(x,y);
			//evaluo si el proyectil aparece encima de un enemigo o objeto
			if( projectiles.back()->ComputeEnemyCollisions(enemies) || projectiles.back()->CollidesObjUD(proj_collision_map,false) || projectiles.back()->CollidesObjUD(proj_collision_map,true))
				projectiles.back()->SetState(STATE_ENDINGUP);
		}
		else if(GetState() == STATE_ATTACKDOWN)
		{
			projectiles.push_back(new cFireball);
			x = GetX() + (GetBaseWidth() - projectiles.back()->GetHitWidth())/2;
			y = GetY() - projectiles.back()->GetHitHeight() + (proyectile_step - offset_y)%proyectile_step;
			projectiles.back()->SetState(STATE_LEVITATEDOWN);
			projectiles.back()->SetPosition(x,y);
			//evaluo si el proyectil aparece encima de un enemigo o objeto
			if( projectiles.back()->ComputeEnemyCollisions(enemies) || projectiles.back()->CollidesObjUD(proj_collision_map,false) )
				projectiles.back()->SetState(STATE_ENDINGDOWN);
		}
		else if(GetState() == STATE_ATTACKLEFT)
		{
			projectiles.push_back(new cFireball);
			x = GetX() - projectiles.back()->GetHitWidth() + (proyectile_step - offset_x)%proyectile_step;
			y = GetY() + (GetBaseHeight() - projectiles.back()->GetHitHeight())/2;
			projectiles.back()->SetState(STATE_LEVITATELEFT);
			projectiles.back()->SetPosition(x,y);
			//evaluo si el proyectil aparece encima de un enemigo o objeto
			if( projectiles.back()->ComputeEnemyCollisions(enemies) || projectiles.back()->CollidesObjLR(proj_collision_map,false) )
				projectiles.back()->SetState(STATE_ENDINGLEFT);
		}
		else if(GetState() == STATE_ATTACKRIGHT)
		{
			projectiles.push_back(new cFireball);
			x = GetX() + GetBaseWidth() - offset_x;
			y = GetY() + (GetBaseHeight() - projectiles.back()->GetHitHeight())/2;
			projectiles.back()->SetState(STATE_LEVITATERIGHT);
			projectiles.back()->SetPosition(x,y);
			//evaluo si el proyectil aparece encima de un enemigo o objeto
			if( projectiles.back()->ComputeEnemyCollisions(enemies) || projectiles.back()->CollidesObjLR(proj_collision_map,false) || projectiles.back()->CollidesObjLR(proj_collision_map,true))
				projectiles.back()->SetState(STATE_ENDINGRIGHT);
		}
		SetSP(GetSP() - ATTACK_COST);
		return true;
	}
	else if(GetState() == STATE_SUPERATTACK && GetDelay() == 0 && GetSP() >= ATTACK_COST/2)
	{
		if(GetFrame() == 3)
		{
			projectiles.push_back(new cFireball);
			x = GetX() + (GetBaseWidth() - projectiles.back()->GetHitWidth())/2;
			y = TILE_SIZE + GetY() + GetBaseHeight() - offset_y;
			projectiles.back()->SetState(STATE_LEVITATEUP);
			projectiles.back()->SetPosition(x,y);
			//evaluo si el proyectil aparece encima de un enemigo o objeto
			if( projectiles.back()->ComputeEnemyCollisions(enemies) || projectiles.back()->CollidesObjUD(proj_collision_map,false) || projectiles.back()->CollidesObjUD(proj_collision_map,true))
				projectiles.back()->SetState(STATE_ENDINGUP);
			SetSP(GetSP() - ATTACK_COST/2);
			return true;
		}
		else if(GetFrame() == 4)
		{
			projectiles.push_back(new cFireball);
			x = GetX() + GetBaseWidth() - offset_x;
			y = TILE_SIZE + GetY() + (GetBaseHeight() - projectiles.back()->GetHitHeight())/2;
			projectiles.back()->SetState(STATE_LEVITATERIGHT);
			projectiles.back()->SetPosition(x,y);
			//evaluo si el proyectil aparece encima de un enemigo o objeto
			if( projectiles.back()->ComputeEnemyCollisions(enemies) || projectiles.back()->CollidesObjLR(proj_collision_map,false) || projectiles.back()->CollidesObjLR(proj_collision_map,true))
				projectiles.back()->SetState(STATE_ENDINGRIGHT);
			projectiles.back()->SetDisplaceUp(true);
			SetSP(GetSP() - ATTACK_COST/2);
			return true;
		}
		else if(GetFrame() == 5)
		{
			projectiles.push_back(new cFireball);
			x = GetX() + GetBaseWidth() - offset_x;
			y = TILE_SIZE + GetY() + (GetBaseHeight() - projectiles.back()->GetHitHeight())/2;
			projectiles.back()->SetState(STATE_LEVITATERIGHT);
			projectiles.back()->SetPosition(x,y);
			//evaluo si el proyectil aparece encima de un enemigo o objeto
			if( projectiles.back()->ComputeEnemyCollisions(enemies) || projectiles.back()->CollidesObjLR(proj_collision_map,false) || projectiles.back()->CollidesObjLR(proj_collision_map,true))
				projectiles.back()->SetState(STATE_ENDINGRIGHT);
			SetSP(GetSP() - ATTACK_COST/2);
			return true;
		}
		else if(GetFrame() == 6)
		{
			projectiles.push_back(new cFireball);
			x = GetX() + GetBaseWidth() - offset_x;
			y = TILE_SIZE + GetY() + (GetBaseHeight() - projectiles.back()->GetHitHeight())/2;
			projectiles.back()->SetState(STATE_LEVITATERIGHT);
			projectiles.back()->SetPosition(x,y);
			//evaluo si el proyectil aparece encima de un enemigo o objeto
			if( projectiles.back()->ComputeEnemyCollisions(enemies) || projectiles.back()->CollidesObjLR(proj_collision_map,false) || projectiles.back()->CollidesObjLR(proj_collision_map,true))
				projectiles.back()->SetState(STATE_ENDINGRIGHT);
			projectiles.back()->SetDisplaceDown(true);
			SetSP(GetSP() - ATTACK_COST/2);
			return true;
		}
		else if(GetFrame() == 7)
		{
			projectiles.push_back(new cFireball);
			x = GetX() + (GetBaseWidth() - projectiles.back()->GetHitWidth())/2;
			y = TILE_SIZE + GetY() - projectiles.back()->GetHitHeight() + (proyectile_step - offset_y)%proyectile_step;
			projectiles.back()->SetState(STATE_LEVITATEDOWN);
			projectiles.back()->SetPosition(x,y);
			//evaluo si el proyectil aparece encima de un enemigo o objeto
			if( projectiles.back()->ComputeEnemyCollisions(enemies) || projectiles.back()->CollidesObjUD(proj_collision_map,false) )
				projectiles.back()->SetState(STATE_ENDINGDOWN);
			SetSP(GetSP() - ATTACK_COST/2);
			return true;
		}
		else if(GetFrame() == 8)
		{
			projectiles.push_back(new cFireball);
			x = GetX() - projectiles.back()->GetHitWidth() + (proyectile_step - offset_x)%proyectile_step;
			y = TILE_SIZE + GetY() + (GetBaseHeight() - projectiles.back()->GetHitHeight())/2;
			projectiles.back()->SetState(STATE_LEVITATELEFT);
			projectiles.back()->SetPosition(x,y);
			//evaluo si el proyectil aparece encima de un enemigo o objeto
			if( projectiles.back()->ComputeEnemyCollisions(enemies) || projectiles.back()->CollidesObjLR(proj_collision_map,false) )
				projectiles.back()->SetState(STATE_ENDINGLEFT);
			projectiles.back()->SetDisplaceDown(true);
			SetSP(GetSP() - ATTACK_COST/2);
			return true;
		}
		else if(GetFrame() == 9)
		{
			projectiles.push_back(new cFireball);
			x = GetX() - projectiles.back()->GetHitWidth() + (proyectile_step - offset_x)%proyectile_step;
			y = TILE_SIZE + GetY() + (GetBaseHeight() - projectiles.back()->GetHitHeight())/2;
			projectiles.back()->SetState(STATE_LEVITATELEFT);
			projectiles.back()->SetPosition(x,y);
			//evaluo si el proyectil aparece encima de un enemigo o objeto
			if( projectiles.back()->ComputeEnemyCollisions(enemies) || projectiles.back()->CollidesObjLR(proj_collision_map,false) )
				projectiles.back()->SetState(STATE_ENDINGLEFT);
			SetSP(GetSP() - ATTACK_COST/2);
			return true;
		}
		else if(GetFrame() == 10)
		{
			projectiles.push_back(new cFireball);
			x = GetX() - projectiles.back()->GetHitWidth() + (proyectile_step - offset_x)%proyectile_step;
			y = TILE_SIZE + GetY() + (GetBaseHeight() - projectiles.back()->GetHitHeight())/2;
			projectiles.back()->SetState(STATE_LEVITATELEFT);
			projectiles.back()->SetPosition(x,y);
			//evaluo si el proyectil aparece encima de un enemigo o objeto
			if( projectiles.back()->ComputeEnemyCollisions(enemies) || projectiles.back()->CollidesObjLR(proj_collision_map,false) )
				projectiles.back()->SetState(STATE_ENDINGLEFT);
			projectiles.back()->SetDisplaceUp(true);
			SetSP(GetSP() - ATTACK_COST/2);
			return true;
		}
		else if(GetFrame() == 11)
		{
			projectiles.push_back(new cFireball);
			x = GetX() + (GetBaseWidth() - projectiles.back()->GetHitWidth())/2;
			y = TILE_SIZE + GetY() + GetBaseHeight() - offset_y;
			projectiles.back()->SetState(STATE_LEVITATEUP);
			projectiles.back()->SetPosition(x,y);
			//evaluo si el proyectil aparece encima de un enemigo o objeto
			if( projectiles.back()->ComputeEnemyCollisions(enemies) || projectiles.back()->CollidesObjUD(proj_collision_map,false) || projectiles.back()->CollidesObjUD(proj_collision_map,true))
				projectiles.back()->SetState(STATE_ENDINGUP);
			SetSP(GetSP() - ATTACK_COST/2);
			return true;
		}
	}
	return false;
}