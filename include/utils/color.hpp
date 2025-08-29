#pragma once

#include <SFML/Graphics/Color.hpp>
#include <utils/rng.hpp>
#include <utils/hsl.hpp>

namespace utils {

	namespace color {

		sf::Color Randomize(sf::Color color, float variationStrength = 10.f);

		inline sf::Color Darken(sf::Color color, float amount, float minLuminance) {

			HSL hsl = TurnToHSL(color);
			hsl.Luminance = std::max(minLuminance, static_cast<float>(hsl.Luminance) - amount);
			color = hsl.TurnToRGB();

			return color;
		}

		inline float GetLuminance(const sf::Color& color) {
			HSL hsl = TurnToHSL(color);

			return static_cast<float>(hsl.Luminance);
		}
	}
}