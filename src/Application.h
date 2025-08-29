#include "Piano.h"

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