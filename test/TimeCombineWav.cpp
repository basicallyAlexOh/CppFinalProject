#include "../include/core/TrackManager.h"
#include <chrono>
#include <iostream>
#include <string>

static const std::string pathPrefix = "audio_samples/timed_samples/";

auto test_durations(std::string duration) {
    auto start = std::chrono::steady_clock::now();
    TrackManager tm(48000);

    tm.addTrack(pathPrefix + "drums_" + duration + ".wav");
    tm.addTrack(pathPrefix + "acoustics_" + duration + ".wav");
    tm.addTrack(pathPrefix + "shaker_" + duration + ".wav");

    auto combined = tm.combineAll();
    combined->saveToWav("combined.wav");
    auto end = std::chrono::steady_clock::now();

    std::chrono::duration<double> elapsed = end - start;
    return elapsed.count();
}

auto test_durations_sox(std::string duration) {
    auto start = std::chrono::steady_clock::now();
    system(("sox -m " + pathPrefix + "drums_" + duration + ".wav " + 
            pathPrefix + "acoustics_" + duration + ".wav " + 
            pathPrefix + "shaker_" + duration + ".wav combined.wav").c_str());
    auto end = std::chrono::steady_clock::now();

    std::chrono::duration<double> elapsed = end - start;
    return elapsed.count();
}

auto test_num_wavs(int num_wavs) {
    auto start = std::chrono::steady_clock::now();
    TrackManager tm(48000);

    for (int i = 0; i < num_wavs; ++i) {
        tm.addTrack(pathPrefix + "acoustics_60s.wav");
    }

    auto combined = tm.combineAll();
    combined->saveToWav("combined.wav");
    auto end = std::chrono::steady_clock::now();

    std::chrono::duration<double> elapsed = end - start;
    return elapsed.count();
}

auto test_num_wavs_sox(int num_wavs) {
    std::string tracks = "";
    for (int i = 0; i < num_wavs; ++i) {
        tracks += pathPrefix + "acoustics_60s.wav ";
    }

    auto start = std::chrono::steady_clock::now();
    system(("sox -m " + tracks + "combined.wav").c_str());
    auto end = std::chrono::steady_clock::now();

    std::chrono::duration<double> elapsed = end - start;
    return elapsed.count();
}

int main() {
    constexpr int ITERATIONS = 3;

    std::cout << "=== TIME TO COMBINE .WAV FILES OF INCREASING DURATION ===\n";
    constexpr std::array<std::string, 6> TEST_DURATIONS = {
        "10s", "30s", "60s", "120s", "300s", "600s"
    };
    for (const std::string& duration : TEST_DURATIONS) {
        double trackManagerSum = 0.0;
        double soxSum = 0.0;

        std::cout << "File Length: " << duration << "\n";

        for (int i = 0; i < ITERATIONS; ++i) {
            trackManagerSum += test_durations(duration);
            soxSum += test_durations_sox(duration);
        }
        std::cout << "- Avg " << trackManagerSum / ITERATIONS << " seconds to combine with TrackManager.\n";
        std::cout << "- Avg " << soxSum / ITERATIONS << " seconds to combine with SoX.\n";
    }
    std::cout << "\n";

    std::cout << "=== TIME TO COMBINE INCREASING NUM OF .WAV FILES ===\n";
    constexpr std::array<int, 5> TEST_NUM_WAVS = {2, 5, 10, 20, 50};
    for (const int num_wavs : TEST_NUM_WAVS) {
        double trackManagerSum = 0.0;
        double soxSum = 0.0;

        std::cout << "Num Files: " << num_wavs << "\n";

        for (int i = 0; i < ITERATIONS; ++i) {
            trackManagerSum += test_num_wavs(num_wavs);
            soxSum += test_num_wavs_sox(num_wavs);
        }
        std::cout << "- Avg " << trackManagerSum / ITERATIONS << " seconds to combine with TrackManager.\n";
        std::cout << "- Avg " << soxSum / ITERATIONS << " seconds to combine with SoX.\n";
    }

    return 0;
}
