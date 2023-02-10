#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <cstdlib>
#include <ctime>
#include <list>
#include <SFML/Graphics.hpp>
#include "Platform.hpp"

class Generator
{
public:
	Generator( int aWndWidth, int aWndHeight );

	void update( float aDt );
	void move( float aDistance );
	float collision( const sf::Sprite& aPlayer );
	void draw( sf::RenderTarget& targe ) const;

private:
	void generate( float aGenX, float aGenY );
	void generateInit();
	void generateNext();
	int randomizer( int aMin, int aMax );

private:
	float mWndWidth;
	float mWndHeight;
	std::list<Platform> mPlatforms;
	float mTotalMove;

	sf::Texture mTexSolid;
	sf::Texture mTexUnstable;
	sf::Texture mTexJump;
	sf::Texture mTexFloat;
};

#endif // GENERATOR_HPP
