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
		int note;
		double duration;
		double timeToNextNote;
		double startTime;
		sf::Clock nextNoteClock;
		sf::Clock durationClock;
		bool hasBeenStruck = false;
	};


	std::array<sf::SoundBuffer, g_number_of_keys> m_sound_buffers;
	std::array<sf::Sound, g_number_of_keys> m_sound_wave_table;

	std::array<PianoKey, g_number_of_keys> m_keys;
	std::vector<NoteEvent> m_note_events;

	std::vector<std::future<void>> m_key_sound_futures;

	std::vector<int> m_pressed_note_indices;

	std::vector<sf::Int16> m_samples;

	float m_sample_rate;

	int m_current_note_index;

private:

	void GenerateWaveTable();
	void GenerateKeyWaveForm(int keyNumber, float duration);
	std::vector<sf::Int16> GenerateKeySamples(PianoKey& key, int keyNumber, float duration);
	float GenerateKeyOvertones(PianoKey& key, int maxOvertones, float time, float normalizedFrequency);
	float ADSR(float t, float duration, int keyNumber);
	float GenerateKeyFrequency(int keyNumber);
	void GenerateKeyFrequencies();
	void LoadMusicFile(const std::string& fileName);

	bool IsKeyBlack(int keyNumber);

public:

	Piano();
	~Piano() = default;

	void DrawKeys(sf::RenderWindow& window);

	void StrikeKey(int keyNumber);

	void ReleaseKey(int keyNumber);

	void PlaySong();

	void SetKeyPositions(float windowWidth, float windowHeight);

	PianoKey& GetKey(int keyNumber) { return m_keys[keyNumber - 21]; }
	std::array<PianoKey, g_number_of_keys>& GetKeys() { return m_keys; }
};