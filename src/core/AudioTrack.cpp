#include "core/AudioTrack.h"
#include "util/importer.h"
#include <algorithm>
#include <string>

AudioTrack::AudioTrack() : sample_rate(44100), channels(1) {}


AudioTrack::AudioTrack(std::vector<double> audio, int sample_rate) :
    sample_rate(sample_rate), 
    channels(1),
    L(audio),
    R(audio),
    originalL(audio),
    originalR(audio)
{}


AudioTrack::AudioTrack(std::vector<int16_t> audio, int sample_rate) : 
    sample_rate(sample_rate), 
    channels(1),
    L(audio.begin(), audio.end()),
    R(audio.begin(), audio.end())
{
    std::transform(L.begin(), L.end(), L.begin(), [](int16_t x){return (double) x / 32768;});
    R = L;
    originalL = L;
    originalR = R;
}


AudioTrack::AudioTrack(std::vector<double> l, std::vector<double> r, int sample_rate) :
    sample_rate(sample_rate),
    channels(2),
    L(l),
    R(r),
    originalL(l),
    originalR(r)
{}


AudioTrack::AudioTrack(std::vector<int16_t> l, std::vector<int16_t> r, int sample_rate) : 
    sample_rate(sample_rate), 
    channels(2),
    L(l.begin(), l.end()),
    R(r.begin(), r.end())
{
    std::transform(L.begin(), L.end(), L.begin(), [](int16_t x){return (double) x / 32768;});
    std::transform(R.begin(), R.end(), R.begin(), [](int16_t x){return (double) x / 32768;});
    originalL = L;
    originalR = R;
}



double AudioTrack::duration() {
    return (double) L.size() / sample_rate;
}


