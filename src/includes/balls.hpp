
#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cmath>
#include "./constants.hpp"

struct sBall
{
	sf::CircleShape shape;
	sf::Vector2f vel, accel;
	float mass;
	size_t id;
};

class Balls 
{
private:
	std::vector<sBall> vecBalls;
	sf::Time time;
	std::vector<std::pair<sBall*, sBall*>> vecCollidingBalls;
public:
	Balls();
	~Balls();

	void AddBall(sf::Vector2f pos, float rad, sf::Vector2f vel, sf::Vector2f accel);
	void BoundaryCollision(sBall* ball);
	void DynamicCollision(sBall* currentB, sBall* targetB);
	void StaticCollision(sBall* currentB, sBall* targetB);

	void AddBallTowardsMouse(sf::Vector2f mousePos, float rad);
	void Update(sf::Clock &clock);
	void Draw(sf::RenderWindow &window);
};