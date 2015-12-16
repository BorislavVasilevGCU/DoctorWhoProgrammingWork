#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#define GLX_GLXEXT_LEGACY //Must be declared so that our local glxext.h is picked up, rather than the system one


#include <windows.h>
#include "windowOGL.h"
#include "GameConstants.h"
#include "cWNDManager.h"
#include "cInputMgr.h"
#include "cSprite.h"
#include "cBkGround.h"
#include "cFontMgr.h"
#include "asteroidsGame.h"
#include "cFileHandler.h"
#include "cButton.h"
#include "cSoundMgr.h"
#include "mmsystem.h"


int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR cmdLine,
	int cmdShow)
{

	//Set our window settings
	const int windowWidth = 1024;
	const int windowHeight = 768;
	const int windowBPP = 16;



	//This is our window
	static cWNDManager* pgmWNDMgr = cWNDManager::getInstance();

	// This is the input manager
	static cInputMgr* theInputMgr = cInputMgr::getInstance();

	// This is the sound manager
	static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();

	// This is the Font manager
	static cFontMgr* theFontMgr = cFontMgr::getInstance();


	//The example OpenGL code
	windowOGL theOGLWnd;

	//Attach our the OpenGL window
	pgmWNDMgr->attachOGLWnd(&theOGLWnd);

	// Attach the keyboard manager
	pgmWNDMgr->attachInputMgr(theInputMgr);

	// Cube

	//Attempt to create the window
	if (!pgmWNDMgr->createWND(windowWidth, windowHeight, windowBPP))
	{
		//If it fails

		MessageBox(NULL, "Unable to create the OpenGL Window", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
		return 1;
	}

	if (!theOGLWnd.initOGL(windowWidth, windowHeight)) //Initialize our example
	{
		MessageBox(NULL, "Could not initialize the application", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
		return 1;
	}

	// load game sounds
	// Load Sound
	LPCSTR gameSounds[2] = { "Audio/131432__senitiel__lasershot.wav", "Audio/theme12.wav" };

	//theSoundMgr->add("Theme", gameSounds[0]);
	theSoundMgr->add("Shot", gameSounds[0]);
	theSoundMgr->add("ThemeMusic", gameSounds[1]);

	// load game fontss
	// Load Fonts
	LPCSTR gameFonts[2] = { "Fonts/digital-7.ttf", "Fonts/space age.ttf" };

	theFontMgr->addFont("SevenSeg", gameFonts[0], 24);
	theFontMgr->addFont("Space", gameFonts[1], 24);

	// Create vector array of textures for the enemies 
	LPCSTR enemyTextures[] = { "Images//dalek1.png", "Images//dalek2.png", "Images//dalek3.png", "Images//dalek4.png", "Images//dalek5.png", "Images//dalek6.png", "Images//Laser.png", };
	for (int tCount = 0; tCount < 7; tCount++)
	{
		theGameTextures.push_back(new cTexture());
		theGameTextures[tCount]->createTexture(enemyTextures[tCount]);
	}

	/// Create vector array of textures for the 4 backgrounds for the menus
	vector<cTexture*> textureBkgList;
	LPCSTR bkgTexturesToUse[] = { "Images/mainlevelbkgsprite.png", "Images/mainmenusprite.jpg", "Images/endgamsprite.jpg", "Images/keyboardbinds.png" };
	for (int tCount = 0; tCount < 4; tCount++)
	{
		textureBkgList.push_back(new cTexture());
		textureBkgList[tCount]->createTexture(bkgTexturesToUse[tCount]);
	}

	cTexture transSprite;
	transSprite.createTexture("Images/blank.png");

	//setting the main level background sprite
	cBkGround mainLevelBkg; 
	mainLevelBkg.setSpritePos(glm::vec2(0.0f, 0.0f));
	mainLevelBkg.setTexture(textureBkgList[0]->getTexture());
	mainLevelBkg.setTextureDimensions(textureBkgList[0]->getTWidth(), textureBkgList[0]->getTHeight());

	//setting the main menu background sprite
	cBkGround startMenuBkg;
	startMenuBkg.setSpritePos(glm::vec2(0.0f, 0.0f));
	startMenuBkg.setTexture(textureBkgList[1]->getTexture());
	startMenuBkg.setTextureDimensions(textureBkgList[1]->getTWidth(), textureBkgList[1]->getTHeight());

	//setting the "controls" sprite
	cBkGround keybinds;
	keybinds.setSpritePos(glm::vec2(0.0f, 200.0f));
	keybinds.setTexture(textureBkgList[3]->getTexture());
	keybinds.setTextureDimensions(963.0f, 450.0f);

	//setting the end game background sprite
	cBkGround endScreenBkg;
	endScreenBkg.setSpritePos(glm::vec2(0.0f, 0.0f));
	endScreenBkg.setTexture(textureBkgList[2]->getTexture());
	endScreenBkg.setTextureDimensions(textureBkgList[2]->getTWidth(), textureBkgList[2]->getTHeight());

	//creating an array of textures for the buttons
	vector<cTexture*> btnTextureList;
	LPCSTR btnTexturesToUse[] = { "Images/StartButton.png", "Images/HowToPlayButton.png", "Images/ExitButton.png", "Images/ReplayButton.png", "Images/BackButton.png" };
	for (int tCount = 0; tCount < 5; tCount++)
	{
		btnTextureList.push_back(new cTexture());
		btnTextureList[tCount]->createTexture(btnTexturesToUse[tCount]);
	}

	//setting the exit button sprite
	cButton exitButton;
	exitButton.attachInputMgr(theInputMgr);
	exitButton.setTexture(btnTextureList[2]->getTexture());
	exitButton.setTextureDimensions(btnTextureList[2]->getTWidth(), btnTextureList[2]->getTHeight());

	//setting the play button sprite
	cButton playButton;
	playButton.attachInputMgr(theInputMgr);
	playButton.setTexture(btnTextureList[0]->getTexture());
	playButton.setTextureDimensions(btnTextureList[0]->getTWidth(), btnTextureList[0]->getTHeight());

	//setting the replay button sprite
	cButton replayButton;
	replayButton.attachInputMgr(theInputMgr);
	replayButton.setTexture(btnTextureList[3]->getTexture());
	replayButton.setTextureDimensions(btnTextureList[3]->getTWidth(), btnTextureList[3]->getTHeight());

	//setting the how to play button sprite
	cButton howToPlayButton;
	howToPlayButton.attachInputMgr(theInputMgr);
	howToPlayButton.setTexture(btnTextureList[1]->getTexture());
	howToPlayButton.setTextureDimensions(btnTextureList[1]->getTWidth(), btnTextureList[1]->getTHeight());

	//setting the back button sprite
	cButton backButton;
	backButton.attachInputMgr(theInputMgr);
	backButton.setTexture(btnTextureList[4]->getTexture());
	backButton.setTextureDimensions(btnTextureList[4]->getTWidth(), btnTextureList[4]->getTHeight());



	//array of messages which are to be displayed 
	string outputMsg;
	string strMsg[] = { "Destroy as many enemy Daleks as possible!", "Use the keys to pilot the TARDIS.", "Doctor Who", "Thanks for playing!", "See you again soon!"};

	cTexture rocketTxt;
	rocketTxt.createTexture("Images\\Doctor-Who-TARDIS-Air-Freshenersmall.png");//creating the texture for the player's ship
	cRocket rocketSprite;
	rocketSprite.attachInputMgr(theInputMgr); // Attach the input manager to the sprite
	rocketSprite.setSpritePos(glm::vec2(512.0f, 380.0f)); //setting the sprite position on the screen 
	rocketSprite.setTexture(rocketTxt.getTexture()); //setting the texture for the player's ship
	rocketSprite.setTextureDimensions(rocketTxt.getTWidth(), rocketTxt.getTHeight()); //getting the texture dimensions
	rocketSprite.setSpriteCentre(); //setting the sprite center
	rocketSprite.setRocketVelocity(glm::vec2(0.0f, 0.0f));

	// Attach sound manager to rocket sprite
	rocketSprite.attachSoundMgr(theSoundMgr);

	//main loop for spawning the enemy sprites
	for (int astro = 0; astro < 60; astro++)
	{
		theAsteroids.push_back(new cAsteroid);
		theAsteroids[astro]->setSpritePos(glm::vec2(windowWidth / (rand() % 10 + 1), -40.0f));
		theAsteroids[astro]->setSpriteTranslation(glm::vec2((rand() % 5 + 1), 100.0f));
		theAsteroids[astro]->setTexture(theGameTextures[rand() % 6]->getTexture());
		theAsteroids[astro]->setTextureDimensions(theGameTextures[rand() % 6]->getTWidth(), theGameTextures[rand() % 6]->getTHeight());
		theAsteroids[astro]->setSpriteCentre();
		theAsteroids[astro]->setAsteroidVelocity(glm::vec2(glm::vec2(0.0f, 0.0f)));
		theAsteroids[astro]->setActive(true);
		theAsteroids[astro]->setMdlRadius();
		}


		gameState theGameState = MENU;
		btnTypes theBtnType = EXIT;

		while (pgmWNDMgr->isWNDRunning())
		{
			pgmWNDMgr->processWNDEvents(); //Process any window events

			//We get the time that passed since the last frame
			float elapsedTime = pgmWNDMgr->getElapsedSeconds();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//This is the mainloop, we render frames until isRunning returns false
			switch (theGameState)
			{
			case MENU:
			{
						 
						 theSoundMgr->getSnd("ThemeMusic")->playAudio(AL_TRUE); //main menu theme music 
						 startMenuBkg.render(); // render the menu background

						 playButton.setSpritePos(glm::vec2(400.0f, 300.0f)); //setting the start button sprite position
						 howToPlayButton.setSpritePos(glm::vec2(390.0f, 375.0f)); //setting the how to play button sprite position
						 exitButton.setSpritePos(glm::vec2(400.0f, 450.0f)); //setting the exit button sprite position
						 playButton.render(); //render the play button 
						 howToPlayButton.render(); //render the how to play button
						 exitButton.render(); //render the exit button 



						 theGameState = playButton.update(theGameState, PLAYING); //changing the game state when the play button is pressed 
						 theGameState = howToPlayButton.update(theGameState, INSTRUCTIONS); //changing the game state when the how to play button is pressed 
						 if (howToPlayButton.getClicked())
						 {
							 PlaySound(TEXT("Audio/107154__bubaproducer__button-7.wav"), NULL, SND_FILENAME | SND_ASYNC); // the sound for when the button is clicked
							 Sleep(20);
						 }

						 exitButton.update(elapsedTime); //updating the exit button 

						 outputMsg = strMsg[2]; // getting a message from the array
						 theFontMgr->getFont("Space")->printText(outputMsg.c_str(), FTPoint(400, 15, 0.0f)); //displaying the message on screen at the given position

						 if (exitButton.getClicked())
						 {
							 SendMessage(pgmWNDMgr->getWNDHandle(), WM_CLOSE, NULL, NULL); //if the exit button is clicked a message is send to close the window 
							 PlaySound(TEXT("Audio/107154__bubaproducer__button-7.wav"), NULL, SND_FILENAME | SND_ASYNC); //the sound for when exit button is clicked
						 }
						
						
						 
			}
				break;

			case INSTRUCTIONS: //next case for the "How to play" menu
			{
								 startMenuBkg.render(); //render the background for the case 
								 keybinds.render(); //render the "How to Play" sprite 


								 backButton.setSpritePos(glm::vec2(400.0f, 650.0f)); //setting the back button position on screen
								 backButton.render(); //render back button  on screen

								 theGameState = backButton.update(theGameState, MENU); //changing the game state when the back button is pressed
								 if(backButton.getClicked())
								 {
									 PlaySound(TEXT("Audio/107154__bubaproducer__button-7.wav"), NULL, SND_FILENAME | SND_SYNC); // the sound for when the button is clicked
									 Sleep(10);
								 }

								 //messages to display on the screen
								 outputMsg = strMsg[2];
								 theFontMgr->getFont("Space")->printText(outputMsg.c_str(), FTPoint(400, 15, 0.0f));
								 outputMsg = strMsg[0];
								 theFontMgr->getFont("Space")->printText(outputMsg.c_str(), FTPoint(100, 100, 0.0f));
								 outputMsg = strMsg[1];
								 theFontMgr->getFont("Space")->printText(outputMsg.c_str(), FTPoint(100, 150, 0.0f));
			}
				break;


			case PLAYING:  //next case the main gameplay scene
			{
							
							mainLevelBkg.render(); // render main level sprite background

							exitButton.setSpritePos(glm::vec2(800.0f, 675.0f)); //setting the exit button position on screen
							exitButton.render(); //render exit button on screen
							theGameState = exitButton.update(theGameState, END); //changing the game state when the exit button is pressed

							outputMsg = strMsg[2]; //message to display on screen
							theFontMgr->getFont("Space")->printText(outputMsg.c_str(), FTPoint(400, 15, 0.0f));




							vector<cAsteroid*>::iterator index = theAsteroids.begin();
							while (index != theAsteroids.end())
							{
								if ((*index)->isActive() == false)
								{
									index = theAsteroids.erase(index);
								}
								else
								{
									(*index)->update(elapsedTime);
									(*index)->render();
									++index;
								}
							}
							
							
							rocketSprite.update(elapsedTime);
							rocketSprite.render();



			}
				break;

			case END:  //next case the ending scene
			{
						endScreenBkg.render(); // render end screen background sprite

						playButton.setClicked(false); //set play button to false
						exitButton.setClicked(false); //set exit button to false
						
						playButton.setSpritePos(glm::vec2(400.0f, 375.0f)); //set play button sprite position on screen 
						exitButton.setSpritePos(glm::vec2(400.0f, 450.0f)); //set exit button sprite position on screen
						playButton.render(); //render the play button
						exitButton.render(); //render the exit button

						theGameState = playButton.update(theGameState, MENU); //changing the game state when the play button is pressed
						exitButton.update(elapsedTime); //updating the exit button

						//message to display on screen 
						outputMsg = strMsg[2];
						theFontMgr->getFont("Space")->printText(outputMsg.c_str(), FTPoint(400, 15, 0.0f));
						outputMsg = strMsg[3];
						theFontMgr->getFont("Space")->printText(outputMsg.c_str(), FTPoint(100, 100, 0.0f));
						outputMsg = strMsg[4];
						theFontMgr->getFont("Space")->printText(outputMsg.c_str(), FTPoint(100, 150, 0.0f));
						if (exitButton.getClicked())
						{
							SendMessage(pgmWNDMgr->getWNDHandle(), WM_CLOSE, NULL, NULL);
						}
			}
				break;




			}
			pgmWNDMgr->swapBuffers();
			theInputMgr->clearBuffers(theInputMgr->KEYS_DOWN_BUFFER | theInputMgr->KEYS_PRESSED_BUFFER | theInputMgr->MOUSE_BUFFER);
		}

		theOGLWnd.shutdown(); //Free any resources
		pgmWNDMgr->destroyWND(); //Destroy the program window

		return 0; //Return success
	}
