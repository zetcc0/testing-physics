#include "./includes/balls.hpp"



int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML window");

	// Creating the Balls
	Balls balls;
	std::srand(12345251);
	for (float id=0; id<100.0f; id++)
	{	
		float vely = static_cast<float>(std::rand()%10)-5.0f;
		float velx = static_cast<float>(std::rand()%10)-5.0f;
		float offset = static_cast<float>(std::rand()%200)-100.0f;
		balls.AddBall(sf::Vector2f(id+25.0f,300.0f + offset), 5, sf::Vector2f(velx, vely),sf::Vector2f(0,0));
	}

	// Steps
	sf::Clock clock;
	
	// Sound stuff (The sound.wav is me hitting the wall lol)
	/*sf::SoundBuffer buffer;
	buffer.loadFromFile("sound.wav");
	sf::Sound sound;
	sound.setBuffer(buffer);*/

    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::KeyPressed || event.type == sf::Event::Closed)
                window.close();
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					balls.AddBallTowardsMouse(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)), 20.0f);
				}
				if (event.mouseButton.button == sf::Mouse::Right)
				{
					balls.AddBallTowardsMouse(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)), 120.0f);
				}
			}	
        }
		balls.Update(clock);
		clock.restart();
		window.clear(sf::Color::White);
		balls.Draw(window);
        window.display();
		
    }
 
    return EXIT_SUCCESS;
}