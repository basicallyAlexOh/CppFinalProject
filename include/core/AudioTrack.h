#include <vector>
#include <string>
#include <numeric>


class AudioTrack {
    public:
        // Default Constructor - creates an empty audio track
        explicit AudioTrack();

        // Constructor from mono-channel audio
        explicit AudioTrack(std::vector<double>, int sample_rate=44100);

        explicit AudioTrack(std::vector<int16_t>, int sample_rate=44100);

        explicit AudioTrack(std::vector<double>, std::vector<double>, int sample_rate=44100);

        explicit AudioTrack(std::vector<int16_t>, std::vector<int16_t>, int sample_rate=44100);

        // Constructor from File Path
        explicit AudioTrack(std::string filePath);

        // Copy Constructor
        AudioTrack(const AudioTrack&) = default;

        // Copy assignment
        AudioTrack& operator=(const AudioTrack&) = default;

        // Move Constructor
        AudioTrack(AudioTrack&&) = default;

        // Move Assignment
        AudioTrack& operator=(AudioTrack&&) = default;

        double duration();




    private:
        int sample_rate;
        int channels;
        std::vector<double> L;
        std::vector<double> R;
        
        // TODO: consider caching only if changed
        std::vector<double> originalL;
        std::vector<double> originalR;
};