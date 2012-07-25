#include <memory>
#include <iostream>

#include "IntroState.hpp"
#include "PlayState.hpp"

IntroState::IntroState( bool replace ) : GameState( replace ), mHover( false )
{
	if( !mTexTitle.loadFromFile( "res/title.png" ) )
		std::cout << "Resource 'title.png' is missing!" << std::endl;
	mSpTitle.setTexture( mTexTitle, true );

	if( !mTexHover.loadFromFile( "res/hover.png" ) )
		std::cout << "Resource 'hover.png' is missing!" << std::endl;
	mSpHover.setTexture( mTexHover );
	mSpHover.setPosition( 0.0f, 461.0f );

	mHoverRect = sf::IntRect( 0, 461, 350, 40 );

	std::cout << "IntroState Init" << std::endl;
}

IntroState::~IntroState()
{
	std::cout << "IntroState Cleanup" << std::endl;
}

void IntroState::Pause()
{
	std::cout << "IntroState Pause" << std::endl;
}

void IntroState::Resume()
{
	std::cout << "IntroState Resume" << std::endl;
}

void IntroState::HandleEvents( GameEngine& game )
{
	sf::Event event;

	while( game.screen.pollEvent( event ) )
	{
		switch( event.type )
		{
			case sf::Event::Closed:
				game.Quit();
				break;

			case sf::Event::MouseButtonReleased:
				if( mHover )
				{
					next = game.Build<PlayState>();
				}
				break;
		}
	}
}

void IntroState::Update( GameEngine& game ) 
{
	if( mHoverRect.contains( sf::Mouse::getPosition( game.screen ) ) )
		mHover = true;
	else
		mHover = false;
}

void IntroState::Draw( GameEngine& game ) 
{
	// Clear the previous drawing
	game.screen.clear();

	game.screen.draw( mSpTitle );

	if( mHover )
		game.screen.draw( mSpHover );

	game.screen.display();
}

