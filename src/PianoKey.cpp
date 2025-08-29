#include "PianoKey.h"
#include <print>

PianoKey::PianoKey(float frequency, float midiNote, KeyType keyType, sf::Color color)
	: m_key_type(keyType),
	m_width(0.f),
	m_height(0.f),
	m_frequency(frequency),
	m_midi_note(midiNote) {

	InitShape();
	m_key_shape.setFillColor(color);
}

void PianoKey::InitShape() {

	float width = 0.f;

	if(!IsBlack())
		width = (1600.f) / (52.f);
	else
		width = (1600.f) / (88.f);

	m_width = width;

	m_key_shape.setSize({ width - 2.f, IsBlack() ? 150.f : 200.f});
}

void PianoKey::SetKeyPosition(const sf::Vector2f& position) {

	m_key_shape.setPosition(position);
}

void PianoKey::Draw(sf::RenderWindow& window) {

	window.draw(m_key_shape);
}