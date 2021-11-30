#ifndef __HUD_H__
#define __HUD_H__

#include "game.h"
#include <map>

class Player;
class Button;

enum class ButtonTypes
{
	PLAY, CLOSE_GAME, CLOSE_TO_MENU, STOP_PLAYING, HELP, POWERUP, POWERUP_WINDOW
};

enum class Buttons
{
	MainMenuPlayButton, MainMenuCloseButton, MainMenuPowerupButton,
	IngameEndButton, HelpButton,
	ResultPlayButton, ResultCloseButton, ResultPowerupButton,
	PowerupCloseButton,
	Powerup_Health, Powerup_Damage, Powerup_AtkSpeed, Powerup_MoveSpeed
};

class Hud
{
public:
	~Hud();

	static Hud& GetInstance();
	static void DestroyInstance();
	static void InitialiseSprites(BackBuffer* backBuffer);

	void Initialise(BackBuffer* backBuffer);

	void DrawMainMenu();
	void DrawIngame(Player* player);
	void DrawResult(Player* player);
	void DrawPowerUp(Player* player);
	void DrawInstructions();

	bool ButtonClick(int x, int y);

private:
	Hud();
	Hud(const Hud& hud);
	Hud& operator=(const Hud& hud);

	bool IsMouseOverButton(int x, int y, Button* button);

private:
	BackBuffer* m_pBackBuffer;
	static std::map<ButtonTypes, GameStates> sm_buttonAction;

	static Sprite* sm_pBackgroundSprite;
	static Sprite* sm_pResultPanelSprite;
	static Sprite* sm_pInstructionSprite;
	static Sprite* sm_pHealthBarSprite;
	static Sprite* sm_pHealthBarInnerSprite;
	static Sprite* sm_pPowerUpBGSprite;

	static Hud* sm_pInstance;

	bool m_bShowInstructions;
	std::map<Buttons, Button*> m_pButtons;
};

#endif // !__HUD_H__