#include "Piano.h"
#include <print>

Piano::Piano()
	: m_sample_rate(44100) {

	GenerateKeyFrequencies();
	GenerateWaveTable();

	LOG("-- Piano has been initialized --");
}

void Piano::GenerateWaveTable() {

	LOG("-- Generating key waveforms --");

	m_sound_buffers.resize(88);
	m_sound_wave_table.clear();
	m_sound_wave_table.resize(88);

	float duration = 3.f;

	LOG("-- Creating threads --");
	// Generate samples for each key
	for (int keyNumber = 0; keyNumber < 88; keyNumber++) {

		m_key_sound_futures.emplace_back(std::async(std::launch::async, &Piano::GenerateKeyWaveForm, this, keyNumber, duration));
	}

	LOG("-- Waiting for threads to finish --");
	// Wait for all threads to finish before using the buffers
	for (auto& future : m_key_sound_futures) {
		future.get();
	}
	m_key_sound_futures.clear();
}

void Piano::GenerateKeyWaveForm(int keyNumber, float duration) {

	PianoKey& key = m_keys[keyNumber];

	std::vector<sf::Int16> samples = GenerateKeySamples(key, keyNumber, duration);

	m_sound_buffers[keyNumber].loadFromSamples(samples.data(), samples.size(), 1, (unsigned int)m_sample_rate);
	m_sound_wave_table[keyNumber].setBuffer(m_sound_buffers[keyNumber]);

	LOG("-- Key: {} has been generated --", keyNumber);
}

std::vector<sf::Int16> Piano::GenerateKeySamples(PianoKey& key, int keyNumber, float duration) {

	int totalSamplesCount = (int)(m_sample_rate * duration);

	std::vector<sf::Int16> samples;
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

		sf::Int16 sample = (sf::Int16)(32767 * 0.1f * sampleValue);
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

void Piano::LoadMusicFile(const std::string& fileName) {

	LOG("-- Loading file: {} --", fileName);

	m_note_events.clear();

	std::ifstream file(fileName, std::ios::binary);

	smf::MidiFile midifile;

	if (!midifile.read(file)) {
		LOG("-- File: {} couldn't be loaded --", fileName);
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
				n.duration = 0.f;

				m_note_events.emplace_back(n);
			}
		}
	}

	for (int i = 0; i + 1 < m_note_events.size(); i++) {

		m_note_events[i].duration = m_note_events[i + 1].startTime - m_note_events[i].startTime;

		if (m_note_events[i + 1].startTime - m_note_events[i].startTime < 0.001f)
			m_note_events[i].duration + 0.2;
	}

	LOG("-- File: {} has been successfully loaded --", fileName);
}

void Piano::PlaySong() {

	int i = 1;
	for (auto& n : m_note_events) {
		std::println("Playing Note ({}/{}): {}", i, m_note_events.size(), n.note);
		StrikeKey(n.note);
		sf::sleep(sf::seconds((float)n.duration));
		i++;
	}
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

	m_keys.reserve(88);

	for (int i = 0; i < 88; i++) {

		float frequency = GenerateKeyFrequency(i + 1);

		m_keys.emplace_back(PianoKey(frequency, i + 21, 
			IsKeyBlack(i + 21) ? BLACK : WHITE,
			IsKeyBlack(i + 21) ? sf::Color::Black : sf::Color::White));
	}
}
void Piano::SetKeyPositions(float windowWidth, float windowHeight) {

	float gap = 2.f;
	int numKeys = 88;
	int numWhiteKeys = 52;
	int numBlackKeys = 36;

	float totalGapWidth = (numWhiteKeys - 1) * gap;
	float totalKeyWidth = windowWidth - totalGapWidth;
	float keyWidth = totalKeyWidth / numWhiteKeys;

	// Space out white keys
	int keyIndex = 0;
	for (int i = 0; i < numKeys; i++) {

		if (IsKeyBlack(i + 21))
			continue;

		sf::Vector2f position = { keyIndex * (keyWidth + gap), windowHeight - m_keys[i].GetHeight()};
		m_keys[i].SetKeyPosition(position);
		m_keys[i].SetKeyWidth(keyWidth);

		keyIndex++;
	}

	// Space out black keys
	for (int i = 0; i < numKeys; i++) {

		if (!IsKeyBlack(i + 21))
			continue;

		m_keys[i].SetKeyWidth(keyWidth * 0.5f);
		sf::Vector2f position = { (m_keys[i - 1].GetKeyPosition().x + m_keys[i - 1].GetWidth() - m_keys[i].GetWidth() / 2.f), m_keys[i - 1].GetKeyPosition().y };
		m_keys[i].SetKeyPosition(position);
		m_keys[i].SetKeyHeight(m_keys[i].GetHeight() * 0.6f);
	}
}

void Piano::DrawKeys(sf::RenderWindow& window) {

	for (int i = 0; i < 88; i++) {

		if(!IsKeyBlack(i + 21))
			m_keys[i].Draw(window);
	}

	for (int i = 0; i < 88; i++) {

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