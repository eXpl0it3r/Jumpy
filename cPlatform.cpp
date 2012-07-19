#include "cPlatform.hpp"

cPlatform::cPlatform()
{
   Next = NULL;
   Last = NULL;
   Platform = NULL;
   Type = 0;
   moveType = 0;
   relPos = 0;
   dFactor = 1;
   Speed = 80;
}
cPlatform::cPlatform(sf::Sprite* aPlatform, int aType)
{
   Next = NULL;
   Last = NULL;
   Platform = aPlatform;
   Type = aType;
   moveType = 0;
   relPos = 0;
   dFactor = 1;
   Speed = 80;
}

void cPlatform::Move(float FPS)
{
   if(moveType == 1) // Links-Rechts
   {
      Platform->SetPosition(Platform->GetPosition().x + (Speed/FPS)*dFactor, Platform->GetPosition().y);
   }
   else if(moveType == 2) // Hoch-Runter
   {
      Platform->SetPosition(Platform->GetPosition().x, Platform->GetPosition().y + (Speed/FPS)*dFactor);
   }
   else
      moveType = 1 + (rand() % 2);
   relPos += (Speed/FPS)*dFactor;
   if(relPos >= 60)
      dFactor = -1;
   else if(relPos <= -60)
      dFactor = 1;

}

cPlatform* cPlatform::Remove()
{
   if(Last != NULL) // Nicht Head
      Last->Next = Next;
   if(Next != NULL) // Nicht Tail
      Next->Last = Last;
   if(Platform !=NULL)
      delete Platform;
   
   Next = NULL;
   Last = NULL;
   Platform = NULL;
   Type = 0;

   return this;
}

void cPlatform::Insert(sf::Sprite* aPlatform, int aType)
{
   if(Next != NULL) // Nicht Tail & nicht einzeln
   {
      Next->Last = new cPlatform(aPlatform, aType);
      Next->Last->Next = Next;
      Next = Next->Last;
      Next->Last = this;
   }
   else
   {
      Next = new cPlatform(aPlatform, aType);
      Next->Last = this;
   }
}