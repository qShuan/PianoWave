#include "color.hpp"

namespace utils {

	namespace color {

		sf::Color Randomize(sf::Color color, float variationStrength) {

			HSL hsl = TurnToHSL(color);

			hsl.Hue = floor(hsl.Hue);

			hsl.Luminance = std::max(0.0f, static_cast<float>(hsl.Luminance) + (utils::rng::FastRandom() * -variationStrength));
			hsl.Saturation = std::max(0.0f, static_cast<float>(hsl.Saturation) + (utils::rng::FastRandom() * -variationStrength));

			color = hsl.TurnToRGB();

			return color;
		}
	}
}