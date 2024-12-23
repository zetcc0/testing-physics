#include "./includes/balls.hpp"



int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");

	// Creating the Balls
	Balls balls;
	std::srand(1325251);
	for (int id=0; id<5; id++)
	{	
		float vely = std::rand()%66-20;
		float velx = std::rand()%96-20;
		balls.AddBall(sf::Vector2f(180*id,200), 20, sf::Vector2f(velx, vely),sf::Vector2f(0,0));
	}

	// Steps
	sf::Clock clock;
	sf::Time time;
	
	// Sound stuff (The sound.wav is me hitting the wall lol)
	sf::SoundBuffer buffer;
	buffer.loadFromFile("sound.wav");
	sf::Sound sound;
	sound.setBuffer(buffer);

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
				
        }
		balls.Update(clock,sound,10,0.016);
		clock.restart();
		window.clear();
		balls.Draw(window);
        window.display();
		
    }
 
    return EXIT_SUCCESS;
}