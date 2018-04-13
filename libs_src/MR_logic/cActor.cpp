#include "../MR_logic/cActor.h"

cActor::cActor(int objID_) {
	SetBaseTilesHeight(1);
	SetBaseTilesWidth(1);
	SetHitHeight(TILE_SIZE + TILE_SIZE / 2);
	SetHitWidth(TILE_SIZE);
	frame_walk_delay = 4;
	frame_attack_delay = 2;
	frame_texture_tiles = 4;
	step_length = STEP_LENGTH;
	cooldown = 0;
	/* ported */
	SetMaxHealth(150);
	SetHealth(150);
	SetMaxSP(150);
	SetSP(150);
	SetCharge(0);
	SetDamage(10);
	SetLogicState(STATE_LOOKDOWN);
	setAnimationStateId(STATE_STANDING);
	callbackTimerList = new std::vector<cCallbackTimer*>;
	//bicho
	seq = 0;
	delay = 0;
	state = STATE_LOOKDOWN;

	setType(objID_);
}
cActor::~cActor() {
}

void cActor::NextFrame(int max) {
	SetDelay(GetDelay() + 1);
	if (GetState() == STATE_ATTACKUP || GetState() == STATE_ATTACKRIGHT
			|| GetState() == STATE_ATTACKDOWN || GetState() == STATE_ATTACKLEFT
			|| GetState() == STATE_SUPERATTACK) {
		if (GetDelay() == frame_attack_delay) {
			SetFrame(GetFrame() + 1);
			SetFrame(GetFrame() % max);
			SetDelay(0);
		}
	} else {
		if (GetDelay() == frame_walk_delay) {
			SetFrame(GetFrame() + 1);
			SetFrame(GetFrame() % max);
			SetDelay(0);
		}
	}
}

int cActor::GetFrameTop() {
	return GetY() + GetHitHeight()
			+ (frame_texture_tiles * TILE_SIZE - GetHitHeight()) / 2;
}
int cActor::GetFrameBottom() {
	return GetY() - (frame_texture_tiles * TILE_SIZE - GetHitHeight()) / 2;
}

int cActor::GetFrameTexTiles() {
	return frame_texture_tiles;
}

