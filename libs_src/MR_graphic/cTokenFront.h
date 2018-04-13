#ifndef C_TOKEN_FRONT_H
#define C_TOKEN_FRONT_H

#include "cSound.h"
#include "../MR_logic/cToken.h"

class cTokenFront {
private:
	cToken* targetPtr;
	cSound* soundPtr;
public:
	cTokenFront();
	virtual ~cTokenFront();

	void setTarget(cToken* targetPtr_);
	void setSound(cSound* soundPtr_);
	void front(std::vector<std::string>* frontList);
	void cmd(std::string cmd_);

	void Draw(int tex_id, int tex_w, int tex_h, bool run);
	void NextFrame(int max);
	int GetFrame();
};

#endif

