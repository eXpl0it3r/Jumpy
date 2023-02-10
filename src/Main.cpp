#include "GameEngine.hpp"
#include "IntroState.hpp"

int main ()
{
	// initialize the engine
	GameEngine game( "Jumpy", 350U, 570U );
	game.Run( game.Build<IntroState>() );

	// main loop
	while( game.Running() )
	{
		game.NextState();
		game.HandleEvents();
		game.Update();
		game.Draw();
	}

	// leaving the scope of 'game' will cleanup the engine
    return 0;
}

