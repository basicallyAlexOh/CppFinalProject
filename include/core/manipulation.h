#pragma once
#include <vector>



void reverse_audio(std::vector<double>&);


void adjust_speed(std::vector<double>&, double ratio);


void adjust_speed_resample(std::vector<double>&, double ratio);


void adjust_pitch(std::vector<double>&, double semitones);