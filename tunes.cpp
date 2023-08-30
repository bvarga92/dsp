/* Simple tune generator.
 *
 * The keys_durations vector holds ordered pairs of key numbers and note durations. The key numbers refer to a normal 88-key
 * piano keyboard (see https://en.wikipedia.org/wiki/Piano_key_frequencies). A key number of zero indicates a pause. The
 * bmp variable determines the duration of the shortest note (i.e. a note with duration of 1) in beats per minute.
 *
 * Compilation: g++ -std=c++17 -o tunes.exe tunes.cpp
 */

#include <cstdint>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <cmath>

using namespace std;

typedef struct
{
    string name;
    uint16_t bpm;
    vector<pair<uint8_t, uint8_t>> keys_durations;
} song_t;

const vector<song_t> songs
{
    {
        "boci_boci_tarka",
        100,
        {{40, 1}, {44, 1}, {40, 1}, {44, 1}, {47, 2}, {47, 2}, {40, 1}, {44, 1}, {40, 1}, {44, 1}, {47, 2}, {47, 2}, {52, 1}, {51, 1}, {49, 1}, {47, 1}, {45, 2}, {49, 2}, {47, 1}, {45, 1}, {44, 1}, {42, 1}, {40, 2}, {40, 2}, {0, 2}}
    },
    {
        "bella_ciao",
        170,
        {{44, 1}, {49, 1}, {51, 1}, {52, 1}, {49, 4}, {44, 1}, {49, 1}, {51, 1}, {52, 1}, {49, 4}, {44, 1}, {49, 1}, {51, 1}, {52, 2}, {51, 1}, {49, 1}, {52, 2}, {51, 1}, {49, 1}, {56, 2}, {56, 2}, {56, 1}, {56, 1}, {54, 1}, {56, 1}, {57, 1}, {57, 4}, {57, 1}, {56, 1}, {54, 1}, {57, 1}, {56, 4}, {56, 1}, {54, 1}, {52, 1}, {51, 2}, {56, 2}, {52, 2}, {51, 2}, {49, 4}, {0, 2}}
    },
    {
        "katyusha",
        150,
        {{49, 3}, {51, 1}, {52, 3}, {49, 1}, {52, 1}, {52, 1}, {51, 1}, {49, 1}, {51, 2}, {44, 2}, {51, 3}, {52, 1}, {54, 3}, {51, 1}, {54, 1}, {54, 1}, {52, 1}, {51, 1}, {49, 4}, {56, 2}, {61, 2}, {59, 2}, {61, 1}, {59, 1}, {57, 1}, {57, 1}, {56, 1}, {54, 1}, {56, 2}, {49, 2}, {57, 3}, {54, 1}, {56, 3}, {52, 1}, {51, 1}, {44, 1}, {52, 1}, {51, 1}, {49, 4}, {0, 2}}
    },
    {
        "fur_elise",
        150,
        {{56, 1}, {55, 1}, {56, 1}, {55, 1}, {56, 1}, {51, 1}, {54, 1}, {52, 1}, {49, 2}, {0, 1}, {40, 1}, {44, 1}, {49, 1}, {51, 2}, {0, 1}, {44, 1}, {48, 1}, {51, 1}, {52, 2}, {0, 1}, {44, 1}, {56, 1}, {55, 1}, {56, 1}, {55, 1}, {56, 1}, {51, 1}, {54, 1}, {52, 1}, {49, 2}, {0, 1}, {40, 1}, {44, 1}, {49, 1}, {51, 2}, {0, 1}, {44, 1}, {52, 1}, {51, 1}, {49, 2}, {0, 2}}
    },
    {
        "grieg_mountain_king",
        250,
        {{46, 1}, {48, 1}, {49, 1}, {51, 1}, {53, 1}, {49, 1}, {53, 2}, {52, 1}, {48, 1}, {52, 2}, {51, 1}, {47, 1}, {51, 2}, {46, 1}, {48, 1}, {49, 1}, {51, 1}, {53, 1}, {49, 1}, {53, 1}, {58, 1}, {56, 1}, {53, 1}, {49, 1}, {53, 1}, {56, 4}, {53, 1}, {55, 1}, {57, 1}, {58, 1}, {60, 1}, {57, 1}, {60, 2}, {61, 1}, {57, 1}, {61, 2}, {60, 1}, {57, 1}, {60, 2}, {53, 1}, {55, 1}, {57, 1}, {58, 1}, {60, 1}, {57, 1}, {60, 2}, {61, 1}, {57, 1}, {61, 2}, {60, 4}, {0, 2}}
    },
    {
        "jingle_bells",
        300,
        {{35, 2}, {44, 2}, {42, 2}, {40, 2}, {35, 6}, {35, 1}, {35, 1}, {35, 2}, {44, 2}, {42, 2}, {40, 2}, {37, 6}, {0, 2}, {37, 2}, {45, 2}, {44, 2}, {42, 2}, {39, 6}, {0, 2}, {47, 2}, {47, 2}, {45, 2}, {42, 2}, {44, 6}, {0, 2}, {35, 2}, {44, 2}, {42, 2}, {40, 2}, {35, 6}, {0, 2}, {35, 2}, {44, 2}, {42, 2}, {40, 2}, {37, 6}, {37, 2}, {37, 2}, {45, 2}, {44, 2}, {42, 2}, {47, 2}, {47, 2}, {47, 2}, {47, 2}, {49, 2}, {47, 2}, {45, 2}, {42, 2}, {40, 4}, {47, 4}, {44, 2}, {44, 2}, {44, 4}, {44, 2}, {44, 2}, {44, 4}, {44, 2}, {47, 2}, {40, 3}, {42, 1}, {44, 6}, {0, 2}, {45, 2}, {45, 2}, {45, 3}, {45, 1}, {45, 2}, {44, 2}, {44, 2}, {44, 1}, {44, 1}, {44, 2}, {42, 2}, {42, 2}, {44, 2}, {42, 4}, {47, 4}, {44, 2}, {44, 2}, {44, 4}, {44, 2}, {44, 2}, {44, 4}, {44, 2}, {47, 2}, {40, 3}, {42, 1}, {44, 6}, {0, 2}, {45, 2}, {45, 2}, {45, 3}, {45, 1}, {45, 2}, {44, 2}, {44, 2}, {44, 1}, {44, 1}, {47, 2}, {47, 2}, {45, 2}, {42, 2}, {40, 8}, {0, 2}}
    },
    {
        "cantina_band",
        400,
        {{51, 2}, {56, 2}, {51, 2}, {56, 2}, {51, 1}, {56, 2}, {51, 1}, {0, 1}, {50, 1}, {51, 2}, {51, 1}, {50, 1}, {51, 1}, {49, 1}, {0, 1}, {48, 1}, {49, 1}, {48, 1}, {47, 3}, {44, 5}, {51, 2}, {56, 2}, {51, 2}, {56, 2}, {51, 1}, {56, 2}, {51, 1}, {0, 1}, {50, 1}, {51, 2}, {49, 1}, {0, 1}, {49, 3}, {48, 1}, {49, 2}, {54, 1}, {52, 2}, {51, 2}, {49, 3}, {51, 2}, {56, 2}, {51, 2}, {56, 2}, {51, 1}, {56, 2}, {51, 1}, {0, 1}, {50, 1}, {51, 2}, {54, 1}, {0, 1}, {54, 3}, {51, 1}, {49, 2}, {47, 3}, {44, 5}, {44, 4}, {47, 4}, {51, 4}, {54, 4}, {57, 2}, {56, 2}, {50, 1}, {51, 2}, {47, 1}, {0, 4}}
    },
    {
        "priate",
        300,
        {{49, 1}, {52, 1}, {54, 1}, {0, 1}, {54, 1}, {0, 1}, {54, 1}, {56, 1}, {57, 1}, {0, 1}, {57, 1}, {0, 1}, {57, 1}, {59, 1}, {56, 1}, {0, 1}, {56, 1}, {0, 1}, {54, 1}, {52, 1}, {52, 1}, {54, 1}, {0, 2}, {49, 1}, {52, 1}, {54, 1}, {0, 1}, {54, 1}, {0, 1}, {54, 1}, {56, 1}, {57, 1}, {0, 1}, {57, 1}, {0, 1}, {57, 1}, {59, 1}, {56, 1}, {0, 1}, {56, 1}, {0, 1}, {54, 1}, {52, 1}, {54, 2}, {0, 2}, {49, 1}, {52, 1}, {54, 1}, {0, 1}, {54, 1}, {0, 1}, {54, 1}, {57, 1}, {59, 1}, {0, 1}, {59, 1}, {0, 1}, {59, 1}, {61, 1}, {62, 1}, {0, 1}, {62, 1}, {0, 1}, {61, 1}, {59, 1}, {61, 1}, {54, 1}, {0, 2}, {54, 1}, {56, 1}, {57, 1}, {0, 1}, {57, 1}, {0, 1}, {59, 1}, {0, 1}, {61, 1}, {54, 1}, {0, 2}, {54, 1}, {57, 1}, {56, 1}, {0, 1}, {56, 1}, {0, 1}, {57, 1}, {54, 1}, {56, 1}, {0, 4}}
    }
};

