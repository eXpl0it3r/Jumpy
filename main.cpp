#include <stdlib.h>
#include <time.h>
#include <sstream>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "cPlatform.hpp"

// Vorabdeklaration für die Kollisions-Funktion
bool Collision(sf::Sprite* Player, sf::Sprite* Collision, int iToleranz=0);
// Vorabdeklaration für den Zufallszahlgenerator
int Random(int min, int max);

int main(int argc, char** argv)
{
   // Definiere Einstellungsvariabeln
   int iWindowWidth  = 350;
   int iWindowHeight = 570;
   int iGamestate    = 2; // @TODO (Test)

   // Erstelle das Hauptfenster
   sf::RenderWindow Game(sf::VideoMode(iWindowWidth, iWindowHeight), "Doodle Jumpy", sf::Style::Titlebar|sf::Style::Close);
   
   // Interne Variabeln
   sf::Event Events; // Handle für Events
   const sf::Input& DirectInput = Game.GetInput(); // Handle für direkte Inputs
   std::ostringstream ssLogfile; // Logfile
   sf::IntRect rectView(0, 0, iWindowWidth*2, iWindowHeight); // Spezifische Hintergrund Ansicht
   sf::Clock clFrame; // FPS-Timer
   int iFrame = 0; // Frame-Zähler
   float fFPS = 600.0f; // Frames Pro Sekunde
   float fToper = 570;
   cPlatform* pfHead = NULL;
   cPlatform* pfTail = NULL;

   // Lade das Hintergrundbild
   sf::Image imgBackground;
   if(!imgBackground.LoadFromFile("res/background.png"))
      ssLogfile << "Resource 'background.png' is missing!" << std::endl;
   sf::Sprite spBackground(imgBackground);
   spBackground.SetSubRect(rectView);

   // Titel / Menü
   sf::Image imgTitle;
   if(!imgTitle.LoadFromFile("res/title.png"))
      ssLogfile << "Resource 'title.png' is missing!" << std::endl;
   sf::Sprite spTitle(imgTitle);
   sf::Image imgHover;
   if(!imgHover.LoadFromFile("res/hover.png"))
      ssLogfile << "Resource 'hover.png' is missing!" << std::endl;
   sf::Sprite spHover(imgHover);
   spHover.SetPosition(0.0f, 488.0f);

   // "Game Over"-Bild laden
   sf::Image imgGameover;
   if(!imgGameover.LoadFromFile("res/gameover2.png"))
      ssLogfile << "Resource 'gameover2.png' is missing!" << std::endl;
   sf::Sprite spGameover(imgGameover);
   spGameover.SetPosition(25, 25);

   // Spielervariabeln
   sf::Image imgPlayer;
   if(!imgPlayer.LoadFromFile("res/player.png"))
      ssLogfile << "Resource 'player.png' is missing!" << std::endl;
   sf::Image imgPlayerGameover;
   if(!imgPlayerGameover.LoadFromFile("res/player-go.png"))
      ssLogfile << "Resource 'player-go.png' is missing!" << std::endl;
   sf::Sprite spPlayer(imgPlayer);
   //spPlayer.SetOrigin(0,0);
   spPlayer.SetPosition(iWindowWidth/2-25, iWindowHeight/2-1);
   float vlr = 0.0f; // Geschwindigkeit links/rechts
   float vud = 0.0f; // Geschwindigkeit hoch/runter
   sf::Vector2< float > fOldPos = spPlayer.GetPosition();
   float fPoints = 0; // Punkte
   bool bGameover = false;

   // @TODO (Debug Info)
   sf::Font FreeFont;
   if(!FreeFont.LoadFromFile("res/FreeSansBold.ttf"))
      ssLogfile << "Resource 'FreeSansBold.ttf' is missing!" << std::endl;
   sf::Text Score("Doodle Jumper", FreeFont, 20);
   Score.SetColor(sf::Color(153,34,17,255));
   Score.SetPosition(10,10);

   // Plattformen laden
   sf::Image imgSolid;
   if(!imgSolid.LoadFromFile("res/platform-solid.png"))
      ssLogfile << "Resource 'Platform-solid.png' is missing!" << std::endl;
   sf::Image imgUnstable;
   if(!imgUnstable.LoadFromFile("res/platform-unstable.png"))
      ssLogfile << "Resource 'Platform-unstable.png' is missing!" << std::endl;
   sf::Image imgJump;
   if(!imgJump.LoadFromFile("res/platform-jump.png"))
      ssLogfile << "Resource 'Platform-jump.png' is missing!" << std::endl;
   sf::Image imgFloat;
   if(!imgFloat.LoadFromFile("res/platform-float.png"))
      ssLogfile << "Resource 'Platform-float.png' is missing!" << std::endl;

   sf::Sprite* spGen;
   int genX=0, genY=0;
   float fMoved = 0.0f;

   // Setzte den Zähler zurück
   clFrame.Reset();

   while(Game.IsOpened())
   {
      // Verarbeite Ereignisse
      while(Game.PollEvent(Events))
      {
         // Schliesse Fenster : Exit
         if(Events.Type == sf::Event::Closed)
         {
            iGamestate = 0;
            Game.Close();
         }
         else if(Events.Type == sf::Event::MouseButtonReleased && iGamestate == 2 && DirectInput.GetMouseY() > 488 && DirectInput.GetMouseY() <= 532)
         {
            iGamestate = 3;
         }
      }
      if(iGamestate == 3) // Game
      {
         // Verarbeite direkte Inputs
         if(!DirectInput.IsKeyDown(sf::Key::Left) && !DirectInput.IsKeyDown(sf::Key::Right) && vlr < 0.3 && vlr > 0.3)
            vlr = 0;
         else if(!DirectInput.IsKeyDown(sf::Key::Left) && !DirectInput.IsKeyDown(sf::Key::Right) && vlr < 0)
            vlr += 2.0f;
         else if(!DirectInput.IsKeyDown(sf::Key::Left) && !DirectInput.IsKeyDown(sf::Key::Right) && vlr > 0)
            vlr -= 2.0f;
         else if(!DirectInput.IsKeyDown(sf::Key::Left) && DirectInput.IsKeyDown(sf::Key::Right))
            vlr = 200.0f;
         else if(DirectInput.IsKeyDown(sf::Key::Left) && !DirectInput.IsKeyDown(sf::Key::Right))
            vlr = -200.0f;
         else if(DirectInput.IsKeyDown(sf::Key::Left) && DirectInput.IsKeyDown(sf::Key::Right))
            vlr = 0;

         if(!bGameover)
         {
            if(pfHead == NULL)
            {
               srand(time(0));

               for(int y=0; y < 7; y++)
                  for(int x=0; x < 3; x++)
                  {
                     genX = Random(x*115, x*115+45);
                     genY = (iWindowHeight-Random(y*114, y*114+84))+10;
                     if(x == 0 && y == 0)
                     {
                        pfHead = new cPlatform;
                        pfHead->Platform = new sf::Sprite(imgSolid);
                        pfHead->Platform->SetPosition(iWindowWidth/2+34, iWindowHeight-40);
                        pfHead->Type = 1;
                        pfTail = pfHead;
                     }
                     else
                     {
                        switch(Random(1,12))
                        {
                        // 1-4 & Default: Nichts
                        case 5:
                        case 6:
                        case 7:
                           spGen = new sf::Sprite(imgSolid);
                           spGen->SetPosition(genX, genY);
                           pfTail->Insert(spGen, 1);
                           pfTail = pfTail->Next;
                           break;
                        case 8:
                        case 9:
                           spGen = new sf::Sprite(imgUnstable);
                           spGen->SetPosition(genX, genY);
                           pfTail->Insert(spGen, 2);
                           pfTail = pfTail->Next;
                           break;
                        case 10:
                           spGen = new sf::Sprite(imgJump);
                           spGen->SetPosition(genX, genY);
                           pfTail->Insert(spGen, 3);
                           pfTail = pfTail->Next;
                           break;
                        case 11:
                        case 12:
                           spGen = new sf::Sprite(imgFloat);
                           spGen->SetPosition(genX, genY);
                           pfTail->Insert(spGen, 4);
                           pfTail = pfTail->Next;
                        }
                     }
                  }
            }

            // Überprüfe Kollisionen
            if(vud < 0) // Ist der Spieler am Fallen?
            {
               for(cPlatform* Index = pfHead; Index != NULL; Index = Index->Next)
               {
                  if(Collision(&spPlayer, Index->Platform))
                  {
                     switch(Index->Type)
                     {
                     case 1: // Solid
                     case 4: // Float
                        vud = 400;
                        break;
                     case 2: // Unstable
                        vud = 380;

                        if(Index == pfHead)
                           pfHead = Index->Next;
                        else if(Index == pfTail)
                           pfTail = Index->Last;
                        Index->Remove();
                        delete Index;
                        Index = NULL;

                        break;
                     case 3: // Jump
                        vud = 600;
                        break;
                     }
                     // Jede Plattform Berührung kostet 10p
                     if(fPoints >= 10)
                        fPoints -= 10;
                     break;
                  }
               }
            }
            if(spPlayer.GetPosition().y+spPlayer.GetImage()->GetHeight() >= iWindowHeight) // Am unteren Rand?
            {
               // Game Over
               spPlayer.SetImage(imgPlayerGameover);
               bGameover = true;
            }
         
            // Berechne neue Positionen
            fOldPos = spPlayer.GetPosition();
         
            // Hintergrund & Plattformen verschieben
            if(spPlayer.GetPosition().y <= iWindowHeight/3)
            {
               // Der Spieler fällt wieder hinunter
               if(vud < 0)
                  spPlayer.SetPosition(fOldPos.x+(vlr/fFPS), fOldPos.y+1);
               else
                  spPlayer.SetPosition(fOldPos.x+(vlr/fFPS), fOldPos.y);

               // Hintergrund verschieben
               fToper -= (vud/fFPS);
               rectView.Top = fToper;
               spBackground.SetSubRect(rectView);
               if(rectView.Top <= 0)
               {
                  rectView.Top = 570;
                  fToper = 570.0f;
               }
            
               // Plattformen verschieben
               for(cPlatform* Index = pfHead; Index != NULL; Index = Index->Next)
               {
                  Index->Platform->SetPosition(Index->Platform->GetPosition().x, Index->Platform->GetPosition().y+(vud/fFPS));
                  if(Index->Platform->GetPosition().y > iWindowHeight)
                  {
                     cPlatform* Last = Index->Last;
                     if(Index == pfHead)
                        pfHead = Index->Next;
                     else if(Index == pfTail)
                        pfTail = Index->Last;
                     Index->Remove();
                     delete Index;
                     if(Last == NULL)
                        break;
                     Index = Last;
                  }
               }

               // Generiere Plattformen
               fMoved += vud/fFPS;

               if(fMoved > 100)
               {
                  fMoved = 0.0f;
                  for(int x=0; x < 2; x++)
                  {
                     genX = Random(x*140, x*140+140);
                     genY = 0-Random(84, 0);
                     switch(Random(1,12))
                     {
                     // 1-3 & Default: Nichts
                     case 3:
                     case 4:
                     case 5:
                        spGen = new sf::Sprite(imgSolid);
                        spGen->SetPosition(genX, genY);
                        pfTail->Insert(spGen, 1);
                        pfTail = pfTail->Next;
                        break;
                     case 6:
                     case 7:
                     case 8:
                        spGen = new sf::Sprite(imgUnstable);
                        spGen->SetPosition(genX, genY);
                        pfTail->Insert(spGen, 2);
                        pfTail = pfTail->Next;
                        break;
                     case 9:
                     case 10:
                        spGen = new sf::Sprite(imgJump);
                        spGen->SetPosition(genX, genY);
                        pfTail->Insert(spGen, 3);
                        pfTail = pfTail->Next;
                        break;
                     case 11:
                     case 12:
                        spGen = new sf::Sprite(imgFloat);
                        spGen->SetPosition(genX, genY);
                        pfTail->Insert(spGen, 4);
                        pfTail = pfTail->Next;
                     }
                  }
               }
               // @TODO (Generate new platforms with 'random' position)
            }
            // Spieler verschieben
            else
               spPlayer.SetPosition(fOldPos.x+(vlr/fFPS),fOldPos.y-(vud/fFPS));

            // Punkteberechnung
            fPoints += (vud/fFPS)/10;
            if(fPoints < 0)
               fPoints = 0;
         }
         else
         {
            // Berechne neue Positionen
            fOldPos = spPlayer.GetPosition();

            spPlayer.SetPosition(fOldPos.x+(vlr/fFPS), fOldPos.y);

            // Hintergrund verschieben
            fToper -= (vud/fFPS);
            rectView.Top = fToper;
            spBackground.SetSubRect(rectView);
            if(rectView.Top >= 570)
            {
               rectView.Top = 0;
               fToper = 0.0f;
            }
            
            // Plattformen verschieben
            for(cPlatform* Index = pfHead; Index != NULL; Index = Index->Next)
            {
               Index->Platform->SetPosition(Index->Platform->GetPosition().x, Index->Platform->GetPosition().y+(vud/fFPS));
               if(Index->Platform->GetPosition().y < -500)
               {
                  cPlatform* Last = Index->Last;
                  if(Index == pfHead)
                     pfHead = Index->Next;
                  else if(Index == pfTail)
                     pfTail = Index->Last;
                  Index->Remove();
                  delete Index;
                  if(Last == NULL)
                     break;
                  Index = Last;
               }
            }
         }

         vud -= 0.6f;
         
         // Fensteroberfläche einfärben
         //Game.Clear();

         // Zeichne das Hintergrundbild
         Game.Draw(spBackground);

         // Zeichne die Plattformen
         for(cPlatform* Index = pfHead; Index != NULL; Index = Index->Next)
         {
            if(Index->Type == 4)
               Index->Move(fFPS);
            Game.Draw(*(Index->Platform));
         }
         // Zeichne den Spieler
         Game.Draw(spPlayer);

         // Game Over
         if(bGameover && pfHead == NULL)
         {
            Game.Draw(spGameover);
            std::ostringstream out;
            out << "Du hast\n" << static_cast< int >(fPoints) << " Punkte\nerreicht!";
            Score.SetString(out.str());
            Score.SetCharacterSize(25);
            Score.SetPosition(100, 300);

            Game.Draw(Score);
            // @TODO Highscore
         }
         else // Score
         {
            std::ostringstream out;
            out << static_cast< int >(fPoints);
            Score.SetString(out.str());
            Game.Draw(Score);
         }

         // Aktualisiere das Fenster
         Game.Display();

         // Erhöhe den Frame-Zähler
         iFrame++;
      
         // Berechne die Frames pro Sekunde
         if(clFrame.GetElapsedTime() >= 1.0f)
         {
            fFPS = static_cast< float >(iFrame)/clFrame.GetElapsedTime();
            clFrame.Reset();
            iFrame = 0;
         }
      }
      else if(iGamestate == 2) // Menü
      {
         // Fensteroberfläche einfärben
         Game.Clear();
         
         Game.Draw(spTitle);

         if(DirectInput.GetMouseY() > 488 && DirectInput.GetMouseY() <= 532)
            Game.Draw(spHover);
         // Aktualisiere das Fenster
         Game.Display();

         // Erhöhe den Frame-Zähler
         iFrame++;
      
         // Berechne die Frames pro Sekunde
         if(clFrame.GetElapsedTime() >= 1.0f)
         {
            fFPS = static_cast< float >(iFrame)/clFrame.GetElapsedTime();
            clFrame.Reset();
            iFrame = 0;
         }
      }
      else if(iGamestate == 1) // Splash
      {
         // Fensteroberfläche einfärben
         Game.Clear();
         // Aktualisiere das Fenster
         Game.Display();
      }
      else if(iGamestate == -1) // Credits
      {
         // Fensteroberfläche einfärben
         Game.Clear();
         // Aktualisiere das Fenster
         Game.Display();
      }
   }
 
   return EXIT_SUCCESS;
}

bool Collision(sf::Sprite* Player, sf::Sprite* Collision, int iToleranz)
{
   if(Player->GetPosition().y+Player->GetImage()->GetHeight() >= Collision->GetPosition().y &&
      Player->GetPosition().y+(Player->GetImage()->GetHeight()) <= Collision->GetPosition().y+Collision->GetImage()->GetHeight() &&
      Player->GetPosition().x+Player->GetImage()->GetWidth() > Collision->GetPosition().x &&
      Player->GetPosition().x < Collision->GetPosition().x+Collision->GetImage()->GetWidth())
      return true;
   return false;
}

int Random(int min, int max)
{
   return min + (rand() % (max - min + 1));
}