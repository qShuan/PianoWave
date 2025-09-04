#include "Piano.h"
#include <print>

Piano::Piano()
	: m_midi_file_duration(0),
	m_sample_rate(44100),
	m_composition_elapsed_time(0.f),
	m_composition_playback_speed(1.f),
	m_volume(100.f),
	m_is_composition_playing(false) {

	GenerateKeyFrequencies();
	GenerateWaveTable();

	LOG("-- Piano has been initialized --");
}

void Piano::GenerateWaveTable() {

	LOG("-- Generating key waveforms --");

	float duration = 3.f;

	LOG("-- Creating threads --");
	// Generate samples for each key
	for (int keyNumber = 0; keyNumber < g_number_of_keys; keyNumber++) {

		m_key_sound_futures.emplace_back(std::async(std::launch::async, &Piano::GenerateKeyWaveForm, this, keyNumber, duration));
	}

	LOG("-- Waiting for threads to finish --");
	// Wait for all threads to finish before using the buffers
	for (auto& future : m_key_sound_futures) {
		future.get();
	}
	m_key_sound_futures.clear();

	// Assign sounds here because multithreading breaks the order
	m_sound_wave_table.reserve(g_number_of_keys);
	for (int keyNumber = 0; keyNumber < g_number_of_keys; keyNumber++) {

		m_sound_wave_table.emplace_back(m_sound_buffers[keyNumber]);
	}
}

void Piano::GenerateKeyWaveForm(int keyNumber, float duration) {

	PianoKey& key = m_keys[keyNumber];

	std::vector<int16_t> samples = GenerateKeySamples(key, keyNumber, duration);

	(void)m_sound_buffers[keyNumber].loadFromSamples(samples.data(), samples.size(), 1, (unsigned int)m_sample_rate, {sf::SoundChannel::FrontLeft, sf::SoundChannel::FrontRight});

	LOG("-- Key: {} has been generated --", keyNumber);
}

std::vector<int16_t> Piano::GenerateKeySamples(PianoKey& key, int keyNumber, float duration) {

	int totalSamplesCount = (int)(m_sample_rate * duration);

	std::vector<int16_t> samples;
	samples.resize(totalSamplesCount);

	float maxFreq = 4187.f;
	float normalizedFrequency = key.GetFrequency() / maxFreq;

	int maxOvertones = std::min(10, (int)(m_sample_rate / (2 * key.GetFrequency())));

	for (int i = 0; i < totalSamplesCount; i++) {

		float t = (float)i / (m_sample_rate);

		float sampleValue = 0.f;
		float hammerStrikeEffectValue = 0.f;

		// A hammer strike effect
		if (t < 0.1f) {

			hammerStrikeEffectValue += utils::rng::FastRandom() * exp(-t * 1000.f);

			for (int ot = maxOvertones / 2; ot <= maxOvertones; ot++) {
				float overtoneFrequency = key.GetFrequency() * ot;
				hammerStrikeEffectValue += 1.6f * cos(0.2f * (float)M_PI * overtoneFrequency * t) * exp(-t * 1000.f);
			}
		}

		sampleValue += hammerStrikeEffectValue;

		// Add overtones
		float overtonesValue = GenerateKeyOvertones(key, maxOvertones, t, normalizedFrequency);
		sampleValue += overtonesValue;

		// Apply amplitude modifications
		float env = ADSR(t, duration, keyNumber);
		sampleValue *= 2.f / (float)M_PI + 8.f / ((float)M_PI * (float)M_PI);
		sampleValue *= env;

		int16_t sample = (int16_t)(32767 * 0.1f * sampleValue);
		samples[i] = sample;
	}

	return samples;
}

float Piano::GenerateKeyOvertones(PianoKey& key, int maxOvertones, float time, float normalizedFrequency) {

	float brightnessBoost = 1.f;
	float decayFactor = 1.f;

	float overtonesValue = 0.f;

	for (int ot = 1; ot <= maxOvertones; ot++) {

		float sign = (ot % 2 == 0 ? 1.f : -1.f);

		float overtoneFrequency = key.GetFrequency() * ot;

		//The higher the note, the faster it decays
		decayFactor += (3.f - decayFactor) * normalizedFrequency;

		// The higher the note, the more quiet it becomes
		brightnessBoost += (0.5f - brightnessBoost) * normalizedFrequency;

		// Sawtooth wave
		overtonesValue += brightnessBoost * (
			sign * sin(2.f * (float)M_PI * overtoneFrequency * time) / ((float)ot)
			) * exp(-time * ot * decayFactor);

		//Triangle wave
		overtonesValue += brightnessBoost * (
			sign * sin(2.f * (float)M_PI * (2.f * ot - 1) * key.GetFrequency() * time) / ((2.f * ot - 1) * (2.f * ot - 1) * 2.f)
			) * exp(-time * ot * decayFactor);

		// Detuning
		float detune = 1.002f;
		overtonesValue += 0.02f * sin(2.0f * (float)M_PI * key.GetFrequency() * detune * time) * exp(-time * (float)ot);
	}

	return overtonesValue;
}

