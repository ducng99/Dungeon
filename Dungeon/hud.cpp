#include "hud.h"
#include "backbuffer.h"
#include "sprite.h"
#include "game.h"
#include "button.h"
#include "player.h"
#include "texture.h"

#include <string>
#include <cassert>

using namespace std;

map<ButtonTypes, GameStates> Hud::sm_buttonAction;
Sprite* Hud::sm_pBackgroundSprite = 0;
Sprite* Hud::sm_pResultPanelSprite = 0;
Sprite* Hud::sm_pInstructionSprite = 0;
Sprite* Hud::sm_pHealthBarSprite = 0;
Sprite* Hud::sm_pHealthBarInnerSprite = 0;
Sprite* Hud::sm_pPowerUpBGSprite = 0;
Hud* Hud::sm_pInstance = 0;

Hud::Hud()
: m_bShowInstructions(false)
{

}

Hud::~Hud()
{
	m_pBackBuffer = 0;

	delete sm_pBackgroundSprite;
	sm_pBackgroundSprite = 0;
	delete sm_pResultPanelSprite;
	sm_pResultPanelSprite = 0;
	delete sm_pInstructionSprite;
	sm_pInstructionSprite = 0;
	delete sm_pHealthBarSprite;
	sm_pHealthBarSprite = 0;
	delete sm_pHealthBarInnerSprite;
	sm_pHealthBarInnerSprite = 0;
	delete sm_pPowerUpBGSprite;
	sm_pPowerUpBGSprite = 0;

	std::map<Buttons, Button*>::iterator it = m_pButtons.begin();

	while (it != m_pButtons.end())
	{
		delete it->second;
		it->second = 0;

		it++;
	}

	m_pButtons.clear();
}

Hud & Hud::GetInstance()
{
	if (sm_pInstance == 0)
	{
		sm_pInstance = new Hud();
	}

	assert(sm_pInstance);
	return *sm_pInstance;
}

void Hud::DestroyInstance()
{
	delete sm_pInstance;
	sm_pInstance = 0;
}

void Hud::InitialiseSprites(BackBuffer* backBuffer)
{
	sm_pBackgroundSprite = backBuffer->CreateSprite("assets\\imgs\\hud\\bg.png");

	sm_pResultPanelSprite = backBuffer->CreateSprite("assets\\imgs\\hud\\resultPanel.png");
	sm_pResultPanelSprite->SetX(static_cast<int>(Game::GetInstance().GetWindowWidth() / 2.f - sm_pResultPanelSprite->GetWidth() / 2.f));
	sm_pResultPanelSprite->SetY(180);

	sm_pInstructionSprite = backBuffer->CreateSprite("assets\\imgs\\hud\\instructions.png");
	sm_pInstructionSprite->SetX(static_cast<int>(Game::GetInstance().GetWindowWidth() / 2.f - sm_pInstructionSprite->GetWidth() / 2.f));
	sm_pInstructionSprite->SetY(static_cast<int>(Game::GetInstance().GetWindowHeight() / 2.f - sm_pInstructionSprite->GetHeight() / 2.f));

	sm_pHealthBarSprite = backBuffer->CreateSprite("assets\\imgs\\hud\\healthbar.png");
	sm_pHealthBarSprite->SetX(10);
	sm_pHealthBarSprite->SetY(Game::GetInstance().GetWindowHeight() - 10 - sm_pHealthBarSprite->GetHeight());

	sm_pHealthBarInnerSprite = backBuffer->CreateSprite("assets\\imgs\\hud\\healthbar-inner.png");
	sm_pHealthBarInnerSprite->SetX(10);
	sm_pHealthBarInnerSprite->SetY(static_cast<int>(roundf(sm_pHealthBarSprite->GetY() + sm_pHealthBarSprite->GetHeight() / 2.f - sm_pHealthBarInnerSprite->GetHeight() / 2.f)));

	sm_pPowerUpBGSprite = backBuffer->CreateSprite("assets\\imgs\\hud\\powerupBG.png");
	sm_pPowerUpBGSprite->SetX(static_cast<int>(Game::GetInstance().GetWindowWidth() / 2.f - sm_pPowerUpBGSprite->GetWidth() / 2.f));
	sm_pPowerUpBGSprite->SetY(static_cast<int>(Game::GetInstance().GetWindowHeight() / 2.f - sm_pPowerUpBGSprite->GetHeight() / 2.f));

	sm_buttonAction[ButtonTypes::PLAY] = GameStates::IN_GAME;
	sm_buttonAction[ButtonTypes::STOP_PLAYING] = GameStates::RESULT_SCREEN;
	sm_buttonAction[ButtonTypes::CLOSE_TO_MENU] = GameStates::MAIN_MENU;
	sm_buttonAction[ButtonTypes::CLOSE_GAME] = GameStates::END_GAME;
	sm_buttonAction[ButtonTypes::POWERUP_WINDOW] = GameStates::POWER_UPS;
}

