// COMP710 GP 2D Framework 2020

// Library includes:
#include <SDL.h>
#include <ctime>
#include <crtdbg.h>

// Local includes:
#include "game.h"
#include "logmanager.h"
#include "iniparser.h"
#include "sound.h"
#include "debugger.h"

int main(int argc, char* argv[])
{
	srand(static_cast<unsigned int>(time(0)));
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	IniParser::GetInstance().LoadIniFile("Settings.ini");

	SoundManager::GetInstance().Initialise();

	SoundManager::GetInstance().LoadSound(Sounds::DIE, "assets\\sounds\\die.wav");
	SoundManager::GetInstance().LoadSound(Sounds::EXPLOSION, "assets\\sounds\\explosion.wav");
	SoundManager::GetInstance().LoadSound(Sounds::HIT, "assets\\sounds\\hit.wav");

	Game& gameInstance = Game::GetInstance();
	if (!gameInstance.Initialise())
	{
		LogManager::GetInstance().Log("Game initialise failed!");

		return (1);
	}

	if (argc > 1 && strcmp(argv[1], "--nosplash") == 0)
	{
		Game::GetInstance().SetGameState(GameStates::MAIN_MENU);
	}

#ifdef _DEBUG
	Debugger::GetInstance().Initialize(&gameInstance);
#endif // _DEBUG
	
	while (gameInstance.DoGameLoop())
	{
#ifdef _DEBUG
		Debugger::GetInstance().Draw();
#endif
	}

	Game::DestroyInstance();
	LogManager::DestroyInstance();
	IniParser::DestroyInstance();
	SoundManager::DestroyInstance();

#ifdef _DEBUG
	Debugger::DestroyInstance();
#endif // _DEBUG


	return (0);
}
