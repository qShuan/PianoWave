#pragma once

#include <cmath>

#include <SFML/System/Vector2.hpp>
#include <utils/rng.hpp>

#define PI 3.14159265358979323846

#define RADIANS(x) ((x * PI) / 180)
#define DEGREES(x) ((x * 180) / PI)

namespace utils {

	namespace vec2 {

		inline float GetMagnitude(const sf::Vector2f& v) {

			return sqrt((v.y * v.y) + (v.x * v.x));
		}

		inline sf::Vector2f Normalize(sf::Vector2f& v) {

			float mag = GetMagnitude(v);

			if (mag == 0)
				return sf::Vector2f(0.f, 0.f);

			v /= mag;

			return v;
		}

		inline float ZCrossProduct(const sf::Vector2f& a, const sf::Vector2f& b) {

			return ((a.x * b.y) - (a.y * b.x));
		}

		inline float GetDirectionAngle(const sf::Vector2f& v) {

			float angleRad = atan2(v.y, v.x);
			float degrees = static_cast<float>(DEGREES(angleRad)) + 90.f;

			int result = (360 + static_cast<int>(roundf(degrees))) % 360;

			return (float)result;
		}

		inline sf::Vector2f GetNormal(const sf::Vector2f& v) {

			return sf::Vector2f(-v.y, v.x);
		}

		sf::Vector2f RotatePointAboutOrigin(const sf::Vector2f& origin, const sf::Vector2f& p, float angleInRads);

		inline sf::Vector2f& Randomize(sf::Vector2f& v, float randomMagnitude) {

			sf::Vector2f randomDirection = { utils::rng::RandomBetween(-1.f, 1.f), utils::rng::RandomBetween(-1.f, 1.f) };
			randomDirection *= randomMagnitude;

			v += randomDirection;

			return v;
		}

		inline float CrossProduct(const sf::Vector2f& a, const sf::Vector2f& b) {

			return ((a.x * b.y) - (a.y * b.x));
		}
	}
}