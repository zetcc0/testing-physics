#include "./includes/circles.hpp"



Circles::Circles()
{
}
Circles::~Circles()
{
}

void Circles::AddBall(sf::Vector2f pos, int rad, sf::Vector2f vel, sf::Vector2f accel, id_t id)
{
	shapes.emplace_back(sf::CircleShape());
	shapes[id].setPosition(pos);
	shapes[id].setRadius(rad);
	shapes[id].setFillColor(sf::Color::White);
	vels.emplace_back(vel);
	accels.emplace_back(accel);
}

void Circles::Update(sf::Clock &clock, sf::Sound &sound, float gravity, float step)
{
	time += clock.getElapsedTime();

	if (time.asMilliseconds()>step*1000)
	{
		//sf::Vector2f(vel.x*step,vel.y*step)
		float vel_after_collide = 0.85;
		int end = shapes.size();
		for (int id = 0; id < end; id++)
		{

			//std::cout << vel.y << "  " << vel.x << "  " << time.asMilliseconds() << "  " << gravity*step << "  " << fabs(vel.y) << "\n";
			// Calculate bouncing
			for (int collide_id = 0; collide_id < end ; collide_id++ )
			{
				if(collide_id==id) continue;


				float vectorY = shapes[collide_id].getPosition().y-shapes[id].getPosition().y;
				float vectorX = shapes[collide_id].getPosition().x-shapes[id].getPosition().x;
				float distance = pow(pow(vectorY, 2) + pow(vectorX, 2), 0.5);
				float no_overlaping_distance = shapes[collide_id].getRadius() + shapes[id].getRadius();
				if(distance < no_overlaping_distance)
				{

					float inverse_distance = 1.0f/distance;
					sf::Vector2f vector_normalized = sf::Vector2f(vectorX*inverse_distance, vectorY*inverse_distance);
					float overlap = (distance-no_overlaping_distance)/2;

					shapes[id].setPosition(sf::Vector2f(shapes[id].getPosition().x+vector_normalized.x*overlap,shapes[id].getPosition().y+vector_normalized.y*overlap));
					shapes[collide_id].setPosition(sf::Vector2f(shapes[collide_id].getPosition().x-vector_normalized.x*overlap,shapes[collide_id].getPosition().y-vector_normalized.y*overlap));


					vels[id] = sf::Vector2f(vels[id].x + vector_normalized.x, vels[id].y + vector_normalized.y);
					vels[collide_id] = sf::Vector2f(-vels[collide_id].x*vector_normalized.x, -vels[collide_id].y*vector_normalized.y);



				}
			}
			if (shapes[id].getGlobalBounds().top < 0)
			{
				vels[id].y = fabs(vels[id].y)*vel_after_collide;
				sound.play();
			}
			if ((shapes[id].getGlobalBounds().top+shapes[id].getGlobalBounds().height) > 600)
			{
				vels[id].y = -fabs(vels[id].y)*vel_after_collide;
				//std::cout << vel.y << " " << vel.y*vel.y << std::endl;
				if (vels[id].y*vels[id].y < 0.5f)
				{
					vels[id].y = 0;
				}
				if (vels[id].y > 0 || vels[id].y < 0)
				{
					sound.play();
				}
			}
			if (shapes[id].getGlobalBounds().left < 0)
			{
				vels[id].x = fabs(vels[id].x)*vel_after_collide;
				if (vels[id].x > 0 || vels[id].x < 0)
				{
					sound.play();
				}
			}
			if ((shapes[id].getGlobalBounds().left+shapes[id].getGlobalBounds().width) > 800)
			{
				vels[id].x = -fabs(vels[id].x)*vel_after_collide;
				if (vels[id].x > 0 || vels[id].x < 0)
				{
					sound.play();
				}
			}		
			shapes[id].setPosition(shapes[id].getPosition()+vels[id]);
			accels[id] = sf::Vector2f(0, gravity);
			vels[id] += accels[id]*step;
		}
		time = time.Zero;
	}

}

void Circles::Draw(sf::RenderWindow &window)
{
	int end = shapes.size();
	for (int id = 0; id < end; id++)
	{
    	window.draw(shapes[id]);
	}
}
