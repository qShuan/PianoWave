#include "vec2.hpp"

namespace utils {

	namespace vec2 {

		sf::Vector2f RotatePointAboutOrigin(const sf::Vector2f& origin, const sf::Vector2f& p, float angleInRads) {

			float s = sin(angleInRads);
			float c = cos(angleInRads);

			sf::Vector2f rotatedPoint;

			rotatedPoint.x = p.x - origin.x;
			rotatedPoint.y = p.y - origin.y;

			float newX = c * rotatedPoint.x - s * rotatedPoint.y;
			float newY = s * rotatedPoint.x + c * rotatedPoint.y;

			rotatedPoint.x = newX + origin.x;
			rotatedPoint.y = newY + origin.y;

			return rotatedPoint;
		}
	}
}