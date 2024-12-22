#include "./includes/circles.hpp"



int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");

	// Creating the circles
	Circles circles;
	for (int id=0; id<5; id++)
	{
		circles.AddBall(sf::Vector2f(180*id,200), 20, sf::Vector2f(-2*id+2,-2-id),sf::Vector2f(0,0), id);
	}

	// Steps
	sf::Clock clock;
	sf::Time time;
	
	// Sound stuff
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
		circles.Update(clock,sound,10,0.01);
		clock.restart();
		window.clear();
		circles.Draw(window);
        window.display();
		
    }
 
    return EXIT_SUCCESS;
}