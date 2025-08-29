#include "PianoKey.h"
#include <print>

PianoKey::PianoKey(float frequency)
	: m_frequency(frequency),
	m_time(0.f),
	m_release_time(0.f),
	m_is_playing(false) {}

void PianoKey::Activate() {
	m_is_playing = true;
	is_releasing = false;
	m_time = 0.f;
	m_release_time = 0.f;
}

void PianoKey::Deactivate() {

	m_is_playing = false;
	is_releasing = true;
	m_release_time = m_time;
}