#include "AudioLibBass.h"

namespace AudioLibBass {

AudioDevice::AudioDevice() {
	int device = -1; // Default Sounddevice
	int freq = 44100; // Sample rate (Hz)
	BASS_Init(device, freq, 0, 0, NULL);
}

AudioDevice::~AudioDevice() {
	BASS_Free();
}

bool AudioDevice::update() {
	return true;
}

OutputStream::OutputStream() {

}

OutputStream::OutputStream(HSTREAM streamHandle_) {
	streamHandle = streamHandle_;
}

OutputStream::~OutputStream() {
	BASS_SampleFree(streamHandle);
}

void OutputStream::setVolume(float volume_) {
	BASS_ChannelSetAttribute(streamHandle, BASS_ATTRIB_VOL, volume_);
}
void OutputStream::play() {
	BASS_ChannelPlay(streamHandle, false);
}
void OutputStream::stop() {
	BASS_ChannelStop(streamHandle);
}

AudioDevice* OpenDevice() {
	return new AudioDevice;
}

OutputStream* OpenSound(AudioDevice *device_, std::string filename_,
		bool stream_) {
	char *cstr = new char[filename_.length() + 1];
	strcpy(cstr, filename_.c_str());
	HSTREAM streamHandle = BASS_StreamCreateFile(false, cstr, 0, 0, 0);
	return new OutputStream(streamHandle);
}

}
