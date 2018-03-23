#pragma once

#include "cScene.h"

#define STEP_LENGTH	  (TILE_SIZE/8)

#define STATE_LOOKUP		1
#define STATE_LOOKRIGHT		2
#define STATE_LOOKDOWN		3
#define STATE_LOOKLEFT		4

#define STATE_WALKUP		6
#define STATE_WALKRIGHT		7
#define STATE_WALKDOWN		8
#define STATE_WALKLEFT		9

#define STATE_DAMAGEUP		10
#define STATE_DAMAGERIGHT	11
#define STATE_DAMAGEDOWN	12
#define STATE_DAMAGELEFT	13

#define STATE_ATTACKUP		14
#define STATE_ATTACKRIGHT	15
#define STATE_ATTACKDOWN	16
#define STATE_ATTACKLEFT	17

#define STATE_DYING			0

class cBicho
{
public:
	cBicho(void);
	virtual ~cBicho(void);

	void SetPosition(int x,int y);
	void SetX(int x);
	int  GetX();
	void SetY(int y);
	int  GetY();
	void SetBaseTilesHeight(int bth);
	int  GetBaseHeight();
	void SetBaseTilesWidth(int btw);
	int  GetBaseWidth();
	void SetHitHeight(int hh);
	int GetHitHeight();
	void SetHitWidth(int hw);
	int GetHitWidth();


	std::vector<int> GetTiles(); //nos devuelve las posiciones de las tiles con las que intersecta la base del cBicho  
	cRect GetHitBox();
	bool Intersection(cRect box1, cRect box2);
	bool Intersection(cRect box1,int px,int py);
	void SetTile(int tx,int ty);

	virtual void Draw(int tex_id,int tex_w,int tex_h,bool run) {}; //cuando llamemos a una funcion virtual se ejecutara la funcion del mismo nombre de la subclase correspondiente

	void Stop();

	void StartAttack();

	void SetState(int s);
	int  GetState();

	void SetFrame(int frame);
	int  GetFrame();

	void SetDelay(int d);
	int  GetDelay();

	bool IsLooking();
	bool IsWalking();
	bool IsAttacking();
	bool IsDamaged();
	
	void SetMaxHealth(int max_h);
	int  GetMaxHealth();
	void SetHealth(int health);
	int  GetHealth();
	void SetDamage(int damage);
	int  GetDamage();
	
private:
	int x,y, //el pixel inferior izquierdo de la hitbox del bicho
		hit_w,hit_h, //dimensiones en pixeles de la parte golpeable de un frame del sprite, no es necesario que sean divisible por TILE_SIZE, pero en los sprites asociados la hitbox ha de estar en el centro/centro de cada frame 
		base_tiles_w, base_tiles_h, //dimensiones en tiles de la base colisionable
		state,
		seq,delay, //seq es el frame de la sequencia de animacion que toca pintar ahora, delay es la espera de uno de esos frames a otro
		max_health,health,damage;
};