bool cActor::CollidesWithEnemy(std::vector<cActor*> &enemies, int next_tile) {
	unsigned int i;
	std::vector<cActor*>::iterator it;
	for (it = enemies.begin(); it != enemies.end(); it++) {
		if ((*it)->GetState() != STATE_DYING) {
			std::vector<int> tiles;
			tiles = (*it)->GetTiles();
			for (i = 0; i < tiles.size(); i++)
				if (tiles[i] == next_tile)
					return true;
		}
	}
	return false;
}
bool cActor::CollidesUD(bool *collision_map, std::vector<cActor*> &enemies,
		bool up) {
	int tile_x, tile_y; //son las coords en tiles donde esta el bicho despues de dar el paso
	int width_tiles; //num de tiles en horizontal con las que colisiona

	tile_x = GetX() / TILE_SIZE;
	if (up && ((GetY() + GetBaseHeight()) / TILE_SIZE) < SCENE_HEIGHT)
		tile_y = (GetY() + GetBaseHeight()) / TILE_SIZE; //la comprobacion de SCENE_HEIGHT esta para evitar hacer calculos con posiciones fuera de la matriz de colision
	else
		tile_y = GetY() / TILE_SIZE;

	width_tiles = GetBaseWidth() / TILE_SIZE;
	if ((GetX() % TILE_SIZE) != 0)
		width_tiles++;

	//evaluo collisiones contra enemigos, no producen slide
	for (int j = 0; j < width_tiles; j++) {
		if (CollidesWithEnemy(enemies, (tile_x + j) + (tile_y * SCENE_WIDTH)))
			return true;
	}

	//evaluo collisiones contra el escenario, producen slide
	for (int j = 1; j < (width_tiles - 1); j++) {
		if (collision_map[(tile_x + j) + (tile_y * SCENE_WIDTH)])
			return true;
	}
	//si todas las tiles consultadas menos la ultima son traspasables y tengo potencial para girar a la izquierda, hago slideL
	if (!collision_map[(tile_x) + (tile_y * SCENE_WIDTH)]) {
		if (collision_map[(tile_x + (width_tiles - 1)) + (tile_y * SCENE_WIDTH)]) {
			if (GetState() != STATE_WALKLEFT && GetState() != STATE_WALKRIGHT
					&& GetState() != STATE_SKILLWALKLEFT
					&& GetState() != STATE_SKILLWALKRIGHT /*&& (x % TILE_SIZE) < TILE_SIZE/2*/)
				SetX(GetX() - step_length);
			return true;
		}
		return false;
	}
	//si todas las tiles consultadas menos la primera son traspasables y tengo potencial para girar a la derecha, hago slideR
	else {
		if (!collision_map[(tile_x + (width_tiles - 1)) + (tile_y * SCENE_WIDTH)]) {
			if (GetState() != STATE_WALKLEFT && GetState() != STATE_WALKRIGHT
					&& GetState() != STATE_SKILLWALKLEFT
					&& GetState() != STATE_SKILLWALKRIGHT /*&& (x % TILE_SIZE) >= TILE_SIZE/2*/)
				SetX(GetX() + step_length);
		}
		return true;
	}
}
bool cActor::CollidesLR(bool *collision_map, std::vector<cActor*> &enemies,
		bool right, bool vertical) {
	int tile_x, tile_y; //son las coords en tiles donde esta el bicho despues de dar el paso
	int height_tiles; //num de tiles en vertical con las que colisiona

	if (right && ((GetX() + GetBaseWidth()) / TILE_SIZE) < SCENE_WIDTH)
		tile_x = (GetX() + GetBaseWidth()) / TILE_SIZE;
	else
		tile_x = GetX() / TILE_SIZE;
	tile_y = GetY() / TILE_SIZE;

	height_tiles = GetBaseHeight() / TILE_SIZE;
	if ((GetY() % TILE_SIZE) != 0)
		height_tiles++;

	//evaluo collisiones contra enemigos, no producen slide
	for (int j = 0; j < height_tiles; j++) {
		if (CollidesWithEnemy(enemies, (tile_x) + ((tile_y + j) * SCENE_WIDTH)))
			return true;
	}

	//evaluo collisiones contra el escenario, producen slide
	for (int j = 1; j < (height_tiles - 1); j++) {
		return (collision_map[(tile_x) + ((tile_y + j) * SCENE_WIDTH)]);
	}
	//si todas las tiles consultadas menos la mas alta son traspasables y tengo potencial para ir hacia abajo, hago slideD
	if (!collision_map[(tile_x) + (tile_y * SCENE_WIDTH)]) {
		if (collision_map[(tile_x)
				+ ((tile_y + (height_tiles - 1)) * SCENE_WIDTH)]) {
			if (!vertical /*&& (y % TILE_SIZE) < TILE_SIZE/2*/)
				SetY(GetY() - step_length);
			return true;
		}
		return false;
	}
	//si todas las tiles consultadas menos la mas baja son traspasables y tengo potencial para ir hacia arriba, hago slideU
	else {
		if (!collision_map[(tile_x)
				+ ((tile_y + (height_tiles - 1)) * SCENE_WIDTH)]) {
			if (!vertical /*&& (y % TILE_SIZE) >= TILE_SIZE/2*/)
				SetY(GetY() + step_length);
		}
		return true;
	}
}

