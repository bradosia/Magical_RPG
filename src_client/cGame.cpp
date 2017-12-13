#include "cGame.h"

cGame::cGame(void)
{
}

cGame::~cGame(void)
{
}

bool cGame::Init(int lvl)
{
	bool res=true;
	int tex_w, tex_h;
	state = STATE_RUN;
	level = lvl;
	time = 1.0f;
	ia = true;
	epilepsia_mode = false;

	//Graphics initialization
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	visible_area.left = TILE_SIZE;
	visible_area.right = GAME_WIDTH+TILE_SIZE;
	visible_area.bottom = TILE_SIZE;
	visible_area.top = GAME_HEIGHT+TILE_SIZE;
	//la profundidad de cada objeto viene definida por su valor z al hacer las llamadas a glVertex, el campo de vision ira de z=0(near) a SCENE_HEIGHT*TILE_SIZE(far)
	glOrtho(visible_area.left,visible_area.right,visible_area.bottom,visible_area.top,3,-SCENE_HEIGHT*TILE_SIZE-2-1); //ultimos 2 valores son znear y zfar, el 3 es por los overlays de barras/vidas, interfaz y menu, el -2 es por las z del terreno, rango: [-3,(SCENE_HEIGHT*TILE_SIZE)+2]
	glMatrixMode(GL_MODELVIEW);
	
	glAlphaFunc(GL_GREATER, 0.05f);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_DEPTH_TEST);

	//Texture initialization
	Data.Load();

	//Terrain initialization
	Data.GetSize(IMG_TILESET,&tex_w,&tex_h);
	res = Scene.LoadLevel(level,tex_w, tex_h);
	if(!res) return false;

	Scene.ComputeCollisionMaps();

	//Overlays initialization
	Overlay1.SetBarOffsetX(TILE_SIZE*2);
	Overlay1.SetX(visible_area.left);
	Overlay1.SetY(visible_area.bottom);
	Overlay2.SetBarOffsetX(TILE_SIZE/2);
	Overlay2.SetX(visible_area.right - Overlay2.GetWidth());
	Overlay2.SetY(visible_area.bottom);

	//Player, Enemies and Token initialization
	LoadDynamicLayer(lvl);

	//Sound initialization
	Sound.Load();
	if(lvl == 1) Sound.Play(SOUND_AMBIENT1);
	if(lvl == 2) Sound.Play(SOUND_AMBIENT2);

	//Shader initialization
	Shader.Load();

	return res;
}

bool cGame::LoadDynamicLayer(int lvl)
{
	bool res;
	FILE *fd;
	char file[32];
	int i,j,id;
	char c;

	res=true;

	if(level<10) sprintf(file,"Levels/%s0%d%s",(char *)DYNAMIC_FILENAME,lvl,(char *)FILENAME_EXT);
	else		 sprintf(file,"Levels/%s%d%s",(char *)DYNAMIC_FILENAME,lvl,(char *)FILENAME_EXT);

	fd=fopen(file,"r");
	if(fd==NULL) return false;

	for(j=SCENE_HEIGHT-1;j>=0;j--)
	{
		for(i=0;i<SCENE_WIDTH;i++)
		{
			fscanf(fd,"%i",&id);
			if(id == 1) Player1.SetTile(i,j);
			else if(id == 2) Player2.SetTile(i,j);
			else if(id == 3)
			{
				enemies.push_back(new cMonster1);
				enemies.back()->SetTile(i,j);
			}
			else if(id == 4)
			{
				enemies.push_back(new cMonster2);
				enemies.back()->SetTile(i,j);
			}
			else if(id == 5)
			{
				tokens.push_back(new cHeart);
				tokens.back()->SetTile(i,j);
			}
			fscanf(fd,"%c",&c); //pass comma or enter
		}
	}

	fclose(fd);
	return res;
}

