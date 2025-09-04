#include <SFML/Graphics.hpp>

enum KeyType {WHITE, BLACK};

const sf::Color g_pressed_key_color(184, 184, 184);

class PianoKey {

private:

	sf::RectangleShape m_shape;

	sf::Color m_original_color;

	KeyType m_type;

	float m_width;
	float m_height;

	float m_frequency;

	int m_midi_note;

	bool m_has_been_struck;

private:

	void UpdateSize();

public:

	PianoKey();
	PianoKey(float frequency, int midiNote, KeyType keyType, sf::Color color = sf::Color::White);
	~PianoKey() = default;

	void Draw(sf::RenderWindow& window);

	void SetPosition(const sf::Vector2f& position);
	void SetHeight(float height);
	void SetWidth(float width);
	void SetColor(const sf::Color& color);
	void SetStruck(bool hasBeenStruck);

	sf::Vector2f GetPosition() { return m_shape.getPosition(); }
	sf::RectangleShape& GetShape() { return m_shape; }
	float& GetFrequency() { return m_frequency; }
	float& GetWidth() { return m_width; }
	float& GetHeight() { return m_height; }
	sf::Color& GetOriginalColor() { return m_original_color; }
	int& GetMidiNote() { return m_midi_note; }

	bool& HasBeenStruck() { return m_has_been_struck; }
	bool IsBlack() const { return m_type == BLACK; }
	bool IsPointInsideBounds(const sf::Vector2f& point);
};