void cActor::RestrictedMoveUp(bool *collision_map,
		std::vector<cActor*> &enemies, int h1, int h2, int game_height) {
	if (h1 <= h2 || h1 - h2 <= game_height - TILE_SIZE)
		MoveUp(collision_map, enemies);
	else {
		if (GetState() != STATE_WALKLEFT && GetState() != STATE_WALKRIGHT
				&& GetState() != STATE_SKILLWALKLEFT
				&& GetState() != STATE_SKILLWALKRIGHT) {
			if (GetState() != STATE_WALKUP && GetState() != STATE_SKILLWALKUP) {
				if (IsDoingSkill())
					SetState(STATE_SKILLWALKUP);
				else
					SetState(STATE_WALKUP);
				SetFrame(0);
				SetDelay(0);
			}
		}
	}
}
void cActor::RestrictedMoveDown(bool *collision_map,
		std::vector<cActor*> &enemies, int h1, int h2, int game_height,
		int bottom_limit) {
	if (h1 > bottom_limit && (h1 > h2 || h2 - h1 <= game_height - TILE_SIZE))
		MoveDown(collision_map, enemies);
	else {
		if (GetState() != STATE_WALKLEFT && GetState() != STATE_WALKRIGHT
				&& GetState() != STATE_SKILLWALKLEFT
				&& GetState() != STATE_SKILLWALKRIGHT) {
			if (GetState() != STATE_WALKDOWN
					&& GetState() != STATE_SKILLWALKDOWN) {
				if (IsDoingSkill())
					SetState(STATE_SKILLWALKDOWN);
				else
					SetState(STATE_WALKDOWN);
				SetFrame(0);
				SetDelay(0);
			}
		}
	}
}
void cActor::MoveUp(bool *collision_map, std::vector<cActor*> &enemies) {
	int yaux;

	setAnimationStateId(STATE_WALK);
	//Whats next tile?
	if ((GetY() % TILE_SIZE) == 0) {
		yaux = GetY();
		SetY(GetY() + step_length);
		if (CollidesUD(collision_map, enemies, true))
			SetY(yaux);
	}
	//Advance, no problem
	else
		SetY(GetY() + step_length);
}
void cActor::MoveDown(bool *collision_map, std::vector<cActor*> &enemies) {
	int yaux;

	setAnimationStateId(STATE_WALK);
	//Whats next tile?
	if ((GetY() % TILE_SIZE) == 0) {
		yaux = GetY();
		SetY(GetY() - step_length);
		if (CollidesUD(collision_map, enemies, false))
			SetY(yaux);
	}
	//Advance, no problem
	else
		SetY(GetY() - step_length);
}
void cActor::MoveLeft(bool *collision_map, std::vector<cActor*> &enemies,
		bool vertical) {
	//std::cout << "GetX()=" << GetX() << " GetY()=" << GetY() << std::endl;
	int xaux;

	if (GetState() != STATE_WALKLEFT && GetState() != STATE_SKILLWALKLEFT) {
		if (IsDoingSkill())
			SetState(STATE_SKILLWALKLEFT);
		else
			SetState(STATE_WALKLEFT);
		SetFrame(0);
		SetDelay(0);
	}
	//Whats next tile?
	if ((GetX() % TILE_SIZE) == 0)   //habra un cambio de tile si me muevo
			{
		xaux = GetX();
		SetX(GetX() - step_length);
		if (CollidesLR(collision_map, enemies, false, vertical))
			SetX(xaux);
	}
	//Advance, no problem
	else
		SetX(GetX() - step_length);
}
void cActor::MoveRight(bool *collision_map, std::vector<cActor*> &enemies,
		bool vertical) {
	//std::cout << "GetX()=" << GetX() << " GetY()=" << GetY() << std::endl;
	int xaux;

	if (GetState() != STATE_WALKRIGHT && GetState() != STATE_SKILLWALKRIGHT) {
		if (IsDoingSkill())
			SetState(STATE_SKILLWALKRIGHT);
		else
			SetState(STATE_WALKRIGHT);
		SetFrame(0);
		SetDelay(0);
	}
	//Whats next tile?
	if ((GetX() % TILE_SIZE) == 0) {
		xaux = GetX();
		SetX(GetX() + step_length);

		if (CollidesLR(collision_map, enemies, true, vertical))
			SetX(xaux);
	}
	//Advance, no problem
	else
		SetX(GetX() + step_length);
}