/* A simple piecewise linear ADSR envelope function. */
inline double adsr(double ratio)
{
    if (ratio < 0.2) // attack
    {
        return 5.0 * ratio;
    }
    else if (ratio < 0.3) // decay
    {
        return 1.2 - 1.0 * ratio;
    }
    else if (ratio < 0.7) // sustain
    {
        return 0.9;
    }
    else if (ratio < 1) // release
    {
        return 3.0 - 3.0 * ratio;
    }
    else
    {
        return 0;
    }
}

/* Instrument waveform. Period must be 2pi, range must be -1 ... +1. For best results, waveform(0) should return a value close to zero. */
inline double waveform(double phase)
{
    //return sin(phase);
    return 0.7241 * sin(phase) + 0.32 * sin(2 * phase) + 0.2281 * sin(3 * phase) + 0.0601 * sin(4 * phase);
}

/* Fills the signal vector (assumed empty) accorcing to the song parameters. */
void generateSoundData(vector<uint8_t> &signal, const vector<pair<uint8_t, uint8_t>> &keys_durations, uint32_t bpm)
{
    static const double PI = 3.14159265359;
    double phase = 0;
    for (auto [key, duration] : keys_durations)
    {
        double frequency = (key == 0) ? 0 : (440 * pow(2, (key - 49) / 12.0));
        int numSamples = duration * 60 * 44100 / bpm;
        for (int i = 0; (i < numSamples) || (phase > 0.1); i++)
        {
            signal.push_back(static_cast<uint8_t>(round(127.5 * (1 + waveform(phase) * adsr(i / double(numSamples))))));
            phase += 2 * PI * frequency / 44100;
            if (phase > 2 * PI)
            {
                phase -= 2 * PI;
            }
        }
    }
}

