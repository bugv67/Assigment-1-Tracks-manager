#include "AudioTrack.h"
#include <iostream>
#include <cstring>
#include <random>

AudioTrack::AudioTrack(const std::string &title, const std::vector<std::string> &artists,
                       int duration, int bpm, size_t waveform_samples)
    : title(title), artists(artists), duration_seconds(duration), bpm(bpm),
      waveform_size(waveform_samples)
{

    // Allocate memory for waveform analysis
    waveform_data = new double[waveform_size];

    // Generate some dummy waveform data for testing
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(-1.0, 1.0);

    for (size_t i = 0; i < waveform_size; ++i)
    {
        waveform_data[i] = dis(gen);
    }
#ifdef DEBUG
    std::cout << "AudioTrack created: " << title << " by " << std::endl;
    for (const auto &artist : artists)
    {
        std::cout << artist << " ";
    }
    std::cout << std::endl;
#endif
}

// ========== TODO: STUDENTS IMPLEMENT RULE OF 5 ==========

AudioTrack::~AudioTrack()
{
// TODO: Implement the destructor
#ifdef DEBUG
    std::cout << "AudioTrack destructor called for: " << title << std::endl;
#endif
    // Your code here...
    // need to delete obejects waveform_data
    delete[] waveform_data;
}

AudioTrack::AudioTrack(const AudioTrack &other) : title(other.title), artists(other.artists),
                                                  duration_seconds(other.duration_seconds), bpm(other.bpm), waveform_size(other.waveform_size)
{
    // TODO: Implement the copy constructor
    // deep copy: double* waveform_data
    // string ?
    // vector ?
    artists = std::vector<std::string>(other.artists);
    waveform_data = new double[other.waveform_size];

    for (int i = 0; i < other.waveform_size; ++i)
    {
        waveform_data[i] = other.waveform_data[i];
    }

#ifdef DEBUG
    std::cout << "AudioTrack copy constructor called for: " << other.title << std::endl;
#endif
    // Your code here...
}

AudioTrack &AudioTrack::operator=(const AudioTrack &other)
{
    // TODO: Implement the copy assignment operator

#ifdef DEBUG
    std::cout << "AudioTrack copy assignment called for: " << other.title << std::endl;
#endif
    // Your code here...
    if (this == &other)
    {
        return *this;
    }
    delete[] waveform_data;

    title = other.title;
    duration_seconds = other.duration_seconds;
    bpm = other.bpm;
    waveform_size = other.waveform_size;

    artists.clear();
    // TODO: Should this be a deep copy of the strings?
    artists.insert(artists.end(), other.artists.begin(), other.artists.end());
    waveform_data = new double[waveform_size];
    for (int i = 0; i < waveform_size; i++)
    {
        waveform_data[i] = other.waveform_data[i];
    }
    return *this;
}

AudioTrack::AudioTrack(AudioTrack &&other) noexcept : title(other.title), duration_seconds(other.duration_seconds), bpm(other.bpm),
                                                      waveform_size(other.waveform_size)
{

// TODO: Implement the move constructor
#ifdef DEBUG
    std::cout << "AudioTrack move constructor called for: " << other.title << std::endl;
#endif
    // Your code here...
    // set pointers to null - vector is not a pointer so no nullptr
    //                     - string?

    artists.clear();
    artists.insert(artists.end(), other.artists.begin(), other.artists.end());
    waveform_data = other.waveform_data; // init list?

    // What does it mean to zero out?
    other.waveform_data = new double[0];
    // other.waveform_data = nullptr;
    other.title = "";
    other.duration_seconds = 0;
    other.bpm = 0;
    other.waveform_size = 0;
    other.artists.clear();
}

AudioTrack &AudioTrack::operator=(AudioTrack &&other) noexcept
{
    // TODO: Implement the move assignment operator

#ifdef DEBUG
    std::cout << "AudioTrack move assignment called for: " << other.title << std::endl;
#endif
    // Your code here...
    if (this == &other)
    {
        return *this;
    }
    delete[] waveform_data;

    title = other.title;
    duration_seconds = other.duration_seconds;
    bpm = other.bpm;
    waveform_size = other.waveform_size;

    artists.clear();
    artists.insert(artists.end(), other.artists.begin(), other.artists.end());
    waveform_data = other.waveform_data;

    return *this;
}

void AudioTrack::get_waveform_copy(double *buffer, size_t buffer_size) const
{
    if (buffer && waveform_data && buffer_size <= waveform_size)
    {
        std::memcpy(buffer, waveform_data, buffer_size * sizeof(double));
    }
}