void cActor::SetMaxSP(int max_sp) {
	max_SP = max_sp;
}
int cActor::GetMaxSP() {
	return max_SP;
}
void cActor::SetSP(int sp) {
	SP = sp;
}
int cActor::GetSP() {
	return SP;
}
void cActor::SetCharge(int c) {
	charge = c;
}
int cActor::GetCharge() {
	return charge;
}
void cActor::SetFrameAttackDelay(int fad) {
	frame_attack_delay = fad;
}
int cActor::GetFrameAttackDelay() {
	return frame_attack_delay;
}

void cActor::StartSuperAttack() {
	SetState(STATE_SUPERATTACK);

	SetFrame(0);
	SetDelay(0);
}
bool cActor::IsAttacking() {
	if (getAnimationStateId() == STATE_ATTACK
			|| getAnimationStateId() == STATE_SUPERATTACK)
		return true;
	return false;
}
bool cActor::IsSuperAttacking() {
	if (getAnimationStateId() == STATE_SUPERATTACK)
		return true;
	return false;
}
bool cActor::IsDoingSkill() {
	if (getAnimationStateId() == STATE_SKILL
			|| getAnimationStateId() == STATE_SKILLWALK)
		return true;
	return false;
}
bool cActor::IsWalking() {
	if (getAnimationStateId() == STATE_STANDING
			|| getAnimationStateId() == STATE_SKILLWALK)
		return true;
	return false;
}

void cActor::setDead(bool flag_) {
	deadFlag = flag_;
}

bool cActor::getDead() {
	return deadFlag;
}

void cActor::Stop() {
	setAnimationStateId(STATE_STANDING);
}
void cActor::StopDoingSkill() {
	setAnimationStateId(STATE_STANDING);
}

void cActor::setFaceDir(double xy_, double xz_) {
	faceXY = xy_;
	faceXZ = xz_;
}
void cActor::setMoveDir(double xy_, double xz_) {
}
void cActor::setMoveFlag(bool flag_) {
	moveFlag = flag_;
}
void cActor::doAttackInstant(std::chrono::microseconds delay, int damage) {
}
void cActor::doAttackProjectile(std::chrono::microseconds delay, int projId,
		std::chrono::microseconds speed, int damage) {
}

bool cActor::doHit(double xy_, double xz_, int damage_) {
	if (getShieldState() && testShieldProtection(xy_, xz_, damage_)) {
		return true;
	} else {
		int new_health = GetHealth() - damage_;
		if (new_health <= 0) {
			setDead(true);
			setAnimationStateId(STATE_DYING);
			SetFrame(0);
			SetDelay(0);
		} else {
			double xyInverse = xy_ - 180.0;
			if (xyInverse < 0)
				xyInverse += 360;
			double xzInverse = xz_ - 180.0;
			if (xzInverse < 0)
				xzInverse += 360;
			setFaceDir(xyInverse, xzInverse);
			setAnimationStateId(STATE_DAMAGE);
			SetFrame(0);
			SetDelay(0);
		}
		if (new_health < 0)
			new_health = 0;
		SetHealth(new_health);
	}
	return false;
}

bool cActor::testShieldProtection(double xy_, double xz_, int damage_) {
	// angles come from direction of projectile or attack
	bool shield_useful = false;

	double xyInverse = xy_ - 180.0;
	if (xyInverse < 0)
		xyInverse += 360;
	double xzInverse = xz_ - 180.0;
	if (xzInverse < 0)
		xzInverse += 360;

	if (faceXY == xyInverse) {
		shield_useful = true;
	}

	if (shield_useful) {
		int new_sp = GetSP() - damage_;
		if (new_sp >= 0) {
			SetSP(new_sp);
			return true;
		}
	}
	return false;
}

void cActor::SetCooldown(int c) {
	cooldown = c;
}
int cActor::GetCooldown() {
	return cooldown;
}

/* proximity */
void cActor::proximityProjectileActorEvent(cProjectile* projectilePtr) {
	front("takeHit");
}
void cActor::proximityTokenActorEvent(cToken* tokenPtr) {
	front("takePowerup");
}

void cActor::front(std::string cmd_) {
	frontList.push_back(cmd_);
}
void cActor::setDeleteFlag(bool flag_) {
	deleteFlag = flag_;
}
bool cActor::getDeleteFlag() {
	return deleteFlag;
}

