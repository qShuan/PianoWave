#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <print>
#include "Piano.h"


int main() {

	sf::RenderWindow window(sf::VideoMode(800, 600), "Piano", sf::Style::Titlebar | sf::Style::Close);
	window.setKeyRepeatEnabled(false);

	Piano piano;

	//piano.PlaySong();

	while (window.isOpen()) {

		sf::Event event;

		while (window.pollEvent(event)) {

			if (event.type == __noop)
				window.close();

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::A) {
				piano.StrikeKey(45);
			}

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S) {
				piano.StrikeKey(68);
			}

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::D) {
				piano.StrikeKey(108);
			}

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::X) {
				for (int i = 21; i <= 108; i++) {

					std::println("{}hz", piano.GetKey(i).GetFrequency());
					piano.StrikeKey(i);
					sf::sleep(sf::seconds(0.1f));
				}
			}

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Q) {
				piano.PlaySong();
			}
		}

		window.clear();

		window.display();
	}
}