#ifndef C_ACTOR_FRONT_H
#define C_ACTOR_FRONT_H

#include "cSound.h"
#include "../MR_logic/cActor.h"

class cActorFront {
private:
	cActor* targetPtr;
	cSound* soundPtr;
public:
	cActorFront();
	virtual ~cActorFront();

	void setTarget(cActor* targetPtr_);
	void setSound(cSound* soundPtr_);
	void front(std::vector<std::string>* frontList);
	void cmd(std::string cmd_);

	void draw(int tex_id, int tex_w, int tex_h, bool run);
	void drawPlayer1(int tex_id, int tex_w, int tex_h, bool run);
	void drawRect(int tex_id, float xo, float yo, float xf, float yf);
};

#endif

