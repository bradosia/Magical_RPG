#include "../MR_logic/cToken.h"

cToken::cToken(int objID_) {
	seq = 0;
	delay = 0;
	frame_delay = 3;
	height_overground = TILE_SIZE / 4;
	deleteFlag = false;
	setType(objID_);
}
cToken::~cToken() {
}

void cToken::SetTile(int tx, int ty) {
	x = tx * TILE_SIZE;
	y = ty * TILE_SIZE;
}
int cToken::GetX() {
	return x;
}
int cToken::GetY() {
	return y;
}
cRect cToken::GetHitBox() {
	cRect hitbox;
	hitbox.top = y + TILE_SIZE;
	hitbox.bottom = y;
	hitbox.left = x;
	hitbox.right = x + TILE_SIZE;
	return hitbox;
}

void cToken::front(std::string cmd_) {
	frontList.push_back(cmd_);
}
void cToken::proximityTokenActorEvent(cActor* actorPtr) {
	if (actorPtr->GetHealth() < actorPtr->GetMaxHealth()) {
		int new_health = actorPtr->GetHealth() + 40;
		if (new_health > actorPtr->GetMaxHealth())
			actorPtr->SetHealth(actorPtr->GetMaxHealth());
		else
			actorPtr->SetHealth(new_health);
		setDeleteFlag(true);
		front("give_powerup");
	}
}

void cToken::setDeleteFlag(bool flag_) {
	deleteFlag = flag_;
}
bool cToken::getDeleteFlag() {
	return deleteFlag;
}

void cProjectile::setType(int objID_) {
	objID = objID_;
	if (objID == 0) {
		//heart
	}
}
int cProjectile::getType() {
	return objID;
}
