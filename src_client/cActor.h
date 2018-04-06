#pragma once

#include "cBicho.h"
#include "cFireball.h"
#include "cArrow.h"

#define CHARGE_BREAK   30
#define COOLDOWN		6 //frames de espera minima entre ataque y ataque

#define SUPERATTACK_COST 60
#define ATTACK_COST		 10

#define STATE_SUPERATTACK	 18

#define STATE_SKILLUP		 19
#define STATE_SKILLRIGHT	 20
#define STATE_SKILLDOWN		 21
#define STATE_SKILLLEFT		 22

#define STATE_SKILLWALKUP	 23
#define STATE_SKILLWALKRIGHT 24
#define STATE_SKILLWALKDOWN	 25
#define STATE_SKILLWALKLEFT	 26

class cActor: public cBicho {
private:
	int max_SP, SP, charge, cooldown, frame_walk_delay, frame_attack_delay,
			frame_texture_tiles, //the number in texture tiles that occupies the side of a frame of the sprite
			step_length, //TILE_SIZE has to be multiple of step_length
			logic_state,
			teamNumber; // same team numbers can't hurt each other
public:
	cActor();
	~cActor();

	void DrawRect(int tex_id, float xo, float yo, float xf, float yf);
	void NextFrame(int max);  // max es el total de frames de la animacion

	int GetFrameTop();
	int GetFrameBottom();
	int GetFrameTexTiles();

	bool CollidesWithEnemy(std::list<cBicho*> &enemies, int next_tile);
	bool CollidesUD(bool *collision_map, std::list<cBicho*> &enemies, bool up);
	bool CollidesLR(bool *collision_map, std::list<cBicho*> &enemies,
			bool right, bool vertical);

	void RestrictedMoveUp(bool *collision_map, std::list<cBicho*> &enemies,
			int h1, int h2, int game_height);
	void RestrictedMoveDown(bool *collision_map, std::list<cBicho*> &enemies,
			int h1, int h2, int game_height, int bottom_limit);
	void MoveUp(bool *collision_map, std::list<cBicho*> &enemies);
	void MoveDown(bool *collision_map, std::list<cBicho*> &enemies);
	void MoveRight(bool *collision_map, std::list<cBicho*> &enemies,
			bool vertical);
	void MoveLeft(bool *collision_map, std::list<cBicho*> &enemies,
			bool vertical);

	void SetMaxSP(int max_sp);
	int GetMaxSP();
	void SetSP(int sp);
	int GetSP();
	void SetCharge(int c);
	int GetCharge();
	void SetFrameAttackDelay(int fad);
	int GetFrameAttackDelay();

	void StartSuperAttack();
	bool IsAttacking();
	bool IsSuperAttacking();
	bool IsDoingSkill();
	bool IsWalking();

	void TransformIntoSkillState();

	void Stop();
	void StopDoingSkill();

	void SetCooldown(int c);
	int GetCooldown();

	/* ported */
	void Draw(int tex_id, int tex_w, int tex_h, bool run);

	bool Attack(cBicho* enemy);

	void SetShieldState(int render_state);

	void StopP1();
	void StopDoingSkillP1();

	void TestShieldProtection(int attack_state, int damage, int x, int y);

	void SetLogicState(int input_state);
	int GetLogicState();
};
