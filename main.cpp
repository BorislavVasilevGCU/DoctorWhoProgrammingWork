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
#include "mazeMaker.h"
#include "cFileHandler.h"
#include "cButton.h"

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

	//Clear key buffers
	theInputMgr->clearBuffers(theInputMgr->KEYS_DOWN_BUFFER | theInputMgr->KEYS_PRESSED_BUFFER | theInputMgr->MOUSE_BUFFER);

	int map[8][8] = { { 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 } };

	// load game fontss
	// Load Fonts
	LPCSTR gameFonts[2] = { "Fonts/digital-7.ttf", "Fonts/space age.ttf" };

	theFontMgr->addFont("SevenSeg", gameFonts[0], 24);
	theFontMgr->addFont("Space", gameFonts[1], 24);

	// Create vector array of textures
	vector<cTexture*> textureList;
	LPCSTR texturesToUse[] = { "Images/blank.png","Images/Track/straightHR.png", "Images/Track/straightVR.png", "Images/Track/bendLD.png", "Images/Track/bendRD.png", "Images/Track/bendLU.png", "Images/Track/bendRU.png"};
	for (int tCount = 0; tCount < 7; tCount++)
	{
		textureList.push_back(new cTexture());
		textureList[tCount]->createTexture(texturesToUse[tCount]);
	}

	/// Create vector array of textures
	vector<cTexture*> textureBkgList;
	LPCSTR bkgTexturesToUse[] = { "Images/mainlevelbkgsprite.png", "Images/mainmenusprite.jpg", "Images/endgamsprite.jpg" };
	for (int tCount = 0; tCount < 3; tCount++)
	{
		textureBkgList.push_back(new cTexture());
		textureBkgList[tCount]->createTexture(bkgTexturesToUse[tCount]);
	}

	cTexture transSprite;
	transSprite.createTexture("Images/blank.png");

	cBkGround mainLevelBkg;
	mainLevelBkg.setSpritePos(glm::vec2(0.0f, 0.0f));
	mainLevelBkg.setTexture(textureBkgList[0]->getTexture());
	mainLevelBkg.setTextureDimensions(textureBkgList[0]->getTWidth(), textureBkgList[0]->getTHeight());

	cBkGround startMenuBkg;
	startMenuBkg.setSpritePos(glm::vec2(0.0f, 0.0f));
	startMenuBkg.setTexture(textureBkgList[1]->getTexture());
	startMenuBkg.setTextureDimensions(textureBkgList[1]->getTWidth(), textureBkgList[1]->getTHeight());

	cBkGround endScreenBkg;
	endScreenBkg.setSpritePos(glm::vec2(0.0f, 0.0f));
	endScreenBkg.setTexture(textureBkgList[2]->getTexture());
	endScreenBkg.setTextureDimensions(textureBkgList[2]->getTWidth(), textureBkgList[2]->getTHeight());

	cTileMap tileMap(textureList, map);
	tileMap.attachInputMgr(theInputMgr); // Attach the input manager to the sprite
	tileMap.setSpritePos(glm::vec2(100.0f, 100.0f));

	cTilePicker roadMap(textureList, map);
	roadMap.attachInputMgr(theInputMgr); // Attach the input manager to the sprite
	roadMap.setSpritePos(glm::vec2(740.0f, 100.0f));

	cTile tileToDrag;
	tileToDrag.attachInputMgr(theInputMgr);
	tileToDrag.setSpritePos(glm::vec2(0.0f, 00.0f));
	tileToDrag.setTexture(transSprite.getTexture());
	tileToDrag.setTextureDimensions(transSprite.getTWidth(), transSprite.getTHeight());

	vector<cTexture*> btnTextureList;
	LPCSTR btnTexturesToUse[] = { "Images/StartButton.png", "Images/HowToPlayButton.png", "Images/ExitButton.png", "Images/ReplayButton.png", "Images/BackButton.png" };
	for (int tCount = 0; tCount < 5; tCount++)
	{
		btnTextureList.push_back(new cTexture());
		btnTextureList[tCount]->createTexture(btnTexturesToUse[tCount]);
	}

	cButton exitButton;
	exitButton.attachInputMgr(theInputMgr);
	exitButton.setTexture(btnTextureList[2]->getTexture());
	exitButton.setTextureDimensions(btnTextureList[2]->getTWidth(), btnTextureList[2]->getTHeight());

	cButton playButton;
	playButton.attachInputMgr(theInputMgr);
	playButton.setTexture(btnTextureList[0]->getTexture());
	playButton.setTextureDimensions(btnTextureList[0]->getTWidth(), btnTextureList[0]->getTHeight());

	cButton replayButton;
	replayButton.attachInputMgr(theInputMgr);
	replayButton.setTexture(btnTextureList[3]->getTexture());
	replayButton.setTextureDimensions(btnTextureList[3]->getTWidth(), btnTextureList[3]->getTHeight());

	cButton howToPlayButton;
	howToPlayButton.attachInputMgr(theInputMgr);
	howToPlayButton.setTexture(btnTextureList[1]->getTexture());
	howToPlayButton.setTextureDimensions(btnTextureList[1]->getTWidth(), btnTextureList[1]->getTHeight());

	// include an exit button

	cFileHandler theFile("Data/usermap.dat");
	string mapData;

	string outputMsg;
	string strMsg[] = { "Create and Save your own Track!","Drag and Drop the tiles.", "Road Mapper", "Thanks for playing!","See you again soon!" };

	if (!theFile.openFile(ios::in)) //open file for input output
	{
		MessageBox(NULL, "Could not open specified file.", "An error occurred", MB_ICONERROR | MB_OK);
	}
	else
	{
		mapData = theFile.readDataFromFile();
		theFile.closeFile();
		// Fill map with data from file
		int strPos = 0;
		for (int row = 0; row < 8; row++)
		{
			for (int column = 0; column < 8; column++)
			{
				if (mapData[strPos] != ',')
				{
					map[row][column] = mapData[strPos] - 48;
					strPos+= 2;
				}
			}
		}
	}

	tileMap.initialiseMap(map);

	gameState theGameState = MENU;
	btnTypes theBtnType = EXIT;

	//This is the mainloop, we render frames until isRunning returns false
	while (pgmWNDMgr->isWNDRunning())
    {
		pgmWNDMgr->processWNDEvents(); //Process any window events

        //We get the time that passed since the last frame
		float elapsedTime = pgmWNDMgr->getElapsedSeconds();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		switch (theGameState)
		{
		case MENU:
			startMenuBkg.render();

			playButton.setSpritePos(glm::vec2(400.0f, 300.0f));
			exitButton.setSpritePos(glm::vec2(400.0f, 400.0f));
			playButton.render();
			exitButton.render();

			theGameState = playButton.update(theGameState, PLAYING);
			exitButton.update();


			outputMsg = strMsg[2];
			theFontMgr->getFont("Space")->printText(outputMsg.c_str(), FTPoint(10, 15, 0.0f));
			outputMsg = strMsg[0];
			theFontMgr->getFont("Space")->printText(outputMsg.c_str(), FTPoint(100, 100, 0.0f));
			outputMsg = strMsg[1];
			theFontMgr->getFont("Space")->printText(outputMsg.c_str(), FTPoint(100, 150, 0.0f));
			if (exitButton.getClicked())
			{
				SendMessage(pgmWNDMgr->getWNDHandle(), WM_CLOSE, NULL, NULL);
			}
			break;
		case PLAYING:
			mainLevelBkg.render();

			roadMap.update();
			roadMap.render();
			tileMap.update();
			tileMap.render();
			tileMap.renderGridLines();
			tileToDrag.update();
			tileToDrag.render();

			// render button and reset clicked to false

			exitButton.setSpritePos(glm::vec2(740.0f, 575.0f));
			exitButton.render();
			theGameState = exitButton.update(theGameState, END);

			outputMsg = strMsg[2];
			theFontMgr->getFont("Space")->printText(outputMsg.c_str(), FTPoint(10, 15, 0.0f));

			break;
		case END:
			endScreenBkg.render();

			playButton.setClicked(false);
			exitButton.setClicked(false);
			replayButton.setClicked(false);

			playButton.setSpritePos(glm::vec2(400.0f, 300.0f));
			replayButton.setSpritePos(glm::vec2(400.0f, 400.0f));
			exitButton.setSpritePos(glm::vec2(400.0f, 450.0f));
			playButton.render();
			replayButton.render();
			exitButton.render();

			theGameState = playButton.update(theGameState, PLAYING);
			exitButton.update();

			outputMsg = strMsg[2];
			theFontMgr->getFont("Space")->printText(outputMsg.c_str(), FTPoint(10, 15, 0.0f));
			outputMsg = strMsg[3];
			theFontMgr->getFont("Space")->printText(outputMsg.c_str(), FTPoint(100, 100, 0.0f));
			outputMsg = strMsg[4];
			theFontMgr->getFont("Space")->printText(outputMsg.c_str(), FTPoint(100, 150, 0.0f));
			if (exitButton.getClicked())
			{
				SendMessage(pgmWNDMgr->getWNDHandle(), WM_CLOSE, NULL, NULL);
			}

			break;
		}

		pgmWNDMgr->swapBuffers();
		//theInputMgr->clearBuffers(theInputMgr->KEYS_DOWN_BUFFER | theInputMgr->KEYS_PRESSED_BUFFER | theInputMgr->MOUSE_BUFFER);
    }

	theOGLWnd.shutdown(); //Free any resources
	pgmWNDMgr->destroyWND(); //Destroy the program window

    return 0; //Return success
}
