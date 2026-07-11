#include "Piano.h"

Piano::Piano()
	: m_midi_file_duration(0),
	m_sample_rate(44100),
	m_composition_elapsed_time(0.f),
	m_composition_playback_speed(1.f),
	m_volume(100.f),
	m_number_of_voices(512),
	m_is_composition_playing(false) {

	GenerateKeyFrequencies();
	GenerateKeySounds();
	
	InitSounds();

	LOG("Piano has been initialized");
}

void Piano::GenerateKeySounds() {

	LOG("Generating key waveforms");

	float duration = 3.f;

	LOG("Creating threads");

	// Generate samples for each key
	for (int keyNumber = 0; keyNumber < g_number_of_keys; keyNumber++) {

		m_key_sound_futures.emplace_back(std::async(std::launch::async, &Piano::GenerateKeyWaveForm, this, keyNumber, duration));
	}

	LOG("Waiting for threads to finish");

	// Wait for all threads to finish before using the buffers
	for (auto& future : m_key_sound_futures) {
		future.get();
	}
	m_key_sound_futures.clear();
}

void Piano::GenerateKeyWaveForm(int keyNumber, float duration) {

	PianoKey& key = m_keys[keyNumber];

	std::vector<int16_t> samples = GenerateKeySamples(key, duration);

	(void)m_sound_buffers[keyNumber].loadFromSamples(samples.data(), samples.size(), 1, (unsigned int)m_sample_rate, {sf::SoundChannel::FrontLeft, sf::SoundChannel::FrontRight});

	LOG("Key: {} has been generated", keyNumber);
}


float Lerp(float a, float b, float t) {
	return a + (b - a) * t;
}

float InverseLerp(float a, float b, float value) {

	if (std::abs(a - b) < 0.0001f) return 0.f;

	float t = (value - a) / (b - a);

	return std::max(0.f, std::min(1.0f, t));
}

std::vector<int16_t> Piano::GenerateKeySamples(PianoKey& key, float duration) {

	int totalSamplesCount = (int)(m_sample_rate * duration);

	std::vector<int16_t> samples;
	samples.resize(totalSamplesCount);

	for (int i = 0; i < totalSamplesCount; i++) {

		float time = (float)i / (m_sample_rate);

		int16_t sample = (int16_t)GenerateKeyOvertones(key, time);
		samples[i] = sample;
	}

	return samples;
}

float Piano::GenerateKeyOvertones(PianoKey& key, float time) {

	float overtonesValue = 0.f;

	float maxAmplitude = 32767.f;
	float amplitudeMultiplier = 0.2f;

	float midi = (float)key.GetMidiNote();

	// Get the right segment on the piano
	int segment = 0;
	for (int i = 0; i < g_midi_points.size() - 1; i++) {

		if (midi <= g_midi_points[i + 1]) {

			segment = i;
			break;
		}
	}

	float t = InverseLerp(g_midi_points[segment], g_midi_points[segment + 1], midi);

	const auto& profileA = g_amp_profiles[segment];
	const auto& profileB = g_amp_profiles[segment + 1];

	for (int ot = 1; ot <= g_max_overtones; ot++) {

		// float sign = (ot % 2 == 0 ? 1.f : -1.f);

		float overtoneFrequency = key.GetFrequency() * ot;

		float targetAmplitude = Lerp(profileA[ot - 1], profileB[ot - 1], t);
		targetAmplitude *= ADSR(time, 5.f, key.GetMidiNote() - 21) * maxAmplitude * amplitudeMultiplier;

		// Sawtooth wave
		/*overtonesValue += (
			sign * sin(2.f * (float)M_PI * overtoneFrequency * time) / ((float)ot)
			) * targetAmplitude;*/

		//Triangle wave
		/*overtonesValue += (
			sign * sin(2.f * (float)M_PI * (2.f * ot - 1) * overtoneFrequency * time) / ((2.f * ot - 1) * (2.f * ot - 1) * 2.f)
			) * targetAmplitude;*/

		// Detuning
		float detune = 1.002f;
		float detuneMultiplier = 0.02f;

		float phase = 2.f * (float)M_PI * overtoneFrequency * time;
		
		overtonesValue += targetAmplitude * sin(phase);
		overtonesValue += targetAmplitude * detuneMultiplier * sin(phase);
	}

	return overtonesValue;
}

