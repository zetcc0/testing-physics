#include "./includes/balls.hpp"

Balls::Balls()
{
}

Balls::~Balls()
{
}

void Balls::AddBall(sf::Vector2f pos, float rad, sf::Vector2f vel, sf::Vector2f accel)
{
	sBall b;
	b.shape.setPosition(pos);
	b.shape.setRadius(rad);
	b.shape.setFillColor(sf::Color::Black);
	b.vel = vel;
	b.accel = accel;
	b.id = vecBalls.size();
	b.mass = M_PIf*b.shape.getRadius()*b.shape.getRadius();
	vecBalls.emplace_back(b);
}

void Balls::BoundaryCollision(sBall* ball)
{
			float left = ball->shape.getPosition().x;
			float right = left+ball->shape.getRadius()*2;
			float top = ball->shape.getPosition().y;
			float bottom = top+ball->shape.getRadius()*2;
			bool is_colliding_top = top < 0;
			bool is_colliding_bottom = bottom > WINDOW_HEIGHT;
			bool is_colliding_left = left < 0;
			bool is_colliding_right = right > WINDOW_WIDTH;

			if (is_colliding_top)
			{
				ball->vel.y = -ball->vel.y*FRICTION;
				ball->shape.move(0, -top);
				//std::cout << "Soundtop" << std::endl;
			}
			if (is_colliding_bottom)
			{
				ball->vel.y = -ball->vel.y*FRICTION;
				ball->shape.move(0, WINDOW_HEIGHT - bottom);

				if (fabs(ball->vel.y) < 10)
				{
					ball->vel.y = 0;
				}
			}
			if (is_colliding_left)
			{
				ball->vel.x = -ball->vel.x*FRICTION;
				ball->shape.move(-left, 0);
				if (fabs(ball->vel.x) < 5)
				{
					ball->vel.x = 0;
				}
			}
			if (is_colliding_right)
			{
				ball->vel.x = -ball->vel.x*FRICTION;
				ball->shape.move(WINDOW_WIDTH-right, 0);
				if (fabs(ball->vel.x) < 5)
				{
					ball->vel.x = 0;
				}
			}		
}

void Balls::StaticCollision(sBall* currentB, sBall* targetB)
{
				
	float distance_y = currentB->shape.getPosition().y + currentB->shape.getRadius() - targetB->shape.getPosition().y - targetB->shape.getRadius();
	float distance_x = currentB->shape.getPosition().x + currentB->shape.getRadius() - targetB->shape.getPosition().x - targetB->shape.getRadius();
	float distance = sqrtf(distance_y*distance_y + distance_x*distance_x);
	float no_overlaping_distance = currentB->shape.getRadius() + targetB->shape.getRadius();

	if(distance <= no_overlaping_distance)
	{
		vecCollidingBalls.push_back({ currentB, targetB });
		
		float inverse_distance = 1.0f/distance;
		sf::Vector2f vector_normalized(distance_x*inverse_distance, distance_y*inverse_distance);
		float halfoverlap = (distance-no_overlaping_distance)*0.5f;

		currentB->shape.move(-vector_normalized.x*halfoverlap, -vector_normalized.y*halfoverlap);
		targetB->shape.move(vector_normalized.x*halfoverlap, vector_normalized.y*halfoverlap);
	}
}

void Balls::DynamicCollision(sBall* currentB, sBall* targetB)
{				
	float distance_y = currentB->shape.getPosition().y + currentB->shape.getRadius() - targetB->shape.getPosition().y - targetB->shape.getRadius();
	float distance_x = currentB->shape.getPosition().x + currentB->shape.getRadius() - targetB->shape.getPosition().x - targetB->shape.getRadius();
	float distance = sqrtf(distance_y*distance_y + distance_x*distance_x);

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
				b.vel += (b.accel + sf::Vector2f(0, GRAVITY))*TIME_STEP/SUBSTEPS;
				b.shape.move(b.vel*TIME_STEP/SUBSTEPS);
			}
			
			// does it collide with borders?
			for (auto &b : vecBalls)
			{
				BoundaryCollision(&b);
			}
			
			// does it collide with any other ball?
			for (auto &currentB : vecBalls)
			{
				for (auto &targetB : vecBalls)
				{
					if(currentB.id >= targetB.id) continue; // I don't want id to check collide with itself neither with a ball I already check collision
					
					StaticCollision(&currentB, &targetB);
				}
			}

			// update dynamic collision
			for (auto collidedBalls : vecCollidingBalls)
			{
				Balls::DynamicCollision(collidedBalls.first, collidedBalls.second);
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
    	window.draw(b.shape);
	}
}
