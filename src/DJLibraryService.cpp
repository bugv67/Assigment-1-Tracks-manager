#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>
#include <filesystem>


DJLibraryService::DJLibraryService(const Playlist& playlist) 
    : playlist(playlist), library() {}

DJLibraryService::~DJLibraryService() {
    for (AudioTrack* track : library)
        delete track;
    library.clear();
}
/**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */
void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo>& library_tracks) {
    //Todo: Implement buildLibrary method
    for (const SessionConfig::TrackInfo& track : library_tracks) {
        AudioTrack* newTrack = nullptr;
        
        if (track.type == "MP3") {
            newTrack = new MP3Track(track.title, track.artists, track.duration_seconds, track.bpm,
            track.extra_param1, track.extra_param2);
        } else if (track.type == "WAV") {
            newTrack = new WAVTrack(track.title, track.artists, track.duration_seconds, track.bpm,
            track.extra_param1, track.extra_param2);

        }
        if (newTrack != nullptr) {
            library.push_back(newTrack);
        }
        // else?? handle exceptions??
    }
        std::cout << "[INFO] Track library built: " << library.size() << " tracks loaded\n";
}

/**
 * @brief Display the current state of the DJ library playlist
 * 
 */
void DJLibraryService::displayLibrary() const {
    std::cout << "=== DJ Library Playlist: " 
              << playlist.get_name() << " ===" << std::endl;

    if (playlist.is_empty()) {
        std::cout << "[INFO] Playlist is empty.\n";
        return;
    }

    // Let Playlist handle printing all track info
    playlist.display();

    std::cout << "Total duration: " << playlist.get_total_duration() << " seconds" << std::endl;
}

/**
 * @brief Get a reference to the current playlist
 * 
 * @return Playlist& 
 */
Playlist& DJLibraryService::getPlaylist() {
    // Your implementation here
    return playlist;
}

/**
 * TODO: Implement findTrack method
 * 
 * HINT: Leverage Playlist's find_track method
 */
AudioTrack* DJLibraryService::findTrack(const std::string& track_title) {
    // Your implementation here
   return playlist.find_track(track_title);
}

void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, 
                                               const std::vector<int>& track_indices) {
    // Your implementation here
     std::cout << "[INFO] Loading playlist: " << playlist_name << std::endl;
     // maybe it should be on the stack??
     //new playlist or playlist we have
    Playlist newPlaylist(playlist_name);
    for(int oneIdx : track_indices) {
        int idx = oneIdx - 1;
        if (idx < 0 || idx >= library.size()) {
             std::cout << "[WARNING] Invalid track index: " << idx << std::endl;
            continue;
        }

        PointerWrapper<AudioTrack> clone = library[idx]->clone();
        if (clone.get() == nullptr) {
            // how to log??
            std::cout << "[ERROR] failed to clone" << std::endl;
            continue;
        }
        clone->load();
        clone->analyze_beatgrid();
        newPlaylist.add_track(clone.get());
    }
    playlist = newPlaylist;
    std::cout << "[INFO] Playlist loaded: " << playlist_name
          << " (" << playlist.getTracks().size() << " tracks)\n";

}
/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {
    // Your implementation here
    std::vector<std::string> titles;
    for(AudioTrack* track : playlist.getTracks()) {
        std::string title = track->get_title();
        titles.push_back(title);
    }
    return titles;
}
