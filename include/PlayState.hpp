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
	void ProcessInput();
	void CollisionDetection();
	void MoveObjects();
	void CalcGamePoints();

private:
	int mWndWidth;
	int mWndHeight;

	Generator mGenerator;

	sf::Texture mTexBackground;
	sf::Sprite mSpBackground;
	sf::IntRect mRectBackground;
	float mToper;

	sf::Texture mTexGameOver;
	sf::Sprite mSpGameOver;
	sf::Texture mTexPause;
	sf::Sprite mSpPause;

	sf::Texture mTexPlayer;
	sf::Texture mTexPlayerGameover;
	sf::Sprite mSpPlayer;

	sf::Font mFreeFont;
	sf::Text mScore;

	float mVelocityH; // Horizontal velocity
	float mVelocityV; // Vertical velocity
	float mDt; // Delta Time
	sf::Clock mFPS;

	bool mFirstMove;
	bool mPause;
	bool mGameOver;
	float mPoints;
};

#endif // PLAYSTATE_HPP