void Hud::Initialise(BackBuffer * backBuffer)
{
	m_pBackBuffer = backBuffer;

	m_pButtons[Buttons::MainMenuPlayButton] = new Button();
	m_pButtons[Buttons::MainMenuPlayButton]->Initialise(backBuffer->CreateSprite("assets\\imgs\\hud\\playButton.png"), ButtonTypes::PLAY);
	m_pButtons[Buttons::MainMenuPlayButton]->SetPosition(static_cast<int>(Game::GetInstance().GetWindowWidth() / 2.f - m_pButtons[Buttons::MainMenuPlayButton]->GetWidth() - 10), static_cast<int>(Game::GetInstance().GetWindowHeight() / 2.f - m_pButtons[Buttons::MainMenuPlayButton]->GetHeight() / 2.f));

	m_pButtons[Buttons::MainMenuCloseButton] = new Button();
	m_pButtons[Buttons::MainMenuCloseButton]->Initialise(backBuffer->CreateSprite("assets\\imgs\\hud\\closeButton.png"), ButtonTypes::CLOSE_GAME);

	m_pButtons[Buttons::MainMenuPowerupButton] = new Button();
	m_pButtons[Buttons::MainMenuPowerupButton]->Initialise(backBuffer->CreateSprite("assets\\imgs\\hud\\powerupButton.png"), ButtonTypes::POWERUP_WINDOW);
	m_pButtons[Buttons::MainMenuPowerupButton]->SetPosition(static_cast<int>(Game::GetInstance().GetWindowWidth() / 2.f + 10), static_cast<int>(Game::GetInstance().GetWindowHeight() / 2.f - m_pButtons[Buttons::MainMenuPlayButton]->GetHeight() / 2.f));

	m_pButtons[Buttons::IngameEndButton] = new Button();
	m_pButtons[Buttons::IngameEndButton]->Initialise(backBuffer->CreateSprite("assets\\imgs\\hud\\closeButton.png"), ButtonTypes::STOP_PLAYING);

	m_pButtons[Buttons::ResultPlayButton] = new Button();
	m_pButtons[Buttons::ResultPlayButton]->Initialise(backBuffer->CreateSprite("assets\\imgs\\hud\\playButton.png"), ButtonTypes::PLAY);
	m_pButtons[Buttons::ResultPlayButton]->SetPosition(static_cast<int>(Game::GetInstance().GetWindowWidth() / 2.f - m_pButtons[Buttons::ResultPlayButton]->GetWidth() - 10), static_cast<int>(Game::GetInstance().GetWindowHeight() / 2.f + 200));

	m_pButtons[Buttons::ResultCloseButton] = new Button();
	m_pButtons[Buttons::ResultCloseButton]->Initialise(backBuffer->CreateSprite("assets\\imgs\\hud\\closeButton.png"), ButtonTypes::CLOSE_TO_MENU);

	m_pButtons[Buttons::ResultPowerupButton] = new Button();
	m_pButtons[Buttons::ResultPowerupButton]->Initialise(backBuffer->CreateSprite("assets\\imgs\\hud\\powerupButton.png"), ButtonTypes::POWERUP_WINDOW);
	m_pButtons[Buttons::ResultPowerupButton]->SetPosition(static_cast<int>(Game::GetInstance().GetWindowWidth() / 2.f + 10), static_cast<int>(Game::GetInstance().GetWindowHeight() / 2.f + 200));

	m_pButtons[Buttons::HelpButton] = new Button();
	m_pButtons[Buttons::HelpButton]->Initialise(backBuffer->CreateSprite("assets\\imgs\\hud\\helpButton.png"), ButtonTypes::HELP);
	m_pButtons[Buttons::HelpButton]->SetPosition(Game::GetInstance().GetWindowWidth() - m_pButtons[Buttons::HelpButton]->GetWidth(), 0);

	m_pButtons[Buttons::PowerupCloseButton] = new Button();
	m_pButtons[Buttons::PowerupCloseButton]->Initialise(backBuffer->CreateSprite("assets\\imgs\\hud\\closeButton.png"), ButtonTypes::CLOSE_TO_MENU);

	m_pButtons[Buttons::Powerup_Damage] = new Button();
	m_pButtons[Buttons::Powerup_Damage]->Initialise(backBuffer->CreateSprite("assets\\imgs\\hud\\powerup_damageButton.png"), ButtonTypes::POWERUP);
	m_pButtons[Buttons::Powerup_Damage]->SetPosition(sm_pPowerUpBGSprite->GetX() + 92, sm_pPowerUpBGSprite->GetY() + 400);

	m_pButtons[Buttons::Powerup_AtkSpeed] = new Button();
	m_pButtons[Buttons::Powerup_AtkSpeed]->Initialise(backBuffer->CreateSprite("assets\\imgs\\hud\\powerup_atkSpeedButton.png"), ButtonTypes::POWERUP);
	m_pButtons[Buttons::Powerup_AtkSpeed]->SetPosition(m_pButtons[Buttons::Powerup_Damage]->GetX() + m_pButtons[Buttons::Powerup_Damage]->GetWidth() + 14, sm_pPowerUpBGSprite->GetY() + 400);

	m_pButtons[Buttons::Powerup_MoveSpeed] = new Button();
	m_pButtons[Buttons::Powerup_MoveSpeed]->Initialise(backBuffer->CreateSprite("assets\\imgs\\hud\\powerup_moveSpeedButton.png"), ButtonTypes::POWERUP);
	m_pButtons[Buttons::Powerup_MoveSpeed]->SetPosition(m_pButtons[Buttons::Powerup_AtkSpeed]->GetX() + m_pButtons[Buttons::Powerup_AtkSpeed]->GetWidth() + 14, sm_pPowerUpBGSprite->GetY() + 400);

	m_pButtons[Buttons::Powerup_Health] = new Button();
	m_pButtons[Buttons::Powerup_Health]->Initialise(backBuffer->CreateSprite("assets\\imgs\\hud\\powerup_HPButton.png"), ButtonTypes::POWERUP);
	m_pButtons[Buttons::Powerup_Health]->SetPosition(m_pButtons[Buttons::Powerup_MoveSpeed]->GetX() + m_pButtons[Buttons::Powerup_MoveSpeed]->GetWidth() + 14, sm_pPowerUpBGSprite->GetY() + 400);
}

