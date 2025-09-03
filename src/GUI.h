#include <imgui/imgui.h>
#include <imgui/imgui-SFML.h>

#include <SFML/Graphics.hpp>

#include <utils/logging.hpp>

class GUI {

private:

	ImGuiStyle* m_style;

public:

	GUI();
	GUI(sf::RenderWindow& window);
	~GUI() = default;

	void Init(sf::RenderWindow& window);
	void Update(sf::RenderWindow& window, sf::Time dt);
	void ProcessEvent(sf::RenderWindow& window, const std::optional<sf::Event>& event);
	void Render(sf::RenderWindow& window);
	void Close();
	void InitStyle();

};