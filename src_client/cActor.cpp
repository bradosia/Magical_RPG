#include "cActor.h"

cActor::cActor() {
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
}
cActor::~cActor() {
}

void cActor::DrawRect(int tex_id, float xo, float yo, float xf, float yf) {
	int screen_x, screen_y;
	cRect hitbox = GetHitBox();
	int hitbox_w, hitbox_h;

	screen_x = GetX() + SCENE_Xo;
	screen_y = GetY() + SCENE_Yo;
	hitbox_w = hitbox.right - hitbox.left;
	hitbox_h = hitbox.top - hitbox.bottom;

	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, tex_id);
	glBegin(GL_QUADS);
	/*std::cout << "xo=" << xo << " yo=" << yo << " xf=" << xf << " yf=" << yf << std::endl;
	 std::cout << "screen_x=" << screen_x << " screen_y=" << screen_y << " GetX()=" << GetX() << " GetY()=" << GetY() << std::endl;*/
	glTexCoord2f(xo, yo);
	glVertex3i(screen_x - (TILE_SIZE * frame_texture_tiles - hitbox_w) / 2,
			screen_y - (TILE_SIZE * frame_texture_tiles - hitbox_h) / 2,
			screen_y);
	glTexCoord2f(xf, yo);
	glVertex3i(screen_x + (TILE_SIZE * frame_texture_tiles + hitbox_w) / 2,
			screen_y - (TILE_SIZE * frame_texture_tiles - hitbox_h) / 2,
			screen_y);
	glTexCoord2f(xf, yf);
	glVertex3i(screen_x + (TILE_SIZE * frame_texture_tiles + hitbox_w) / 2,
			screen_y + (TILE_SIZE * frame_texture_tiles + hitbox_h) / 2,
			screen_y);
	glTexCoord2f(xo, yf);
	glVertex3i(screen_x - (TILE_SIZE * frame_texture_tiles - hitbox_w) / 2,
			screen_y + (TILE_SIZE * frame_texture_tiles + hitbox_h) / 2,
			screen_y);
	glEnd();

	glDisable(GL_TEXTURE_2D);
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

