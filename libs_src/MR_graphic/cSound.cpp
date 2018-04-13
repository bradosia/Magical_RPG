#include "cSound.h"

cSound::cSound(void)
{
	devicePtr = AudioLib::OpenDevice();
}

cSound::~cSound(void)
{
	/*
	 * When you are done using AudioLib, just let the RefPtr objects go out of
	 scope, and they will automatically clean themselves up.  If you really
	 _must_ delete an object before its pointer goes out of scope, just set
	 the pointer to 0.
	 */
	/*for (int i = 0; i < NUM_SOUNDS; i++)
	 soundsPtr[i] = 0;
	 devicePtr = 0;*/
}

bool cSound::Load()
{
	ambient1ChannelPtr = AudioLib::OpenSound(devicePtr, "Sounds/ambient1.mp3",
			true);
	ambient2ChannelPtr = AudioLib::OpenSound(devicePtr, "Sounds/ambient2.mp3",
			false);
	soundsPtr[SOUND_GAMEOVER] = AudioLib::OpenSound(devicePtr, "Sounds/gameover.mp3", false);
	soundsPtr[SOUND_SLASH] = AudioLib::OpenSound(devicePtr, "Sounds/slash.wav",
			false);
	soundsPtr[SOUND_SUPERSLASH] = AudioLib::OpenSound(devicePtr,
			"Sounds/superslash.wav", false);
	soundsPtr[SOUND_HIT] = AudioLib::OpenSound(devicePtr, "Sounds/hit.wav",
			false);
	soundsPtr[SOUND_EXPLOSION] = AudioLib::OpenSound(devicePtr,
			"Sounds/explosion.wav", false);
	soundsPtr[SOUND_FIREBALL] = AudioLib::OpenSound(devicePtr,
			"Sounds/fireball.wav", false);
	soundsPtr[SOUND_SUPERFIREBALL] = AudioLib::OpenSound(devicePtr,
			"Sounds/superfireball.wav", false);
	soundsPtr[SOUND_ARROW] = AudioLib::OpenSound(devicePtr, "Sounds/arrow.wav",
			false);
	soundsPtr[SOUND_DEATH] = AudioLib::OpenSound(devicePtr, "Sounds/death.wav",
			false);
	soundsPtr[SOUND_TOKEN] = AudioLib::OpenSound(devicePtr, "Sounds/token.wav",
			false);
	soundsPtr[SOUND_FAIL] = AudioLib::OpenSound(devicePtr, "Sounds/fail.wav",
			false);
	soundsPtr[SOUND_CHARGED] = AudioLib::OpenSound(devicePtr,
			"Sounds/charged.wav", false);
	soundsPtr[SOUND_VICTORY] = AudioLib::OpenSound(devicePtr,
			"Sounds/victory.mp3", false);
	return true;
}

void cSound::Play(int sound_id)
{
	if (sound_id == SOUND_AMBIENT1)
	{
		if (ambient1ChannelPtr)
		{
			ambient1ChannelPtr->setVolume(0.25f); //between 0 and 1
			ambient1ChannelPtr->play();
		}
	}
	else if (sound_id == SOUND_AMBIENT2)
	{
		if (ambient2ChannelPtr)
		{
			ambient2ChannelPtr->setVolume(0.25f); //between 0 and 1
			ambient2ChannelPtr->play();
		}
	}
	else if (soundsPtr[sound_id])
		soundsPtr[sound_id]->play();
}

void cSound::StopAll()
{
	if (ambient1ChannelPtr)
	{
		ambient1ChannelPtr->stop();
	}
	if (ambient2ChannelPtr)
	{
		ambient2ChannelPtr->stop();
	}
}

void cSound::Update()
{
	//crash
	//devicePtr->update();
}
