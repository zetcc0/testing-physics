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

	if (time.asMilliseconds()>step)
	{
		float vel_after_collide = 0.85;

		// update position
		for (auto &b : vecBalls)
		{
			b.vel += (b.accel + sf::Vector2f(0, gravity))*step;
			b.shape.move(b.vel*step);
		}
		
		// does it collide with borders?
		for (auto &b : vecBalls)
		{
			float left = b.shape.getPosition().x;
			float right = left+b.shape.getRadius()*2;
			float top = b.shape.getPosition().y;
			float bottom = top+b.shape.getRadius()*2;
			bool is_colliding_top = top < 0;
			bool is_colliding_bottom = bottom > 600;
			bool is_colliding_left = left < 0;
			bool is_colliding_right = right > 800;

			if (is_colliding_top)
			{
				b.vel.y = -b.vel.y*vel_after_collide;
				b.shape.move(0, -top);
				sound.play();
				//std::cout << "Soundtop" << std::endl;
			}
			if (is_colliding_bottom)
			{
				b.vel.y = -b.vel.y*vel_after_collide;
				b.shape.move(0, 600 - bottom);

				if (abs(b.vel.y) < 10)
				{
					b.vel.y = 0;
				}
				if (b.vel.y != 0)
				{
					sound.play();
					//std::cout << "Soundbottom" << " " << b.vel.y << " " << b.vel.y*b.vel.y << std::endl;
				}
			}
			if (is_colliding_left)
			{
				b.vel.x = -b.vel.x*vel_after_collide;
				b.shape.move(-left, 0);
				if (abs(b.vel.x) < 5)
				{
					b.vel.x = 0;
				}
				if (b.vel.x != 0)
				{
					sound.play();
					//std::cout << "Soundleft" << std::endl;
				}
			}
			if (is_colliding_right)
			{
				b.vel.x = -b.vel.x*vel_after_collide;
				b.shape.move(800-right, 0);
				if (abs(b.vel.x) < 5)
				{
					b.vel.x = 0;
				}
				if (b.vel.x != 0)
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
				
				float distance_y = currentB.shape.getPosition().y - targetB.shape.getPosition().y;
				float distance_x = currentB.shape.getPosition().x - targetB.shape.getPosition().x;
				float distance = sqrt(distance_y*distance_y + distance_x*distance_x);
				float no_overlaping_distance = targetB.shape.getRadius() + currentB.shape.getRadius();

				if(distance <= no_overlaping_distance)
				{
					vecCollidingBalls.push_back({ &currentB, &targetB });

					float inverse_distance = 1.0f/distance;
					sf::Vector2f vector_normalized(distance_x*inverse_distance, distance_y*inverse_distance);
					float halfoverlap = (distance-no_overlaping_distance)*0.5f;

					currentB.shape.move(-vector_normalized.x*halfoverlap, -vector_normalized.y*halfoverlap);
					targetB.shape.move(vector_normalized.x*halfoverlap, vector_normalized.y*halfoverlap);
				}
			}
		}

		// update dynamic collition
		for (auto collidedBalls : vecCollidingBalls)
		{
			sBall *currentB = collidedBalls.first;
			sBall *targetB = collidedBalls.second;
			
			float distance_y = currentB->shape.getPosition().y - targetB->shape.getPosition().y;
			float distance_x = currentB->shape.getPosition().x - targetB->shape.getPosition().x;
			float distance = sqrt(distance_y*distance_y + distance_x*distance_x);

			float inverse_distance = 1.0f/distance;
			sf::Vector2f vectorNorm(distance_x*inverse_distance, distance_y*inverse_distance);
			sf::Vector2f vectorTan(-vectorNorm.y, vectorNorm.x);

			float dpTanCurrent = vectorTan.x*currentB->vel.x + vectorTan.y*currentB->vel.y;
			float dpTanTarget = vectorTan.x*targetB->vel.x + vectorTan.y*targetB->vel.y;

			float dpNormCurrent = vectorNorm.x*currentB->vel.x + vectorNorm.y*currentB->vel.y;
			float dpNormTarget = vectorNorm.x*targetB->vel.x + vectorNorm.y*targetB->vel.y;

			float mCurrent = (dpNormCurrent * (currentB->mass - targetB->mass) + 2.0f * targetB->mass * dpNormTarget) / (currentB->mass + targetB->mass); 
			float mTarget = (dpNormTarget * (targetB->mass - currentB->mass) + 2.0f * currentB->mass * dpNormCurrent) / (currentB->mass + targetB->mass);

			currentB->vel = (vectorTan*dpTanCurrent + vectorNorm*mCurrent);
			targetB->vel = (vectorTan*dpTanTarget + vectorNorm*mTarget);
			
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