bool cGame::Loop()
{
	bool res=true;
	int t1,t2;
	t1 = glutGet(GLUT_ELAPSED_TIME);

	if(state == STATE_RUN)
	{
		res = Process();
		if(res) Render();
	}
	if(state == STATE_LEVELCHANGE)
	{
		Sound.StopAll();
		Sound.Play(SOUND_VICTORY);
		do { t2 = glutGet(GLUT_ELAPSED_TIME);
		if((t2-t1-3000) > 0) time = (float)(t2-t1-3000)/(float)(6000-3000);
		Render();
		} while(t2-t1 < 6000);

		std::list<cBicho*>::iterator it_e;
		for(it_e=enemies.begin(); it_e!=enemies.end(); it_e++) delete (*it_e);
		enemies.clear();
		std::list<cProjectile*>::iterator it_p;
		for(it_p=projectiles.begin(); it_p!=projectiles.end(); it_p++) delete (*it_p);
		projectiles.clear();
		std::list<cToken*>::iterator it_t;
		for(it_t=tokens.begin(); it_t!=tokens.end(); it_t++) delete (*it_t);
		tokens.clear();

		Init(level+1);
		state = STATE_RUN;
	}
	if(state == STATE_GAMEOVER){
		Sound.StopAll();
		Sound.Play(SOUND_GAMEOVER);
		do { t2 = glutGet(GLUT_ELAPSED_TIME);
		time = (float)(t2-t1)/(float)3000;
		if(time > 1) time = 1.0f;
		if((t2-t1-7000) > 0) time += (float)(t2-t1-7000)/(float)(10700-7000);
		Render();
		} while(t2-t1 < 10700);
		res=false;
	}
	if(state == STATE_ENDGAME)
	{
		Sound.StopAll();
		Sound.Play(SOUND_VICTORY);
		do { t2 = glutGet(GLUT_ELAPSED_TIME);
		if((t2-t1-3000) > 0) time = (float)(t2-t1-3000)/(float)(6000-3000);
		Render();
		} while(t2-t1 < 6000);
		res=false;
	}

	do { t2 = glutGet(GLUT_ELAPSED_TIME);
	} while(t2-t1 < 1000/30);   //30 fps = 1000/30

	return res;
}

void cGame::Finalize()
{
}

//Input
void cGame::ReadKeyboard(unsigned char key, int x, int y, bool press)
{
	if(key >='A' && key <= 'Z') key += 32;

	//cuando se deja de pulsar una tecla de direccion lateral cuando se desplaza en diagonal, los frames de los player pueden cambiar
	if(!press && ( key==P1_LEFT  || key==P1_RIGHT ) && !Player1.IsAttacking()) Player1.StopP1();
	
	//cuando se deja de pulsar la tecla de skill, los frames de los player cambian
	if(!press && key==P1_SKILL) Player1.StopDoingSkillP1();
	if(!press && key==P2_SKILL) Player2.StopDoingSkill();

	//solo se reconoce un ataque si la tecla de ataque se despulsa despues de estar pulsada y no se estaba pulsando la tecla de skill
	if(!press && key == P1_ATTACK && keys[P1_ATTACK] && !keys[P1_SKILL]) p1_attacks=true;
	if(!press && key == P2_ATTACK && keys[P2_ATTACK] && !keys[P2_SKILL]) p2_attacks=true;

	//en el momento en que se pulsa la tecla de skill la carga se pierde
	if(press && key == P1_SKILL) Player1.SetCharge(0);
	if(press && key == P2_SKILL) Player2.SetCharge(0);

	if(key != GLUT_KEY_UP && key != GLUT_KEY_DOWN && key != GLUT_KEY_LEFT && key != GLUT_KEY_RIGHT) keys[key] = press;
}

void cGame::ReadSpecialKeyboard(unsigned char key, int x, int y, bool press)
{
	//cuando se deja de pulsar una tecla de direccion lateral cuando se desplaza en diagonal, los frames de los player pueden cambiar
	if(!press && ( key==P2_LEFT  || key==P2_RIGHT ) && !Player2.IsAttacking()) Player2.Stop();
	
	if(key == GLUT_KEY_UP || key == GLUT_KEY_DOWN || key == GLUT_KEY_LEFT || key == GLUT_KEY_RIGHT) keys[key] = press;
	if(key == GLUT_KEY_F1 && press) ia = !ia;
	if(key == GLUT_KEY_F10 && press) epilepsia_mode = !epilepsia_mode;
	if(key == GLUT_KEY_F10 && !press) time = 0.0f;
}

void cGame::ReadMouse(int button, int state, int x, int y)
{
}

