#include "../include/core/TrackManager.h"
#include "../include/util/importer.h"
#include "../include/util/dsp.h"
#include <chrono>
#include <iostream>
#include <string>

static const std::string audioPath = "audio_samples/timed_samples/acoustics_60s.wav";
constexpr double pitchShift = 0.5;

auto testRepitch(int N) {
    const AudioTrack* track = audioFromFile(audioPath);
    const auto [L, R] = track->getChannels();

    const auto start = std::chrono::steady_clock::now();
    const auto [newL, newR] = pv_pitch_shift_stereo(L, R, pitchShift, N);
    const auto end = std::chrono::steady_clock::now();

    // Save file to prevent code from being optimized out
    const AudioTrack repitchedTrack(newL, newR, 48000);
    repitchedTrack.saveToWav("repitched.wav");

    std::chrono::duration<double> elapsed = end - start;
    return elapsed.count();
}

int main() {
    constexpr int ITERATIONS = 3;

    std::cout << "=== TIME TO REPITCH WITH INCREASING # SAMPLES PER FFT ===\n";
    constexpr std::array<int, 6> SAMPLES = {
        512, 1024, 2048, 4096, 8192, 16384
    };
    for (const int N : SAMPLES) {
        double trialsSum = 0.0;

        std::cout << "Samples: " << N << "\n";

        for (int i = 0; i < ITERATIONS; ++i) {
            trialsSum += testRepitch(N);
        }
        std::cout << "- Avg " << trialsSum / ITERATIONS << " seconds.\n";
    }

    return 0;
}
