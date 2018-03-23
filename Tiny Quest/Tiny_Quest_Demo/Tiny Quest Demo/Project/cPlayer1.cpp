#include "cPlayer1.h"

cPlayer1::cPlayer1()
{
	SetMaxHealth(150);
	SetHealth(150);
	SetMaxSP(150);
	SetSP(150);
	SetCharge(0);
	SetDamage(10);
	SetLogicState(STATE_LOOKDOWN);
}
cPlayer1::~cPlayer1(){}

void cPlayer1::Draw(int tex_id,int tex_w,int tex_h,bool run)
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
								if(run) NextFrame(7);
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

bool cPlayer1::Attack(cBicho* enemy)
{
	bool hit = false;
	if(GetState() != STATE_SUPERATTACK && GetFrame() == 3 && GetDelay() == 0)
	{
		if(GetState() == STATE_ATTACKUP)
		{
			cRect attack_area;
			attack_area.top = GetY()+GetBaseHeight()+TILE_SIZE;
			attack_area.bottom = GetY()+GetBaseHeight();
			attack_area.left = GetX()-TILE_SIZE;
			attack_area.right = GetX()+GetBaseWidth()+TILE_SIZE;

			if( enemy->GetState() != STATE_DYING && Intersection(attack_area,enemy->GetHitBox()) ) 
			{
				int new_health = enemy->GetHealth() - GetDamage();
				if(new_health <= 0) enemy->SetState(STATE_DYING);
				else enemy->SetState(STATE_DAMAGEDOWN);
				if(new_health < 0) new_health = 0;
				enemy->SetHealth(new_health);
				enemy->SetFrame(0);
				enemy->SetDelay(0);
				hit = true;
			}
		}
		else if(GetState() == STATE_ATTACKDOWN)
		{
			cRect attack_area;
			attack_area.top = GetY();
			attack_area.bottom = GetY()-TILE_SIZE/2;
			attack_area.left = GetX()-TILE_SIZE;
			attack_area.right = GetX()+GetBaseWidth()+TILE_SIZE;

			if( enemy->GetState() != STATE_DYING && Intersection(attack_area,enemy->GetHitBox()) ) 
			{
				int new_health = enemy->GetHealth() - GetDamage();
				if(new_health <= 0) enemy->SetState(STATE_DYING);
				else enemy->SetState(STATE_DAMAGEUP);
				if(new_health < 0) new_health = 0;
				enemy->SetHealth(new_health);
				enemy->SetFrame(0);
				enemy->SetDelay(0);
				hit = true;
			}
		}
		else if(GetState() == STATE_ATTACKLEFT)
		{
			cRect attack_area;
			attack_area.top = GetY()+GetBaseHeight();
			attack_area.bottom = GetY();
			attack_area.left = GetX()-(TILE_SIZE/5)*7;
			attack_area.right = GetX();

			if( enemy->GetState() != STATE_DYING && Intersection(attack_area,enemy->GetHitBox()) ) 
			{
				int new_health = enemy->GetHealth() - GetDamage();
				if(new_health <= 0) enemy->SetState(STATE_DYING);
				else enemy->SetState(STATE_DAMAGERIGHT);
				if(new_health < 0) new_health = 0;
				enemy->SetHealth(new_health);
				enemy->SetFrame(0);
				enemy->SetDelay(0);
				hit = true;
			}
		}
		else if(GetState() == STATE_ATTACKRIGHT)
		{
			cRect attack_area;
			attack_area.top = GetY()+GetBaseHeight();
			attack_area.bottom = GetY();
			attack_area.left = GetX()+GetBaseWidth();
			attack_area.right = GetX()+GetBaseWidth()+(TILE_SIZE/5)*7;

			if( enemy->GetState() != STATE_DYING && Intersection(attack_area,enemy->GetHitBox()) ) 
			{
				int new_health = enemy->GetHealth() - GetDamage();
				if(new_health <= 0) enemy->SetState(STATE_DYING);
				else enemy->SetState(STATE_DAMAGELEFT);
				if(new_health < 0) new_health = 0;
				enemy->SetHealth(new_health);
				enemy->SetFrame(0);
				enemy->SetDelay(0);
				hit = true;
			}
		}	
	}
	else if(GetState() == STATE_SUPERATTACK && (GetFrame() > 0 || GetFrame() < 5) && GetDelay() == 0)
	{
		if(GetFrame() == 1)
		{
			cRect attack_area;
			attack_area.top = GetY()+GetBaseHeight()+TILE_SIZE*2;
			attack_area.bottom = GetY();
			attack_area.left = GetX()-TILE_SIZE*2;
			attack_area.right = GetX()+GetBaseWidth();

			if( enemy->GetState() != STATE_DYING && Intersection(attack_area,enemy->GetHitBox()) ) 
			{
				int new_health = enemy->GetHealth() - GetDamage();
				if(new_health <= 0) enemy->SetState(STATE_DYING);
				else enemy->SetState(STATE_DAMAGEDOWN);
				if(new_health < 0) new_health = 0;
				enemy->SetHealth(new_health);
				enemy->SetFrame(0);
				enemy->SetDelay(0);
				hit = true;
			}
		}
		else if(GetFrame() == 2)
		{
			cRect attack_area;
			attack_area.top = GetY()+GetBaseHeight();
			attack_area.bottom = GetY()-TILE_SIZE;
			attack_area.left = GetX()-TILE_SIZE*2;
			attack_area.right = GetX()+GetBaseWidth();

			if( enemy->GetState() != STATE_DYING && Intersection(attack_area,enemy->GetHitBox()) ) 
			{
				int new_health = enemy->GetHealth() - GetDamage();
				if(new_health <= 0) enemy->SetState(STATE_DYING);
				else enemy->SetState(STATE_DAMAGERIGHT);
				if(new_health < 0) new_health = 0;
				enemy->SetHealth(new_health);
				enemy->SetFrame(0);
				enemy->SetDelay(0);
				hit = true;
			}
		}
		else if(GetFrame() == 3)
		{
			cRect attack_area;
			attack_area.top = GetY()+GetBaseHeight();
			attack_area.bottom = GetY()-TILE_SIZE;
			attack_area.left = GetX();
			attack_area.right = GetX()+GetBaseWidth()+TILE_SIZE*2;

			if( enemy->GetState() != STATE_DYING && Intersection(attack_area,enemy->GetHitBox()) ) 
			{
				int new_health = enemy->GetHealth() - GetDamage();
				if(new_health <= 0) enemy->SetState(STATE_DYING);
				else enemy->SetState(STATE_DAMAGEUP);
				if(new_health < 0) new_health = 0;
				enemy->SetHealth(new_health);
				enemy->SetFrame(0);
				enemy->SetDelay(0);
				hit = true;
			}
		}
		else if(GetFrame() == 4)
		{
			cRect attack_area;
			attack_area.top = GetY()+GetBaseHeight()+TILE_SIZE*2;
			attack_area.bottom = GetY();
			attack_area.left = GetX();
			attack_area.right = GetX()+GetBaseWidth()+TILE_SIZE*2;

			if( enemy->GetState() != STATE_DYING && Intersection(attack_area,enemy->GetHitBox()) ) 
			{
				int new_health = enemy->GetHealth() - GetDamage();
				if(new_health <= 0) enemy->SetState(STATE_DYING);
				else enemy->SetState(STATE_DAMAGELEFT);
				if(new_health < 0) new_health = 0;
				enemy->SetHealth(new_health);
				enemy->SetFrame(0);
				enemy->SetDelay(0);
				hit = true;
			}
		}
	}
	return hit;
}