//Process
bool cGame::Process()
{
	bool res=true;
	bool stand=true;
	bool skill_p1=false;
	bool skill_p2=false;
	bool p1_defends=false;
	int h1, h2;
	h1 = Player1.GetY();
	h2 = Player2.GetY();

	//Process Input
	if(keys[27])	res=false;

	if(!Player1.IsAttacking() && !Player1.IsDamaged())
	{
		int input_state = Player1.GetState();
		bool doing_skill = Player1.IsDoingSkill();

		if(doing_skill) Player1.SetState(Player1.GetLogicState()); //cuando una skill esta activa, el state se queda en una direccion fija por lo que en vez del atributo state, uso el logic_state de Player1 para poder calcular el movimiento y el slide
		if(keys[P1_UP])
		{
			Player1.RestrictedMoveUp(Scene.GetCollisionMap(),enemies,h1,h2,GAME_HEIGHT); 
			stand=false;
		}
		else if(keys[P1_DOWN])
		{
			Player1.RestrictedMoveDown(Scene.GetCollisionMap(),enemies,h1,h2,GAME_HEIGHT,visible_area.bottom);
			stand=false;
		}
		if(keys[P1_LEFT])
		{
			bool vertical = keys[P1_UP] || keys[P1_DOWN];
			Player1.MoveLeft(Scene.GetCollisionMap(),enemies,vertical);
			stand=false;
		}
		else if(keys[P1_RIGHT])
		{
			bool vertical = keys[P1_UP] || keys[P1_DOWN];
			Player1.MoveRight(Scene.GetCollisionMap(),enemies,vertical);
			stand=false;
		}
		Player1.SetLogicState(Player1.GetState());  //despues de los posibles movimientos actualizo el logic_state de Player1 por si fuera necesario usarlo en la siguiente iteracion del gameloop
		if(!doing_skill) input_state = Player1.GetState(); //en este caso imput_state nunca sera un state de skill

		if(keys[P1_SKILL])
		{
			if(Player1.GetSP() > 0) Player1.SetShieldState(input_state);
			else Player1.StopDoingSkillP1();
		}
		else if(keys[P1_ATTACK])
		{
			Player1.SetCharge(Player1.GetCharge()+1); 
			if(Player1.GetCharge() == CHARGE_BREAK) Sound.Play(SOUND_CHARGED);
		}
		if(stand)						 Player1.StopP1(); //si no hago nada muestro un estado LOOK

		stand=true;
	}
	if(keys[P1_SKILL]) p1_defends=true;

	if(!Player2.IsAttacking() && !Player2.IsDamaged())
	{
		if(keys[P2_UP])
		{
			Player2.RestrictedMoveUp(Scene.GetCollisionMap(),enemies,h2,h1,GAME_HEIGHT);
			stand=false;
		}
		else if(keys[P2_DOWN])
		{
			Player2.RestrictedMoveDown(Scene.GetCollisionMap(),enemies,h2,h1,GAME_HEIGHT,visible_area.bottom);
			stand=false;
		}
		if(keys[P2_LEFT])
		{
			bool vertical = keys[P2_UP] || keys[P2_DOWN];
			Player2.MoveLeft(Scene.GetCollisionMap(),enemies,vertical);
			stand=false;
		}
		else if(keys[P2_RIGHT])
		{
			bool vertical = keys[P2_UP] || keys[P2_DOWN];
			Player2.MoveRight(Scene.GetCollisionMap(),enemies,vertical);
			stand=false;
		}
		if(keys[P2_SKILL])				 
		{
			if(Player2.GetSP() < Player2.GetMaxSP()) {
				Player2.SetSP(Player2.GetSP()+1);
				if(!Player2.IsDoingSkill()) Player2.TransformIntoSkillState();
			}
			else Player2.StopDoingSkill();
		}
		else if(keys[P2_ATTACK])
		{
			Player2.SetCharge(Player2.GetCharge()+1); 
			if(Player2.GetCharge() == CHARGE_BREAK) Sound.Play(SOUND_CHARGED);
		}
		if(stand)						 Player2.Stop(); //si no hago nada muestro un estado LOOK
	}
	
	//Game Logic

	std::list<cProjectile*>::iterator it_p;
	std::list<cBicho*>::iterator it_e;
	std::list<cToken*>::iterator it_t;
	std::list<cBicho*> players;
	players.push_back(&Player1);
	players.push_back(&Player2);

	//limpia disparos usados, enemigos derrotados y estados terminados
	for(it_p=projectiles.begin(); it_p!=projectiles.end();)
	{
		if((*it_p)->IsEnding() && (*it_p)->GetFrame() == 0 && (*it_p)->GetDelay() == 0) 
		{
			delete(*it_p); //libero la memoria dinamica asignada por el new que creo al proyectil
			it_p = projectiles.erase(it_p); //borro el puntero al proyectil y actualizo el iterador
		}
		else
		{
			cRect framebox = (*it_p)->GetFrameBox();
			if( framebox.left   >= visible_area.right - SCENE_Xo*2 || framebox.right <= visible_area.left ||
				framebox.bottom >= visible_area.top - SCENE_Yo*2   || framebox.top   <= visible_area.bottom ) //si sale del campo de vision lo borro
			{
				delete(*it_p); //libero la memoria dinamica asignada por el new que creo al proyectil
				it_p = projectiles.erase(it_p); //borro el puntero al proyectil y actualizo el iterador
			}
			else it_p++;
		}
	}
	for(it_e=enemies.begin(); it_e!=enemies.end();)
	{
		if((*it_e)->GetState() == STATE_DYING && (*it_e)->GetFrame() == 0 && (*it_e)->GetDelay() == 0)
		{
			delete (*it_e); //libero la memoria dinamica asignada por el new que creo al enemigo
			it_e = enemies.erase(it_e); //borro el puntero al enemigo y actualizo el iterador
		}
		else {
			if (dynamic_cast<cEnemy*>(*it_e)->GetFrameTop() < visible_area.bottom) //si sale del campo de vision lo borro
			{
				delete (*it_e); //libero la memoria dinamica asignada por el new que creo al enemigo
				it_e = enemies.erase(it_e); //borro el puntero al enemigo y actualizo el iterador
			}
			else
			{
				if(((*it_e)->IsAttacking() || (*it_e)->IsDamaged()) && (*it_e)->GetFrame() == 0 && (*it_e)->GetDelay() == 0) (*it_e)->Stop();
				if((*it_e)->IsWalking() && (*it_e)->GetX()%TILE_SIZE == 0 && (*it_e)->GetY()%TILE_SIZE == 0) (*it_e)->Stop();
				it_e++;
			}
		}
	}
	if((Player1.IsAttacking() || Player1.IsDamaged()) && Player1.GetFrame() == 0 && Player1.GetDelay() == 0) Player1.StopP1();
	if((Player2.IsAttacking() || Player2.IsDamaged()) && Player2.GetFrame() == 0 && Player2.GetDelay() == 0) Player2.Stop();

	//logica e IA enemiga

	if(!Player1.IsAttacking() && !Player1.IsDamaged() && p1_attacks && Player1.GetCooldown() == 0)
	{
		if(Player1.GetCharge() >= CHARGE_BREAK)
		{
			if(Player1.GetSP() >= SUPERATTACK_COST)
			{
				Player1.StartSuperAttack();
				Player1.SetSP(Player1.GetSP() - SUPERATTACK_COST);
				Sound.Play(SOUND_SUPERSLASH);
			}
			else Sound.Play(SOUND_FAIL);
		}
		else
		{
			if(Player1.GetSP() >= ATTACK_COST)
			{
				Player1.StartAttack();
				Player1.SetSP(Player1.GetSP() - ATTACK_COST);
				Sound.Play(SOUND_SLASH);
			}
			else Sound.Play(SOUND_FAIL);
		}
		Player1.SetCharge(0);
		p1_attacks=false;
		Player1.SetCooldown(COOLDOWN);
	}
	if(!Player2.IsAttacking()  && !Player2.IsDamaged() && p2_attacks && Player2.GetCooldown() == 0)
	{
		if(Player2.GetCharge() >= CHARGE_BREAK)
		{
			if(Player2.GetSP() >= ATTACK_COST)
			{
				Player2.StartSuperAttack();
				Sound.Play(SOUND_SUPERFIREBALL);
			}
			else Sound.Play(SOUND_FAIL);
		}
		else
		{
			if(Player2.GetSP() >= ATTACK_COST)
			{
				Player2.StartAttack();
				Sound.Play(SOUND_FIREBALL);
			}
			else Sound.Play(SOUND_FAIL);
		}
		Player2.SetCharge(0);
		p2_attacks=false;
		Player2.SetCooldown(COOLDOWN);
	}
	if(!Player1.IsAttacking())
	{
		if(Player1.GetCooldown() > 0) Player1.SetCooldown(Player1.GetCooldown()-1);
		if(!Player1.IsDamaged() && !p1_defends && /*Player1.GetCharge() == 0 &&*/ Player1.GetSP() < Player1.GetMaxSP()) Player1.SetSP(Player1.GetSP()+1);
	}
	else
	{
		std::list<cBicho*>::iterator it;
		for(it=enemies.begin(); it!=enemies.end(); it++)
		{
			if(Player1.Attack(*it))
			{
				Sound.Play(SOUND_HIT);
				if((*it)->GetState() == STATE_DYING) Sound.Play(SOUND_DEATH);
			}
		}
	}
	if(!Player2.IsAttacking())
	{
		if(Player2.GetCooldown() > 0) Player2.SetCooldown(Player2.GetCooldown()-1);
	}
	else
	{
		if( Player2.Attack(enemies,projectiles,Scene.GetProjCollisionMap()) )
		{
			if( projectiles.back()->IsEnding() ) Sound.Play(SOUND_EXPLOSION);
		}
	}

	for(it_e=enemies.begin(); it_e!=enemies.end(); it_e++) 
	{
		if(ia)
		{
			if(dynamic_cast<cEnemy*>(*it_e)->GetFrameBottom() <= visible_area.top)
			{
				if((*it_e)->IsWalking()) dynamic_cast<cEnemy*>(*it_e)->Move();
				else if((*it_e)->IsAttacking())
				{
					if(dynamic_cast<cMonster1*>(*it_e) != NULL) //verificamos a que subclase pertenece el puntero
					{
						std::list<cBicho*>::iterator it;
						for(it=players.begin(); it!=players.end(); it++)
						{
							if( dynamic_cast<cMonster1*>(*it_e)->Attack(*it) ) Sound.Play(SOUND_HIT);
						}
					}
					if(dynamic_cast<cMonster2*>(*it_e) != NULL) //verificamos a que subclase pertenece el puntero
					{
						dynamic_cast<cMonster2*>(*it_e)->Attack(players,projectiles,Scene.GetProjCollisionMap());
					}
				}
				else if((*it_e)->IsLooking() && (*it_e)->GetY() <= visible_area.top) //la IA solo se ejecuta cuando no tiene tareas ya asignadas
				{
					if(dynamic_cast<cMonster1*>(*it_e) != NULL) //verificamos a que subclase pertenece el puntero
					{
						if( dynamic_cast<cMonster1*>(*it_e)->IA(Scene.GetCollisionMap(),enemies,&Player1,&Player2) ) Sound.Play(SOUND_SLASH);
					}
					if(dynamic_cast<cMonster2*>(*it_e) != NULL) //verificamos a que subclase pertenece el puntero
					{
						if( dynamic_cast<cMonster2*>(*it_e)->IA(Scene.GetCollisionMap(),Scene.GetProjCollisionMap(),enemies,&Player1,&Player2) ) Sound.Play(SOUND_ARROW);
					}
				}
			}
		}
		else if(!(*it_e)->IsDamaged()) (*it_e)->Stop();
	}

	for(it_p=projectiles.begin(); it_p!=projectiles.end(); it_p++)
	{
		if(dynamic_cast<cFireball*>(*it_p) != NULL) //verificamos a que subclase pertenece el puntero
		{
			if( (*it_p)->Logic(Scene.GetProjCollisionMap(),enemies) ) Sound.Play(SOUND_EXPLOSION);
		}
		if(dynamic_cast<cArrow*>(*it_p) != NULL) //verificamos a que subclase pertenece el puntero
		{
			(*it_p)->Logic(Scene.GetProjCollisionMap(),players);
		}
	}

	for(it_t=tokens.begin(); it_t!=tokens.end();)
	{
		bool used=false;
		if( Player1.Intersection(Player1.GetHitBox(),(*it_t)->GetHitBox()) )
		{
			if(dynamic_cast<cHeart*>(*it_t) != NULL) //verificamos a que subclase pertenece el puntero
			{
				used = dynamic_cast<cHeart*>(*it_t)->Effect(Player1);
			}
		}
		else if( Player2.Intersection(Player2.GetHitBox(),(*it_t)->GetHitBox()) )
		{
			if(dynamic_cast<cHeart*>(*it_t) != NULL) //verificamos a que subclase pertenece el puntero
			{
				used = dynamic_cast<cHeart*>(*it_t)->Effect(Player2);
			}
		}
		if(used)
		{
			delete (*it_t);
			it_t = tokens.erase(it_t);
			Sound.Play(SOUND_TOKEN);
		}
		else it_t++;
	}

	//Updates
	if(Player1.GetState() == STATE_DYING || Player2.GetState() == STATE_DYING) state = STATE_GAMEOVER;
	else if(Player1.GetY() >= visible_area.top - TILE_SIZE && Player2.GetY() >= visible_area.top - TILE_SIZE)
	{
		if(level < TOTAL_LEVELS) state = STATE_LEVELCHANGE;
		else state = STATE_ENDGAME;
	}
	else {
		UpdateCamera(h1,h2);
		Sound.Update();
	}

	return res;
}