void cActor::setType(int objID_) {
	objID = objID_;
	if (objID == 0) {
		//player1
		SetMaxHealth(150);
		SetHealth(150);
		SetMaxSP(150);
		SetSP(150);
		SetCharge(0);
		SetDamage(10);
		SetLogicState(STATE_LOOKDOWN);
	} else if (objID == 1) {
		//player2
		SetMaxHealth(100);
		SetHealth(100);
		SetMaxSP(150);
		SetSP(150);
		SetCharge(0);
	} else if (objID == 2) {
		//monster1
		SetBaseTilesHeight(1);
		SetBaseTilesWidth(1);
		SetHitHeight(TILE_SIZE + TILE_SIZE / 2);
		SetHitWidth(TILE_SIZE);
		SetFrameTexTiles(4);
		SetMaxHealth(30);
		SetHealth(30);
		SetDamage(10);
	} else if (objID == 3) {
		//monster2
		SetBaseTilesHeight(1);
		SetBaseTilesWidth(1);
		SetHitHeight(TILE_SIZE + TILE_SIZE / 2);
		SetHitWidth(TILE_SIZE);
		SetFrameTexTiles(4);
		SetMaxHealth(30);
		SetHealth(30);
	}
}
int cActor::getType() {
	return objID;
}

bool cActor::Attack(cActor* enemy) {
	bool hit = false;
	if (GetState() != STATE_SUPERATTACK && GetFrame() == 3 && GetDelay() == 0) {
		if (GetState() == STATE_ATTACKUP) {
			cRect attack_area;
			attack_area.top = GetY() + GetBaseHeight() + TILE_SIZE;
			attack_area.bottom = GetY() + GetBaseHeight();
			attack_area.left = GetX() - TILE_SIZE;
			attack_area.right = GetX() + GetBaseWidth() + TILE_SIZE;

			if (enemy->GetState() != STATE_DYING
					&& Intersection(attack_area, enemy->GetHitBox())) {
				int new_health = enemy->GetHealth() - GetDamage();
				if (new_health <= 0)
					enemy->SetState(STATE_DYING);
				else
					enemy->SetState(STATE_DAMAGEDOWN);
				if (new_health < 0)
					new_health = 0;
				enemy->SetHealth(new_health);
				enemy->SetFrame(0);
				enemy->SetDelay(0);
				hit = true;
			}
		} else if (GetState() == STATE_ATTACKDOWN) {
			cRect attack_area;
			attack_area.top = GetY();
			attack_area.bottom = GetY() - TILE_SIZE / 2;
			attack_area.left = GetX() - TILE_SIZE;
			attack_area.right = GetX() + GetBaseWidth() + TILE_SIZE;

			if (enemy->GetState() != STATE_DYING
					&& Intersection(attack_area, enemy->GetHitBox())) {
				int new_health = enemy->GetHealth() - GetDamage();
				if (new_health <= 0)
					enemy->SetState(STATE_DYING);
				else
					enemy->SetState(STATE_DAMAGEUP);
				if (new_health < 0)
					new_health = 0;
				enemy->SetHealth(new_health);
				enemy->SetFrame(0);
				enemy->SetDelay(0);
				hit = true;
			}
		} else if (GetState() == STATE_ATTACKLEFT) {
			cRect attack_area;
			attack_area.top = GetY() + GetBaseHeight();
			attack_area.bottom = GetY();
			attack_area.left = GetX() - (TILE_SIZE / 5) * 7;
			attack_area.right = GetX();

			if (enemy->GetState() != STATE_DYING
					&& Intersection(attack_area, enemy->GetHitBox())) {
				int new_health = enemy->GetHealth() - GetDamage();
				if (new_health <= 0)
					enemy->SetState(STATE_DYING);
				else
					enemy->SetState(STATE_DAMAGERIGHT);
				if (new_health < 0)
					new_health = 0;
				enemy->SetHealth(new_health);
				enemy->SetFrame(0);
				enemy->SetDelay(0);
				hit = true;
			}
		} else if (GetState() == STATE_ATTACKRIGHT) {
			cRect attack_area;
			attack_area.top = GetY() + GetBaseHeight();
			attack_area.bottom = GetY();
			attack_area.left = GetX() + GetBaseWidth();
			attack_area.right = GetX() + GetBaseWidth() + (TILE_SIZE / 5) * 7;

			if (enemy->GetState() != STATE_DYING
					&& Intersection(attack_area, enemy->GetHitBox())) {
				int new_health = enemy->GetHealth() - GetDamage();
				if (new_health <= 0)
					enemy->SetState(STATE_DYING);
				else
					enemy->SetState(STATE_DAMAGELEFT);
				if (new_health < 0)
					new_health = 0;
				enemy->SetHealth(new_health);
				enemy->SetFrame(0);
				enemy->SetDelay(0);
				hit = true;
			}
		}
	} else if (GetState() == STATE_SUPERATTACK
			&& (GetFrame() > 0 || GetFrame() < 5) && GetDelay() == 0) {
		if (GetFrame() == 1) {
			cRect attack_area;
			attack_area.top = GetY() + GetBaseHeight() + TILE_SIZE * 2;
			attack_area.bottom = GetY();
			attack_area.left = GetX() - TILE_SIZE * 2;
			attack_area.right = GetX() + GetBaseWidth();

			if (enemy->GetState() != STATE_DYING
					&& Intersection(attack_area, enemy->GetHitBox())) {
				int new_health = enemy->GetHealth() - GetDamage();
				if (new_health <= 0)
					enemy->SetState(STATE_DYING);
				else
					enemy->SetState(STATE_DAMAGEDOWN);
				if (new_health < 0)
					new_health = 0;
				enemy->SetHealth(new_health);
				enemy->SetFrame(0);
				enemy->SetDelay(0);
				hit = true;
			}
		} else if (GetFrame() == 2) {
			cRect attack_area;
			attack_area.top = GetY() + GetBaseHeight();
			attack_area.bottom = GetY() - TILE_SIZE;
			attack_area.left = GetX() - TILE_SIZE * 2;
			attack_area.right = GetX() + GetBaseWidth();

			if (enemy->GetState() != STATE_DYING
					&& Intersection(attack_area, enemy->GetHitBox())) {
				int new_health = enemy->GetHealth() - GetDamage();
				if (new_health <= 0)
					enemy->SetState(STATE_DYING);
				else
					enemy->SetState(STATE_DAMAGERIGHT);
				if (new_health < 0)
					new_health = 0;
				enemy->SetHealth(new_health);
				enemy->SetFrame(0);
				enemy->SetDelay(0);
				hit = true;
			}
		} else if (GetFrame() == 3) {
			cRect attack_area;
			attack_area.top = GetY() + GetBaseHeight();
			attack_area.bottom = GetY() - TILE_SIZE;
			attack_area.left = GetX();
			attack_area.right = GetX() + GetBaseWidth() + TILE_SIZE * 2;

			if (enemy->GetState() != STATE_DYING
					&& Intersection(attack_area, enemy->GetHitBox())) {
				int new_health = enemy->GetHealth() - GetDamage();
				if (new_health <= 0)
					enemy->SetState(STATE_DYING);
				else
					enemy->SetState(STATE_DAMAGEUP);
				if (new_health < 0)
					new_health = 0;
				enemy->SetHealth(new_health);
				enemy->SetFrame(0);
				enemy->SetDelay(0);
				hit = true;
			}
		} else if (GetFrame() == 4) {
			cRect attack_area;
			attack_area.top = GetY() + GetBaseHeight() + TILE_SIZE * 2;
			attack_area.bottom = GetY();
			attack_area.left = GetX();
			attack_area.right = GetX() + GetBaseWidth() + TILE_SIZE * 2;

			if (enemy->GetState() != STATE_DYING
					&& Intersection(attack_area, enemy->GetHitBox())) {
				int new_health = enemy->GetHealth() - GetDamage();
				if (new_health <= 0)
					enemy->SetState(STATE_DYING);
				else
					enemy->SetState(STATE_DAMAGELEFT);
				if (new_health < 0)
					new_health = 0;
				enemy->SetHealth(new_health);
				enemy->SetFrame(0);
				enemy->SetDelay(0);
				hit = true;
			}
		}
	}
	return hit;
}

