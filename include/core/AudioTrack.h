#pragma once
#include <vector>
#include <string>
#include <numeric>


class AudioTrack {
    public:
        friend class TrackManager;

        // Default Constructor - creates an empty audio track
        explicit AudioTrack();

        // Constructor from mono-channel audio
        explicit AudioTrack(std::vector<double>, int sample_rate=44100);

        explicit AudioTrack(std::vector<int16_t>, int sample_rate=44100);

        explicit AudioTrack(std::vector<double>, std::vector<double>, int sample_rate=44100);

        explicit AudioTrack(std::vector<int16_t>, std::vector<int16_t>, int sample_rate=44100);

        // Construct from moved vector
        // explicit AudioTrack(std::vector<double>&&, std::vector<double>&&, int sample_rate=44100);


        // Copy Constructor
        AudioTrack(const AudioTrack&) = default;

        // Copy assignment
        AudioTrack& operator=(const AudioTrack&) = default;

        // Move Constructor
        AudioTrack(AudioTrack&&) = default;

        // Move Assignment
        AudioTrack& operator=(AudioTrack&&) = default;

        double getStartTime() const;

        void reset();

        void disable();

        void enable();

        // TODO: implement
        void shift_start(double time);

        double duration() const;

        size_t numSamples() const;

        void reverse();

        void adjustSpeed(double ratio);

        void adjustSpeed_resample(double ratio);

        void repitch(double semitones);

        void normalize(double peak = 0.99);

        void saveToWav(const std::string& path) const;


    private:
        int sample_rate;
        int channels;
        double start_time;
        bool enabled;

        std::vector<double> L;
        std::vector<double> R;
        
        // TODO: consider caching only if changed?
        std::vector<double> originalL;
        std::vector<double> originalR;
};