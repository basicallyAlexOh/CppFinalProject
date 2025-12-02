#include "core/manipulation.h"
#include <algorithm>


void reverse_audio(std::vector<double>& audio)
{
    std::reverse(audio.begin(), audio.end());
}

void adjust_speed(std::vector<double>& audio, double ratio)
{
    // TODO: implement this
}


void adjust_speed_resample(std::vector<double>& audio, double ratio)
{
    // TODO: implement this
}

void adjust_pitch(std::vector<double>& audio, double semitones)
{

}