#include "../include/core/TrackManager.h"
#include <iostream>

int main() {
    TrackManager tm(24000);

    tm.addTrack("audio_samples/drums.wav");
    tm.addTrack("audio_samples/acoustics.wav");
    tm.addTrack("audio_samples/shaker.wav");

    auto combined = tm.combineAll();
    combined->saveToWav("combined.wav");

    std::cout << "Wrote combined.wav\n";
    return 0;
}