void Hud::DrawMainMenu()
{
	m_pBackBuffer->DrawSprite(*sm_pBackgroundSprite);

	m_pButtons[Buttons::MainMenuPlayButton]->Draw(m_pBackBuffer);
	m_pButtons[Buttons::MainMenuCloseButton]->Draw(m_pBackBuffer);
	m_pButtons[Buttons::MainMenuPowerupButton]->Draw(m_pBackBuffer);
	m_pButtons[Buttons::HelpButton]->Draw(m_pBackBuffer);
}

void Hud::DrawIngame(Player* player)
{
	sm_pHealthBarInnerSprite->SetWidth(static_cast<int>(roundf(sm_pHealthBarInnerSprite->GetTexture()->GetWidth() * player->GetHealthRatio())));
	m_pBackBuffer->DrawSprite(*sm_pHealthBarInnerSprite);

	m_pBackBuffer->DrawSprite(*sm_pHealthBarSprite);

	m_pButtons[Buttons::IngameEndButton]->Draw(m_pBackBuffer);
	m_pButtons[Buttons::HelpButton]->Draw(m_pBackBuffer);
}

void Hud::DrawResult(Player* player)
{
	m_pBackBuffer->DrawSprite(*sm_pBackgroundSprite);

	m_pBackBuffer->DrawSprite(*sm_pResultPanelSprite);

	m_pBackBuffer->SetTextColour(0xE, 0xE, 0xE);
	m_pBackBuffer->DrawText(static_cast<int>(Game::GetInstance().GetWindowWidth() / 2.f - 200), 290, ("Money: " + to_string(player->GetMoney())).c_str());
	m_pBackBuffer->DrawText(static_cast<int>(Game::GetInstance().GetWindowWidth() / 2.f - 200), 330, ("Kill counter: " + to_string(player->GetKillCount())).c_str());

	m_pButtons[Buttons::ResultPlayButton]->Draw(m_pBackBuffer);
	m_pButtons[Buttons::ResultPowerupButton]->Draw(m_pBackBuffer);
	m_pButtons[Buttons::ResultCloseButton]->Draw(m_pBackBuffer);
	m_pButtons[Buttons::HelpButton]->Draw(m_pBackBuffer);
}