void cPlayer1::SetShieldState(int input_state)
{
	//si el input_state era uno de skill en parado debemos transformarlo en uno de movimiento por si nos movieramos con las teclas, ya que el input_state se mantiene mientras se realiza la skill. Si nos mantuvieramos parados la funcion Stop nos corregiria el estado
	if(input_state == STATE_SKILLUP   || input_state == STATE_SKILLDOWN ||
	   input_state == STATE_SKILLLEFT || input_state == STATE_SKILLRIGHT )
	{
		if(input_state == STATE_SKILLUP) SetState(STATE_SKILLWALKUP);
		else if(input_state == STATE_SKILLDOWN) SetState(STATE_SKILLWALKDOWN);
		else if(input_state == STATE_SKILLLEFT) SetState(STATE_SKILLWALKLEFT);
		else if(input_state == STATE_SKILLRIGHT) SetState(STATE_SKILLWALKRIGHT);
	}
	else if(input_state == STATE_SKILLWALKUP   || input_state == STATE_SKILLWALKDOWN || 
			input_state == STATE_SKILLWALKLEFT || input_state == STATE_SKILLWALKRIGHT )
	{
		SetState(input_state);
	}
	//si el input_state no era de skill lo transformamos en uno de skill 
	else TransformIntoSkillState();
}

