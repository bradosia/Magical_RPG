#ifndef C_ACTOR_H
#define C_ACTOR_H

#include <ctime>
#include <chrono>

#include "../MR_graphic/cScene.h"
#include "../MR_logic/cCallbackTimer.h"
#include "../MR_logic/cProjectile.h"
#include "../MR_logic/cToken.h"

class cToken;
class cProjectile;
class cCallbackTimer;

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

#define CHARGE_BREAK   30
#define COOLDOWN		6 //frames de espera minima entre ataque y ataque

#define SUPERATTACK_COST 60
#define ATTACK_COST		 10

#define STATE_STANDING			30
#define STATE_SKILL				31
#define STATE_WALK				32
#define STATE_SKILLWALK			33
#define STATE_DAMAGE			34
#define STATE_ATTACK			35
#define STATE_SUPERATTACK		36
#define STATE_DYING				37

#define STATE_SUPERATTACK		18

#define STATE_SKILLUP			19
#define STATE_SKILLRIGHT		20
#define STATE_SKILLDOWN			21
#define STATE_SKILLLEFT			22

#define STATE_SKILLWALKUP		23
#define STATE_SKILLWALKRIGHT	24
#define STATE_SKILLWALKDOWN		25
#define STATE_SKILLWALKLEFT		26

/*
 * Actors: character with position, movement, model, and ability information.
 * Model: texture, animation, and frame information
 * Key Handler -> Key Map (user-defined) -> Key Action -> Main Actor -> Action Map (data-defined) ->
 * Sub Action
 */
class cActor {
public:
	int max_SP, SP, charge, cooldown, frame_walk_delay, frame_attack_delay,
			frame_texture_tiles, //the number in texture tiles that occupies the side of a frame of the sprite
			step_length, //TILE_SIZE has to be multiple of step_length
			logic_state, teamNumber, // same team numbers can't hurt each othe
			animationStateId, objID, rame_dying_delay;
	double faceXY, faceXZ, moveXY, moveXZ, moveSpeed // units per time
			;
	// bicho
	int x, y, //el pixel inferior izquierdo de la hitbox del bicho
			hit_w, hit_h, //dimensiones en pixeles de la parte golpeable de un frame del sprite, no es necesario que sean divisible por TILE_SIZE, pero en los sprites asociados la hitbox ha de estar en el centro/centro de cada frame
			base_tiles_w, base_tiles_h, //dimensiones en tiles de la base colisionable
			state, seq, delay, //seq es el frame de la sequencia de animacion que toca pintar ahora, delay es la espera de uno de esos frames a otro
			max_health, health, damage;

	std::chrono::microseconds moveTime;
	int animationMap[100];
	bool moveFlag, shieldFlag, deadFlag;
	std::vector<std::string> frontList;
	bool deleteFlag;
	std::vector<cCallbackTimer*>* callbackTimerList;
public:

	cActor(int objID_);
	~cActor();

	void NextFrame(int max);  // max es el total de frames de la animacion

	int GetFrameTop();
	int GetFrameBottom();
	int GetFrameTexTiles();

	bool CollidesWithEnemy(std::vector<cActor*> &enemies, int next_tile);
	bool CollidesUD(bool *collision_map, std::vector<cActor*> &enemies,
			bool up);
	bool CollidesLR(bool *collision_map, std::vector<cActor*> &enemies,
			bool right, bool vertical);

	void RestrictedMoveUp(bool *collision_map, std::vector<cActor*> &enemies,
			int h1, int h2, int game_height);
	void RestrictedMoveDown(bool *collision_map, std::vector<cActor*> &enemies,
			int h1, int h2, int game_height, int bottom_limit);
	void MoveUp(bool *collision_map, std::vector<cActor*> &enemies);
	void MoveDown(bool *collision_map, std::vector<cActor*> &enemies);
	void MoveRight(bool *collision_map, std::vector<cActor*> &enemies,
			bool vertical);
	void MoveLeft(bool *collision_map, std::vector<cActor*> &enemies,
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
	void setDead(bool flag_);
	bool getDead();

	void Stop();
	void StopDoingSkill();

	void setFaceDir(double xy_, double xz_);
	void setMoveDir(double xy_, double xz_);
	void setMoveFlag(bool flag_);
	/* performs a frontal attack with instant damage after a delay */
	void doAttackInstant(std::chrono::microseconds delay, int damage);
	/* creates a projectile in front after a delay */
	void doAttackProjectile(std::chrono::microseconds delay, int projId,
			std::chrono::microseconds speed, int damage);
	bool doHit(double xy_, double xz_, int damage_);
	bool testShieldProtection(double xy_, double xz_, int damage_);

	void SetCooldown(int c);
	int GetCooldown();

	/* proximity */
	void proximityProjectileActorEvent(cProjectile* projectilePtr);
	void proximityTokenActorEvent(cToken* tokenPtr);

	void front(std::string cmd_);
	void setDeleteFlag(bool flag_);
	bool getDeleteFlag();
	void setType(int objID_);
	int getType();

	/* ported */

	bool Attack(cActor* enemy);

	void setShieldState(bool flag_);
	bool getShieldState();

	void StopP1();
	void StopDoingSkillP1();

	void SetLogicState(int input_state);
	int GetLogicState();

	void setAnimationStateId(int animationStateId_);
	int getAnimationStateId();

	/* enemy */
	void SetFrameTexTiles(int ftt);

	void Move();
	// bicho
	void SetPosition(int x, int y);
	void SetX(int x);
	int GetX();
	void SetY(int y);
	int GetY();
	void SetBaseTilesHeight(int bth);
	int GetBaseHeight();
	void SetBaseTilesWidth(int btw);
	int GetBaseWidth();
	void SetHitHeight(int hh);
	int GetHitHeight();
	void SetHitWidth(int hw);
	int GetHitWidth();

	std::vector<int> GetTiles(); //nos devuelve las posiciones de las tiles con las que intersecta la base del cActor
	cRect GetHitBox();
	bool Intersection(cRect box1, cRect box2);
	bool Intersection(cRect box1, int px, int py);
	void SetTile(int tx, int ty);

	virtual void Draw(int tex_id, int tex_w, int tex_h, bool run) {
	}
	; //cuando llamemos a una funcion virtual se ejecutara la funcion del mismo nombre de la subclase correspondiente

	void Stop();

	void StartAttack();

	void SetState(int s);
	int GetState();

	void SetFrame(int frame);
	int GetFrame();

	void SetDelay(int d);
	int GetDelay();

	bool IsLooking();
	bool IsWalking();
	bool IsAttacking();
	bool IsDamaged();

	void SetMaxHealth(int max_h);
	int GetMaxHealth();
	void SetHealth(int health);
	int GetHealth();
	void SetDamage(int damage);
	int GetDamage();
};

#endif

