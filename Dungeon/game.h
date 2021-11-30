// COMP710 GP 2D Framework 2020
#ifndef __GAME_H__
#define __GAME_H__

#include <vector>

// Forward Declarations
class BackBuffer;
class InputHandler;
class ParticleManager;
class Sprite;
class Player;
class Enemy;
class Bullet;
class Map;
class Particle;

enum class GameStates
{
	GAME_START, AUT_SPLASH_SCREEN, MIDDLEWARE_SPLASH_SCREEN, MAIN_MENU, POWER_UPS, IN_GAME, RESULT_SCREEN, END_GAME
};

class Game
{
	//Member Methods:
public:
	static Game& GetInstance();
	static void DestroyInstance();
	~Game();

	bool Initialise();
	bool DoGameLoop();
	void Quit();

	void SpawnBullet(float dest[2]);
	Player* GetPlayer();
	Map* GetMap();

	void SetupNewGame();
	GameStates GetGameState();
	void SetGameState(GameStates state);

	int GetWindowWidth();
	int GetWindowHeight();
protected:
	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

private:
	Game(const Game& game);
	Game& operator=(const Game& game);
	
	Game();

	//Member Data:
public:

protected:
	static Game* sm_pInstance;
	BackBuffer* m_pBackBuffer;
	InputHandler* m_pInputHandler;
	ParticleManager* m_pParticleManager;
	bool m_looping;

	// Simulation Counters:
	float m_elapsedSeconds;
	float m_lag;
	float m_executionTime;
	__int64 m_lastTime;
	int m_frameCount;
	int m_FPS;
	int m_numUpdates;
	bool m_drawDebugInfo;
private:
	int windowWidth;
	int windowHeight;

	Map* m_pMap;

	const float SPLASH_SHOW_TIME = 4;
	GameStates m_gameState;

	float m_splashTimer;
	Sprite* m_pAUTLogoSprite;
	Sprite* m_pMWLogosSprite;

	Player* m_pPlayer;
	std::vector<Enemy*> m_pEnemies;
	std::vector<Bullet*> m_pBullets;
	std::vector<Particle*> m_pParticles;
};

#endif // __GAME_H__
