#include "../MR_logic/cLogicExt.h"

cLogicExt::cLogicExt() {

}
cLogicExt::cLogicExt(cGame* gamePtr_) {
	gamePtr = gamePtr_;
}
cLogicExt::~cLogicExt() {
}

bool cLogicExt::testIntersection(cRect box1, cRect box2) {
	if (box1.top > box2.bottom && box2.top > box1.bottom) {
		if (box1.left < box2.right && box2.left < box1.right)
			return true;
	}
	return false;
}

bool cLogicExt::proximityProjectileActor(std::vector<cProjectile*>* projectiles,
		std::vector<cActor*>* actors) {
	bool hitFlag = false;
	std::vector<cProjectile*>::iterator it_p;
	std::vector<cActor*>::iterator it_a;
	for (it_p = projectiles->begin(); it_p != projectiles->end(); it_p++) {
		for (it_a = actors->begin(); it_a != actors->end();) {
			if (testIntersection((*it_a)->GetHitBox(), (*it_p)->GetHitBox())) {
				hitFlag = true;
				(*it_a)->proximityProjectileActorEvent((*it_p));
				(*it_p)->proximityProjectileActorEvent((*it_a));
			}
		}
	}
	return hitFlag;
}

bool cLogicExt::proximityTokenActor(std::vector<cToken*>* tokens,
		std::vector<cActor*>* actors) {
	bool hitFlag = false;
	std::vector<cToken*>::iterator it_t;
	std::vector<cActor*>::iterator it_a;
	for (it_t = tokens->begin(); it_t != tokens->end(); it_t++) {
		for (it_a = actors->begin(); it_a != actors->end();) {
			if (testIntersection((*it_a)->GetHitBox(), (*it_t)->GetHitBox())) {
				hitFlag = true;
				(*it_a)->proximityTokenActorEvent((*it_t));
				(*it_t)->proximityTokenActorEvent((*it_a));
			}
		}
	}
	return hitFlag;
}

bool deleteActor(std::vector<cActor*>* tokens) {
	bool delFlag = false;
	std::vector<cActor*>::iterator it_t;
	for (it_t = tokens->begin(); it_t != tokens->end();) {
		if ((*it_t)->getDeleteFlag()) {
			delFlag = true;
			delete (*it_t);
			it_t = tokens->erase(it_t);
		} else
			it_t++;
	}
	return delFlag;
}

bool deleteProjectile(std::vector<cProjectile*>* tokens) {
	bool delFlag = false;
	std::vector<cProjectile*>::iterator it_t;
	for (it_t = tokens->begin(); it_t != tokens->end();) {
		if ((*it_t)->getDeleteFlag()) {
			delFlag = true;
			delete (*it_t);
			it_t = tokens->erase(it_t);
		} else
			it_t++;
	}
	return delFlag;
}

bool cLogicExt::deleteToken(std::vector<cToken*>* tokens) {
	bool delFlag = false;
	std::vector<cToken*>::iterator it_t;
	for (it_t = tokens->begin(); it_t != tokens->end();) {
		if ((*it_t)->getDeleteFlag()) {
			delFlag = true;
			delete (*it_t);
			it_t = tokens->erase(it_t);
		} else
			it_t++;
	}
	return delFlag;
}

