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

class Piano {

private:

	struct NoteEvent {
		int note;
		double duration;
		double startTime;
	};

	std::vector<sf::SoundBuffer> m_sound_buffers;
	std::vector<sf::Sound> m_sound_wave_table;

	std::vector<PianoKey> m_keys;
	std::vector<NoteEvent> m_note_events;

	std::vector<sf::Int16> m_samples;

	float m_sample_rate;

private:

	void GenerateWaveTable();
	float ADSR(float t, float duration, int keyNumber);
	float GenerateKeyFrequency(int keyNumber);
	void GenerateKeyFrequencies();
	void SetKeyPositions();
	void LoadMusicFile(const std::string& fileName);

	bool IsKeyBlack(int keyNumber);

public:

	Piano();
	~Piano() = default;

	void DrawKeys(sf::RenderWindow& window);

	void StrikeKey(int keyNumber) {

		m_sound_wave_table[keyNumber - 21].play();
	}

	void PlaySong();

	PianoKey& GetKey(int keyNumber) { return m_keys[keyNumber - 21]; }
};