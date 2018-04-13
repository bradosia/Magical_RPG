#include "cGraphic.h"

cGraphic::cGraphic(cLogic* logicPtr) {
	actorMain = new cActor(0);
	logicPtr = new cLogic(this);
	actorFrontPtr = new cActorFront;
	projectileFrontPtr = new cProjectileFront;
	tokenFrontPtr = new cTokenFront;
	memset(keyPress, 0, sizeof(keyPress));
	memset(keyBind, 0, sizeof(keyBind));
	memset(keyAction, 0, sizeof(keyAction));
	keyBind[87] = KEY_ACTOR_UP;
	keyBind[265] = KEY_ACTOR_UP;
	keyBind[83] = KEY_ACTOR_DOWN;
	keyBind[264] = KEY_ACTOR_DOWN;
	keyBind[65] = KEY_ACTOR_LEFT;
	keyBind[263] = KEY_ACTOR_LEFT;
	keyBind[68] = KEY_ACTOR_RIGHT;
	keyBind[262] = KEY_ACTOR_RIGHT;
	keyBind[81] = KEY_ACTOR_ATTACK;
	keyBind[46] = KEY_ACTOR_ATTACK;
	keyBind[69] = KEY_ACTOR_SKILL;
	keyBind[47] = KEY_ACTOR_SKILL;
	limitFPS = 1.0 / 30.0;
	fadeTime = 1;
	frameTime = std::chrono::microseconds(0);
}

cGraphic::~cGraphic(void) {
}

bool cGraphic::Init(int lvl) {
	bool res = true;
	int tex_w, tex_h;
	state = STATE_RUN;
	level = lvl;
	fadeTime = 1.0;
	ia = true;
	epilepsia_mode = false;
	lastTime = std::chrono::steady_clock::now();

	//Graphics initialization
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	visible_area.left = TILE_SIZE;
	visible_area.right = GAME_WIDTH + TILE_SIZE;
	visible_area.bottom = TILE_SIZE;
	visible_area.top = GAME_HEIGHT + TILE_SIZE;
	std::cout << "w=" << GAME_WIDTH << "h=" << GAME_HEIGHT << std::endl;
	//the depth of each object is defined by its z value when making calls to glVertex, the field of vision anger of z=0(near) a SCENE_HEIGHT*TILE_SIZE(far)
	glOrtho(visible_area.left, visible_area.right, visible_area.bottom,
			visible_area.top, 3, -SCENE_HEIGHT * TILE_SIZE - 2 - 1); //last 2 values ​​are znear and zfar, the 3 is for the overlays of bars / lives, interface and menu, the -2 is for the z of the terrain, range: [-3,(SCENE_HEIGHT*TILE_SIZE)+2]
	glMatrixMode(GL_MODELVIEW);

	glAlphaFunc(GL_GREATER, 0.05f);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_DEPTH_TEST);
	//Texture initialization
	Data.Load();

	//Terrain initialization
	Data.GetSize(IMG_TILESET, &tex_w, &tex_h);
	res = Scene.LoadLevel(level, tex_w, tex_h);
	if (!res)
		return false;
	Scene.ComputeCollisionMaps();

	//Overlays initialization
	Overlay1.SetBarOffsetX(TILE_SIZE * 2);
	Overlay1.SetX(visible_area.left);
	Overlay1.SetY(visible_area.bottom);
	/*Overlay2.SetBarOffsetX(TILE_SIZE / 2);
	 Overlay2.SetX(visible_area.right - Overlay2.GetWidth());
	 Overlay2.SetY(visible_area.bottom);*/

	//Player, Enemies and Token initialization
	LoadDynamicLayer(lvl);

	//Sound initialization
	Sound.Load();
	if (lvl == 1)
		//Sound.Play(SOUND_AMBIENT1);
		if (lvl == 2)
			//Sound.Play(SOUND_AMBIENT2);

			//Shader initialization
			Shader.Load();

	return res;
}

