#include "Piano.h"
#include "GUI.h"
#include "NFDHandler.h"

struct WindowSettings {

	const char* name = "Piano Wave";
	const unsigned int width = 1080;
	const unsigned int height = 400;
};

const WindowSettings g_window_settings;

class Application {

private:

	sf::RenderWindow m_window;

	GUI m_gui;

	Piano m_piano;

	std::array<PianoKey, g_number_of_keys>& m_piano_keys;

	NFDHandler m_nfd_handler;

	bool m_is_mouse_left_pressed;

private:

	void HandleEvents(const std::optional<sf::Event>& event);
	void HandlePianoKeyStrikes();
	void HandleGUI();

public:

	Application();
	~Application() = default;

	void Run();
};