#include <SFML/Graphics.hpp>

enum KeyType {WHITE, BLACK};

class PianoKey {

private:

	sf::RectangleShape m_key_shape;

	KeyType m_key_type;

	float m_width;
	float m_height;

	float m_frequency;

	int m_midi_note;

private:

	void UpdateSize();

public:

	PianoKey();
	PianoKey(float frequency, int midiNote, KeyType keyType, sf::Color color = sf::Color::White);
	~PianoKey() = default;

	void Draw(sf::RenderWindow& window);

	void SetKeyPosition(const sf::Vector2f& position);
	void SetKeyHeight(float height);
	void SetKeyWidth(float width);

	sf::Vector2f GetKeyPosition() { return m_key_shape.getPosition(); }
	sf::RectangleShape& GetKeyShape() { return m_key_shape; }
	float& GetFrequency() { return m_frequency; }
	float& GetWidth() { return m_width; }
	float& GetHeight() { return m_height; }

	bool IsBlack() { return m_key_type == BLACK; }
};