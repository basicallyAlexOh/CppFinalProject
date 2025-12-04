#include "core/manipulation.h"
#include "util/dsp.h"
#include <algorithm> //std::min
#include <cmath> //std::lerp std::pow std::abs
#include <limits> //std::numeric_limits


void reverse_audio(std::vector<double>& audio)
{
    std::reverse(audio.begin(), audio.end());
}

void adjust_speed(std::vector<double>& audio, double ratio)
{
    if (ratio <= 0.0 || audio.size() < 2) {
        return;
    }

    const std::size_t old_size = audio.size();
    const std::size_t new_size =
        static_cast<std::size_t>(old_size / ratio);

    std::vector<double> out;
    out.reserve(new_size);

    for (std::size_t i = 0; i < new_size; ++i) {
        double src_pos = i * ratio;
        std::size_t src_idx = static_cast<std::size_t>(src_pos);
        if (src_idx >= old_size) {
            src_idx = old_size - 1; // clamp
        }
        out.push_back(audio[src_idx]); // nearest neighbor
    }

    audio.swap(out); //in-place update for the caller
}


void adjust_speed_resample(std::vector<double>& audio, double ratio)
{
    if (ratio <= 0.0 || audio.size() < 2) {
        return;
    }

    const auto old_size = audio.size();
    const auto new_size =
        static_cast<std::size_t>(old_size / ratio);

    std::vector<double> out;
    out.reserve(new_size);

    for (std::size_t i = 0; i < new_size; ++i) {
        const double src_pos = i * ratio;
        auto i0 = static_cast<std::size_t>(src_pos);

        //if we're at or beyond the last valid interpolation pair, just repeat the final sample
        if (i0 + 1 >= old_size) {
            out.push_back(audio.back());
            continue;
        }

        const double frac = src_pos - static_cast<double>(i0);
        const double s0 = audio[i0];
        const double s1 = audio[i0 + 1];

        //Linear interpolation between s0 and s1
        out.push_back(std::lerp(s0, s1, frac));
    }

    audio.swap(out); // in-place update for the caller
}

void adjustSpeed_nodistort(std::vector<double>& audio, double ratio)
{
    audio = pv_time_stretch_mono(audio, ratio);
}

void adjust_pitch(std::vector<double>& audio, double semitones)
{
    audio = pv_pitch_shift_mono(audio, std::pow(2.0, semitones / 12.0));
}

