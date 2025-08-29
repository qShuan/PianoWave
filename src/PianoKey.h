#include <SFML/Graphics.hpp>

class PianoKey {

private:

	float m_frequency;

public:

	PianoKey(float frequency);
	~PianoKey() = default;

	float GetFrequency() { return m_frequency; }

};