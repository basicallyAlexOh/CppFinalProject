#include <sndfile.hh>
#include <vector>
#include <stdexcept>
#include "core/AudioTrack.h"

AudioTrack* audioFromFile(const std::string& path);

AudioTrack* analogAudioFromFile(const std::string& path);

// TODO: implement digital audio
// AudioTrack digitalAudioFromFile(const std::string& path);