void cActor::setShieldState(bool flag_) {
	/* set animation frame back to 0 during a toggle */
	if (flag_ == !shieldFlag) {
		SetFrame(0);
		SetDelay(0);
	}
	shieldFlag = flag_;
	if (flag_) {
		if (animationStateId == STATE_STANDING) {
			animationStateId = STATE_SKILL;
		} else if (animationStateId == STATE_WALK) {
			animationStateId = STATE_SKILLWALK;
		}
	} else {
		if (animationStateId == STATE_SKILL) {
			animationStateId = STATE_STANDING;
		} else if (animationStateId == STATE_SKILLWALK) {
			animationStateId = STATE_WALK;
		}
	}
}

bool cActor::getShieldState() {
	return shieldFlag;
}

void cActor::StopP1() {
	Stop();
	SetLogicState(GetState());
}
void cActor::StopDoingSkillP1() {
	StopDoingSkill();
	SetLogicState(GetState());
}

void cActor::SetLogicState(int state) {
	logic_state = state;
}
int cActor::GetLogicState() {
	return logic_state;
}

void cActor::setAnimationStateId(int animationStateId_) {
	animationStateId = animationStateId_;
}
int cActor::getAnimationStateId() {
	return animationStateId;
}

void cActor::SetFrameTexTiles(int ftt) {
	frame_texture_tiles = ftt;
}