void Hud::DrawPowerUp(Player * player)
{
	m_pBackBuffer->DrawSprite(*sm_pBackgroundSprite);
	m_pBackBuffer->DrawSprite(*sm_pPowerUpBGSprite);

	m_pBackBuffer->SetTextColour(0xE, 0xE, 0xE);
	int textX = static_cast<int>(sm_pPowerUpBGSprite->GetX() + sm_pPowerUpBGSprite->GetWidth() / 2.f - 50);
	m_pBackBuffer->DrawText(textX, sm_pPowerUpBGSprite->GetY() + 90, "Power Ups!");

	char msg[50];
	sprintf_s(msg, "Damage: %.2f", player->GetDamage());
	m_pBackBuffer->DrawText(textX, sm_pPowerUpBGSprite->GetY() + 130, msg);
	sprintf_s(msg, "Attack speed: %.2f hits/sec", 1.f / player->GetAttackSpeed());
	m_pBackBuffer->DrawText(textX, sm_pPowerUpBGSprite->GetY() + 160, msg);
	sprintf_s(msg, "Movement speed: %.1f", player->GetMovementSpeed());
	m_pBackBuffer->DrawText(textX, sm_pPowerUpBGSprite->GetY() + 190, msg);
	sprintf_s(msg, "Max Health: %.0f", player->GetMaxHealth());
	m_pBackBuffer->DrawText(textX, sm_pPowerUpBGSprite->GetY() + 220, msg);

	sprintf_s(msg, "Money: %d coins", player->GetMoney());
	m_pBackBuffer->DrawText(textX, sm_pPowerUpBGSprite->GetY() + 260, msg);

	// Damage increase cost
	sprintf_s(msg, "Cost: %dc", static_cast<unsigned int>(player->GetDamage() * 40));
	m_pBackBuffer->DrawText(sm_pPowerUpBGSprite->GetX() + 110, sm_pPowerUpBGSprite->GetY() + 350, msg);

	// Atk. speed increase cost
	sprintf_s(msg, "Cost: %dc", static_cast<unsigned int>(powf(1.f / player->GetAttackSpeed(), 16)));
	m_pBackBuffer->DrawText(sm_pPowerUpBGSprite->GetX() + 300, sm_pPowerUpBGSprite->GetY() + 350, msg);

	// Move speed increase cost
	sprintf_s(msg, "Cost: %dc", static_cast<unsigned int>(player->GetMovementSpeed() * 400 + powf(player->GetMovementSpeed(), 9)));
	m_pBackBuffer->DrawText(sm_pPowerUpBGSprite->GetX() + 490, sm_pPowerUpBGSprite->GetY() + 350, msg);

	// Health increase cost
	sprintf_s(msg, "Cost: %dc", static_cast<unsigned int>(player->GetMaxHealth() * 4.2f));
	m_pBackBuffer->DrawText(sm_pPowerUpBGSprite->GetX() + 680, sm_pPowerUpBGSprite->GetY() + 350, msg);

	m_pButtons[Buttons::PowerupCloseButton]->Draw(m_pBackBuffer);
	m_pButtons[Buttons::Powerup_Damage]->Draw(m_pBackBuffer);
	m_pButtons[Buttons::Powerup_AtkSpeed]->Draw(m_pBackBuffer);
	m_pButtons[Buttons::Powerup_MoveSpeed]->Draw(m_pBackBuffer);
	m_pButtons[Buttons::Powerup_Health]->Draw(m_pBackBuffer);
}

