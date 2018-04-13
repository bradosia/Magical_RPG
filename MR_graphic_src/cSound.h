#ifndef C_SOUND_H
#define C_SOUND_H

#include "../client_src/AudioLibBass.h"

#define AudioLib AudioLibBass

//Sound array size
#define NUM_SOUNDS 15

//Sound identifiers
#define	SOUND_AMBIENT1      0
#define	SOUND_AMBIENT2      1

#define SOUND_GAMEOVER      2
#define	SOUND_SLASH         3
#define	SOUND_SUPERSLASH    4
#define	SOUND_HIT           5
#define	SOUND_EXPLOSION     6
#define	SOUND_FIREBALL      7
#define	SOUND_SUPERFIREBALL 8
#define	SOUND_ARROW         9
#define	SOUND_DEATH		    10
#define	SOUND_TOKEN		    11
#define	SOUND_FAIL		    12
#define	SOUND_CHARGED		13
#define SOUND_VICTORY       14

class cSound
{
public:
	cSound(void);
	virtual ~cSound(void);

	bool Load();
	void Play(int sound_id);
	void Stop(int sound_id);
	void SmoothStop(int sound_id);
	void StopAll();
	void Update();

	AudioLib::AudioDevice *devicePtr;
	AudioLib::OutputStream *soundsPtr[NUM_SOUNDS];
	AudioLib::OutputStream *ambient1ChannelPtr;
	AudioLib::OutputStream *ambient2ChannelPtr;
};

#endif