void Piano::LoadMidiFile(const std::string& fileName) {

	LOG("-- Loading file: {} --", fileName);

	m_note_events.clear();
	m_pressed_note_indices.clear();

	std::ifstream file(fileName, std::ios::binary);

	if (!file.is_open()) {
		LOG("-- File {} couldn't be loaded --", fileName);
		return;
	}

	smf::MidiFile midifile;

	if (!midifile.read(file)) {
		LOG("-- Midifile {} couldn't be loaded --", fileName);
		return;
	}

	midifile.doTimeAnalysis();
	midifile.linkNotePairs();

	int tracks = midifile.getTrackCount();

	for (int track = 0; track < tracks; track++) {

		for (int event = 0; event < midifile[track].size(); event++) {

			if (midifile[track][event].isNoteOn()) {

				NoteEvent n;
				n.note = midifile[track][event].getKeyNumber();
				n.startTime = midifile[track][event].seconds;
				n.duration = midifile[track][event].getDurationInSeconds();

				// Clamp the duration (prevents key presses not being visible)
				if (n.duration < 0.1)
					n.duration = 0.1;

				m_note_events.emplace_back(n);
			}
		}
	}

	m_is_composition_playing = false;
	m_midi_file_duration = midifile.getFileDurationInSeconds();

	LOG("-- File {} has been successfully loaded --", fileName);
}

void Piano::StrikeKey(int keyNumber) {

	m_sound_wave_table[keyNumber - 21].play();
	m_keys[keyNumber - 21].SetKeyColor(g_pressed_key_color);
	m_keys[keyNumber - 21].SetStruck(true);
}

void Piano::ReleaseKey(int keyNumber) {

	m_keys[keyNumber - 21].SetKeyColor(m_keys[keyNumber - 21].GetOriginalColor());
	m_keys[keyNumber - 21].SetStruck(false);
}

void Piano::PlayComposition() {

	if (m_note_events.empty() || !m_is_composition_playing)
		return;

	m_composition_elapsed_time = m_composition_clock.getElapsedTime().asSeconds() * m_composition_playback_speed;

	for (int i = 0; i < m_note_events.size(); i++) {

		if (m_composition_elapsed_time >= m_note_events[i].startTime && !m_note_events[i].hasBeenStruck) {

			StrikeKey(m_note_events[i].note);

			// Save the note index
			m_pressed_note_indices.emplace_back(i);
			m_note_events[i].hasBeenStruck = true;
		}
	}

	// Release all keys that have been held for their full duration
	for (int i = (int)m_pressed_note_indices.size() - 1; i >= 0; i--) {

		int noteIndex = m_pressed_note_indices[i];

		if (m_composition_elapsed_time >= m_note_events[noteIndex].startTime + m_note_events[noteIndex].duration) {

			ReleaseKey(m_note_events[noteIndex].note);
			m_pressed_note_indices.erase(m_pressed_note_indices.begin() + i);
		}
	}

	if (m_composition_elapsed_time >= m_midi_file_duration) {

		m_is_composition_playing = false;
	}
}

void Piano::StartComposition() {

	if (m_note_events.empty() || m_is_composition_playing)
		return;

	if (m_composition_clock.isRunning()) {

		m_composition_clock.restart();

		for (size_t i = 0; i < m_note_events.size(); i++) {

			m_note_events[i].hasBeenStruck = false;
		}
	}
	else {

		m_composition_clock.start();
	}

	m_is_composition_playing = true;
}

void Piano::PauseComposition() {

	if (m_note_events.empty())
		return;

	m_is_composition_playing = false;
	m_composition_clock.stop();
}

void Piano::RestartComposition() {

	if (m_note_events.empty())
		return;

	m_composition_elapsed_time = 0;
	m_composition_clock.restart();

	for (size_t i = 0; i < m_note_events.size(); i++) {

		m_note_events[i].hasBeenStruck = false;
	}

	m_pressed_note_indices.clear();

	m_is_composition_playing = true;
}

