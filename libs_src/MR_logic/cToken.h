#ifndef C_TOKEN_H
#define C_TOKEN_H

#include "../MR_logic/cActor.h"

class cActor;

class cToken {
public:
	int x, y, seq, delay, frame_delay, height_overground;
	std::vector<std::string> frontList;
	bool deleteFlag;
public:
	cToken(int objID_);
	virtual ~cToken();

	void SetTile(int tx, int ty);
	int GetX();
	int GetY();
	cRect GetHitBox();

	void front(std::string cmd_);
	void proximityTokenActorEvent(cActor* actorPtr);
	void setDeleteFlag(bool flag_);
	bool getDeleteFlag();
	void setType(int objID_);
	int getType();
};

#endif
