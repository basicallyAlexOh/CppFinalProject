#include "core/AudioTrack.h"
#include "core/manipulation.h"
#include "util/importer.h"
#include <algorithm>
#include <string>

AudioTrack::AudioTrack() : 
    sample_rate(44100), 
    channels(1),
    enabled(true),
    start_time(0),
    end_time(0)
{}


AudioTrack::AudioTrack(std::vector<double> audio, int sample_rate) :
    sample_rate(sample_rate), 
    channels(1),
    enabled(true),
    L(audio),
    R(audio),
    originalL(audio),
    originalR(audio),
    start_time(0),
    end_time(start_time + audio.size() / sample_rate)
{}


AudioTrack::AudioTrack(std::vector<int16_t> audio, int sample_rate) : 
    sample_rate(sample_rate), 
    channels(1),
    enabled(true),
    L(audio.begin(), audio.end()),
    R(audio.begin(), audio.end()),
    start_time(0),
    end_time(start_time + audio.size() / sample_rate)
{
    std::transform(L.begin(), L.end(), L.begin(), [](int16_t x){return (double) x / 32768;});
    R = L;
    originalL = L;
    originalR = R;
}


AudioTrack::AudioTrack(std::vector<double> l, std::vector<double> r, int sample_rate) :
    sample_rate(sample_rate),
    channels(2),
    enabled(true),
    L(l),
    R(r),
    originalL(l),
    originalR(r),
    start_time(0),
    end_time(start_time + l.size() / sample_rate)
{}


AudioTrack::AudioTrack(std::vector<int16_t> l, std::vector<int16_t> r, int sample_rate) : 
    sample_rate(sample_rate), 
    channels(2),
    enabled(true),
    L(l.begin(), l.end()),
    R(r.begin(), r.end()),
    start_time(0),
    end_time(start_time + l.size() / sample_rate)
{
    std::transform(L.begin(), L.end(), L.begin(), [](int16_t x){return (double) x / 32768;});
    std::transform(R.begin(), R.end(), R.begin(), [](int16_t x){return (double) x / 32768;});
    originalL = L;
    originalR = R;
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

double AudioTrack::duration() {
    return end_time - start_time;
}


void AudioTrack::reverse() {
    reverse_audio(L);
    reverse_audio(R);
}


void AudioTrack::adjustSpeed(double ratio){
    adjust_speed(L, ratio);
    adjust_speed(R, ratio);
    end_time = sample_rate * L.size() + start_time;
}


void AudioTrack::repitch(double factor) {
    adjust_pitch(L, factor);
    adjust_pitch(R, factor);
}