#ifndef C_LOGIC_H
#define C_LOGIC_H

#include <MR_logic/cLogic.h>

class cLogicExt {
private:
	cGame* gamePtr;
public:
	cLogicExt();
	cLogicExt(cGame* gamePtr_);
	~cLogicExt();

	bool testIntersection(cRect box1, cRect box2);
	bool proximityProjectileActor(std::vector<cProjectile*>* projectiles,
			std::vector<cActor*>* actors);
	bool proximityTokenActor(std::vector<cToken*>* tokens,
			std::vector<cActor*>* actors);
	bool deleteActor(std::vector<cActor*>* tokens);
	bool deleteProjectile(std::vector<cProjectile*>* tokens);
	bool deleteToken(std::vector<cToken*>* tokens);

};

#endif
