#include "Piano.h"
#include "GUI.h"
#include "NFDHandler.h"

struct WindowSettings {

	const char* name = "Piano Wave";
	unsigned int width = 1080;
	unsigned int height = 420;
};


class Application {

private:

	Piano m_piano;
	std::array<PianoKey, g_number_of_keys>& m_piano_keys;

	WindowSettings m_window_settings;
	sf::RenderWindow m_window;

	NFDHandler m_nfd_handler;

	GUI m_gui;

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