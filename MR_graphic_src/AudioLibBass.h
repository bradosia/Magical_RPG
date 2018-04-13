#ifndef AUDIO_LIB_BASS_H
#define AUDIO_LIB_BASS_H

#include <string>

#include "../client_src/Globals.h"

namespace AudioLibBass
{

class AudioDevice
{
public:
	AudioDevice();
	virtual ~AudioDevice();

	bool update();
};

class OutputStream
{
private:
	HSTREAM streamHandle;
public:
	OutputStream();
	OutputStream(HSTREAM streamHandle_);
	virtual ~OutputStream();

	void setVolume(float volume_);
	void play();
	void stop();
};

AudioDevice* OpenDevice();
OutputStream* OpenSound(AudioDevice *device_, std::string filename_, bool stream_);

}

#endif
