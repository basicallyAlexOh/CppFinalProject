#include "../include/core/TrackManager.h"
#include <chrono>
#include <iostream>
#include <string>

auto test_durations(std::string duration) {
    TrackManager tm(48000);

    std::string path_prefix = "audio_samples/timed_samples/";

    tm.addTrack(path_prefix + "drums_" + duration + ".wav");
    tm.addTrack(path_prefix + "acoustics_" + duration + ".wav");
    tm.addTrack(path_prefix + "shaker_" + duration + ".wav");

    auto start = std::chrono::steady_clock::now();
    auto combined = tm.combineAll();
    auto end = std::chrono::steady_clock::now();

    combined->saveToWav("combined.wav");

    std::chrono::duration<double> elapsed = end - start;
    return elapsed.count();
}

auto test_num_wavs(int num_wavs) {
    TrackManager tm(48000);

    std::string path_prefix = "audio_samples/timed_samples/";

    for (int i = 0; i < num_wavs; ++i) {
        tm.addTrack(path_prefix + "acoustics_60s.wav");
    }

    auto start = std::chrono::steady_clock::now();
    auto combined = tm.combineAll();
    auto end = std::chrono::steady_clock::now();

    combined->saveToWav("combined.wav");

    std::chrono::duration<double> elapsed = end - start;
    return elapsed.count();
}

int main() {
    constexpr int ITERATIONS = 3;

    constexpr std::array<std::string, 6> TEST_DURATIONS = {
        "10s", "30s", "60s", "120s", "300s", "600s"
    };
    for (const std::string& duration : TEST_DURATIONS) {
        double duration_sum = 0.0;

        for (int i = 0; i < ITERATIONS; ++i) {
            duration_sum += test_durations(duration);
        }
        std::cout << "Combined " << duration << " in " << duration_sum / ITERATIONS << " seconds on average.\n";
    }

    constexpr std::array<int, 6> TEST_NUM_WAVS = {1, 2, 5, 10, 20, 50};
    for (const int num_wavs : TEST_NUM_WAVS) {
        double num_wavs_sum = 0.0;

        for (int i = 0; i < ITERATIONS; ++i) {
            num_wavs_sum += test_num_wavs(num_wavs);
        }
        std::cout << "Combined " << num_wavs << " WAVs in " << num_wavs_sum / ITERATIONS << " seconds on average.\n";
    }

    return 0;
}