bool cActor::CollidesWithEnemy(std::list<cBicho*> &enemies, int next_tile) {
	unsigned int i;
	std::list<cBicho*>::iterator it;
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
bool cActor::CollidesUD(bool *collision_map, std::list<cBicho*> &enemies,
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
bool cActor::CollidesLR(bool *collision_map, std::list<cBicho*> &enemies,
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

void cActor::RestrictedMoveUp(bool *collision_map, std::list<cBicho*> &enemies,
		int h1, int h2, int game_height) {
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
		std::list<cBicho*> &enemies, int h1, int h2, int game_height,
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
void cActor::MoveUp(bool *collision_map, std::list<cBicho*> &enemies) {
	//std::cout << "GetX()=" << GetX() << " GetY()=" << GetY() << std::endl;
	int yaux;

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
void cActor::MoveDown(bool *collision_map, std::list<cBicho*> &enemies) {
	//std::cout << "GetX()=" << GetX() << " GetY()=" << GetY() << std::endl;
	int yaux;

	if (GetState() != STATE_WALKLEFT && GetState() != STATE_WALKRIGHT
			&& GetState() != STATE_SKILLWALKLEFT
			&& GetState() != STATE_SKILLWALKRIGHT) {
		if (GetState() != STATE_WALKDOWN && GetState() != STATE_SKILLWALKDOWN) {
			if (IsDoingSkill())
				SetState(STATE_SKILLWALKDOWN);
			else
				SetState(STATE_WALKDOWN);
			SetFrame(0);
			SetDelay(0);
		}
	}
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
void cActor::MoveLeft(bool *collision_map, std::list<cBicho*> &enemies,
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
void cActor::MoveRight(bool *collision_map, std::list<cBicho*> &enemies,
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
	if (GetState() == STATE_ATTACKUP || GetState() == STATE_ATTACKDOWN
			|| GetState() == STATE_ATTACKLEFT || GetState() == STATE_ATTACKRIGHT
			|| GetState() == STATE_SUPERATTACK)
		return true;
	return false;
}
bool cActor::IsSuperAttacking() {
	if (GetState() == STATE_SUPERATTACK)
		return true;
	return false;
}
bool cActor::IsDoingSkill() {
	if (GetState() == STATE_SKILLUP || GetState() == STATE_SKILLDOWN
			|| GetState() == STATE_SKILLLEFT || GetState() == STATE_SKILLRIGHT
			|| GetState() == STATE_SKILLWALKUP
			|| GetState() == STATE_SKILLWALKDOWN
			|| GetState() == STATE_SKILLWALKLEFT
			|| GetState() == STATE_SKILLWALKRIGHT)
		return true;
	return false;
}
bool cActor::IsWalking() {
	if (GetState() == STATE_WALKUP || GetState() == STATE_WALKDOWN
			|| GetState() == STATE_WALKLEFT || GetState() == STATE_WALKRIGHT
			|| GetState() == STATE_SKILLWALKUP
			|| GetState() == STATE_SKILLWALKDOWN
			|| GetState() == STATE_SKILLWALKLEFT
			|| GetState() == STATE_SKILLWALKRIGHT)
		return true;
	return false;
}

void cActor::TransformIntoSkillState() {
	switch (GetState()) {
	case STATE_LOOKUP:
		SetState(STATE_SKILLUP);
		break;
	case STATE_LOOKRIGHT:
		SetState(STATE_SKILLRIGHT);
		break;
	case STATE_LOOKDOWN:
		SetState(STATE_SKILLDOWN);
		break;
	case STATE_LOOKLEFT:
		SetState(STATE_SKILLLEFT);
		break;

	case STATE_WALKUP:
		SetState(STATE_SKILLWALKUP);
		break;
	case STATE_WALKRIGHT:
		SetState(STATE_SKILLWALKRIGHT);
		break;
	case STATE_WALKDOWN:
		SetState(STATE_SKILLWALKDOWN);
		break;
	case STATE_WALKLEFT:
		SetState(STATE_SKILLWALKLEFT);
		break;
	}
	SetFrame(0);
	SetDelay(0);
}

void cActor::Stop() {
	switch (GetState()) {
	case STATE_WALKUP:
		SetState(STATE_LOOKUP);
		break;
	case STATE_WALKRIGHT:
		SetState(STATE_LOOKRIGHT);
		break;
	case STATE_WALKDOWN:
		SetState(STATE_LOOKDOWN);
		break;
	case STATE_WALKLEFT:
		SetState(STATE_LOOKLEFT);
		break;

	case STATE_SKILLWALKUP:
		SetState(STATE_SKILLUP);
		break;
	case STATE_SKILLWALKRIGHT:
		SetState(STATE_SKILLRIGHT);
		break;
	case STATE_SKILLWALKDOWN:
		SetState(STATE_SKILLDOWN);
		break;
	case STATE_SKILLWALKLEFT:
		SetState(STATE_SKILLLEFT);
		break;

	case STATE_ATTACKUP:
		SetState(STATE_LOOKUP);
		break;
	case STATE_ATTACKRIGHT:
		SetState(STATE_LOOKRIGHT);
		break;
	case STATE_ATTACKDOWN:
		SetState(STATE_LOOKDOWN);
		break;
	case STATE_ATTACKLEFT:
		SetState(STATE_LOOKLEFT);
		break;

	case STATE_DAMAGEUP:
		SetState(STATE_LOOKUP);
		break;
	case STATE_DAMAGERIGHT:
		SetState(STATE_LOOKRIGHT);
		break;
	case STATE_DAMAGEDOWN:
		SetState(STATE_LOOKDOWN);
		break;
	case STATE_DAMAGELEFT:
		SetState(STATE_LOOKLEFT);
		break;

	case STATE_SUPERATTACK:
		SetState(STATE_LOOKDOWN);
		break;
	}
}
void cActor::StopDoingSkill() {
	switch (GetState()) {
	case STATE_SKILLUP:
		SetState(STATE_LOOKUP);
		break;
	case STATE_SKILLRIGHT:
		SetState(STATE_LOOKRIGHT);
		break;
	case STATE_SKILLDOWN:
		SetState(STATE_LOOKDOWN);
		break;
	case STATE_SKILLLEFT:
		SetState(STATE_LOOKLEFT);
		break;

	case STATE_SKILLWALKUP:
		SetState(STATE_LOOKUP);
		break;
	case STATE_SKILLWALKRIGHT:
		SetState(STATE_LOOKRIGHT);
		break;
	case STATE_SKILLWALKDOWN:
		SetState(STATE_LOOKDOWN);
		break;
	case STATE_SKILLWALKLEFT:
		SetState(STATE_LOOKLEFT);
		break;
	}
}

void cActor::SetCooldown(int c) {
	cooldown = c;
}
int cActor::GetCooldown() {
	return cooldown;
}

void cActor::Draw(int tex_id, int tex_w, int tex_h, bool run) {
	float xo, yo, xf, yf;
	bool reverse = false;
	float tex_offset_x, tex_offset_y;

	tex_offset_x = 1.0f
			/ float(tex_w / (GetFrameTexTiles() * TEXTURE_TILE_SIZE));
	tex_offset_y = 1.0f
			/ float(tex_h / (GetFrameTexTiles() * TEXTURE_TILE_SIZE));

	switch (GetState()) {
	case STATE_LOOKUP:
		xo = tex_offset_x * 1;
		yo = tex_offset_y + tex_offset_y * 0;
		break;
	case STATE_LOOKRIGHT:
		xo = tex_offset_x * 2;
		yo = tex_offset_y + tex_offset_y * 0;
		break;
	case STATE_LOOKDOWN:
		xo = tex_offset_x * 0;
		yo = tex_offset_y + tex_offset_y * 0;
		break;
	case STATE_LOOKLEFT:
		xo = tex_offset_x * 2;
		yo = tex_offset_y + tex_offset_y * 0;
		reverse = true;
		break;

	case STATE_SKILLUP:
		xo = tex_offset_x * 1;
		yo = tex_offset_y + tex_offset_y * 9 + (GetFrame() * tex_offset_y);
		if (run)
			NextFrame(2);
		break;
	case STATE_SKILLRIGHT:
		xo = tex_offset_x * 2;
		yo = tex_offset_y + tex_offset_y * 9 + (GetFrame() * tex_offset_y);
		if (run)
			NextFrame(2);
		break;
	case STATE_SKILLDOWN:
		xo = tex_offset_x * 0;
		yo = tex_offset_y + tex_offset_y * 9 + (GetFrame() * tex_offset_y);
		if (run)
			NextFrame(2);
		break;
	case STATE_SKILLLEFT:
		xo = tex_offset_x * 2;
		yo = tex_offset_y + tex_offset_y * 9 + (GetFrame() * tex_offset_y);
		if (run)
			NextFrame(2);
		reverse = true;
		break;

	case STATE_WALKUP:
		xo = tex_offset_x * 1;
		yo = tex_offset_y + tex_offset_y * 1 + (GetFrame() * tex_offset_y);
		if (run)
			NextFrame(4);
		break;
	case STATE_WALKRIGHT:
		xo = tex_offset_x * 2;
		yo = tex_offset_y + tex_offset_y * 1 + (GetFrame() * tex_offset_y);
		if (run)
			NextFrame(4);
		break;
	case STATE_WALKDOWN:
		xo = tex_offset_x * 0;
		yo = tex_offset_y + tex_offset_y * 1 + (GetFrame() * tex_offset_y);
		if (run)
			NextFrame(4);
		break;
	case STATE_WALKLEFT:
		xo = tex_offset_x * 2;
		yo = tex_offset_y + tex_offset_y * 1 + (GetFrame() * tex_offset_y);
		if (run)
			NextFrame(4);
		reverse = true;
		break;

	case STATE_SKILLWALKUP:
		xo = tex_offset_x * 1;
		yo = tex_offset_y + tex_offset_y * 10 + (GetFrame() * tex_offset_y);
		if (run)
			NextFrame(2);
		break;
	case STATE_SKILLWALKRIGHT:
		xo = tex_offset_x * 2;
		yo = tex_offset_y + tex_offset_y * 10 + (GetFrame() * tex_offset_y);
		if (run)
			NextFrame(2);
		break;
	case STATE_SKILLWALKDOWN:
		xo = tex_offset_x * 0;
		yo = tex_offset_y + tex_offset_y * 10 + (GetFrame() * tex_offset_y);
		if (run)
			NextFrame(2);
		break;
	case STATE_SKILLWALKLEFT:
		xo = tex_offset_x * 2;
		yo = tex_offset_y + tex_offset_y * 10 + (GetFrame() * tex_offset_y);
		if (run)
			NextFrame(2);
		reverse = true;
		break;

	case STATE_DAMAGEUP:
		xo = tex_offset_x * 1;
		yo = tex_offset_y + tex_offset_y * 12 + (GetFrame() * tex_offset_y);
		if (run)
			NextFrame(2);
		break;
	case STATE_DAMAGERIGHT:
		xo = tex_offset_x * 2;
		yo = tex_offset_y + tex_offset_y * 12 + (GetFrame() * tex_offset_y);
		if (run)
			NextFrame(2);
		break;
	case STATE_DAMAGEDOWN:
		xo = tex_offset_x * 0;
		yo = tex_offset_y + tex_offset_y * 12 + (GetFrame() * tex_offset_y);
		if (run)
			NextFrame(2);
		break;
	case STATE_DAMAGELEFT:
		xo = tex_offset_x * 2;
		yo = tex_offset_y + tex_offset_y * 12 + (GetFrame() * tex_offset_y);
		if (run)
			NextFrame(2);
		reverse = true;
		break;

	case STATE_ATTACKUP:
		xo = tex_offset_x * 1;
		yo = tex_offset_y + tex_offset_y * 5 + (GetFrame() * tex_offset_y);
		if (run)
			NextFrame(4);
		break;
	case STATE_ATTACKRIGHT:
		xo = tex_offset_x * 2;
		yo = tex_offset_y + tex_offset_y * 5 + (GetFrame() * tex_offset_y);
		if (run)
			NextFrame(4);
		break;
	case STATE_ATTACKDOWN:
		xo = tex_offset_x * 0;
		yo = tex_offset_y + tex_offset_y * 5 + (GetFrame() * tex_offset_y);
		if (run)
			NextFrame(4);
		break;
	case STATE_ATTACKLEFT:
		xo = tex_offset_x * 2;
		yo = tex_offset_y + tex_offset_y * 5 + (GetFrame() * tex_offset_y);
		reverse = true;
		if (run)
			NextFrame(4);
		break;

	case STATE_SUPERATTACK:
		xo = tex_offset_x * 3;
		yo = tex_offset_y + tex_offset_y * 1 + (GetFrame() * tex_offset_y);
		if (run)
			NextFrame(7);
		break;

	case STATE_DYING:
		xo = tex_offset_x * 3;
		yo = tex_offset_y + tex_offset_y * 0;
		break;
	}
	xf = xo + tex_offset_x;
	yf = yo - tex_offset_y;
	if (reverse) {
		float aux = xo;
		xo = xf;
		xf = aux;
	}
	DrawRect(tex_id, xo, yo, xf, yf);
}

bool cActor::Attack(cBicho* enemy) {
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

void cActor::SetShieldState(int input_state) {
	//si el input_state era uno de skill en parado debemos transformarlo en uno de movimiento por si nos movieramos con las teclas, ya que el input_state se mantiene mientras se realiza la skill. Si nos mantuvieramos parados la funcion Stop nos corregiria el estado
	if (input_state == STATE_SKILLUP || input_state == STATE_SKILLDOWN
			|| input_state == STATE_SKILLLEFT || input_state == STATE_SKILLRIGHT) {
		if (input_state == STATE_SKILLUP)
			SetState(STATE_SKILLWALKUP);
		else if (input_state == STATE_SKILLDOWN)
			SetState(STATE_SKILLWALKDOWN);
		else if (input_state == STATE_SKILLLEFT)
			SetState(STATE_SKILLWALKLEFT);
		else if (input_state == STATE_SKILLRIGHT)
			SetState(STATE_SKILLWALKRIGHT);
	} else if (input_state == STATE_SKILLWALKUP
			|| input_state == STATE_SKILLWALKDOWN
			|| input_state == STATE_SKILLWALKLEFT
			|| input_state == STATE_SKILLWALKRIGHT) {
		SetState(input_state);
	}
	//si el input_state no era de skill lo transformamos en uno de skill
	else
		TransformIntoSkillState();
}

void cActor::StopP1() {
	Stop();
	SetLogicState(GetState());
}
void cActor::StopDoingSkillP1() {
	StopDoingSkill();
	SetLogicState(GetState());
}

void cActor::TestShieldProtection(int attack_state, int damage, int x,
		int y) {
	bool shield_useful = false;
	int damage_state;

	if (attack_state == STATE_ATTACKUP) {
		if ((GetState() == STATE_SKILLDOWN)
				|| (GetState() == STATE_SKILLWALKDOWN)
				|| ((GetState() == STATE_SKILLLEFT
						|| GetState() == STATE_SKILLWALKLEFT) && x < GetX())
				|| ((GetState() == STATE_SKILLRIGHT
						|| GetState() == STATE_SKILLWALKRIGHT) && x > GetX()))
			shield_useful = true;
		damage_state = STATE_DAMAGEDOWN;
	} else if (attack_state == STATE_ATTACKDOWN) {
		if ((GetState() == STATE_SKILLUP) || (GetState() == STATE_SKILLWALKUP)
				|| ((GetState() == STATE_SKILLLEFT
						|| GetState() == STATE_SKILLWALKLEFT) && x < GetX())
				|| ((GetState() == STATE_SKILLRIGHT
						|| GetState() == STATE_SKILLWALKRIGHT) && x > GetX()))
			shield_useful = true;
		damage_state = STATE_DAMAGEUP;
	} else if (attack_state == STATE_ATTACKLEFT) {
		if ((GetState() == STATE_SKILLRIGHT)
				|| (GetState() == STATE_SKILLWALKRIGHT)
				|| ((GetState() == STATE_SKILLDOWN
						|| GetState() == STATE_SKILLWALKDOWN) && y < GetY())
				|| ((GetState() == STATE_SKILLUP
						|| GetState() == STATE_SKILLWALKUP) && y > GetY()))
			shield_useful = true;
		damage_state = STATE_DAMAGERIGHT;
	} else if (attack_state == STATE_ATTACKRIGHT) {
		if ((GetState() == STATE_SKILLLEFT)
				|| (GetState() == STATE_SKILLWALKLEFT)
				|| ((GetState() == STATE_SKILLDOWN
						|| GetState() == STATE_SKILLWALKDOWN) && y < GetY())
				|| ((GetState() == STATE_SKILLUP
						|| GetState() == STATE_SKILLWALKUP) && y > GetY()))
			shield_useful = true;
		damage_state = STATE_DAMAGELEFT;
	}

	if (shield_useful) {
		int new_sp = GetSP() - damage;
		if (new_sp >= 0)
			SetSP(new_sp);
		else {
			SetSP(0);
			int new_health = GetHealth() + new_sp;
			if (new_health <= 0) {
				SetHealth(0);
				SetState(STATE_DYING);
			} else {
				SetHealth(new_health);
				SetState(damage_state);
			}
			SetFrame(0);
			SetDelay(0);
		}
	} else {
		int new_health = GetHealth() - damage;
		if (new_health <= 0) {
			SetHealth(0);
			SetState(STATE_DYING);
		} else {
			SetHealth(new_health);
			SetState(damage_state);
		}
		SetFrame(0);
		SetDelay(0);
	}
}

void cActor::SetLogicState(int state) {
	logic_state = state;
}
int cActor::GetLogicState() {
	return logic_state;
}
