
#pragma once

#include "./ball.hpp"

class Balls 
{
private:
	std::vector<Ball> vecBalls;
	sf::Time time;
	std::vector<std::pair<Ball*, Ball*>> vecCollidingBalls;
public:
	Balls();
	~Balls();

	void AddBall(sf::Vector2f pos, float rad, sf::Vector2f vel, sf::Vector2f accel);

	void AddBallTowardsMouse(sf::Vector2f mousePos, float rad);
	void Update(sf::Clock &clock);
	void Draw(sf::RenderWindow &window);
};