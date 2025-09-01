#include "Application.h"

Application::Application()
	: m_window(sf::RenderWindow(sf::VideoMode(g_window_settings.width, g_window_settings.height), g_window_settings.name, sf::Style::Titlebar | sf::Style::Close)),
	m_gui(GUI(m_window)){

	m_window.setKeyRepeatEnabled(false);
	m_piano.SetKeyPositions((float)g_window_settings.width, (float)g_window_settings.height);
}

void Application::HandleEvents(sf::Event& event) {

	m_gui.ProcessEvent(m_window, event);

	if (event.type == __noop) {
		m_window.close();
	}

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::A) {
		m_piano.StrikeKey(45);
	}

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S) {
		m_piano.StrikeKey(68);
	}

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::D) {
		m_piano.StrikeKey(108);
	}

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::X) {
		for (int i = 21; i <= 108; i++) {

			LOG("{}hz", m_piano.GetKey(i).GetFrequency());
			m_piano.StrikeKey(i);
			sf::sleep(sf::seconds(0.1f));
		}
	}

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Q) {
		m_piano.PlaySong();
	}
}

void Application::Run() {

	sf::Clock clock;

	while (m_window.isOpen()) {

		sf::Event event;

		while (m_window.pollEvent(event)) {

			HandleEvents(event);
		}

		sf::Time sec = clock.restart();
		m_gui.Update(m_window, sec);

		m_window.clear();

		m_piano.DrawKeys(m_window);

		HandleGUI();
		m_gui.Render(m_window);

		m_window.display();
	}

	m_gui.Close();
}

void Application::HandleGUI() {

	ImGui::Begin("File Handling", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	ImGui::SetWindowSize(ImVec2((float)g_window_settings.width / 3.f, (float)g_window_settings.height - 150.f));
	ImGui::SetWindowPos(ImVec2(0.f, 0.f));

	ImGui::End();
}