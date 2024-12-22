#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cmath>


class Circles 
{
private:
	std::vector<sf::CircleShape> shapes;
	std::vector<sf::Vector2f> vels;
	std::vector<sf::Vector2f> accels;
	sf::Time time;
	
public:
	Circles();
	~Circles();

	void AddBall(sf::Vector2f pos, int rad, sf::Vector2f vel, sf::Vector2f accel, id_t id);
	void Update(sf::Clock &clock, sf::Sound &sound, float gravity, float step);
	void Draw(sf::RenderWindow &window);
};