void Hud::DrawInstructions()
{
	if (m_bShowInstructions)
	{
		m_pBackBuffer->DrawSprite(*sm_pInstructionSprite);
	}
}

bool Hud::ButtonClick(int x, int y)
{
	bool clicked = false;

	std::map<Buttons, Button*>::iterator it = m_pButtons.begin();

	while (it != m_pButtons.end() && !clicked)
	{
		bool buttonShowing = false;

		// Check if button is allowed in each game state
		switch (Game::GetInstance().GetGameState())
		{
		case GameStates::MAIN_MENU:
			switch (it->first)
			{
			case Buttons::MainMenuCloseButton:
			case Buttons::MainMenuPlayButton:
			case Buttons::MainMenuPowerupButton:
			case Buttons::HelpButton:
				buttonShowing = true;
				break;
			default:
				break;
			}
			break;
		case GameStates::POWER_UPS:
			switch (it->first)
			{
			case Buttons::PowerupCloseButton:
			case Buttons::Powerup_Damage:
			case Buttons::Powerup_AtkSpeed:
			case Buttons::Powerup_MoveSpeed:
			case Buttons::Powerup_Health:
				buttonShowing = true;
				break;
			default:
				break;
			}
			break;
		case GameStates::IN_GAME:
			switch (it->first)
			{
			case Buttons::IngameEndButton:
			case Buttons::HelpButton:
				buttonShowing = true;
				break;
			default:
				break;
			}
			break;
		case GameStates::RESULT_SCREEN:
			switch (it->first)
			{
			case Buttons::ResultPlayButton:
			case Buttons::ResultCloseButton:
			case Buttons::ResultPowerupButton:
			case Buttons::HelpButton:
				buttonShowing = true;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}

		if (buttonShowing)
		{
			if (IsMouseOverButton(x, y, it->second))
			{
				Player* player = Game::GetInstance().GetPlayer();

				switch (it->first)
				{
				case Buttons::HelpButton:
					m_bShowInstructions = !m_bShowInstructions;
					break;
				case Buttons::Powerup_Damage:
					if (player->GetMoney() >= static_cast<unsigned int>(player->GetDamage() * 40))
					{
						player->SetMoney(player->GetMoney() - static_cast<unsigned int>(player->GetDamage() * 40));
						player->SetDamage(player->GetDamage() + 5);
					}
					break;
				case Buttons::Powerup_AtkSpeed:
					if (player->GetMoney() >= static_cast<unsigned int>(powf(1.f / player->GetAttackSpeed(), 16)))
					{
						player->SetMoney(player->GetMoney() - static_cast<unsigned int>(powf(1.f / player->GetAttackSpeed(), 16)));
						player->SetAttackSpeed(player->GetAttackSpeed() - 0.01f);
					}
					break;
				case Buttons::Powerup_MoveSpeed:
					if (player->GetMoney() >= static_cast<unsigned int>(player->GetMovementSpeed() * 400 + powf(player->GetMovementSpeed(), 9)))
					{
						player->SetMoney(player->GetMoney() - static_cast<unsigned int>(player->GetMovementSpeed() * 400 + powf(player->GetMovementSpeed(), 9)));
						player->SetMovementSpeed(player->GetMovementSpeed() + 0.1f);
					}
					break;
				case Buttons::Powerup_Health:
					if (player->GetMoney() >= static_cast<unsigned int>(player->GetMaxHealth() * 4.2f))
					{
						player->SetMoney(player->GetMoney() - static_cast<unsigned int>(player->GetMaxHealth() * 4.2f));
						player->SetMaxHealth(player->GetMaxHealth() + 5);
					}
					break;
				default:
					if (sm_buttonAction[it->second->GetType()] == GameStates::IN_GAME)
					{
						Game::GetInstance().SetupNewGame();
					}

					Game::GetInstance().SetGameState(sm_buttonAction[it->second->GetType()]);
					break;
				}

				clicked = true;
			}
		}

		it++;
	}

	return clicked;
}

bool Hud::IsMouseOverButton(int x, int y, Button * button)
{
	if (x >= button->GetX() && x <= button->GetX() + button->GetWidth() && y >= button->GetY() && y <= button->GetY() + button->GetHeight())
	{
		return true;
	}

	return false;
}