bool cGraphic::LoadDynamicLayer(int lvl) {
	bool res;
	FILE *fd;
	char file[32];
	int i, j, n, id;
	char c;

	res = true;

	if (level < 10)
		sprintf(file, "Levels/%s0%d%s", (char *) DYNAMIC_FILENAME, lvl,
				(char *) FILENAME_EXT);
	else
		sprintf(file, "Levels/%s%d%s", (char *) DYNAMIC_FILENAME, lvl,
				(char *) FILENAME_EXT);

	fd = fopen(file, "r");
	if (fd == NULL)
		return false;

	for (j = SCENE_HEIGHT - 1; j >= 0; j--) {
		for (i = 0; i < SCENE_WIDTH; i++) {
			fscanf(fd, "%i", &id);
			if (id == 3) {
				enemies.push_back(new cActor(3));
				enemies.back()->SetTile(i, j);
			} else if (id == 4) {
				enemies.push_back(new cActor(2));
				enemies.back()->SetTile(i, j);
			} else if (id == 5) {
				tokens.push_back(new cToken(0));
				tokens.back()->SetTile(i, j);
			}
			fscanf(fd, "%c", &c); //pass comma or enter
		}
	}

	fclose(fd);
	return res;
}

bool cGraphic::Loop() {
	nextFrame = false;
	bool res = false;
	std::chrono::steady_clock::time_point nowTime;
	double deltaTime;
	nowTime = std::chrono::steady_clock::now();
	frameTime += std::chrono::duration_cast<std::chrono::microseconds>(
			nowTime - lastTime);
	deltaTime = ((double) frameTime.count() / 1000000.0) / limitFPS;
	lastTime = nowTime;
	//std::cout << deltaTime << " " << frameTime.count() << std::endl;
	if (deltaTime >= 1.0) {
		nextFrame = true;
		res = true;
		frameTime -= std::chrono::microseconds((int) (limitFPS * 1000000));
		if (state == STATE_RUN) {
			res = Process();
			/*
			 if (res)
			 Render();
			 */
		}
		if (state == STATE_LEVELCHANGE) {
			Sound.StopAll();
			Sound.Play(SOUND_VICTORY);
			/*do {
			 t2 = glfwGetTime();
			 if ((t2 - t1 - 3000) > 0)
			 time = (float) (t2 - t1 - 3000) / (float) (6000 - 3000);
			 //Render();
			 } while (t2 - t1 < 6000);*/

			std::vector<cBicho*>::iterator it_e;
			for (it_e = enemies.begin(); it_e != enemies.end(); it_e++)
				delete (*it_e);
			enemies.clear();
			std::vector<cProjectile*>::iterator it_p;
			for (it_p = projectiles.begin(); it_p != projectiles.end(); it_p++)
				delete (*it_p);
			projectiles.clear();
			std::vector<cToken*>::iterator it_t;
			for (it_t = tokens.begin(); it_t != tokens.end(); it_t++)
				delete (*it_t);
			tokens.clear();

			Init(level + 1);
			state = STATE_RUN;
		}
		if (state == STATE_GAMEOVER) {
			Sound.StopAll();
			Sound.Play(SOUND_GAMEOVER);
			std::cout << "GAMEOVER1\n";
			/*do {
			 t2 = glfwGetTime();
			 time = (float) (t2 - t1) / (float) 3000;
			 if (time > 1)
			 time = 1.0f;
			 if ((t2 - t1 - 7000) > 0)
			 time += (float) (t2 - t1 - 7000) / (float) (10700 - 7000);
			 //Render();
			 } while (t2 - t1 < 10700);*/
			std::cout << "GAMEOVER2\n";
			res = false;
		}
		if (state == STATE_ENDGAME) {
			Sound.StopAll();
			Sound.Play(SOUND_VICTORY);
			/*do {
			 t2 = glfwGetTime();
			 if ((t2 - t1 - 3000) > 0)
			 time = (float) (t2 - t1 - 3000) / (float) (6000 - 3000);
			 //Render();
			 } while (t2 - t1 < 6000);*/
			res = false;
		}

		/*do {
		 t2 = glfwGetTime();
		 } while (t2 - t1 < 1000 / 30);   //30 fps = 1000/30*/

		//MR_edit: callback
		if (CBmap.find("loopEnd") != CBmap.end())
			CBmap["loopEnd"](this);
	}
	return res;
}

