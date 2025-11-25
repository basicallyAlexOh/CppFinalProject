#include "core/AudioTrack.h"
#include <vector>
#include <memory>




class TrackManager {

    using trackPtr = typename std::unique_ptr<AudioTrack>;
    using iterator = typename std::vector<trackPtr>::iterator;
    using const_iterator = typename  std::vector<trackPtr>::const_iterator;


public:
    explicit TrackManager(int sample_rate = 44100);

    void addTrack(std::string filePath);

    void reorderTrack(size_t curInd, size_t newInd);

    void deleteTrack(size_t ind);

    void mergeTrack(size_t ind1, size_t ind2);

    iterator begin();

    iterator end();

    const_iterator begin() const;
    
    const_iterator end() const;

    size_t size() const;

    AudioTrack* combineAll() const;

    // combine all audio between start and end - will use for preview
    AudioTrack* combineTimeRange(double start, double end) const;


private:
    std::vector<trackPtr> tracks;

    int sample_rate;


};