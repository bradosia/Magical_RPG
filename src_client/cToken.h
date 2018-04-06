#pragma once

#include "cScene.h"
#include "cActor.h"

class cToken {
public:
	cToken();
	virtual ~cToken();

	void SetTile(int tx, int ty);
	int GetX();
	int GetY();
	cRect GetHitBox();

	void Draw(int tex_id, int tex_w, int tex_h, bool run);
	void NextFrame(int max);
	int GetFrame();

private:
	int x, y, seq, delay, frame_delay, height_overground;
};

class cHeart: public cToken {
public:
	bool Effect(cActor* playerPtr) {
		if (playerPtr->GetHealth() == playerPtr->GetMaxHealth())
			return false;
		int new_health = playerPtr->GetHealth() + 40;
		if (new_health > playerPtr->GetMaxHealth())
			playerPtr->SetHealth(playerPtr->GetMaxHealth());
		else
			playerPtr->SetHealth(new_health);
		return true;
	}
};