/* Writes the signal to a WAV file (44100 Hz, 8 bit, mono). */
void generateWavFile(vector<uint8_t> &signal, string filename)
{
    uint32_t chunkSize = signal.size() + 36;
    uint8_t header[44] = {
        0x52, 0x49, 0x46, 0x46,                                                                                                                                                                               // "RIFF"
        static_cast<uint8_t>(chunkSize & 0xFF), static_cast<uint8_t>((chunkSize >> 8) & 0xFF), static_cast<uint8_t>((chunkSize >> 16) & 0xFF), static_cast<uint8_t>((chunkSize >> 24) & 0xFF),                // file size - 8
        0x57, 0x41, 0x56, 0x45, 0x66, 0x6d, 0x74, 0x20,                                                                                                                                                       // "WAVEfmt "
        0x10, 0x00, 0x00, 0x00,                                                                                                                                                                               // subchunk 1 size (16 for PCM)
        0x01, 0x00,                                                                                                                                                                                           // audio format (1 for PCM)
        0x01, 0x00,                                                                                                                                                                                           // number of channels (1 for mono, 2 for stereo)
        0x44, 0xac, 0x00, 0x00,                                                                                                                                                                               // sampling rate
        0x44, 0xac, 0x00, 0x00,                                                                                                                                                                               // byte rate (SamplingRate * NumChannels * BitsPerSample / 8)
        0x01, 0x00,                                                                                                                                                                                           // block alignment (NumChannels * BitsPerSample / 8)
        0x08, 0x00,                                                                                                                                                                                           // bits per sample
        0x64, 0x61, 0x74, 0x61,                                                                                                                                                                               // "data"
        static_cast<uint8_t>(signal.size() & 0xFF), static_cast<uint8_t>((signal.size() >> 8) & 0xFF), static_cast<uint8_t>((signal.size() >> 16) & 0xFF), static_cast<uint8_t>((signal.size() >> 24) & 0xFF) // number of bytes in the data chunk
    };
    ofstream fout(filename, ios::out | ios::binary | ios::trunc);
    fout.write(reinterpret_cast<const char *>(header), 44);
    fout.write(reinterpret_cast<const char *>(signal.data()), signal.size());
    fout.close();
}

int main()
{
    vector<uint8_t> signal;
    for (auto [name, bpm, keys_durations] : songs)
    {
        signal.clear();
        generateSoundData(signal, keys_durations, bpm);
        generateWavFile(signal, name + ".wav");
    }
    return 0;
}
