#include "cSound.h"

cSound::cSound(void)
{
	FMOD::System_Create(&system);// create an instance of the game engine
	system->init(32, FMOD_INIT_NORMAL, 0);// initialise the game engine with 32 channels (cantidad de sonido simultaneo que puede haber)
}

cSound::~cSound(void)
{
	for(int i=0; i<NUM_SOUNDS; i++) sounds[i]->release();
    system->release();
}

bool cSound::Load()
{
	system->createStream("Sounds/ambient1.mp3",       FMOD_SOFTWARE | FMOD_LOOP_NORMAL, 0, &sounds[SOUND_AMBIENT1]);
	system->createStream("Sounds/ambient2.mp3",       FMOD_SOFTWARE | FMOD_LOOP_NORMAL, 0, &sounds[SOUND_AMBIENT2]);
	system-> createSound("Sounds/gameover.mp3",       FMOD_HARDWARE, 0, &sounds[SOUND_GAMEOVER]);
	system-> createSound("Sounds/slash.wav",          FMOD_HARDWARE, 0, &sounds[SOUND_SLASH]);
	system-> createSound("Sounds/superslash.wav",     FMOD_HARDWARE, 0, &sounds[SOUND_SUPERSLASH]);
	system-> createSound("Sounds/hit.wav",            FMOD_HARDWARE, 0, &sounds[SOUND_HIT]);
	system-> createSound("Sounds/explosion.wav",      FMOD_HARDWARE, 0, &sounds[SOUND_EXPLOSION]);
	system-> createSound("Sounds/fireball.wav",       FMOD_HARDWARE, 0, &sounds[SOUND_FIREBALL]);
	system-> createSound("Sounds/superfireball.wav",  FMOD_HARDWARE, 0, &sounds[SOUND_SUPERFIREBALL]);
	system-> createSound("Sounds/arrow.wav",          FMOD_HARDWARE, 0, &sounds[SOUND_ARROW]);
	system-> createSound("Sounds/death.wav",	      FMOD_HARDWARE, 0, &sounds[SOUND_DEATH]);
	system-> createSound("Sounds/token.wav",          FMOD_HARDWARE, 0, &sounds[SOUND_TOKEN]);
	system-> createSound("Sounds/fail.wav",           FMOD_HARDWARE, 0, &sounds[SOUND_FAIL]);
	system-> createSound("Sounds/charged.wav",        FMOD_HARDWARE, 0, &sounds[SOUND_CHARGED]);
	system-> createSound("Sounds/victory.mp3",        FMOD_HARDWARE, 0, &sounds[SOUND_VICTORY]);
	return true;
}

void cSound::Play(int sound_id)
{
	if (sound_id == SOUND_AMBIENT1) {
		system->playSound(FMOD_CHANNEL_FREE, sounds[SOUND_AMBIENT1], false, &ambient1Channel);
		ambient1Channel->setVolume( 0.25f ); //between 0 and 1
	}
	else if (sound_id == SOUND_AMBIENT2) {
		system->playSound(FMOD_CHANNEL_FREE, sounds[SOUND_AMBIENT2], false, &ambient2Channel);
		ambient2Channel->setVolume( 0.2f ); //between 0 and 1
	}
	else system->playSound(FMOD_CHANNEL_FREE,sounds[sound_id], false, 0);
}

void cSound::StopAll()
{
	ambient1Channel->stop();
	ambient2Channel->stop();
}

void cSound::Update()
{
	system->update();
}