void cActor::Move() {
	if (GetState() == STATE_WALKUP)
		SetY(GetY() + step_length);
	else if (GetState() == STATE_WALKDOWN)
		SetY(GetY() - step_length);
	else if (GetState() == STATE_WALKRIGHT)
		SetX(GetX() + step_length);
	else if (GetState() == STATE_WALKLEFT)
		SetX(GetX() - step_length);
}

void cActor::SetPosition(int posx, int posy) {
	x = posx;
	y = posy;
}
void cActor::SetX(int posx) {
	x = posx;
}
int cActor::GetX() {
	return x;
}
void cActor::SetY(int posy) {
	y = posy;
}
int cActor::GetY() {
	return y;
}
void cActor::SetBaseTilesHeight(int bth) {
	base_tiles_h = bth;
}
int cActor::GetBaseHeight() {
	return base_tiles_h * TILE_SIZE;
}
void cActor::SetBaseTilesWidth(int btw) {
	base_tiles_w = btw;
}
int cActor::GetBaseWidth() {
	return base_tiles_w * TILE_SIZE;
}
void cActor::SetHitHeight(int hh) {
	hit_h = hh;
}
int cActor::GetHitHeight() {
	return hit_h;
}
void cActor::SetHitWidth(int hw) {
	hit_w = hw;
}
int cActor::GetHitWidth() {
	return hit_w;
}
std::vector<int> cActor::GetTiles() //segun las dimensiones de la base
{
	int i, j;
	std::vector<int> tiles;
	int tile_x, tile_y;  //son las coords en tiles donde esta el bicho
	int occupied_tiles_w, occupied_tiles_h; //son el numero de tiles de terreno que ocupa la base del bicho

	tile_x = x / TILE_SIZE;
	tile_y = y / TILE_SIZE;
	occupied_tiles_w = base_tiles_w;
	occupied_tiles_h = base_tiles_h;
	if ((x % TILE_SIZE) != 0)
		occupied_tiles_w++;
	if ((y % TILE_SIZE) != 0)
		occupied_tiles_h++;

	for (i = 0; i < occupied_tiles_w; i++) {
		for (j = 0; j < occupied_tiles_h; j++)
			tiles.push_back((tile_x + i) + ((tile_y + j) * SCENE_WIDTH));
	}
	return tiles;
}

