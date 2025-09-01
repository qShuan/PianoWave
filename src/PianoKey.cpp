#include "PianoKey.h"
#include <print>

PianoKey::PianoKey()
	: m_key_type(WHITE),
	m_width(0.f),
	m_height(150.f),
	m_frequency(440),
	m_midi_note(21) {

	UpdateSize();
	m_key_shape.setFillColor(sf::Color::White);
}

PianoKey::PianoKey(float frequency, int midiNote, KeyType keyType, sf::Color color)
	: m_key_type(keyType),
	m_width(0.f),
	m_height(150.f),
	m_frequency(frequency),
	m_midi_note(midiNote) {

	UpdateSize();
	m_key_shape.setFillColor(color);
}

void PianoKey::UpdateSize() {

	m_key_shape.setSize({ m_width, m_height });
}

void PianoKey::SetKeyPosition(const sf::Vector2f& position) {

	m_key_shape.setPosition(position);
}

void PianoKey::SetKeyHeight(float height) {
	
	m_height = height;
	UpdateSize();
}

void PianoKey::SetKeyWidth(float width) {

	m_width = width;
	UpdateSize();
}

void PianoKey::Draw(sf::RenderWindow& window) {

	window.draw(m_key_shape);
}