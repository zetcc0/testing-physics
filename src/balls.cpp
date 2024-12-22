#include "./includes/balls.hpp"

Balls::Balls()
{
}

Balls::~Balls()
{
}

void Balls::AddBall(sf::Vector2f pos, int rad, sf::Vector2f vel, sf::Vector2f accel)
{
	sBall b;
	b.shape.setPosition(pos);
	b.shape.setRadius(rad);
	b.shape.setFillColor(sf::Color::White);
	b.vel = vel;
	b.accel = accel;
	b.id = vecBalls.size();
	vecBalls.emplace_back(b);
}

void Balls::Update(sf::Clock &clock, sf::Sound &sound, float gravity, float step)
{
	time += clock.getElapsedTime();

	if (time.asMilliseconds()>step*1000)
	{
		float vel_after_collide = 0.85;
		for (auto &currentB : vecBalls)
		{
			currentB.vel += (currentB.accel + sf::Vector2f(0, gravity))*step;
			currentB.shape.move(currentB.vel*step);
		}
		
		// does it collide with borders?
		for (auto &currentB : vecBalls)
		{
			float left = currentB.shape.getPosition().x;
			float right = left+currentB.shape.getRadius()*2;
			float top = currentB.shape.getPosition().y;
			float bottom = top+currentB.shape.getRadius()*2;
			if (top < 0)
			{
				currentB.vel.y = fabs(currentB.vel.y)*vel_after_collide;
				currentB.shape.move(0, -top);
				sound.play();
				//std::cout << "Soundtop" << std::endl;
			}
			if (bottom > 600)
			{
				currentB.vel.y = -fabs(currentB.vel.y)*vel_after_collide;
				currentB.shape.move(0, 600 - bottom);

				if (abs(currentB.vel.y) < 10)
				{
					currentB.vel.y = 0;
				}
				if (currentB.vel.y != 0)
				{
					sound.play();
					//std::cout << "Soundbottom" << " " << currentB.vel.y << " " << currentB.vel.y*currentB.vel.y << std::endl;
				}
			}
			if (left < 0)
			{
				currentB.vel.x = fabs(currentB.vel.x)*vel_after_collide;
				currentB.shape.move(-left, 0);
				if (abs(currentB.vel.x) < 10)
				{
					currentB.vel.x = 0;
				}
				if (currentB.vel.x != 0)
				{
					sound.play();
					//std::cout << "Soundleft" << std::endl;
				}
			}
			if (right > 800)
			{
				currentB.vel.x = -fabs(currentB.vel.x)*vel_after_collide;
				currentB.shape.move(800-right, 0);
				if (abs(currentB.vel.x) < 10)
				{
					currentB.vel.x = 0;
				}
				if (currentB.vel.x != 0)
				{
					sound.play();
					//std::cout << "Soundleft" <<  std::endl;
				}
			}		
		}

		std::vector<std::pair<sBall*, sBall*>> vecCollidingBalls;
		
		// does it collide with any other ball?
		for (auto &currentB : vecBalls)
		{
			for (auto &targetB : vecBalls)
			{
				if(currentB.id >= targetB.id) continue; // I don't want id to check collide with itself neither with a ball I already check collition
				
				float vector_y = currentB.shape.getPosition().y - targetB.shape.getPosition().y;
				float vector_x = currentB.shape.getPosition().x - targetB.shape.getPosition().x;
				float distance = sqrt(pow(vector_y, 2) + pow(vector_x, 2));
				float no_overlaping_distance = targetB.shape.getRadius() + currentB.shape.getRadius();

				if(distance < no_overlaping_distance)
				{
					vecCollidingBalls.push_back({ &currentB, &targetB });

					float inverse_distance = 1.0f/distance;
					sf::Vector2f vector_normalized = sf::Vector2f(vector_x*inverse_distance, vector_y*inverse_distance);
					float overlap = (distance-no_overlaping_distance)*0.5f;

					currentB.shape.move(sf::Vector2f(-vector_normalized.x*overlap-1, -vector_normalized.y*overlap-1));
					targetB.shape.move(sf::Vector2f(vector_normalized.x*overlap+1, vector_normalized.y*overlap+1));
				}
			}
		}

		// update dynamic collition
		for (auto collidedBalls : vecCollidingBalls)
		{
			sBall *currentB = collidedBalls.first;
			sBall *targetB = collidedBalls.second;

			if (vecCollidingBalls.size() != 0)
			{
				std::cout << "START current (" << currentB->id << "): " << currentB->vel.x << " " << currentB->vel.y << "    target (" << targetB->id << "): " << targetB->vel.x << " " << targetB->vel.y << std::endl;
			}	
			
			float vector_y = currentB->shape.getPosition().y - targetB->shape.getPosition().y;
			float vector_x = currentB->shape.getPosition().x - targetB->shape.getPosition().x;
			float distance = sqrt(vector_y*vector_y + vector_x*vector_x);

			float inverse_distance = 1.0f/distance;
			sf::Vector2f vectorNorm(vector_x*inverse_distance, vector_y*inverse_distance);
			sf::Vector2f vectorTan(vectorNorm.x, -vectorNorm.y);

			float dpTanCurrent = vectorTan.x*currentB->vel.x + vectorTan.y*currentB->vel.y;
			float dpTanTarget = vectorTan.x*targetB->vel.x + vectorTan.y*targetB->vel.y;

			float dpNormCurrent = vectorNorm.x*currentB->vel.x + vectorNorm.y*currentB->vel.y;
			float dpNormTarget = vectorNorm.x*targetB->vel.x + vectorNorm.y*targetB->vel.y;

			float mCurrent = (dpNormCurrent * (currentB->mass - targetB->mass) + 2.0f * targetB->mass * dpNormTarget) / (currentB->mass + targetB->mass); 
			float mTarget = (dpNormTarget * (targetB->mass - currentB->mass) + 2.0f * currentB->mass * dpNormCurrent) / (currentB->mass + targetB->mass);

			currentB->vel = (vectorTan*dpTanCurrent + vectorNorm*mCurrent)*vel_after_collide;
			targetB->vel = (vectorTan*dpTanTarget + vectorNorm*mTarget)*vel_after_collide;
			
			if (vecCollidingBalls.size() != 0)
			{
				std::cout << "END current (" << currentB->id << "): " << currentB->vel.x << " " << currentB->vel.y << "    target (" << targetB->id << "): " << targetB->vel.x << " " << targetB->vel.y << std::endl;
			}	
		}
		if (vecCollidingBalls.size() != 0)
		{
			std::cout << "lesto" << std::endl;
		}


		time = time.Zero;
	}
}

void Balls::Draw(sf::RenderWindow &window)
{
	for (auto &b : vecBalls)
	{
    	window.draw(b.shape);
	}
}