void cGame::UpdateCamera(int h1,int h2)
{
	int mean;
	if(h1 >= h2) mean = h2 + (h1-h2)/2 + SCENE_Yo;
	else mean = h1 + (h2-h1)/2 + SCENE_Yo;

	if(mean > (GAME_HEIGHT/2) && mean < (SCENE_HEIGHT*TILE_SIZE - (GAME_HEIGHT-SCENE_Yo*4)/2) - TILE_SIZE*2 && mean - GAME_HEIGHT/2 + TILE_SIZE > visible_area.bottom) {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		visible_area.bottom = mean - GAME_HEIGHT/2 + TILE_SIZE;
		visible_area.top = mean + GAME_HEIGHT/2 + TILE_SIZE;
		glOrtho(visible_area.left,visible_area.right,visible_area.bottom,visible_area.top,3,-SCENE_HEIGHT*TILE_SIZE-2-1);
		Overlay1.SetY(visible_area.bottom);   //overlays se mueven junto a la camara
		Overlay2.SetY(visible_area.bottom);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
}

void cGame::Reshape(int w,int h) //w y h son las dimensiones de la ventana del juego
{
	//necesitaria modificar game_height y width
	glViewport(0,0,w,h);
}

//Output
void cGame::Render()
{
	int tex_w,tex_h;
	int tex_w2,tex_h2;
	bool run = (state == STATE_RUN);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();


	//Activacion de shaders
	if(epilepsia_mode)
	{
		time += 0.8f;
		if(time >= 180.0f) time = 0.0f;
		glActiveTexture(GL_TEXTURE0);
		Shader.Attach(SHADER_EPILEPTIC_F);
		Shader.Activate();
		Shader.SetUniform("time", abs(sin(time)));
		Shader.SetUniform("tex", 0);
		Shader.SetUniform("flatcolor", 0);
	}
	else if(time > 0.0f)
	{
		glActiveTexture(GL_TEXTURE0);
		if(state == STATE_GAMEOVER) Shader.Attach(SHADER_GREYSCALEFADE_F);
		else Shader.Attach(SHADER_FADE_F);
		Shader.Activate();
		Shader.SetUniform("time", time);
		Shader.SetUniform("tex", 0);
		Shader.SetUniform("flatcolor", 0);
	}

	//Pintado
	Data.GetSize(IMG_TILESET,&tex_w,&tex_h);
	Scene.Draw(Data.GetID(IMG_TILESET),tex_w,tex_h,run);

	Data.GetSize(IMG_PLAYER1,&tex_w,&tex_h);
	if(Player1.GetCharge() >= CHARGE_BREAK && Player1.GetCharge()%4 < 2) Player1.Draw(Data.GetID(IMG_SUPERP1),tex_w,tex_h,run);
	else Player1.Draw(Data.GetID(IMG_PLAYER1),tex_w,tex_h,run);

	Data.GetSize(IMG_PLAYER2,&tex_w,&tex_h);
	if(Player2.GetCharge() >= CHARGE_BREAK && Player2.GetCharge()%4 < 2) Player2.Draw(Data.GetID(IMG_SUPERP2),tex_w,tex_h,run);
	else Player2.Draw(Data.GetID(IMG_PLAYER2),tex_w,tex_h,run);

	Data.GetSize(IMG_MONSTER1,&tex_w,&tex_h);
	Data.GetSize(IMG_MONSTER2,&tex_w2,&tex_h2);
	std::list<cBicho*>::iterator it_e;
	for(it_e=enemies.begin(); it_e!=enemies.end(); it_e++) 
	{
		if(dynamic_cast<cMonster1*>(*it_e) != NULL) //verificamos a que subclase pertenece el puntero
		{
			(*it_e)->Draw(Data.GetID(IMG_MONSTER1),tex_w,tex_h,run);
		}
		if(dynamic_cast<cMonster2*>(*it_e) != NULL) //verificamos a que subclase pertenece el puntero
		{
			(*it_e)->Draw(Data.GetID(IMG_MONSTER2),tex_w2,tex_h2,run);
		}
	}

	Data.GetSize(IMG_HEART,&tex_w,&tex_h);
	std::list<cToken*>::iterator it_t;
	for(it_t=tokens.begin(); it_t!=tokens.end(); it_t++) 
	{
		if(dynamic_cast<cHeart*>(*it_t) != NULL) //verificamos a que subclase pertenece el puntero
		{
			(*it_t)->Draw(Data.GetID(IMG_HEART),tex_w,tex_h,run);
		}
	}

	Data.GetSize(IMG_FIREBALL,&tex_w,&tex_h);
	Data.GetSize(IMG_ARROW,&tex_w2,&tex_h2);
	std::list<cProjectile*>::reverse_iterator it_p;
	glEnable(GL_BLEND);			   // Turn Blending On
	//glDisable(GL_DEPTH_TEST);    // Turn Depth Testing Off
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for(it_p=projectiles.rbegin(); it_p!=projectiles.rend(); it_p++) //iteramos al reves para que las ultimas bolas de fuego creadas se pinten encima de las anteriores, de ese modo visualizaremos los valores correctos de alfa
	{
		if(dynamic_cast<cFireball*>(*it_p) != NULL) //verificamos a que subclase pertenece el puntero
		{
			(*it_p)->Draw(Data.GetID(IMG_FIREBALL),tex_w,tex_h,run);
		}
		if(dynamic_cast<cArrow*>(*it_p) != NULL) //verificamos a que subclase pertenece el puntero
		{
			(*it_p)->Draw(Data.GetID(IMG_ARROW),tex_w2,tex_h2,run);
		}
	}
	
	if(time > 0.0f) Shader.SetUniform("flatcolor", 1);
	Overlay1.DrawBars(Player1.GetMaxHealth(),Player1.GetHealth(),Player1.GetMaxSP(),Player1.GetSP());
	Overlay2.DrawBars(Player2.GetMaxHealth(),Player2.GetHealth(),Player2.GetMaxSP(),Player2.GetSP());
	if(time > 0.0f) Shader.SetUniform("flatcolor", 0);

	Overlay1.Draw(Data.GetID(IMG_OVERLAY1));
	Overlay2.Draw(Data.GetID(IMG_OVERLAY2));
	glDisable(GL_BLEND);          // Turn Blending Off
    //glEnable(GL_DEPTH_TEST);    // Turn Depth Testing On
	glAlphaFunc(GL_GREATER, 0.05f);

	//Desactivacion de shaders
	if(epilepsia_mode)
	{
		Shader.Detach(SHADER_EPILEPTIC_F);
		Shader.Deactivate();
	}
	else if(time > 0.0f)
	{
		if(state == STATE_GAMEOVER) Shader.Detach(SHADER_GREYSCALEFADE_F);
		else {Shader.Detach(SHADER_FADE_F); time -= 0.01f;}
		Shader.Deactivate();
	}

	glutSwapBuffers();
}
