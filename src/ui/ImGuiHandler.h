#include <imgui/imgui.h>
#include <imgui/imgui-SFML.h>

#include <SFML/Graphics.hpp>

#include <utils/logging.hpp>

class ImGuiHandler {

private:

	ImGuiStyle* m_style;

public:

	ImGuiHandler();
	ImGuiHandler(sf::RenderWindow& window);
	~ImGuiHandler() = default;

	void Init(sf::RenderWindow& window);
	void Update(sf::RenderWindow& window, sf::Time dt);
	void ProcessEvent(sf::RenderWindow& window, const std::optional<sf::Event>& event);
	void Render(sf::RenderWindow& window);
	void Close();
	void InitStyle();

};