void cPlayer1::StopP1()
{
	Stop();
	SetLogicState(GetState());
}
void cPlayer1::StopDoingSkillP1()
{
	StopDoingSkill();
	SetLogicState(GetState());
}

void cPlayer1::TestShieldProtection(int attack_state,int damage,int x,int y)
{
	bool shield_useful = false;
	int damage_state;

	if(attack_state == STATE_ATTACKUP)
	{
		if( (GetState() == STATE_SKILLDOWN)                                                        || 
			(GetState() == STATE_SKILLWALKDOWN)                                                    || 
		    ((GetState() == STATE_SKILLLEFT || GetState() == STATE_SKILLWALKLEFT) &&  x < GetX())  ||
			((GetState() == STATE_SKILLRIGHT || GetState() == STATE_SKILLWALKRIGHT) &&  x > GetX()) ) shield_useful = true;
		damage_state = STATE_DAMAGEDOWN;
	}
	else if(attack_state == STATE_ATTACKDOWN)
	{
		if( (GetState() == STATE_SKILLUP)                                                          || 
			(GetState() == STATE_SKILLWALKUP)                                                      || 
		    ((GetState() == STATE_SKILLLEFT || GetState() == STATE_SKILLWALKLEFT) &&  x < GetX())  ||
			((GetState() == STATE_SKILLRIGHT || GetState() == STATE_SKILLWALKRIGHT) &&  x > GetX()) ) shield_useful = true;
		damage_state = STATE_DAMAGEUP;
	}
	else if(attack_state == STATE_ATTACKLEFT)
	{
		if( (GetState() == STATE_SKILLRIGHT)                                                       || 
			(GetState() == STATE_SKILLWALKRIGHT)                                                   || 
		    ((GetState() == STATE_SKILLDOWN || GetState() == STATE_SKILLWALKDOWN) &&  y < GetY())  ||
			((GetState() == STATE_SKILLUP || GetState() == STATE_SKILLWALKUP) &&  y > GetY())       ) shield_useful = true;
		damage_state = STATE_DAMAGERIGHT;
	}
	else if(attack_state == STATE_ATTACKRIGHT)
	{
		if( (GetState() == STATE_SKILLLEFT)                                                        || 
			(GetState() == STATE_SKILLWALKLEFT)                                                    || 
		    ((GetState() == STATE_SKILLDOWN || GetState() == STATE_SKILLWALKDOWN) &&  y < GetY())  ||
			((GetState() == STATE_SKILLUP || GetState() == STATE_SKILLWALKUP) &&  y > GetY())       ) shield_useful = true;
		damage_state = STATE_DAMAGELEFT;
	}
	
	if(shield_useful)
	{
		int new_sp = GetSP() - damage;
		if(new_sp >= 0) SetSP(new_sp);
		else
		{
			SetSP(0);
			int new_health = GetHealth() + new_sp;
			if(new_health <= 0)
			{
				SetHealth(0);
				SetState(STATE_DYING);
			}
			else
			{
				SetHealth(new_health);
				SetState(damage_state);
			}
			SetFrame(0);
			SetDelay(0);
		}
	}
	else
	{
		int new_health = GetHealth() - damage;
		if(new_health <= 0)
		{
			SetHealth(0);
			SetState(STATE_DYING);
		}
		else
		{
			SetHealth(new_health);
			SetState(damage_state);
		}
		SetFrame(0);
		SetDelay(0);
	}
}

void cPlayer1::SetLogicState(int state)
{
	logic_state = state;
}
int cPlayer1::GetLogicState()
{
	return logic_state;
}