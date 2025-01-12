#include "includes/ball.hpp"

Ball::Ball(sf::Vector2f pos, float rad, sf::Vector2f vel, sf::Vector2f accel, size_t id) :
	vel(vel), accel(accel), id(id)
{
	shape.setPosition(pos);
	shape.setRadius(rad);
	shape.setFillColor(sf::Color::Black);
	mass = M_PIf*shape.getRadius()*shape.getRadius();
}

void Ball::UpdatePosition()
{
	vel += (accel + sf::Vector2f(0, GRAVITY))*TIME_STEP/SUBSTEPS;
	shape.move(vel*TIME_STEP/SUBSTEPS);
}

void Ball::BoundaryCollision()
{
	float left = shape.getPosition().x;
	float right = left+shape.getRadius()*2;
	float top = shape.getPosition().y;
	float bottom = top+shape.getRadius()*2;
	bool is_colliding_top = top < 0;
	bool is_colliding_bottom = bottom > WINDOW_HEIGHT;
	bool is_colliding_left = left < 0;
	bool is_colliding_right = right > WINDOW_WIDTH;

	if (is_colliding_top)
	{
		vel.y = -vel.y*FRICTION;
		shape.move(0, -top);
		//std::cout << "Soundtop" << std::endl;
	}
	if (is_colliding_bottom)
	{
		vel.y = -vel.y*FRICTION;
		shape.move(0, WINDOW_HEIGHT - bottom);

		if (fabs(vel.y) < 10)
		{
			vel.y = 0;
		}
	}
	if (is_colliding_left)
	{
		vel.x = -vel.x*FRICTION;
		shape.move(-left, 0);
		if (fabs(vel.x) < 5)
		{
			vel.x = 0;
		}
	}
	if (is_colliding_right)
	{
		vel.x = -vel.x*FRICTION;
		shape.move(WINDOW_WIDTH-right, 0);
		if (fabs(vel.x) < 5)
		{
			vel.x = 0;
		}
	}		
}

void Ball::StaticCollision(Ball* targetB, std::vector<std::pair<Ball*, Ball*>>* vecCollidingBalls)
{
				
	float distance_y = shape.getPosition().y + shape.getRadius() - targetB->shape.getPosition().y - targetB->shape.getRadius();
	float distance_x = shape.getPosition().x + shape.getRadius() - targetB->shape.getPosition().x - targetB->shape.getRadius();
	float distance = sqrtf(distance_y*distance_y + distance_x*distance_x);
	float no_overlaping_distance = shape.getRadius() + targetB->shape.getRadius();

	if(distance <= no_overlaping_distance)
	{
		vecCollidingBalls->push_back({ this, targetB });
		float inverse_distance = 1.0f/distance;
		sf::Vector2f vector_normalized(distance_x*inverse_distance, distance_y*inverse_distance);
		float halfoverlap = (distance-no_overlaping_distance)*0.5f;

		shape.move(-vector_normalized.x*halfoverlap, -vector_normalized.y*halfoverlap);
		targetB->shape.move(vector_normalized.x*halfoverlap, vector_normalized.y*halfoverlap);
	}
}

void Ball::DynamicCollision(Ball* targetB)
{				

	float distance_y = shape.getPosition().y + shape.getRadius() - targetB->shape.getPosition().y - targetB->shape.getRadius();
	float distance_x = shape.getPosition().x + shape.getRadius() - targetB->shape.getPosition().x - targetB->shape.getRadius();
	float distance = sqrtf(distance_y*distance_y + distance_x*distance_x);

	float inverse_distance = 1.0f/distance;
	sf::Vector2f vectorNorm(distance_x*inverse_distance, distance_y*inverse_distance);
	sf::Vector2f vectorTan(-vectorNorm.y, vectorNorm.x);

	float dpTanCurrent = vectorTan.x*vel.x + vectorTan.y*vel.y;
	float dpTanTarget = vectorTan.x*targetB->vel.x + vectorTan.y*targetB->vel.y;

	float dpNormCurrent = vectorNorm.x*vel.x + vectorNorm.y*vel.y;
	float dpNormTarget = vectorNorm.x*targetB->vel.x + vectorNorm.y*targetB->vel.y;

	float mCurrent = (dpNormCurrent * (mass - targetB->mass) + 2.0f * targetB->mass * dpNormTarget) / (mass + targetB->mass); 
	float mTarget = (dpNormTarget * (targetB->mass - mass) + 2.0f * mass * dpNormCurrent) / (mass + targetB->mass);

	vel = (vectorTan*dpTanCurrent + vectorNorm*mCurrent);
	targetB->vel = (vectorTan*dpTanTarget + vectorNorm*mTarget);
}








