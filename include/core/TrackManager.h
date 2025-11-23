#include "core/AudioTrack.h"
#include <vector>

class TrackManager {
    using iterator = typename std::vector<AudioTrack>::iterator;
    using const_iterator = typename  std::vector<AudioTrack>::const_iterator;


public:
    explicit TrackManager();

    void addTrack(AudioTrack&);

    void reorderTrack(size_t curInd, size_t newInd);

    void deleteTrack(size_t ind);

    void combineTracks(size_t ind1, size_t ind2);

    iterator begin();

    iterator end();

    const_iterator begin() const;
    
    const_iterator end() const;

    AudioTrack combineAll() const;

    size_t size() const;

    // combine all audio between start and end - will use for preview
    AudioTrack combineTimeRange(double start, double end) const;


private:
    std::vector<AudioTrack> tracks;

    int sample_rate;



};