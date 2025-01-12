#include "./includes/balls.hpp"

Balls::Balls()
{
}

Balls::~Balls()
{
}

void Balls::AddBall(sf::Vector2f pos, float rad, sf::Vector2f vel, sf::Vector2f accel)
{
	size_t id = vecBalls.size();
	Ball b(pos, rad, vel, accel, id);
	vecBalls.emplace_back(b);
}

void Balls::AddBallTowardsMouse(sf::Vector2f mousePos, float rad)
{
	sf::Vector2f pos(WINDOW_WIDTH/2, WINDOW_HEIGHT/2);
	sf::Vector2f vel = mousePos - pos;
	float distance = sqrtf(vel.x*vel.x + vel.y*vel.y);
	vel /= distance;
	AddBall(pos, rad, vel*120.0f, sf::Vector2f(0,0));
}

void Balls::Update(sf::Clock &clock)
{
	for (float steps=0; steps<SUBSTEPS;steps++)
	{
		time += clock.getElapsedTime();
		float current_time = static_cast<float>(time.asMilliseconds())/1000.0f;
		if (current_time>TIME_STEP/SUBSTEPS)
		{
			// update position
			for (auto &b : vecBalls)
			{
				b.UpdatePosition();
			}
			
			// does it collide with borders?
			for (auto &b : vecBalls)
			{
				b.BoundaryCollision();
			}
			
			// does it collide with any other ball?
			for (auto &currentB : vecBalls)
			{
				for (auto &targetB : vecBalls)
				{
					if(currentB.getId() >= targetB.getId()) continue; // I don't want id to check collide with itself neither with a ball I already check collision
					
					currentB.StaticCollision(&targetB, &vecCollidingBalls);
					
				}
			}

			// update dynamic collision
			for (auto collidedBalls : vecCollidingBalls)
			{
				collidedBalls.first->DynamicCollision(collidedBalls.second);
			}
			vecCollidingBalls.clear();
			time = time.Zero;
		}
	}
}

void Balls::Draw(sf::RenderWindow &window)
{
	for (auto &b : vecBalls)
	{
    	b.Draw(window);
	}
}
