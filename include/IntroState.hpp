#ifndef INTROSTATE_HPP
#define INTROSTATE_HPP

#include <SFML/Graphics.hpp>
#include "GameState.hpp"

class GameEngine;

class IntroState : public GameState
{
public:
	IntroState( bool replace = true );
	~IntroState();

	void Pause();
	void Resume();

	void HandleEvents( GameEngine& game );
	void Update( GameEngine& game );
	void Draw( GameEngine& game );

private:
	sf::Texture mTexTitle;
	sf::Texture mTexHover;
	sf::Sprite mSpTitle;
	sf::Sprite mSpHover;

	sf::IntRect mHoverRect;
	bool mHover;
};

#endif // INTROSTATE_HPP
