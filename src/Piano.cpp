#include "Piano.h"
#include <print>

Piano::Piano()
	: m_sample_rate(44100) {

	GenerateKeyFrequencies();
	SetKeyPositions();
	//GenerateWaveTable();
}

void Piano::GenerateWaveTable() {

	m_sound_buffers.resize(88);
	m_sound_wave_table.clear();
	m_sound_wave_table.resize(88);

	float duration = 3.f;

	// Generate samples for each key
	for (int keyNumber = 0; keyNumber < 88; keyNumber++) {

		PianoKey& key = m_keys[keyNumber];

		std::vector<sf::Int16> samples;
		samples.resize(m_sample_rate * duration);

		float maxFreq = 4187.f;
		float nyquist = maxFreq / 2.f;

		float normalizedFreq = key.GetFrequency() / maxFreq;

		for (int i = 0; i < m_sample_rate * duration; i++) {

			float t = (float)i / (m_sample_rate);

			float overtonesValue = 0.f;

			// Overtones
			int maxOvertones = std::min(10, (int)(m_sample_rate / (2 * key.GetFrequency())));

			// A hammer-like strike effect
			if (t < 0.1f) {

				overtonesValue += utils::rng::FastRandom() * exp(-t * 1000.f);

				for (int ot = maxOvertones / 2; ot <= maxOvertones; ot++) {
					float overtoneFrequency = key.GetFrequency() * ot;
					overtonesValue += 1.6f * cos(0.2f * M_PI * overtoneFrequency * t) * exp(-t * 1000.f);
				}
			}

			float brightnessBoost = 1.f;
			float decayFactor = 1.f;

			for (int ot = 1; ot <= maxOvertones; ot++) {

				float overtoneFrequency = key.GetFrequency() * ot;

				//The higher the note, the faster it decays
				decayFactor += (3.f - decayFactor) * normalizedFreq;

				// The higher the note, the more quiet it becomes
				brightnessBoost += (0.5f - brightnessBoost) * normalizedFreq;

				// Sawtooth wave
				overtonesValue += brightnessBoost * (
					pow(-1.f, (float)ot) * sin(2.f * M_PI * overtoneFrequency * t) / ((float)ot)
					) * exp(-t * ot * decayFactor);

				//Triangle wave
				overtonesValue += brightnessBoost * (
					pow(-1.f, (float)ot) * sin(2.f * M_PI * (2.f * ot - 1) * key.GetFrequency() * t) / ((2.f * ot - 1) * (2.f * ot - 1) * 2.f)
					) * exp(-t * ot * decayFactor);

				// Detuning
				float detune = 1.002f;
				overtonesValue += 0.02f * sin(2.0f * M_PI * key.GetFrequency() * detune * t) * exp(-t * (float)ot);
			}

			// Apply amplitude modifications
			float env = ADSR(t, duration, keyNumber);
			overtonesValue *= 2.f / M_PI + 8.f / (M_PI * M_PI);
			overtonesValue *= env;

			sf::Int16 sample = (sf::Int16)(32767 * 0.1f * overtonesValue);
			samples[i] = sample;
		}

		m_sound_buffers[keyNumber].loadFromSamples(samples.data(), samples.size(), 1, m_sample_rate);
		m_sound_wave_table[keyNumber].setBuffer(m_sound_buffers[keyNumber]);
	}
}

void Piano::LoadMusicFile(const std::string& fileName) {

	m_note_events.clear();

	std::ifstream file(fileName, std::ios::binary);

	smf::MidiFile midifile;

	midifile.read(file);
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

}

void Piano::PlaySong() {

	int i = 1;
	for (auto& n : m_note_events) {
		std::println("Playing Note ({}/{}): {}", i, m_note_events.size(), n.note);
		StrikeKey(n.note);
		sf::sleep(sf::seconds(n.duration));
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

	m_keys.reserve(88);

	for (int i = 0; i < 88; i++) {

		float frequency = GenerateKeyFrequency(i + 1);

		m_keys.emplace_back(PianoKey(frequency, i + 21, 
			IsKeyBlack(i + 21) ? BLACK : WHITE,
			IsKeyBlack(i + 21) ? sf::Color::Black : sf::Color::White));
	}
}
void Piano::SetKeyPositions() {

	float windowWidth = 1600.f;
	float gap = 2.f;
	int numKeys = 88;
	int numBlackKeys = 36;

	float totalGapWidth = (52 - 1) * gap;
	float totalKeyWidth = windowWidth - totalGapWidth;
	float keyWidth = totalKeyWidth / 52;

	// Space out white keys
	int keyIndex = 0;
	for (int i = 0; i < numKeys; i++) {

		if (IsKeyBlack(i + 21)) {

			continue;
		}

		sf::Vector2f position = { keyIndex * (keyWidth + gap), 0.f };
		m_keys[i].SetKeyPosition(position);

		keyIndex++;
	}

	totalGapWidth = (36) * gap;
	totalKeyWidth = windowWidth - totalGapWidth;
	keyWidth = totalKeyWidth / 36;

	// Space out black keys
	for (int i = 0; i < numKeys; i++) {

		if (!IsKeyBlack(i + 21)) {

			continue;
		}

		sf::Vector2f position = { (m_keys[i - 1].GetKeyPosition().x + m_keys[i - 1].GetWidth() - m_keys[i].GetWidth() / 2.f), 0.f};
		m_keys[i].SetKeyPosition(position);
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