void cGraphic::Finalize() {
}

void cGraphic::ReadKeyboard(int key, int, int action, int mods) {
	if (key > 0 && key < 1000) {
		keyPress[key] = action;
		keyAction[keyBind[key]] = action;
		if (action == 0)
			keyUpLast = key;
		else if (action == 1)
			keyDownLast = key;
	}
}

void cGraphic::ReadMouse(int button, int state, int x, int y) {
}

//Process
bool cGraphic::Process() {
	int i, j, k, n;
	bool res = true;
	bool stand = true;
	int h1;
	h1 = actorMain->GetY();
	//h2 = Player2.GetY();
	if ((keyAction[KEY_ACTOR_LEFT] || keyAction[KEY_ACTOR_RIGHT])
			&& actorMain->IsAttacking())
		actorMain->StopP1();
	if (!keyAction[KEY_ACTOR_SKILL])
		actorMain->StopDoingSkillP1();
	if (keyAction[KEY_ACTOR_ATTACK] && !keyAction[KEY_ACTOR_SKILL])
		p1_attacks = true;
	if (keyAction[KEY_ACTOR_SKILL])
		actorMain->SetCharge(0);
	//Process Input
	if (keyAction[KEY_GAME_RENDER])
		res = false;

	if (!actorMain->IsAttacking() && !actorMain->IsDamaged()) {
		int input_state = actorMain->GetState();
		bool doing_skill = actorMain->IsDoingSkill();

		if (doing_skill)
			actorMain->SetState(actorMain->GetLogicState()); //cuando una skill esta activa, el state se queda en una direccion fija por lo que en vez del atributo state, uso el logic_state de Player1 para poder calcular el movimiento y el slide
		if (keyAction[KEY_ACTOR_UP]) {
			actorMain->setFaceDir(90, 0);
			actorMain->setMoveDir(90, 0);
			actorMain->setMoveFlag(true);
			actorMain->MoveUp(Scene.GetCollisionMap(), enemies);
			/*actorMain->RestrictedMoveUp(Scene.GetCollisionMap(), enemies, h1, h2,
			 GAME_HEIGHT);
			 */
			stand = false;
		} else if (keyAction[KEY_ACTOR_DOWN]) {
			actorMain->setFaceDir(270, 0);
			actorMain->setMoveDir(270, 0);
			actorMain->setMoveFlag(true);
			actorMain->MoveDown(Scene.GetCollisionMap(), enemies);
			/*actorMain->RestrictedMoveDown(Scene.GetCollisionMap(), enemies, h1, h2,
			 GAME_HEIGHT, visible_area.bottom);
			 */
			stand = false;
		}
		if (keyAction[KEY_ACTOR_LEFT]) {
			bool vertical = keyAction[KEY_ACTOR_UP]
					|| keyAction[KEY_ACTOR_DOWN];
			actorMain->setFaceDir(180, 0);
			actorMain->setMoveDir(180, 0);
			actorMain->setMoveFlag(true);
			actorMain->MoveLeft(Scene.GetCollisionMap(), enemies, vertical);
			stand = false;
		} else if (keyAction[KEY_ACTOR_RIGHT]) {
			bool vertical = keyAction[KEY_ACTOR_UP]
					|| keyAction[KEY_ACTOR_DOWN];
			actorMain->setFaceDir(0, 0);
			actorMain->setMoveDir(0, 0);
			actorMain->setMoveFlag(true);
			actorMain->MoveRight(Scene.GetCollisionMap(), enemies, vertical);
			stand = false;
		}
		actorMain->SetLogicState(actorMain->GetState()); //despues de los posibles movimientos actualizo el logic_state de Player1 por si fuera necesario usarlo en la siguiente iteracion del gameloop
		if (!doing_skill)
			input_state = actorMain->GetState(); //in this case imput_state will never be a state of skill

		if (keyAction[KEY_ACTOR_SKILL]) {
			if (actorMain->GetSP() > 0)
				actorMain->setShieldState(true);
			else
				actorMain->StopDoingSkillP1();
		} else if (keyAction[KEY_ACTOR_ATTACK]) {
			actorMain->SetCharge(actorMain->GetCharge() + 1);
			if (actorMain->GetCharge() == CHARGE_BREAK)
				Sound.Play(SOUND_CHARGED);
		}
		if (stand) {
			/* if player isn't doing anything do standing animation
			 set facing as last key up*/
			actorMain->StopP1();
			if (keyBind[keyUpLast] == KEY_ACTOR_UP) {
				actorMain->setFaceDir(90, 0); // (xy angle, xz angle)
			} else if (keyBind[keyUpLast] == KEY_ACTOR_DOWN) {
				actorMain->setFaceDir(270, 0);
			} else if (keyBind[keyUpLast] == KEY_ACTOR_LEFT) {
				actorMain->setFaceDir(180, 0);
			} else if (keyBind[keyUpLast] == KEY_ACTOR_RIGHT) {
				actorMain->setFaceDir(0, 0);
			} else {
				actorMain->setFaceDir(90, 0);
			}
			actorMain->setMoveFlag(false);
		}
	}
	if (!keyAction[KEY_ACTOR_SKILL]) {
		actorMain->setShieldState(false);
	}

	//Game Logic
	std::vector<cProjectile*>::iterator it_p;
	std::vector<cBicho*>::iterator it_e;
	std::vector<cToken*>::iterator it_t;
	std::vector<cActor*> players;
	players.push_back(actorMain);
	//players.push_back(&Player2);

	//clean used shots, defeated enemies and finished states
	for (it_p = projectiles.begin(); it_p != projectiles.end();) {
		if ((*it_p)->IsEnding() && (*it_p)->GetFrame() == 0
				&& (*it_p)->GetDelay() == 0) {
			delete (*it_p); //I release the dynamic memory assigned by the new one that I created to the projectile
			it_p = projectiles.erase(it_p); //I delete the pointer to the projectile and update the iterator
		} else {
			cRect framebox = (*it_p)->GetFrameBox();
			if (framebox.left >= visible_area.right - SCENE_Xo * 2
					|| framebox.right <= visible_area.left
					|| framebox.bottom >= visible_area.top - SCENE_Yo * 2
					|| framebox.top <= visible_area.bottom) //if it leaves the field of vision I erase it
							{
				delete (*it_p); //I release the dynamic memory assigned by the new one that I created to the projectile
				it_p = projectiles.erase(it_p); //I delete the pointer to the projectile and update the iterator
			} else
				it_p++;
		}
	}
	for (it_e = enemies.begin(); it_e != enemies.end();) {
		if ((*it_e)->GetState() == STATE_DYING && (*it_e)->GetFrame() == 0
				&& (*it_e)->GetDelay() == 0) {
			delete (*it_e); //I release the dynamic memory assigned by the new that created the enemy
			it_e = enemies.erase(it_e); //I delete the pointer to the enemy and update the iterator
		} else {
			if (dynamic_cast<cEnemy*>(*it_e)->GetFrameTop()
					< visible_area.bottom) //if it leaves the field of vision I erase it
					{
				delete (*it_e); //I release the dynamic memory assigned by the new that created the enemy
				it_e = enemies.erase(it_e); //I delete the pointer to the enemy and update the iterator
			} else {
				if (((*it_e)->IsAttacking() || (*it_e)->IsDamaged())
						&& (*it_e)->GetFrame() == 0 && (*it_e)->GetDelay() == 0)
					(*it_e)->Stop();
				if ((*it_e)->IsWalking() && (*it_e)->GetX() % TILE_SIZE == 0
						&& (*it_e)->GetY() % TILE_SIZE == 0)
					(*it_e)->Stop();
				it_e++;
			}
		}
	}
	if ((actorMain->IsAttacking() || actorMain->IsDamaged())
			&& actorMain->GetFrame() == 0 && actorMain->GetDelay() == 0)
		actorMain->StopP1();
	/*if ((Player2.IsAttacking() || Player2.IsDamaged())
	 && Player2.GetFrame() == 0 && Player2.GetDelay() == 0)
	 Player2.Stop();*/

	//logic and enemy AI
	if (!actorMain->IsAttacking() && !actorMain->IsDamaged() && p1_attacks
			&& actorMain->GetCooldown() == 0) {
		if (actorMain->GetCharge() >= CHARGE_BREAK) {
			if (actorMain->GetSP() >= SUPERATTACK_COST) {
				actorMain->StartSuperAttack();
				actorMain->SetSP(actorMain->GetSP() - SUPERATTACK_COST);
				Sound.Play(SOUND_SUPERSLASH);
			} else
				Sound.Play(SOUND_FAIL);
		} else {
			std::cout << "attack! " << std::endl;
			if (actorMain->GetSP() >= ATTACK_COST) {
				actorMain->StartAttack();
				actorMain->SetSP(actorMain->GetSP() - ATTACK_COST);
				Sound.Play(SOUND_SLASH);
			} else
				Sound.Play(SOUND_FAIL);
		}
		actorMain->SetCharge(0);
		p1_attacks = false;
		actorMain->SetCooldown(COOLDOWN);
	}
	/*
	 if (!Player2.IsAttacking() && !Player2.IsDamaged() && p2_attacks
	 && Player2.GetCooldown() == 0) {
	 if (Player2.GetCharge() >= CHARGE_BREAK) {
	 if (Player2.GetSP() >= ATTACK_COST) {
	 Player2.StartSuperAttack();
	 Sound.Play(SOUND_SUPERFIREBALL);
	 } else
	 Sound.Play(SOUND_FAIL);
	 } else {
	 if (Player2.GetSP() >= ATTACK_COST) {
	 Player2.StartAttack();
	 Sound.Play(SOUND_FIREBALL);
	 } else
	 Sound.Play(SOUND_FAIL);
	 }
	 Player2.SetCharge(0);
	 p2_attacks = false;
	 Player2.SetCooldown(COOLDOWN);
	 }*/

	/* check to see if attacks/abilities resolved
	 * Use the callback timer to time delay the effects of attacks or abilities */
	int n1 = actorList.size();
	std::chrono::steady_clock::time_point nowTime =
			std::chrono::steady_clock::now();
	for (i = 0; i < n1; i++) {
		n = actorList.at(i)->callbackTimerList->size();
		for (j = 0; j < n; j++) {
			if (nowTime < actorList.at(i)->callbackTimerList->at(j)->endTime) {
				actorList.at(i)->callbackTimerList->at(j)->CB(actorList.at(i));
			}
		}
	}

	if (!actorMain->IsAttacking()) {
		if (actorMain->GetCooldown() > 0)
			actorMain->SetCooldown(actorMain->GetCooldown() - 1);
		if (!actorMain->IsDamaged() && !actorMain->getShieldState()
				&& /*actorMain->GetCharge() == 0 &&*/actorMain->GetSP()
						< actorMain->GetMaxSP())
			actorMain->SetSP(actorMain->GetSP() + 1);
	} else {
		std::vector<cBicho*>::iterator it;
		for (it = enemies.begin(); it != enemies.end(); it++) {
			if (actorMain->Attack(*it)) {
				Sound.Play(SOUND_HIT);
				if ((*it)->GetState() == STATE_DYING)
					Sound.Play(SOUND_DEATH);
			}
		}
	}
	/*
	 if (!Player2.IsAttacking()) {
	 if (Player2.GetCooldown() > 0)
	 Player2.SetCooldown(Player2.GetCooldown() - 1);
	 } else {
	 if (Player2.Attack(enemies, projectiles, Scene.GetProjCollisionMap())) {
	 if (projectiles.back()->IsEnding())
	 Sound.Play(SOUND_EXPLOSION);
	 }
	 }*/

	for (it_e = enemies.begin(); it_e != enemies.end(); it_e++) {
		if (ia) {
			if (dynamic_cast<cEnemy*>(*it_e)->GetFrameBottom()
					<= visible_area.top) {
				if ((*it_e)->IsWalking())
					dynamic_cast<cEnemy*>(*it_e)->Move();
				else if ((*it_e)->IsAttacking()) {
					if (dynamic_cast<cMonster1*>(*it_e) != NULL) //verificamos a que subclase pertenece el puntero
					{
						std::vector<cActor*>::iterator it;
						for (it = players.begin(); it != players.end(); it++) {
							if (dynamic_cast<cMonster1*>(*it_e)->Attack(*it))
								Sound.Play(SOUND_HIT);
						}
					}
					if (dynamic_cast<cMonster2*>(*it_e) != NULL) //verificamos a que subclase pertenece el puntero
					{
						dynamic_cast<cMonster2*>(*it_e)->Attack(players,
								projectiles, Scene.GetProjCollisionMap());
					}
				} else if ((*it_e)->IsLooking()
						&& (*it_e)->GetY() <= visible_area.top) //la IA solo se ejecuta cuando no tiene tareas ya asignadas
								{
					if (dynamic_cast<cMonster1*>(*it_e) != NULL) //verificamos a que subclase pertenece el puntero
					{
						if (dynamic_cast<cMonster1*>(*it_e)->IA(
								Scene.GetCollisionMap(), enemies, actorMain))
							Sound.Play(SOUND_SLASH);
					}
					if (dynamic_cast<cMonster2*>(*it_e) != NULL) //verificamos a que subclase pertenece el puntero
					{
						if (dynamic_cast<cMonster2*>(*it_e)->IA(
								Scene.GetCollisionMap(),
								Scene.GetProjCollisionMap(), enemies,
								actorMain))
							Sound.Play(SOUND_ARROW);
					}
				}
			}
		} else if (!(*it_e)->IsDamaged())
			(*it_e)->Stop();
	}

	//Updates
	if (actorMain->GetState() == STATE_DYING)
		state = STATE_GAMEOVER;
	else if (actorMain->GetY() >= visible_area.top - TILE_SIZE) {
		if (level < TOTAL_LEVELS)
			state = STATE_LEVELCHANGE;
		else
			state = STATE_ENDGAME;
	} else {
		// MR_edit: only update for single player
		UpdateCamera(h1);
		//UpdateCamera(h1, h2);
		Sound.Update();
	}

	return res;
}

