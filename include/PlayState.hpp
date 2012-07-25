#ifndef PLAYSTATE_HPP
#define PLAYSTATE_HPP

#include <SFML/Graphics.hpp>
#include "GameState.hpp"
#include "Generator.hpp"

class GameEngine;

class PlayState : public GameState
{
public:
	PlayState( bool replace = true );
	~PlayState();

	void Pause();
	void Resume();

	void HandleEvents( GameEngine& game );
	void Update( GameEngine& game );
	void Draw( GameEngine& game );

private:
	void CalculatePoints();

private:
	int mWndWidth;
	int mWndHeight;

	Generator mGenerator;

	sf::Texture mTexBackground;
	sf::Sprite mSpBackground;
	sf::IntRect mRectBackground;
	float mToper;

	sf::Texture mTexPlayer;
	sf::Texture mTexPlayerGameover;
	sf::Sprite mSpPlayer;

	sf::Font mFreeFont;
	sf::Text mScore;

	sf::Sprite mSpGen;

	float mVelocityH; // Horizontal velocity
	float mVelocityV; // Vertical velocity
	float mDt; // Delta Time
	sf::Clock mFPS;

	float mPoints;
};

#endif // PLAYSTATE_HPP
