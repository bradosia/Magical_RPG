#ifndef C_PROJECTILE_H
#define C_PROJECTILE_H

#include "../MR_graphic/cScene.h"
#include "../MR_logic/cActor.h"

class cActor;

#define STATE_LEVITATEUP		1
#define STATE_LEVITATERIGHT		2
#define STATE_LEVITATEDOWN		3
#define STATE_LEVITATELEFT		4

#define STATE_ENDINGUP			5
#define STATE_ENDINGRIGHT		6
#define STATE_ENDINGDOWN		7
#define STATE_ENDINGLEFT		8

class cProjectile {
public:
	int x,
			y, //el pixel inferior izquierdo de la hitbox de la proyectil
			hit_w,
			hit_h, //dimensiones en pixeles de la parte golpeable de un frame del sprite, no es necesario que sean divisible por TILE_SIZE, pero en los sprites asociados la hitbox ha de estar en el centro/centro de cada frame
			state, seq, delay, damage, frame_delay, frame_texture_tiles,
			step_length, displace_up, displace_down, objID;
	std::vector<std::string> frontList;
	bool deleteFlag;
public:
	cProjectile(int objID_);
	virtual ~cProjectile();

	void SetPosition(int x, int y);
	int GetY();
	int GetX();
	void SetHitHeight(int hh);
	int GetHitHeight();
	void SetHitWidth(int hw);
	int GetHitWidth();
	void SetFrameTexTiles(int ftt);
	int GetFrameTexTiles();
	cRect GetFrameBox();
	cRect GetHitBox();
	bool Intersection(cRect box1, cRect box2);

	bool CollidesObjUD(bool *proj_collision_map, bool up);
	bool CollidesObjLR(bool *proj_collision_map, bool right);

	void LevitateUp(bool *proj_collision_map);
	void LevitateDown(bool *proj_collision_map);
	void LevitateRight(bool *proj_collision_map);
	void LevitateLeft(bool *proj_collision_map);

	bool ComputeEnemyCollisions(std::vector<cActor*> &actors);
	bool ComputeEnemyCollisions(std::vector<cActor*> &enemies);
	bool Logic(bool *proj_collision_map, std::vector<cActor*> &actors);
	bool Logic(bool *proj_collision_map, std::vector<cActor*> &enemies);

	void front(std::string cmd_);
	void proximityProjectileActorEvent(cActor* actorPtr);
	void setDeleteFlag(bool flag_);
	bool getDeleteFlag();
	void setType(int objID_);
	int getType();

	void SetState(int s);
	int GetState();

	void NextFrame(int max);
	int GetFrame();
	int GetDelay();

	void SetDamage(int d);
	int GetDamage();

	int GetStepLength();

	void SetDisplaceUp(bool b);
	void SetDisplaceDown(bool b);
	bool IsEnding();
};

#endif
