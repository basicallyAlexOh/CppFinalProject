#include "core/AudioTrack.h"
#include "core/manipulation.h"
#include "util/importer.h"
#include <algorithm>
#include <string>

AudioTrack::AudioTrack() : 
    sample_rate(44100), 
    channels(1),
    enabled(true),
    start_time(0)
{}


AudioTrack::AudioTrack(std::vector<double> audio, int sample_rate) :
    sample_rate(sample_rate), 
    channels(1),
    enabled(true),
    L(audio),
    R(audio),
    originalL(audio),
    originalR(audio),
    start_time(0)
{}


AudioTrack::AudioTrack(std::vector<int16_t> audio, int sample_rate) : 
    sample_rate(sample_rate), 
    channels(1),
    enabled(true),
    L(audio.begin(), audio.end()),
    R(audio.begin(), audio.end()),
    start_time(0)
{
    std::transform(L.begin(), L.end(), L.begin(), [](int16_t x){return (double) x / 32768;});
    R = L;
    originalL = L;
    originalR = R;
}


AudioTrack::AudioTrack(const std::vector<double> l, const std::vector<double> r, int sample_rate) :
    sample_rate(sample_rate),
    channels(2),
    enabled(true),
    L(l),
    R(r),
    originalL(l),
    originalR(r),
    start_time(0)
{}


// TODO: figure out if there's a way to implement this?
// AudioTrack::AudioTrack(std::vector<double>&& l, std::vector<double>&& r, int sample_rate) :
//     sample_rate(sample_rate),
//     channels(2),
//     enabled(true),
//     L(std::move(l)),
//     R(std::move(r)),
//     originalL(L),
//     originalR(R),
//     start_time(0)
// {}


AudioTrack::AudioTrack(std::vector<int16_t> l, std::vector<int16_t> r, int sample_rate) : 
    sample_rate(sample_rate), 
    channels(2),
    enabled(true),
    L(l.begin(), l.end()),
    R(r.begin(), r.end()),
    start_time(0)
{
    std::transform(L.begin(), L.end(), L.begin(), [](int16_t x){return (double) x / 32768;});
    std::transform(R.begin(), R.end(), R.begin(), [](int16_t x){return (double) x / 32768;});
    originalL = L;
    originalR = R;
}


double AudioTrack::getStartTime() const
{
    return start_time;
}


void AudioTrack::reset() {
    L = originalL;
    R = originalR;
}

void AudioTrack::enable(){
    enabled=true;
}
void AudioTrack::disable(){
    enabled=false;
}

double AudioTrack::duration() const 
{
    return start_time + sample_rate * L.size();
}

size_t AudioTrack::numSamples() const
{
    return L.size();
}

void AudioTrack::reverse() {
    reverse_audio(L);
    reverse_audio(R);
}

void AudioTrack::adjustSpeed(double ratio){
    adjust_speed(L, ratio);
    adjust_speed(R, ratio);
}

void AudioTrack::repitch(double factor) {
    adjust_pitch(L, factor);
    adjust_pitch(R, factor);
}
