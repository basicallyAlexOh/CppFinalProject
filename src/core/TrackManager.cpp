#include "core/TrackManager.h"
#include "util/importer.h"
#include <iostream>
#include <cmath>
#include <span>
#include <algorithm>

TrackManager::TrackManager(int sample_rate) :
    sample_rate(sample_rate)
{}


void TrackManager::addTrack(std::string filePath)
{   
    try {
        tracks.push_back(std::make_unique<AudioTrack>(audioFromFile(filePath)));
    } catch (std::invalid_argument e) {
        std::cerr << e.what() << std::endl;
    }
}

void TrackManager::reorderTrack(size_t curInd, size_t newInd)
{
    if(newInd > size()) newInd = size();
    if(curInd < newInd){
        for(int i = curInd; i < newInd; ++i)
            std::swap(tracks[i], tracks[i+1]);
    } else {
        for(int i = curInd; i > newInd; --i)
            std::swap(tracks[i], tracks[i-1]);
    }
}

void TrackManager::deleteTrack(size_t ind)
{
    tracks.erase(tracks.begin() + ind);
}

void TrackManager::mergeTrack(size_t ind1, size_t ind2)
{
    double new_start_time = std::min(tracks[ind1]->getStartTime(), tracks[ind2]->getStartTime());
    if(tracks[ind1]->getStartTime() > tracks[ind2]->getStartTime()) std::swap(ind1, ind2);

    // track1 always starts first
    size_t track2_start = round((tracks[ind2]->getStartTime() - tracks[ind1]->getStartTime()) * sample_rate);
    size_t track2_end = track2_start + tracks[ind2]->numSamples();
    size_t track1_end = tracks[ind1]->numSamples();
    
    size_t buffer_size = std::max(track2_end, track1_end)+1;
    std::vector<double> l_buffer(buffer_size);
    std::vector<double> r_buffer(buffer_size);

    std::span<double> l_span(l_buffer);
    std::span<double> r_span(r_buffer);
    std::span<double> l_track1 = l_span.subspan(0,track1_end+1);
    std::span<double> l_track2 = l_span.subspan(track2_start, tracks[ind2]->numSamples());
    std::span<double> r_track1 = r_span.subspan(0,track1_end+1);
    std::span<double> r_track2 = r_span.subspan(track2_start, tracks[ind2]->numSamples());

    std::transform(l_track1.begin(), l_track1.end(), tracks[ind1]->L.begin(), l_track1.begin(), std::plus<>()); 
    std::transform(l_track2.begin(), l_track2.end(), tracks[ind2]->L.begin(), l_track2.begin(), std::plus<>());

    std::transform(r_track1.begin(), r_track1.end(), tracks[ind1]->R.begin(), r_track1.begin(), std::plus<>()); 
    std::transform(r_track2.begin(), r_track2.end(), tracks[ind2]->R.begin(), r_track2.begin(), std::plus<>());
    
    // TODO: i think compiler does move semantics already in optimization, but wonder if we can explicity do it
    trackPtr tmp = std::make_unique<AudioTrack>(new AudioTrack(l_buffer, r_buffer, sample_rate)); 
    std::swap(tracks[ind1], tmp);
    deleteTrack(ind2);
}  


inline TrackManager::iterator TrackManager::begin()
{
    return tracks.begin(); 
}

inline TrackManager::iterator TrackManager::end()
{
    return tracks.end();
}

inline TrackManager::const_iterator TrackManager::begin() const 
{
    return tracks.begin();
}

inline TrackManager::const_iterator TrackManager::end() const
{
    return tracks.end();
}


size_t TrackManager::size() const
{
    return tracks.size();
}


AudioTrack* TrackManager::combineAll() const
{
    size_t buffer_size = 0;
    for(int i = 0; i < tracks.size(); ++i){
        size_t start_ind = tracks[i]->getStartTime() * sample_rate;
        buffer_size = std::max(buffer_size, start_ind + tracks[i]->numSamples());
    }

    std::vector<double> l_buffer(buffer_size);
    std::vector<double> r_buffer(buffer_size);
    std::span<double> l_span(l_buffer);
    std::span<double> r_span(r_buffer);

    for(const trackPtr& ptr : tracks){
        size_t start_offset = round(ptr->getStartTime() * sample_rate); 
        size_t len = ptr->numSamples();
        std::span<double> lsub = l_span.subspan(start_offset, len);
        std::span<double> rsub = r_span.subspan(start_offset, len);
        std::transform(ptr->L.begin(), ptr->L.end(), lsub.begin(), lsub.begin(), std::plus<>()); 
        std::transform(ptr->R.begin(), ptr->R.end(), rsub.begin(), rsub.begin(), std::plus<>()); 
    }

    return new AudioTrack(l_buffer, r_buffer, sample_rate);
}

