#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#include <SFML/Audio.hpp>
#include <SFML/System/Time.hpp>

#include <fstream>
#include <sstream>

#include "PianoKey.h"

#include <midifile/MidiFile.h>
#include <utils/rng.hpp>
#include <utils/logging.hpp>

#include <future>
#include <array>
#include <list>

constexpr size_t g_number_of_keys = 88;

struct NoteEvent {

	int note = 21;
	double duration = 0;
	double startTime = 0;
	bool hasBeenStruck = false;
};

class Piano {

private:

	std::array<sf::SoundBuffer, g_number_of_keys> m_sound_buffers;
	std::vector<sf::Sound> m_sounds;

	std::array<PianoKey, g_number_of_keys> m_keys;
	std::vector<NoteEvent> m_note_events;

	std::vector<std::future<void>> m_key_sound_futures;

	std::vector<int> m_pressed_note_indices;

	std::vector<int16_t> m_samples;

	double m_midi_file_duration;

	sf::Clock m_composition_clock;

	float m_sample_rate;

	float m_composition_elapsed_time;
	float m_composition_playback_speed;

	float m_volume;

	int m_number_of_voices;

	bool m_is_composition_playing;

private:

	// Generate samples for each key using threads
	void GenerateKeySounds();

	// Generate the waveform for a specific key
	void GenerateKeyWaveForm(int keyNumber, float duration);

	// Generates the raw audio samples
	std::vector<int16_t> GenerateKeySamples(PianoKey& key, int keyNumber, float duration);

	// Simulates harmonics to create the piano timbre
	float GenerateKeyOvertones(PianoKey& key, int maxOvertones, float time, float normalizedFrequency);

	// Applies the volume envelope
	float ADSR(float t, float duration, int keyNumber);

	// Generate the frequency for a specific key
	float GenerateKeyFrequency(int keyNumber);
	void GenerateKeyFrequencies();

	// Initialize the fixed-size sound pool
	void InitSounds();

	int GetActiveSoundsCount();

	bool IsKeyBlack(int keyNumber) const;

public:

	Piano();
	~Piano() = default;

	void DrawKeys(sf::RenderWindow& window);

	// Play a specific note using the sound pool
	void StrikeKey(int keyNumber);

	// Set the key's color back to it's original and set it's struck flag to false
	void ReleaseKey(int keyNumber);
	void ReleaseKeys();

	// Play the loaded .mid file
	void PlayComposition();

	void StartComposition();
	void PauseComposition();
	void RestartComposition();

	// Set all note events' struck flag to false 
	void ResetNoteEvents();

	// Parse the MIDI file
	void LoadMidiFile(const std::string& fileName);

	// Place the keys in correct positions
	void SetKeyPositions(float windowWidth, float windowHeight);

	void UpdateVolume(float volume);
	void UpdatePlaybackSpeed(float playbackSpeed);

	void StopAllSounds();

	PianoKey& GetKey(int keyNumber) { return m_keys[keyNumber - 21]; }

	std::array<PianoKey, g_number_of_keys>& GetKeys() { return m_keys; }

	double GetMidiFileDuration() const { return m_midi_file_duration; }

	float GetCompositionElapsedTime() const { return m_composition_elapsed_time; }
	float GetCompositionPlaybackSpeed() { return m_composition_playback_speed; }
	float GetVolume() { return m_volume; }

	bool IsCompositionPlaying() const { return m_is_composition_playing; }
};