#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#include <SFML/Audio.hpp>

#include <fstream>

#include "PianoKey.h"

#include <midifile/MidiFile.h>
#include <utils/logging.hpp>

#include <future>
#include <array>

constexpr size_t g_number_of_keys = 88;

struct NoteEvent {

	int note = 21;
	double duration = 0;
	double startTime = 0;
	bool hasBeenStruck = false;
};

constexpr int g_max_overtones = 16;
constexpr int g_midi_sections = 8;

constexpr std::array<float, g_max_overtones> g_amp_A0 = { 0.4f, 0.6f, 0.8f, 1.f, 0.9f, 0.7f, 0.5f, 0.35f, 0.25f, 0.15f, 0.1f, 0.08f, 0.06f, 0.04f, 0.02f, 0.01f };

constexpr std::array<float, g_max_overtones> g_amp_C2 = { 0.6f, 0.8f, 1.f, 0.8f, 0.6f, 0.4f, 0.3f, 0.2f, 0.15f, 0.1f, 0.08f, 0.06f, 0.04f, 0.02f, 0.01f, 0.01f };

constexpr std::array<float, g_max_overtones> g_amp_C3 = { 0.8f, 1.f, 0.7f, 0.5f, 0.35f, 0.25f, 0.15f, 0.1f, 0.08f, 0.05f, 0.04f, 0.03f, 0.02f, 0.01f, 0.015f, 0.f };

constexpr std::array<float, g_max_overtones> g_amp_C4 = { 1.f, 0.7f, 0.4f, 0.25f, 0.15f, 0.1f, 0.08f, 0.05f, 0.04f, 0.03f, 0.02f, 0.01f, 0.01f, 0.01f, 0.01f, 0.f };

constexpr std::array<float, g_max_overtones> g_amp_C5 = { 1.f, 0.4f, 0.2f, 0.1f, 0.05f, 0.03f, 0.02f, 0.01f, 0.01f, 0.01f, 0.005f, 0.005f, 0.005f, 0.f, 0.f, 0.f };

constexpr std::array<float, g_max_overtones> g_amp_C6 = { 1.f, 0.15f, 0.05f, 0.02f, 0.01f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };

constexpr std::array<float, g_max_overtones> g_amp_C7 = { 1.f, 0.12f, 0.03f, 0.01f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };

constexpr std::array<float, g_max_overtones> g_amp_C8 = { 1.f, 0.1f, 0.02f, 0.01f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };

constexpr std::array<float, g_midi_sections> g_midi_points = { 21.f, 36.f, 48.f, 60.f, 72.f, 84.f, 96.f, 108.f };

constexpr std::array<std::array<float, g_max_overtones>, g_midi_sections> g_amp_profiles = {
	g_amp_A0,
	g_amp_C2,
	g_amp_C3,
	g_amp_C4,
	g_amp_C5,
	g_amp_C6,
	g_amp_C7,
	g_amp_C8
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
	std::vector<int16_t> GenerateKeySamples(PianoKey& key, float duration);

	// Simulates harmonics to create the piano timbre
	float GenerateKeyOvertones(PianoKey& key, float time);

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
