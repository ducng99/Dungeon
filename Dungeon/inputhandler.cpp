// COMP710 GP 2D Framework 2020

// This include:
#include "inputhandler.h"

// Local includes:
#include "game.h"
#include "player.h"
#include "logmanager.h"
#include "hud.h"
#include "debugger.h"

// Library includes:
#include <cassert>

InputHandler::InputHandler()
: m_pGameController(0)
{

}

InputHandler::~InputHandler()
{
	if (m_pGameController)
	{
		SDL_JoystickClose(m_pGameController);
		m_pGameController = 0;
	}
}

bool 
InputHandler::Initialise()
{
	int numControllesr = SDL_NumJoysticks();

	m_pGameController = SDL_JoystickOpen(0);

	if (!m_pGameController)
	{
		LogManager::GetInstance().Log("No controller detected!");
	}

	return (true);
}

void 
InputHandler::ProcessInput(Game& game)
{
	SDL_Event event;

	int mouseX = 0;
	int mouseY = 0;
	bool hudButtonClicked = false;
	Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);

	while (SDL_PollEvent(&event) != 0)
	{
#ifdef _DEBUG
		if (event.window.windowID == Debugger::GetInstance().GetWindowID())
		{
			Debugger::GetInstance().ProcessInput(&event);
		}
		else
#endif // _DEBUG
		{
			if (event.type == SDL_QUIT || (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE))
			{
				game.Quit();
			}
			else if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					game.Quit();
				}
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
			{
				hudButtonClicked = Hud::GetInstance().ButtonClick(mouseX, mouseY);
			}
		}
	}

	if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT))
	{
		if (!hudButtonClicked)
		{
			float* mousePos = new float[2]{ (float)mouseX, (float)mouseY };
			game.SpawnBullet(mousePos);

			delete[] mousePos;
			mousePos = 0;
		}
	}

	bool movingHorizontal = false, movingVertical = false;
	const Uint8* state = SDL_GetKeyboardState(NULL);

	if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A] || SDL_JoystickGetHat(m_pGameController, 0) == SDL_HAT_LEFT || SDL_JoystickGetAxis(m_pGameController, 0) < -7000)
	{
		movingHorizontal = true;
		game.GetPlayer()->MoveLeft();
	}

	if (state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D] || SDL_JoystickGetHat(m_pGameController, 0) == SDL_HAT_RIGHT || SDL_JoystickGetAxis(m_pGameController, 0) > 7000)
	{
		movingHorizontal = true;
		game.GetPlayer()->MoveRight();
	}

	if (state[SDL_SCANCODE_DOWN] || state[SDL_SCANCODE_S] || SDL_JoystickGetHat(m_pGameController, 0) == SDL_HAT_DOWN || SDL_JoystickGetAxis(m_pGameController, 1) > 7000)
	{
		movingVertical = true;
		game.GetPlayer()->MoveDown();
	}
	
	if (state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_W] || SDL_JoystickGetHat(m_pGameController, 0) == SDL_HAT_UP || SDL_JoystickGetAxis(m_pGameController, 1) < -7000)
	{
		movingVertical = true;
		game.GetPlayer()->MoveUp();
	}

	if (!movingHorizontal)
	{
		game.GetPlayer()->DecreaseHorizontalVelocity();
	}

	if (!movingVertical)
	{
		game.GetPlayer()->DecreaseVerticalVelocity();
	}
}
