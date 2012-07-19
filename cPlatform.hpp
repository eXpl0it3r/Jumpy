#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#include <stdlib.h>
#include <SFML/Graphics.hpp>

class cPlatform
{
public:
   cPlatform();
   cPlatform(sf::Sprite* aPlatform, int aType);
   cPlatform* Next;
   cPlatform* Last;
   sf::Sprite* Platform;
   int Type;
   
   cPlatform* Remove();
   void Insert(sf::Sprite* aPlatform, int aType);
   void Move(float FPS);

private:
   int moveType;
   float relPos;
   int dFactor;
   int Speed;
};

#endif /* PLATFORM_HPP */