float Piano::ADSR(float t, float duration, int keyNumber) {

	float sustainLevel = 0.7f;
	float attackTime = 0.01f;
	float decayTime = 0.1f;

	if (m_keys[keyNumber].GetFrequency() > 2000.f) {
		sustainLevel = 0.4f;
		attackTime = 0.005f;
		decayTime = 0.05f;
	}

	float releaseTime = 0.35f;
	float sustainTime = duration - (attackTime + decayTime + releaseTime);

	// Attack
	if (t < attackTime) 
		return t / attackTime;

	// Decay
	else if (t < attackTime + decayTime) {
		float prog = (t - attackTime) / decayTime;
		return 1.f - prog * (1.f - sustainLevel);
	}

	// Sustain
	else if(t < duration - releaseTime)
		return sustainLevel;

	// Release
	else if (t < duration) {

		float relProg = (t - (duration - releaseTime)) / releaseTime;
		float ampl = sustainLevel * (1.f - relProg);

		if (ampl < 0.0001f)
			return 0.f;

		return ampl;
	}

	return 0.f;
}

float Piano::GenerateKeyFrequency(int keyNumber) {

	return 440.f * powf(2.f, ((keyNumber - 49) / 12.f));
}

void Piano::GenerateKeyFrequencies() {

	LOG("-- Generating key frequencies --");

	for (int i = 0; i < g_number_of_keys; i++) {

		float frequency = GenerateKeyFrequency(i + 1);

		m_keys[i] = PianoKey(frequency, i + 21, 
			IsKeyBlack(i + 21) ? BLACK : WHITE,
			IsKeyBlack(i + 21) ? sf::Color::Black : sf::Color::White);
	}
}
void Piano::SetKeyPositions(float windowWidth, float windowHeight) {

	UpdateKeyPositions(windowWidth, windowHeight);

	float gap = 2.f;
	int numWhiteKeys = 52;

	float totalGapWidth = (numWhiteKeys - 1) * gap;
	float totalKeyWidth = windowWidth - totalGapWidth;
	float keyWidth = totalKeyWidth / numWhiteKeys;

	// Set the height for black keys
	for (int i = 0; i < g_number_of_keys; i++) {

		if (!IsKeyBlack(i + 21))
			continue;

		m_keys[i].SetKeyHeight(m_keys[i].GetHeight() * 0.6f);
	}
}

void Piano::UpdateKeyPositions(float windowWidth, float windowHeight) {

	float gap = 2.f;
	int numWhiteKeys = 52;
	int numBlackKeys = 36;

	float totalGapWidth = (numWhiteKeys - 1) * gap;
	float totalKeyWidth = windowWidth - totalGapWidth;
	float keyWidth = totalKeyWidth / numWhiteKeys;

	// Space out white keys
	int keyIndex = 0;
	for (int i = 0; i < g_number_of_keys; i++) {

		if (IsKeyBlack(i + 21))
			continue;

		sf::Vector2f position = { keyIndex * (keyWidth + gap), windowHeight - m_keys[i].GetHeight() };
		m_keys[i].SetKeyPosition(position);
		m_keys[i].SetKeyWidth(keyWidth);

		keyIndex++;
	}

	// Space out black keys
	for (int i = 1; i < g_number_of_keys; i++) {

		if (!IsKeyBlack(i + 21))
			continue;

		m_keys[i].SetKeyWidth(keyWidth * 0.5f);
		sf::Vector2f position = { (m_keys[i - 1].GetKeyPosition().x + m_keys[i - 1].GetWidth() - m_keys[i].GetWidth() / 2.f + gap / 2.f), m_keys[i - 1].GetKeyPosition().y };
		m_keys[i].SetKeyPosition(position);
	}
}

void Piano::UpdateVolume() {

	for (size_t i = 0; i < m_sound_wave_table.size(); i++) {

		m_sound_wave_table[i].setVolume(m_volume);
	}
}

void Piano::DrawKeys(sf::RenderWindow& window) {

	for (int i = 0; i < g_number_of_keys; i++) {

		if(!IsKeyBlack(i + 21))
			m_keys[i].Draw(window);
	}

	for (int i = 0; i < g_number_of_keys; i++) {

		if (IsKeyBlack(i + 21))
			m_keys[i].Draw(window);
	}
}

bool Piano::IsKeyBlack(int keyNumber) {

	int noteInOctave = keyNumber % 12;

	return noteInOctave == 1 || noteInOctave == 3 ||
		noteInOctave == 6 || noteInOctave == 8 ||
		noteInOctave == 10;
}