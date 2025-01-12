
#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cmath>
#include "./constants.hpp"


class Ball
{
private:
	sf::CircleShape shape;
	sf::Vector2f vel, accel;
	float mass;
	size_t id;

public:
	Ball(sf::Vector2f pos, float rad, sf::Vector2f vel, sf::Vector2f accel, size_t id);
	
	void UpdatePosition();
	void BoundaryCollision();
	void StaticCollision(Ball* targetB, std::vector<std::pair<Ball*, Ball*>>* vecCollidingBalls);
	void DynamicCollision(Ball* targetB);

	inline void Draw(sf::RenderWindow &window){window.draw(shape);}

	inline size_t getId(){return id;}

};