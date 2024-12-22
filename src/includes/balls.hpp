#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cmath>

struct sBall
{
	sf::CircleShape shape;
	sf::Vector2f vel, accel;
	float mass = 70;
	id_t id;
};

class Balls 
{
private:
	std::vector<sBall> vecBalls;
	sf::Time time;
	
public:
	Balls();
	~Balls();

	void AddBall(sf::Vector2f pos, int rad, sf::Vector2f vel, sf::Vector2f accel);
	void Update(sf::Clock &clock, sf::Sound &sound, float gravity, float step);
	void Draw(sf::RenderWindow &window);
};