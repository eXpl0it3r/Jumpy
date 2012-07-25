#include <memory>
#include <iostream>
#include <sstream>

#include "PlayState.hpp"
#include "GameEngine.hpp"

PlayState::PlayState( bool replace ) :
	GameState( replace ),
	mWndWidth( 350 ),
	mWndHeight( 570 ),
	mGenerator( mWndWidth, mWndHeight ),
	mVelocityH( 0.0f ),
	mVelocityV( 0.0f ),
	mPoints( 0.f )
{
	mRectBackground = sf::IntRect( 0, 0, mWndWidth*2, mWndHeight ); // Background shall be moved around
	mToper = 0.f;

	if( !mTexBackground.loadFromFile( "res/background.png" ) )
		std::cout << "Resource 'background.png' is missing!" << std::endl;
	mSpBackground.setTexture( mTexBackground, true );
	mSpBackground.setTextureRect( mRectBackground );

	if( !mTexPlayer.loadFromFile( "res/player.png" ) )
		std::cout << "Resource 'player.png' is missing!" << std::endl;
	if( !mTexPlayerGameover.loadFromFile( "res/player-go.png" ) )
		std::cout << "Resource 'player-go.png' is missing!" << std::endl;
	mSpPlayer.setTexture( mTexPlayer, true );
	mSpPlayer.setPosition( mWndWidth/2.f-25.f, mWndHeight/2.f-1.f ); // Center the player

	if( !mFreeFont.loadFromFile( "res/FreeSansBold.ttf" ) )
		std::cout << "Resource 'FreeSansBold.ttf' is missing!" << std::endl;
	mScore.setFont( mFreeFont );
	mScore.setCharacterSize( 20U );
	mScore.setString( "Doodle Jumper" );
	mScore.setColor( sf::Color( 153, 34, 17 ) );
	mScore.setPosition( 10, 10 );

	std::cout << "PlayState Init" << std::endl;
}

PlayState::~PlayState()
{
	std::cout << "PlayState Cleanup" << std::endl;
}

void PlayState::Pause()
{
	std::cout << "PlayState Pause" << std::endl;
}

void PlayState::Resume()
{
	std::cout << "PlayState Resume" << std::endl;
}

void PlayState::HandleEvents( GameEngine& game )
{
	sf::Event event;

	if( game.screen.pollEvent( event ) )
	{
		switch( event.type )
		{
			case sf::Event::Closed:
				game.Quit();
				break;
		}
	}
}

void PlayState::Update( GameEngine& game )
{
	// Get delta time
	mDt = mFPS.restart().asSeconds();

	// Get current player location
	sf::Vector2f pos = mSpPlayer.getPosition();

	// Process direct keyboard input

	// Stop horizontal movement
	if(!sf::Keyboard::isKeyPressed( sf::Keyboard::Left ) && !sf::Keyboard::isKeyPressed( sf::Keyboard::Right ) && mVelocityH < 20.0f && mVelocityH > -20.0f )
		mVelocityH = 0;
	// Deaccelerate
	else if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && mVelocityH < 0)
		mVelocityH += 280.0f * mDt;
	else if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && mVelocityH > 0)
		mVelocityH -= 280.0f * mDt;
	// Accelerate
	else if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		mVelocityH = 120.0f;
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		mVelocityH = -120.0f;
	// Stop if both keys are pressed
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		mVelocityH = 0;

	// @TODO DEBUG
	if(sf::Keyboard::isKeyPressed( sf::Keyboard::Up ) )
		mVelocityV = 150;
	if(sf::Keyboard::isKeyPressed( sf::Keyboard::Down ) )
		mVelocityV = -150;

	if( mVelocityV < 0.f )
	{
		float newVelocity = mGenerator.collision( mSpPlayer );
		if( newVelocity != 0.f )
			mVelocityV = newVelocity;
	}

	// Background & Platforms are moving
	if(mSpPlayer.getPosition().y <= mWndHeight / 3.f)
	{
		if(mVelocityV < 0) // Player is still falling
			mSpPlayer.setPosition( pos.x+( mVelocityH * mDt ), pos.y + 30 * mDt);
		else // Keep the player's y position fixed
			mSpPlayer.setPosition( pos.x+( mVelocityH * mDt ), pos.y);
		
		// Move background
		mToper -= mVelocityV * mDt;
		mRectBackground.top = static_cast<int>( mToper );
		mSpBackground.setTextureRect(mRectBackground);
		if(mRectBackground.top <= 0)
		{
			mRectBackground.top = 570;
			mToper = 570.0f;
		}

		mGenerator.move( mVelocityV * mDt );
	}
	else // No need to move the background
		mSpPlayer.setPosition( pos.x+( mVelocityH * mDt ), pos.y - mVelocityV * mDt );

	// Prevent player from leaving the playground
	if( mSpPlayer.getPosition().x < 0.f )
		mSpPlayer.setPosition( 0.f, mSpPlayer.getPosition().y );
	else if( mSpPlayer.getPosition().x > mWndWidth - mSpPlayer.getGlobalBounds().width )
		mSpPlayer.setPosition( mWndWidth - mSpPlayer.getGlobalBounds().width, mSpPlayer.getPosition().y );

	mGenerator.update( mDt );

	CalculatePoints();

	// Player should fall
	mVelocityV -= 280 * mDt;
}

void PlayState::Draw( GameEngine& game )
{
	// Clear the previous drawing
	game.screen.clear();
	game.screen.draw( mSpBackground );
	mGenerator.draw( game.screen );
	game.screen.draw( mSpPlayer );
	game.screen.draw( mScore );
	game.screen.display();
}

void PlayState::CalculatePoints()
{
	// Game points calculation
	mPoints += (mVelocityV * mDt) / 10.f;
	if(mPoints < 0)
		mPoints = 0;

	std::ostringstream out;
	out << static_cast< int >(mPoints);
	mScore.setString(out.str());
}