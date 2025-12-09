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

// copy constractor
MixingEngineService::MixingEngineService(const MixingEngineService &other)
    : active_deck(other.active_deck),
      auto_sync(other.auto_sync),
      bpm_tolerance(other.bpm_tolerance)
{
    // Deep copy both deckes
    for (int i = 0; i < 2; ++i)
    {
        if (other.decks[i])
        {
            PointerWrapper<AudioTrack> clone = other.decks[i]->clone();
            decks[i] = clone.release();
        }
        else
        {
            decks[i] = nullptr;
        }
    }

    // std::cout << "[MixingEngineService] Copy constructor used.\n";
}

//  operator =
MixingEngineService &MixingEngineService::operator=(const MixingEngineService &other)
{
    if (this == &other)
        return *this;

    for (int i = 0; i < 2; ++i)
    {
        if (decks[i])
        {
            delete decks[i];
            decks[i] = nullptr;
        }
    }
    active_deck = other.active_deck;
    auto_sync = other.auto_sync;
    bpm_tolerance = other.bpm_tolerance;
    // copy thw deck
    for (int i = 0; i < 2; ++i)
    {
        if (other.decks[i])
        {
            PointerWrapper<AudioTrack> clone = other.decks[i]->clone();
            decks[i] = clone.release();
        }
        else
        {
            decks[i] = nullptr;
        }
    }

    return *this;
}

/**
 * TODO: Implement loadTrackToDeck method
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 */
int MixingEngineService::loadTrackToDeck(const AudioTrack &track)
{
    // Your implementation here
    std::cout << " \n=== Loading Track to Deck ===" << std::endl;
    PointerWrapper<AudioTrack> clone = track.clone();
    if (clone.get() == nullptr)
    {
        std::cout << track.get_title() << " failed to clone" << std::endl;
        return -1;
    }
    int target = 1 - active_deck;
    std::cout << "[Deck Switch] Target deck: " << target << std::endl;
    // unload
    if (decks[target] != nullptr)
    {
        // std::cout << " [Unload] Unloading previous deck" << active_deck << " (" << decks[active_deck]->get_title() << ")" << std::endl; //???
        delete decks[target];
        decks[target] = nullptr;
    }
    // simulating loading
    clone->load();
    clone->analyze_beatgrid();

    if (decks[active_deck] != nullptr && auto_sync && !can_mix_tracks(clone))
    {
        sync_bpm(clone);
    }
    if (decks[active_deck] == nullptr)
    {
        std::cout << "[Sync BPM] Cannot sync - one of the decks is empty." << std::endl;
    }
    decks[target] = clone.release();
    std::cout << "[Load Complete] '" << track.get_title() << "' is now loaded on deck " << target << std::endl;

    active_deck = target;
    std::cout << "[Active Deck] Switched to deck " << target << std::endl;

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

    if (decks[active_deck] != nullptr && track)
    {
        int active_bpm = decks[active_deck]->get_bpm();
        int clone_bpm = track->get_bpm();
        int absolute_bpm = std::abs(clone_bpm - active_bpm);
        if (absolute_bpm <= bpm_tolerance)
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
        int bpm = track->get_bpm();
        int avg_bpm = (track->get_bpm() + decks[active_deck]->get_bpm()) / 2;
        track->set_bpm(avg_bpm);
        std::cout << "[Sync BPM]: Syncing BPM from " << bpm << " to " << avg_bpm << "\n";
    }
}
