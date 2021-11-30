#ifdef _DEBUG

#include <SDL.h>
#include "imgui.h"
#include "imgui_sdl.h"
#include "imgui_impl_sdl.h"

#include "debugger.h"
#include "game.h"
#include "map.h"
#include "player.h"

#include <cassert>

Debugger* Debugger::m_pInstance = 0;

Debugger::Debugger()
	: m_pWindow(0)
	, m_pRenderer(0)
	, m_windowID(0)
	, m_pGame(0)
{

}

Debugger::~Debugger()
{
	ImGui_ImplSDL2_Shutdown();
	ImGuiSDL::Deinitialize();

	ImGui::DestroyContext();

	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);

	m_pGame = 0;
}

Debugger & Debugger::GetInstance()
{
	if (m_pInstance == 0)
	{
		m_pInstance = new Debugger();
	}

	assert(m_pInstance);
	return *m_pInstance;
}

void Debugger::DestroyInstance()
{
	delete m_pInstance;
	m_pInstance = 0;
}

void Debugger::Initialize(Game* game)
{
	SDL_Init(SDL_INIT_VIDEO);
	m_pWindow = SDL_CreateWindow("Debugger", 480 + Game::GetInstance().GetWindowWidth(), SDL_WINDOWPOS_UNDEFINED, 490, 600, SDL_WINDOW_RESIZABLE);
	m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	m_windowID = SDL_GetWindowID(m_pWindow);

	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForMetal(m_pWindow);
	ImGuiSDL::Initialize(m_pRenderer, 490, 600);

	m_pGame = game;
}

Uint32 Debugger::GetWindowID()
{
	return m_windowID;
}

void Debugger::ProcessInput(SDL_Event* e)
{
	ImGui_ImplSDL2_ProcessEvent(e);
}

void Debugger::Draw()
{
	ImGui_ImplSDL2_NewFrame(m_pWindow);
	ImGui::NewFrame();

	// Player Debugger
	{
		ImGui::Begin("Player debug", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
		ImGui::SetWindowPos(ImVec2(10, 10));
		ImGui::SetWindowSize(ImVec2(470, 200));

		Player* player = m_pGame->GetPlayer();

		if (player != 0 && m_pGame->GetGameState() >= GameStates::MAIN_MENU)
		{
			float pHealth = player->GetHealth();
			ImGui::InputFloat("Health", &pHealth, 5, 10, "%.0f");
			player->SetHealth(pHealth);

			int pMoney = player->GetMoney();
			ImGui::InputInt("Money", &pMoney, 100, 1000);
			player->SetMoney(pMoney);

			ImGui::Text("-- Player stats --");

			float pDamage = player->GetDamage();
			ImGui::InputFloat("Damage", &pDamage, 1, 10, "%.2f");
			player->SetDamage(pDamage);

			float pAtkSpeed = 1.f / player->GetAttackSpeed();
			ImGui::InputFloat("Atk. Speed (hits/sec)", &pAtkSpeed, 0.02f, 0.1f, "%.2f");
			player->SetAttackSpeed(1.f / pAtkSpeed);

			float pMovSpeed = player->GetMovementSpeed();
			ImGui::InputFloat("Move Speed", &pMovSpeed, 1, 10, "%.1f");
			player->SetMovementSpeed(pMovSpeed);

			float pMaxHealth = player->GetMaxHealth();
			ImGui::InputFloat("Max Health", &pMaxHealth, 5, 10, "%.0f");
			player->SetMaxHealth(pMaxHealth);
		}
		else
		{
			ImGui::Text("Please enter a game to start debugging player");
		}

		ImGui::End();
	}

	// Game debugger
	{
		ImGui::Begin("Game debug", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
		ImGui::SetWindowPos(ImVec2(10, 220));
		ImGui::SetWindowSize(ImVec2(470, 320));

		if (m_pGame->GetGameState() == GameStates::IN_GAME)
		{
			if (ImGui::Button("Restart match"))
			{
				m_pGame->SetupNewGame();
			}
		}

		Map* map = m_pGame->GetMap();

		if (map != 0 && m_pGame->GetGameState() >= GameStates::MAIN_MENU)
		{
			int mLevel = map->GetMapLevel();
			ImGui::InputInt("Map Level", &mLevel);
			map->SetMapLevel(mLevel);
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text("Map level is only effective when starting new round.");
				ImGui::EndTooltip();
			}
		}
		else
		{
			ImGui::Text("Please enter a game to start debugging map");
		}

		ImGui::End();
	}

	ImGui::Render();

	SDL_SetRenderDrawColor(m_pRenderer, 0xe, 0xe, 0xe, 0xff);
	SDL_RenderClear(m_pRenderer);

	// There is a bug when moving windows, leaving a trail behind. Clearing renderer doesn't help, however resizing does
	ImGuiSDL::Render(ImGui::GetDrawData());

	SDL_RenderPresent(m_pRenderer);
}

#endif // _DEBUG