void Piano::LoadMidiFile(const std::string& fileName) {

	LOG("Loading file: {}", fileName);

	std::ifstream file(fileName, std::ios::binary);

	if (!file.is_open()) {
		LOG("File {} couldn't be loaded", fileName);
		return;
	}

	smf::MidiFile midifile;

	if (!midifile.read(file)) {
		LOG("Midifile {} couldn't be loaded", fileName);
		return;
	}

	m_note_events.clear();
	m_pressed_note_indices.clear();

	midifile.doTimeAnalysis();
	midifile.linkNotePairs();

	int tracks = midifile.getTrackCount();

	// Get all the necessary information
	for (int track = 0; track < tracks; track++) {
		for (int event = 0; event < midifile[track].size(); event++) {

			// Save the note event
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
	m_composition_clock.reset();
	m_composition_elapsed_time = 0.f;

	m_midi_file_duration = midifile.getFileDurationInSeconds();

	LOG("File {} has been successfully loaded", fileName);
}

void Piano::StrikeKey(int keyNumber) {

	int key = keyNumber - 21;

	if (key < 0 || key >= g_number_of_keys)
		return;

	int activeSounds = GetActiveSoundsCount() + 1;

	// Decrease the volume depending on the amount of active sounds
	float volume = m_volume / std::sqrtf((float)activeSounds);

	// Clamp the volume not to go below 20% of the current m_volume
	float minVolume = m_volume * 0.2f;
	volume = std::max(volume, minVolume);

	// Play the first free sound
	bool played = false;
	for (auto& sound : m_sounds) {

		if (sound.getStatus() != sf::Sound::Status::Playing) {

			sound.setBuffer(m_sound_buffers[key]);
			sound.setVolume(volume);
			sound.play();
			played = true;

			break;
		}
	}

	// If no free sound was found, play on the oldest
	if (!played) {

		m_sounds[0].stop();
		m_sounds[0].setBuffer(m_sound_buffers[key]);
		m_sounds[0].setVolume(volume);
		m_sounds[0].play();
	}

	m_keys[key].SetColor(g_pressed_key_color);
	m_keys[key].SetStruck(true);
}

void Piano::ReleaseKey(int keyNumber) {

	int key = keyNumber - 21;

	if (key < 0 || key >= g_number_of_keys)
		return;

	m_keys[key].SetColor(m_keys[key].GetOriginalColor());
	m_keys[key].SetStruck(false);
}

void Piano::ReleaseKeys() {

	for (size_t i = 0; i < m_keys.size(); i++) {

		ReleaseKey(m_keys[i].GetMidiNote());
	}
}

void Piano::PlayComposition() {

	if (m_note_events.empty() || !m_is_composition_playing)
		return;

	m_composition_elapsed_time += m_composition_clock.restart().asSeconds() * m_composition_playback_speed;

	// Strike every note that has not been struck and the composition's elapsed time has surpassed it's start time
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

	// If the composition has finished, stop playing
	if (m_composition_elapsed_time >= m_midi_file_duration) {

		m_is_composition_playing = false;
	}
}

void Piano::StartComposition() {

	if (m_note_events.empty() || m_is_composition_playing)
		return;

	// If the clock has not been paused, restart, otherwise resume
	if (m_composition_clock.isRunning()) {

		m_composition_clock.restart();
		m_composition_elapsed_time = 0;

		ResetNoteEvents();
		ReleaseKeys();
	}
	else {

		m_composition_clock.start();

		// If the composition hasn't started, release all keys
		if(m_composition_elapsed_time < 0.0001f)
			ReleaseKeys();
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

	m_composition_clock.restart();
	m_composition_elapsed_time = 0;

	ResetNoteEvents();
	ReleaseKeys();

	m_pressed_note_indices.clear();

	m_is_composition_playing = true;
}

void Piano::ResetNoteEvents() {

	for (size_t i = 0; i < m_note_events.size(); i++) {

		m_note_events[i].hasBeenStruck = false;
	}
}

float Piano::ADSR(float t, float duration, int keyNumber) {

	float attackTime = 0.063f;
	float decayTime = 1.5f;

	float value = 0.0f;

	// Attack
	if (t < attackTime) {

		float prog = t / attackTime;

		float steepness = 5.0f;
		value = (1.0f - std::exp(-steepness * prog)) / (1.0f - std::exp(-steepness));
	}

	// Decay
	else if (t < duration) {

		float decayTimer = t - attackTime;
		float prog = decayTimer / decayTime;

		float steepness = 4.0f;
		value = (std::exp(-steepness * prog) - std::exp(-steepness)) / (1.0f - std::exp(-steepness));
	}

	return std::max(0.0f, value);
}

float Piano::GenerateKeyFrequency(int keyNumber) {

	return 440.f * powf(2.f, ((keyNumber - 49) / 12.f));
}

void Piano::GenerateKeyFrequencies() {

	LOG("Generating key frequencies");

	for (int i = 0; i < g_number_of_keys; i++) {

		float frequency = GenerateKeyFrequency(i + 1);

		m_keys[i] = PianoKey(frequency, i + 21, 
			IsKeyBlack(i + 21) ? BLACK : WHITE,
			IsKeyBlack(i + 21) ? sf::Color::Black : sf::Color::White);
	}
}

void Piano::InitSounds() {

	m_sounds.reserve(m_number_of_voices);

	for (int i = 0; i < m_number_of_voices; i++) {

		m_sounds.emplace_back(m_sound_buffers[0]);
	}
}

void Piano::SetKeyPositions(float windowWidth, float windowHeight) {

	float gap = 2.f;
	int numWhiteKeys = 52;
	int numBlackKeys = 36;

	float totalGapWidth = (numWhiteKeys - 1) * gap;
	float totalKeyWidth = windowWidth - totalGapWidth;
	float keyWidth = totalKeyWidth / numWhiteKeys;

	float whiteKeyHeight = windowHeight / 3.f;

	// Space out white keys
	int keyIndex = 0;
	for (int i = 0; i < g_number_of_keys; i++) {

		if (IsKeyBlack(i + 21))
			continue;

		m_keys[i].SetHeight(whiteKeyHeight);
		sf::Vector2f position = { keyIndex * (keyWidth + gap), windowHeight - m_keys[i].GetHeight() };
		m_keys[i].SetPosition(position);
		m_keys[i].SetWidth(keyWidth);

		keyIndex++;
	}

	float blackKeyHeightMultiplier = 0.6f;
	float blackKeyWidthMultiplier = 0.5f;

	// Space out black keys
	for (int i = 1; i < g_number_of_keys; i++) {

		if (!IsKeyBlack(i + 21))
			continue;

		m_keys[i].SetHeight(m_keys[i - 1].GetHeight() * blackKeyHeightMultiplier);
		m_keys[i].SetWidth(keyWidth * blackKeyWidthMultiplier);
		sf::Vector2f position = { (m_keys[i - 1].GetPosition().x + m_keys[i - 1].GetWidth() - m_keys[i].GetWidth() / 2.f + gap / 2.f), m_keys[i - 1].GetPosition().y };
		m_keys[i].SetPosition(position);
	}
}

void Piano::UpdateVolume(float volume) {

	m_volume = volume;

	int activeSounds = GetActiveSoundsCount();

	if (activeSounds == 0)
		return;

	// Decrease the volume depending on the amount of active sounds
	float newVolume = m_volume / std::sqrtf((float)activeSounds);

	// Clamp the volume not to go below 20% of the current m_volume
	float minVolume = m_volume * 0.2f;
	newVolume = std::max(newVolume, minVolume);

	for (auto& sound : m_sounds) {

		sound.setVolume(newVolume);
	}
}

void Piano::UpdatePlaybackSpeed(float playbackSpeed) {

	m_composition_playback_speed = playbackSpeed;
}

void Piano::StopAllSounds() {

	for (auto& sound : m_sounds) {

		sound.stop();
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

int Piano::GetActiveSoundsCount() {

	int activeSounds = 0;
	
	for (int i = 0; i < g_number_of_keys; i++) {

		if (m_sounds[i].getStatus() == sf::Sound::Status::Playing)
			activeSounds++;
	}

	return activeSounds;
}

bool Piano::IsKeyBlack(int keyNumber) const {

	int noteInOctave = keyNumber % 12;

	return noteInOctave == 1 || noteInOctave == 3 ||
		noteInOctave == 6 || noteInOctave == 8 ||
		noteInOctave == 10;
}