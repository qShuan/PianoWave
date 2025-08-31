#include "Piano.h"

struct WindowSettings {

	const char* name = "Piano Wave";
	const unsigned int width = 1080;
	const unsigned int height = 400;
};

const WindowSettings g_window_settings;

class Application {

private:

	sf::RenderWindow m_window;

	Piano m_piano;

public:

	Application();
	~Application() = default;

	void HandleEvents(sf::Event& event);
	void Run();
};