#include <SFML/Graphics.hpp>

enum KeyType {WHITE, BLACK};

const sf::Color g_pressed_key_color(184, 184, 184);

class PianoKey {

private:

	sf::RectangleShape m_key_shape;

	sf::Color m_original_color;

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
	void SetKeyColor(const sf::Color& color);

	sf::Vector2f GetKeyPosition() { return m_key_shape.getPosition(); }
	sf::RectangleShape& GetKeyShape() { return m_key_shape; }
	float& GetFrequency() { return m_frequency; }
	float& GetWidth() { return m_width; }
	float& GetHeight() { return m_height; }
	sf::Color& GetOriginalColor() { return m_original_color; }

	bool IsBlack() { return m_key_type == BLACK; }
};