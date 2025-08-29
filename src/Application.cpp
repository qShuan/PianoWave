#include "Application.h"

Application::Application()
	: m_window(sf::RenderWindow(sf::VideoMode(800, 600), "Piano Wave", sf::Style::Titlebar | sf::Style::Close)) {

	m_window.setKeyRepeatEnabled(false);
}

void Application::HandleEvents(sf::Event& event) {

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

	while (m_window.isOpen()) {

		sf::Event event;

		while (m_window.pollEvent(event)) {

			HandleEvents(event);
		}

		m_window.clear();

		m_window.display();
	}
}