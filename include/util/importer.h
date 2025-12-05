#pragma once
#include <sndfile.hh>
#include <vector>
#include <stdexcept>
#include "core/AudioTrack.h"

AudioTrack* audioFromFile(const std::string& path, int desiredSampleRate=44100);

AudioTrack* analogAudioFromFile(const std::string& path, int desiredSampleRate=44100);

// TODO: implement digital audio
// AudioTrack digitalAudioFromFile(const std::string& path);