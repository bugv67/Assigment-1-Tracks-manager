#include "DJControllerService.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>

DJControllerService::DJControllerService(size_t cache_size)
    : cache(cache_size) {}
/**
 * TODO: Implement loadTrackToCache method
 */
int DJControllerService::loadTrackToCache(AudioTrack& track) {
    // Your implementation here 
     std::string title = track.get_title();
    if (cache.contains(title)) {
        //HIT
        cache.get(title);
        return 1;
    } else {
        //MISS
        PointerWrapper<AudioTrack> clonedWrapper = track.clone();
        AudioTrack* clone = clonedWrapper.get();
        if (clone == nullptr) {
           std::cout << "[ERROR] Track: \"" << track.get_title()
              << "\" failed to clone" << std::endl;
            return 0;
        }  
        clone->load();
        clone->analyze_beatgrid();
        bool evicted = cache.put(std::move(clonedWrapper));
        if (evicted) return -1;
    }
    return 0; // Placeholder
}

void DJControllerService::set_cache_size(size_t new_size) {
    cache.set_capacity(new_size);
}
//implemented
void DJControllerService::displayCacheStatus() const {
    std::cout << "\n=== Cache Status ===\n";
    cache.displayStatus();
    std::cout << "====================\n";
}

/**
 * TODO: Implement getTrackFromCache method
 */
AudioTrack* DJControllerService::getTrackFromCache(const std::string& track_title) {
    // Your implementation here
   return cache.get(track_title);
}
