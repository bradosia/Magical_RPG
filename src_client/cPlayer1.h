#pragma once

#include "cPlayer.h"

class cPlayer1: public cPlayer
{
public:
	cPlayer1();
	~cPlayer1();

	void Draw(int tex_id,int tex_w,int tex_h,bool run);

	bool Attack(cBicho* enemy);

	void SetShieldState(int render_state);

	void StopP1();
	void StopDoingSkillP1();

	void TestShieldProtection(int attack_state,int damage,int x,int y);

	void SetLogicState(int input_state);
	int  GetLogicState();

private:
	int logic_state;
};