cRect cActor::GetHitBox() {
	cRect hitbox;
	hitbox.top = y + hit_h;
	hitbox.bottom = y;
	hitbox.left = x;
	hitbox.right = x + hit_w;
	return hitbox;
}
bool cActor::Intersection(cRect box1, cRect box2) {
	if (box1.top > box2.bottom && box2.top > box1.bottom) {
		if (box1.left < box2.right && box2.left < box1.right)
			return true;
	}
	return false;
}
bool cActor::Intersection(cRect box1, int px, int py) {
	if (px >= box1.left && px < box1.right && py >= box1.bottom
			&& py < box1.top)
		return true;
	return false;
}
void cActor::SetTile(int tx, int ty) {
	x = tx * TILE_SIZE;
	y = ty * TILE_SIZE;

}

void cActor::Stop() {
	switch (state) {
	case STATE_WALKUP:
		state = STATE_LOOKUP;
		break;
	case STATE_WALKRIGHT:
		state = STATE_LOOKRIGHT;
		break;
	case STATE_WALKDOWN:
		state = STATE_LOOKDOWN;
		break;
	case STATE_WALKLEFT:
		state = STATE_LOOKLEFT;
		break;

	case STATE_ATTACKUP:
		state = STATE_LOOKUP;
		break;
	case STATE_ATTACKRIGHT:
		state = STATE_LOOKRIGHT;
		break;
	case STATE_ATTACKDOWN:
		state = STATE_LOOKDOWN;
		break;
	case STATE_ATTACKLEFT:
		state = STATE_LOOKLEFT;
		break;

	case STATE_DAMAGEUP:
		state = STATE_LOOKUP;
		break;
	case STATE_DAMAGERIGHT:
		state = STATE_LOOKRIGHT;
		break;
	case STATE_DAMAGEDOWN:
		state = STATE_LOOKDOWN;
		break;
	case STATE_DAMAGELEFT:
		state = STATE_LOOKLEFT;
		break;
	}
}

void cActor::StartAttack() {
	if (state == STATE_LOOKUP || state == STATE_WALKUP)
		state = STATE_ATTACKUP;
	if (state == STATE_LOOKDOWN || state == STATE_WALKDOWN)
		state = STATE_ATTACKDOWN;
	if (state == STATE_LOOKLEFT || state == STATE_WALKLEFT)
		state = STATE_ATTACKLEFT;
	if (state == STATE_LOOKRIGHT || state == STATE_WALKRIGHT)
		state = STATE_ATTACKRIGHT;

	seq = 0;
	delay = 0;
}

void cActor::SetFrame(int frame) {
	seq = frame;
}
int cActor::GetFrame() {
	return seq;
}
int cActor::GetState() {
	return state;
}
void cActor::SetState(int s) {
	state = s;
}
void cActor::SetDelay(int d) {
	delay = d;
}
int cActor::GetDelay() {
	return delay;
}

void cActor::SetMaxHealth(int max_h) {
	max_health = max_h;
}
int cActor::GetMaxHealth() {
	return max_health;
}
void cActor::SetHealth(int h) {
	health = h;
}
int cActor::GetHealth() {
	return health;
}
void cActor::SetDamage(int d) {
	damage = d;
}
int cActor::GetDamage() {
	return damage;
}

bool cActor::IsLooking() {
	if (state == STATE_LOOKUP || state == STATE_LOOKDOWN
			|| state == STATE_LOOKLEFT || state == STATE_LOOKRIGHT)
		return true;
	return false;
}
bool cActor::IsWalking() {
	if (state == STATE_WALKUP || state == STATE_WALKDOWN
			|| state == STATE_WALKLEFT || state == STATE_WALKRIGHT)
		return true;
	return false;
}
bool cActor::IsAttacking() {
	if (state == STATE_ATTACKUP || state == STATE_ATTACKDOWN
			|| state == STATE_ATTACKLEFT || state == STATE_ATTACKRIGHT)
		return true;
	return false;
}
bool cActor::IsDamaged() {
	if (state == STATE_DAMAGEUP || state == STATE_DAMAGEDOWN
			|| state == STATE_DAMAGELEFT || state == STATE_DAMAGERIGHT)
		return true;
	return false;
}

