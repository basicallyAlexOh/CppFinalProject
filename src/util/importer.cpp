#include "util/importer.h"
#include <vector>
#include <filesystem>
#include <algorithm>
#include <string>
#include <stdexcept>

const std::vector<std::string> analogTypes = {".wav", ".aiff"};
const std::vector<std::string> digitalTypes = {".mp3"};


AudioTrack* audioFromFile(const std::string& path)
{
    std::filesystem::path fp(path);
    std::string ext = fp.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    if (!ext.empty() && ext.front() == '.')
        ext.erase(0, 1);
    if(std::find(analogTypes.begin(), analogTypes.end(), ext) == analogTypes.end()){
        return analogAudioFromFile(path);
    } else if(std::find(digitalTypes.begin(), digitalTypes.end(), ext) == digitalTypes.end()) {
        throw std::invalid_argument("digital not supported yet!"); 
    } else {
        throw std::invalid_argument("unknown file type!");
    }
}



AudioTrack* analogAudioFromFile(const std::string& path)
{
    SndfileHandle file(path);
    if (!file || file.error()) {
        throw std::runtime_error("Failed to open audio file: " + path);
    }

    // Read metadata
    int samplerate = file.samplerate();
    int channels   = file.channels();
    sf_count_t frames = file.frames();

    std::vector<double> samples(frames * channels);

    if(channels > 2){
        throw std::runtime_error("More than 2 channel audio file");
    }
    
    std::vector<double> l(frames);
    std::vector<double> r(frames);
    // libsndfile will convert everything to double automatically
    file.readf(samples.data(), frames);
    
    if(channels == 2){
        for(sf_count_t i = 0; i < frames; ++i){
            l[i] = samples[2*i];
            r[i] = samples[2*i+1];
        }
    } else if (channels == 1){
        for(sf_count_t i = 0; i < frames; ++i){
            l[i] = r[i] = samples[i];
        }
    }
    return new AudioTrack(l,r,samplerate);  
}


