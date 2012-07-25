#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#include <SFML/Graphics.hpp>

class Platform
{
public:
	Platform();

	sf::Sprite Sprite;
	int Type;
   
	void update( float aDt );

private:
	int moveType;
	float relPos;
	int Direction;
	int Speed;
};

#endif // PLATFORM_HPP
