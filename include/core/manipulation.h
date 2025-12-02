#pragma once
#include <vector>



// TODO: Reverses the vector
void reverse_audio(std::vector<double>&);

// TODO: Adjusts speed through linear interpolation
void adjust_speed(std::vector<double>&, double ratio);

// TODO: Adjusts speed with windowed sinc resampler --> no pitch change
void adjust_speed_resample(std::vector<double>&, double ratio);

// TODO: Adjust pitch using Phase Vocoder Pitch Shifting
void adjust_pitch(std::vector<double>&, double semitones);