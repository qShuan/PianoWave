#include "Piano.h"
#include "ImGuiHandler.h"
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

	ImGuiHandler m_imgui_handler;

	std::string m_file_name;

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