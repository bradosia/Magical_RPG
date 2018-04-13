#ifndef C_PROJECTILE_FRONT_H
#define C_PROJECTILE_FRONT_H

#include "../client_src/cSound.h"
#include "../MR_logic/cProjectile.h"

class cProjectileFront {
private:
	cProjectile* targetPtr;
	cSound* soundPtr;
public:
	cProjectileFront();
	virtual ~cProjectileFront();

	void setTarget(cProjectile* targetPtr_);
	void setSound(cSound* soundPtr_);
	void front(std::vector<std::string>* frontList);
	void cmd(std::string cmd_);

	void DrawRect(int tex_id, float xo, float yo, float xf, float yf);
	void Draw(int tex_id, int tex_w, int tex_h, bool run);
	void drawFireball(int tex_id, int tex_w, int tex_h, bool run);
	void drawArrow(int tex_id, int tex_w, int tex_h, bool run);
	void NextFrame(int max);
	int GetFrame();
};

#endif

