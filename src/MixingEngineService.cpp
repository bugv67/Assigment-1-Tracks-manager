#include "MixingEngineService.h"
#include <iostream>
#include <memory>

/**
 * TODO: Implement MixingEngineService constructor
 */
MixingEngineService::MixingEngineService()
    : decks(), active_deck(1), auto_sync(false), bpm_tolerance(0)
{
    decks[0] = nullptr;
    decks[1] = nullptr;

    // log msg??
    std::cout << "[MixingEngineService] Initialized with 2 empty decks." << std::endl;
}

/**
 * TODO: Implement MixingEngineService destructor
 */
MixingEngineService::~MixingEngineService()
{
    // i get a trac as a refence so i do not own it- do not need to delete it
    for (int i = 0; i < 2; i++)
    {
        if (decks[i] != nullptr)
        {
            delete decks[i];
            decks[i] = nullptr;
        }
    }

    decks[0] = nullptr;
    decks[1] = nullptr;

    std::cout << "[MixingEngineService] Cleaning up decks..." << std::endl;
}

/**
 * TODO: Implement loadTrackToDeck method
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 */
int MixingEngineService::loadTrackToDeck(const AudioTrack &track)
{
    // Your implementation here
    std::cout << " \n=== Loading Track to Deck === " << std::endl;
    //  PointerWrapper<MP3Track>

    PointerWrapper<AudioTrack> clone = track.clone();
    if (clone.get() == nullptr)
    {
        std::cout << track.get_title() << " failed to clone" << std::endl;
        return -1;
    }
    int target = 1 - active_deck;
    std::cout << " [Deck Switch] Target deck: " << target << std::endl;
    // unload
    if (decks[target] != nullptr)
    {
        delete decks[target];
        decks[target] = nullptr;
    }
    // simulating loading and bet anal
    clone->load();
    clone->analyze_beatgrid();
    if (decks[active_deck] != nullptr && auto_sync && can_mix_tracks(clone))
    {
        sync_bpm(clone);
    }
    decks[target] = clone.release();
    std::cout << " [Load Complete ] " << track.get_title() << std::endl;
    if (decks[active_deck] != nullptr)
    {
        std::cout << " [Unload] Unloading previous deck" << active_deck << " (" << decks[active_deck]->get_title() << ")" << std::endl;
        delete decks[active_deck];
    }
    active_deck = target;
    std::cout << " [Active deck] Switched to deck" << target << std::endl;

    return target;
}

/**
 * @brief Display current deck status
 */
void MixingEngineService::displayDeckStatus() const
{
    std::cout << "\n=== Deck Status ===\n";
    for (size_t i = 0; i < 2; ++i)
    {
        if (decks[i])
            std::cout << "Deck " << i << ": " << decks[i]->get_title() << "\n";
        else
            std::cout << "Deck " << i << ": [EMPTY]\n";
    }
    std::cout << "Active Deck: " << active_deck << "\n";
    std::cout << "===================\n";
}

/**
 * TODO: Implement can_mix_tracks method
 *
 * Check if two tracks can be mixed based on BPM difference.
 *
 * @param track: Track to check for mixing compatibility
 * @return: true if BPM difference <= tolerance, false otherwise
 */
bool MixingEngineService::can_mix_tracks(const PointerWrapper<AudioTrack> &track) const
{
    // Your implementation here

    if (decks[active_deck] != nullptr && !track)
    {
        int active_bpm = decks[active_deck]->get_bpm();
        int clone_bpm = track->get_bpm();
        int absolote_bpm = std::abs(clone_bpm - active_bpm); /////////////
        if (absolote_bpm <= bpm_tolerance)
        {
            return true;
        }
    }
    return false;
}

/**
 * TODO: Implement sync_bpm method
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack> &track) const
{
    if (decks[active_deck] != nullptr && track)
    {
        int bpm = decks[active_deck]->get_bpm();
        int avg_bpm = (track->get_bpm() + decks[active_deck]->get_bpm()) / 2;
        std::cout << "[Sync BPM]: Syncing BPM from  " << bpm << " to" << avg_bpm << "\n";
    }
}