AudioTrack* TrackManager::combineTimeRange(double start, double end) const 
{
    if(end < start) throw std::invalid_argument("end is after start");
    size_t buffer_size = (end - start) * sample_rate + 1;

    std::vector<double> l_buffer(buffer_size); 
    std::vector<double> r_buffer(buffer_size);

    for(const trackPtr& ptr : tracks){
        if(ptr->start_time > end || ptr->start_time + ptr->duration() < start) continue;

        if(ptr->start_time > start) {
            // starts in middle
            size_t start_offset = round((ptr->start_time - start) * sample_rate);
            if(ptr->start_time + ptr->duration() < end){
                // track fully contained
                //  ------
                //   ---
                std::span<double> L_buffer_subspan(l_buffer.begin() + start_offset, 
                                                    l_buffer.begin() + start_offset + ptr->numSamples());
                std::span<double> R_buffer_subspan(r_buffer.begin() + start_offset, 
                                                    r_buffer.begin() + start_offset + ptr->numSamples());
                std::span<double> L_track_subspan(ptr->L.begin(), ptr->L.end());
                std::span<double> R_track_subspan(ptr->R.begin(), ptr->R.end());

                std::transform(L_buffer_subspan.begin(), L_buffer_subspan.end(), L_track_subspan.begin(), L_buffer_subspan.begin(), std::plus<>()); 
                std::transform(R_buffer_subspan.begin(), R_buffer_subspan.end(), R_track_subspan.begin(), R_buffer_subspan.begin(), std::plus<>()); 


            } else{
                // track extends only after
                //  -----
                //    -----

                // TODO: might need to check if we accidentally overurn our buffer by 1 sample.
                std::span<double> L_buffer_subspan(l_buffer.begin() + start_offset,  l_buffer.end());
                std::span<double> R_buffer_subspan(r_buffer.begin() + start_offset, r_buffer.end());
                std::span<double> L_track_subspan(ptr->L.begin(), ptr->L.begin() + buffer_size - start_offset);
                std::span<double> R_track_subspan(ptr->R.begin(), ptr->R.begin() + buffer_size - start_offset);

                std::transform(L_buffer_subspan.begin(), L_buffer_subspan.end(), L_track_subspan.begin(), L_buffer_subspan.begin(), std::plus<>()); 
                std::transform(R_buffer_subspan.begin(), R_buffer_subspan.end(), R_track_subspan.begin(), R_buffer_subspan.begin(), std::plus<>()); 


            }
        } else {
            // starts before
            size_t start_offset = round((start - ptr->start_time) * sample_rate);

            if(ptr->start_time + ptr->duration() < end){

                // track extends only before
                //    -------------
                //  ------


                std::span<double> L_buffer_subspan(l_buffer.begin(),  l_buffer.begin() + ptr->numSamples() - start_offset);
                std::span<double> R_buffer_subspan(r_buffer.begin(), r_buffer.begin() + ptr->numSamples() - start_offset);
                std::span<double> L_track_subspan(ptr->L.begin() + start_offset, ptr->L.end());
                std::span<double> R_track_subspan(ptr->R.begin() + start_offset, ptr->R.end());

                std::transform(L_buffer_subspan.begin(), L_buffer_subspan.end(), L_track_subspan.begin(), L_buffer_subspan.begin(), std::plus<>()); 
                std::transform(R_buffer_subspan.begin(), R_buffer_subspan.end(), R_track_subspan.begin(), R_buffer_subspan.begin(), std::plus<>()); 


            } else{

                // track extends before and after
                //    -------------
                //  -----------------

                std::span<double> L_buffer_subspan(l_buffer.begin(),  l_buffer.end());
                std::span<double> R_buffer_subspan(r_buffer.begin(), r_buffer.end());
                std::span<double> L_track_subspan(ptr->L.begin() + start_offset, ptr->L.begin() + buffer_size);
                std::span<double> R_track_subspan(ptr->R.begin() + start_offset, ptr->R.begin() + buffer_size);

                std::transform(L_buffer_subspan.begin(), L_buffer_subspan.end(), L_track_subspan.begin(), L_buffer_subspan.begin(), std::plus<>()); 
                std::transform(R_buffer_subspan.begin(), R_buffer_subspan.end(), R_track_subspan.begin(), R_buffer_subspan.begin(), std::plus<>()); 


            }

        }

    }

    return new AudioTrack(l_buffer, r_buffer);

}