void cGraphic::UpdateCamera(int h1, int h2) {
	int mean;
	if (h1 >= h2)
		mean = h2 + (h1 - h2) / 2 + SCENE_Yo;
	else
		mean = h1 + (h2 - h1) / 2 + SCENE_Yo;

	if (mean > (GAME_HEIGHT / 2)
			&& mean
					< (SCENE_HEIGHT * TILE_SIZE
							- (GAME_HEIGHT - SCENE_Yo * 4) / 2) - TILE_SIZE * 2
			&& mean - GAME_HEIGHT / 2 + TILE_SIZE > visible_area.bottom) {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		visible_area.bottom = mean - GAME_HEIGHT / 2 + TILE_SIZE;
		visible_area.top = mean + GAME_HEIGHT / 2 + TILE_SIZE;
		glOrtho(visible_area.left, visible_area.right, visible_area.bottom,
				visible_area.top, 3, -SCENE_HEIGHT * TILE_SIZE - 2 - 1);
		Overlay1.SetY(visible_area.bottom); //overlays se mueven junto a la camara
		//Overlay2.SetY(visible_area.bottom);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
}

void cGraphic::UpdateCamera(int h1) {
	int mean = h1;

	if (mean > (GAME_HEIGHT / 2)
			&& mean
					< (SCENE_HEIGHT * TILE_SIZE
							- (GAME_HEIGHT - SCENE_Yo * 4) / 2) - TILE_SIZE * 2
			&& mean - GAME_HEIGHT / 2 + TILE_SIZE > visible_area.bottom) {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		visible_area.bottom = mean - GAME_HEIGHT / 2 + TILE_SIZE;
		visible_area.top = mean + GAME_HEIGHT / 2 + TILE_SIZE;
		glOrtho(visible_area.left, visible_area.right, visible_area.bottom,
				visible_area.top, 3, -SCENE_HEIGHT * TILE_SIZE - 2 - 1);
		Overlay1.SetY(visible_area.bottom); //overlays se mueven junto a la camara
		//Overlay2.SetY(visible_area.bottom);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
}

void cGraphic::Reshape(int w, int h) //w y h son las dimensiones de la ventana del juego
		{
	//necesitaria modificar game_height y width
	glViewport(0, 0, w, h);
	glOrtho(0, w, 0, h, -100, 100);
}

//Output
void cGraphic::Render() {
	int tex_w, tex_h;
	int tex_w2, tex_h2;
	bool run = nextFrame && (state == STATE_RUN);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	//Activacion de shaders
	if (epilepsia_mode) {
		fadeTime += 0.8;
		if (fadeTime >= 180.0)
			fadeTime = 0.0f;
		glActiveTexture(GL_TEXTURE0);
		Shader.Attach(SHADER_EPILEPTIC_F);
		Shader.Activate();
		Shader.SetUniform("time", abs(sin(fadeTime)));
		Shader.SetUniform("tex", 0);
		Shader.SetUniform("flatcolor", 0);
	} else if (fadeTime > 0.0) {
		glActiveTexture(GL_TEXTURE0);
		if (state == STATE_GAMEOVER)
			Shader.Attach(SHADER_GREYSCALEFADE_F);
		else
			Shader.Attach(SHADER_FADE_F);
		Shader.Activate();
		Shader.SetUniform("time", fadeTime);
		Shader.SetUniform("tex", 0);
		Shader.SetUniform("flatcolor", 0);
	}
	//Pintado
	Data.GetSize(IMG_TILESET, &tex_w, &tex_h);
	Scene.Draw(Data.GetID(IMG_TILESET), tex_w, tex_h, run);

	Data.GetSize(IMG_PLAYER1, &tex_w, &tex_h);
	if (actorMain->GetCharge() >= CHARGE_BREAK
			&& actorMain->GetCharge() % 4 < 2)
		actorMain->Draw(Data.GetID(IMG_SUPERP1), tex_w, tex_h, run);
	else
		actorMain->Draw(Data.GetID(IMG_PLAYER1), tex_w, tex_h, run);

	/*Data.GetSize(IMG_PLAYER2, &tex_w, &tex_h);
	 if (Player2.GetCharge() >= CHARGE_BREAK && Player2.GetCharge() % 4 < 2)
	 Player2.Draw(Data.GetID(IMG_SUPERP2), tex_w, tex_h, run);
	 else
	 Player2.Draw(Data.GetID(IMG_PLAYER2), tex_w, tex_h, run);*/

	Data.GetSize(IMG_MONSTER1, &tex_w, &tex_h);
	Data.GetSize(IMG_MONSTER2, &tex_w2, &tex_h2);
	std::vector<cBicho*>::iterator it_e;
	for (it_e = enemies.begin(); it_e != enemies.end(); it_e++) {
		if (dynamic_cast<cMonster1*>(*it_e) != NULL) //verificamos a que subclase pertenece el puntero
		{
			(*it_e)->Draw(Data.GetID(IMG_MONSTER1), tex_w, tex_h, run);
		}
		if (dynamic_cast<cMonster2*>(*it_e) != NULL) //verificamos a que subclase pertenece el puntero
		{
			(*it_e)->Draw(Data.GetID(IMG_MONSTER2), tex_w2, tex_h2, run);
		}
	}

	Data.GetSize(IMG_HEART, &tex_w, &tex_h);
	std::vector<cToken*>::iterator it_t;
	for (it_t = tokens.begin(); it_t != tokens.end(); it_t++) {
		cTokenFront.setTarget((*it_t));
		(*it_t)->draw(Data.GetID(IMG_HEART), tex_w, tex_h, run);
	}

	Data.GetSize(IMG_FIREBALL, &tex_w, &tex_h);
	Data.GetSize(IMG_ARROW, &tex_w2, &tex_h2);
	std::vector<cProjectile*>::reverse_iterator it_p;
	glEnable(GL_BLEND);			   // Turn Blending On
	//glDisable(GL_DEPTH_TEST);    // Turn Depth Testing Off
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (it_p = projectiles.rbegin(); it_p != projectiles.rend(); it_p++) //iteramos al reves para que las ultimas bolas de fuego creadas se pinten encima de las anteriores, de ese modo visualizaremos los valores correctos de alfa
			{
		if (dynamic_cast<cFireball*>(*it_p) != NULL) //verificamos a que subclase pertenece el puntero
		{
			(*it_p)->Draw(Data.GetID(IMG_FIREBALL), tex_w, tex_h, run);
		}
		if (dynamic_cast<cArrow*>(*it_p) != NULL) //verificamos a que subclase pertenece el puntero
		{
			(*it_p)->Draw(Data.GetID(IMG_ARROW), tex_w2, tex_h2, run);
		}
	}

	if (fadeTime > 0.0)
		Shader.SetUniform("flatcolor", 1);
	Overlay1.DrawBars(actorMain->GetMaxHealth(), actorMain->GetHealth(),
			actorMain->GetMaxSP(), actorMain->GetSP());
	/*Overlay2.DrawBars(Player2.GetMaxHealth(), Player2.GetHealth(),
	 Player2.GetMaxSP(), Player2.GetSP());*/
	if (fadeTime > 0.0)
		Shader.SetUniform("flatcolor", 0);

	Overlay1.Draw(Data.GetID(IMG_OVERLAY1));
	//Overlay2.Draw(Data.GetID(IMG_OVERLAY2));
	glDisable(GL_BLEND);          // Turn Blending Off
	//glEnable(GL_DEPTH_TEST);    // Turn Depth Testing On
	glAlphaFunc(GL_GREATER, 0.05f);

	//Desactivacion de shaders
	if (epilepsia_mode) {
		Shader.Detach(SHADER_EPILEPTIC_F);
		Shader.Deactivate();
	} else if (fadeTime > 0.0) {
		if (state == STATE_GAMEOVER)
			Shader.Detach(SHADER_GREYSCALEFADE_F);
		else {
			Shader.Detach(SHADER_FADE_F);
			fadeTime -= 0.01;
		}
		Shader.Deactivate();
	}

	//glutSwapBuffers();
}

cActor* cGraphic::getActorMain() {
	return actorMain;
}

void cGraphic::on(std::string event, std::function<void(cGraphic*)> CB) {
	CBmap[event] = CB;
}
