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

constexpr size_t g_number_of_keys = 88;

class Piano {

private:

	struct NoteEvent {
		int note = 21;
		double duration = 0;
		double startTime = 0;
		bool hasBeenStruck = false;
	};


	std::array<sf::SoundBuffer, g_number_of_keys> m_sound_buffers;
	std::vector<sf::Sound> m_sound_wave_table;

	std::array<PianoKey, g_number_of_keys> m_keys;
	std::vector<NoteEvent> m_note_events;

	std::vector<std::future<void>> m_key_sound_futures;

	std::vector<int> m_pressed_note_indices;

	std::vector<int16_t> m_samples;

	std::mutex m_sound_table_mutex;

	double m_midi_file_duration;
	sf::Clock m_composition_clock;

	float m_sample_rate;

	float m_composition_elapsed_time;
	float m_composition_playback_speed;

	float m_volume;

	bool m_is_composition_playing;

private:

	void GenerateWaveTable();
	void GenerateKeyWaveForm(int keyNumber, float duration);
	std::vector<int16_t> GenerateKeySamples(PianoKey& key, int keyNumber, float duration);
	float GenerateKeyOvertones(PianoKey& key, int maxOvertones, float time, float normalizedFrequency);
	float ADSR(float t, float duration, int keyNumber);
	float GenerateKeyFrequency(int keyNumber);
	void GenerateKeyFrequencies();

	bool IsKeyBlack(int keyNumber);

public:

	Piano();
	~Piano() = default;

	void DrawKeys(sf::RenderWindow& window);

	void StrikeKey(int keyNumber);

	void ReleaseKey(int keyNumber);

	void PlayComposition();
	void StartComposition();
	void PauseComposition();
	void RestartComposition();
	void LoadMidiFile(const std::string& fileName);

	void SetKeyPositions(float windowWidth, float windowHeight);
	void UpdateVolume();

	PianoKey& GetKey(int keyNumber) { return m_keys[keyNumber - 21]; }
	std::array<PianoKey, g_number_of_keys>& GetKeys() { return m_keys; }

	double GetMidiFileDuration() const { return m_midi_file_duration; }
	float GetCompositionElapsedTime() const { return m_composition_elapsed_time; }
	float* GetCompositionPlaybackSpeed() { return &m_composition_playback_speed; }
	float* GetVolume() { return &m_volume; }

	bool IsCompositionPlaying() const { return m_is_composition_playing; }
};