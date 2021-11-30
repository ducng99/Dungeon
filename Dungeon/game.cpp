// COMP710 GP 2D Framework 2020
#define RANDOM(min, max) rand() % (max - min + 1) + min
#define GET_SPLASH_ALPHA(curTime, maxTime) static_cast<unsigned char>((255 / powf(maxTime / 2, 2)) * powf(curTime - maxTime / 2, 2)) % 256

// This includes:
#include "game.h"

// Local includes:
#include "backbuffer.h"
#include "inputhandler.h"
#include "particlemanager.h"
#include "logmanager.h"
#include "sprite.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "map.h"
#include "hud.h"
#include "sound.h"
#include "particle.h"

// Library includes:
#include <cassert>
#include <SDL.h>
#include <cstdlib>

// Static Members:
Game* Game::sm_pInstance = 0;

Game&
Game::GetInstance()
{
	if (sm_pInstance == 0)
	{ 
		sm_pInstance = new Game();
	}

	assert(sm_pInstance);

	return (*sm_pInstance);
}

void 
Game::DestroyInstance()
{
	delete sm_pInstance; 
	sm_pInstance = 0;
}

Game::Game()
: m_pBackBuffer(0)
, m_pInputHandler(0)
, m_pParticleManager(0)
, m_looping(true)
, m_executionTime(0)
, m_elapsedSeconds(0)
, m_frameCount(0)
, m_FPS(0)
, m_numUpdates(0)
, m_lastTime(0)
, m_lag(0)
, windowHeight(960)
, windowWidth(960)
, m_gameState(GameStates::GAME_START)
, m_splashTimer(0)
{
	
}

Game::~Game()
{
	Hud::DestroyInstance();

	for each (Enemy* e in m_pEnemies)
	{
		delete e;
		e = 0;
	}
	m_pEnemies.clear();
	m_pEnemies.shrink_to_fit();
	Enemy::DestroySprites();

	for each (Bullet* b in m_pBullets)
	{
		delete b;
		b = 0;
	}
	m_pBullets.clear();
	m_pBullets.shrink_to_fit();
	Bullet::DestroySprites();

	for each (Particle* p in m_pParticles)
	{
		delete p;
		p = 0;
	}
	m_pParticles.clear();
	m_pParticles.shrink_to_fit();

	delete m_pMap;
	m_pMap = 0;
	Map::DestroySprites();

	delete m_pPlayer;
	m_pPlayer = 0;
	Player::DestroySprites();

	delete m_pAUTLogoSprite;
	m_pAUTLogoSprite = 0;

	delete m_pMWLogosSprite;
	m_pMWLogosSprite = 0;

	delete m_pBackBuffer;
	m_pBackBuffer = 0;

	delete m_pInputHandler;
	m_pInputHandler = 0;

	delete m_pParticleManager;
	m_pParticleManager = 0;
}

Player * Game::GetPlayer()
{
	return this->m_pPlayer;
}

Map * Game::GetMap()
{
	return this->m_pMap;
}

GameStates Game::GetGameState()
{
	return this->m_gameState;
}

void Game::SetGameState(GameStates state)
{
	this->m_gameState = state;
}

int Game::GetWindowWidth()
{
	return this->windowWidth;
}

int Game::GetWindowHeight()
{
	return this->windowHeight;
}

bool 
Game::Initialise()
{
	windowWidth = 960;
	windowHeight = 960;

	m_pBackBuffer = new BackBuffer();
	if (!m_pBackBuffer->Initialise(windowWidth, windowHeight))
	{
		LogManager::GetInstance().Log("BackBuffer Init Fail!");
		return (false);
	}

	m_pInputHandler = new InputHandler();
	if (!m_pInputHandler->Initialise())
	{
		LogManager::GetInstance().Log("InputHandler Init Fail!");
		return (false);
	}

	m_pParticleManager = new ParticleManager();
	if (m_pParticleManager == 0)
	{
		LogManager::GetInstance().Log("ParticleManager Init Fail!");
		return false;
	}

	Hud::InitialiseSprites(m_pBackBuffer);
	Hud::GetInstance().Initialise(m_pBackBuffer);

	m_lastTime = SDL_GetPerformanceCounter();
	m_lag = 0.0f;

	m_pBackBuffer->SetClearColour(0, 0, 0);

	m_pAUTLogoSprite = m_pBackBuffer->CreateSprite("assets\\imgs\\AUT-logo.jpg");
	m_pAUTLogoSprite->SetX(static_cast<int>(windowWidth / 2.f - m_pAUTLogoSprite->GetWidth() / 2.f));
	m_pAUTLogoSprite->SetY(static_cast<int>(windowHeight / 2.f - m_pAUTLogoSprite->GetHeight() / 2.f));

	m_pMWLogosSprite = m_pBackBuffer->CreateSprite("assets\\imgs\\mw.png");
	m_pMWLogosSprite->SetX(static_cast<int>(windowWidth / 2.f - m_pMWLogosSprite->GetWidth() / 2.f));
	m_pMWLogosSprite->SetY(static_cast<int>(windowHeight / 2.f - m_pMWLogosSprite->GetHeight() / 2.f));

	Map::InitialiseSprites(m_pBackBuffer);
	m_pMap = new Map(static_cast<int>(windowWidth / 32.f), static_cast<int>(windowHeight / 32.f));

	Player::InitializeSprites(m_pBackBuffer);
	Enemy::InitialiseSprites(m_pBackBuffer);
	Bullet::InitialiseSprites(m_pBackBuffer);

	m_pPlayer = new Player();

	SetupNewGame();

	return (true);
}

bool 
Game::DoGameLoop()
{
	const float stepSize = 1.0f / 60.0f;

	assert(m_pInputHandler);
	
	if (m_looping)
	{
		Uint64 current = SDL_GetPerformanceCounter();

		float deltaTime = (current - m_lastTime) / static_cast<float>(SDL_GetPerformanceFrequency());
		
		m_lastTime = current;

		m_executionTime += deltaTime;

		Process(deltaTime);
	
		m_lag += deltaTime;

		int innerLag = 0;

		while (m_lag >= stepSize)
		{
			Process(stepSize);
			m_pInputHandler->ProcessInput(*this);

			m_lag -= stepSize;

			++m_numUpdates;
			++innerLag;
		}

		Draw(*m_pBackBuffer);
	}

	return (m_looping);
}

void 
Game::Process(float deltaTime)
{
	// Count total simulation time elapsed:
	m_elapsedSeconds += deltaTime;

	// Frame Counter:
	if (m_elapsedSeconds > 1)
	{
		m_elapsedSeconds -= 1;
		m_FPS = m_frameCount;
		m_frameCount = 0;
	}

	switch (m_gameState)
	{
	case GameStates::GAME_START:
		m_gameState = GameStates::AUT_SPLASH_SCREEN;
		break;
	case GameStates::AUT_SPLASH_SCREEN:
		if (m_splashTimer < SPLASH_SHOW_TIME)
		{
			m_splashTimer += deltaTime;
		}
		else
		{
			m_splashTimer = 0;
			m_gameState = GameStates::MIDDLEWARE_SPLASH_SCREEN;
		}
		break;
	case GameStates::MIDDLEWARE_SPLASH_SCREEN:
		if (m_splashTimer < SPLASH_SHOW_TIME)
		{
			m_splashTimer += deltaTime;
		}
		else
		{
			m_splashTimer = 0;
			m_gameState = GameStates::MAIN_MENU;
		}
		break;
	case GameStates::MAIN_MENU:
		m_pBackBuffer->SetClearColour(0xE, 0xE, 0xE);
		break;
	case GameStates::IN_GAME:
		for (size_t i = 0; i < m_pBullets.size(); ++i)
		{
			m_pBullets[i]->Process(deltaTime);

			for (size_t n = 0; n < m_pEnemies.size() && !m_pBullets[i]->IsDead(); ++n)
			{
				if (m_pBullets[i]->IsCollidingWith(*m_pEnemies[n]))
				{
					SoundManager::GetInstance().PlaySound(Sounds::EXPLOSION, 0.1f);

					Particle* particle = m_pParticleManager->CreateParticle(m_pBackBuffer, ParticleTypes::EXPLOSION);
					particle->SetPosition(m_pEnemies[n]->GetPositionX(), m_pEnemies[n]->GetPositionY());
					m_pParticles.push_back(particle);

					m_pEnemies[n]->SetHealth(m_pEnemies[n]->GetHealth() - m_pPlayer->GetDamage());
					m_pBullets[i]->SetDead(true);
				}
			}

			if (m_pBullets[i]->IsDead() || m_pBullets[i]->GetPositionX() > windowWidth || m_pBullets[i]->GetPositionX() < 0 || m_pBullets[i]->GetPositionY() > windowHeight || m_pBullets[i]->GetPositionY() < 0)
			{
				delete m_pBullets[i];
				m_pBullets[i] = 0;
				m_pBullets.erase(m_pBullets.begin() + i);
			}
		}

		for (size_t i = 0; i < m_pEnemies.size(); ++i)
		{
			m_pEnemies[i]->MoveTo(m_pPlayer);
			m_pEnemies[i]->Process(deltaTime);
			m_pEnemies[i]->BoundaryDetect(windowWidth, windowHeight);
			if (m_pEnemies[i]->Attack(m_pPlayer))
			{
				SoundManager::GetInstance().PlaySound(Sounds::HIT);

				Particle* particle = m_pParticleManager->CreateParticle(m_pBackBuffer, ParticleTypes::HIT);
				particle->SetPosition(m_pPlayer->GetPositionX(), m_pPlayer->GetPositionY());
				m_pParticles.push_back(particle);
			}

			if (m_pEnemies[i]->IsDead())
			{
				SoundManager::GetInstance().PlaySound(Sounds::DIE);
				m_pPlayer->SetMoney(m_pPlayer->GetMoney() + m_pEnemies[i]->GetKillPrize());
				m_pPlayer->SetKillCount(m_pPlayer->GetKillCount() + 1);

				delete m_pEnemies[i];
				m_pEnemies[i] = 0;
				m_pEnemies.erase(m_pEnemies.begin() + i);
			}
		}

		for (size_t i = 0; i < m_pParticles.size(); ++i)
		{
			m_pParticles[i]->Process(deltaTime);

			if (m_pParticles[i]->IsDead())
			{
				delete m_pParticles[i];
				m_pParticles[i] = 0;

				m_pParticles.erase(m_pParticles.begin() + i);
			}
		}

		m_pPlayer->Process(deltaTime);
		m_pPlayer->BoundaryDetect(windowWidth, windowHeight);

		if (m_pEnemies.size() == 0)
		{
			m_pMap->IncreaseLevel();
			SetGameState(GameStates::RESULT_SCREEN);
		}
		else if (m_pPlayer->IsDead())
		{
			SetGameState(GameStates::RESULT_SCREEN);
		}
		break;
	case GameStates::RESULT_SCREEN:
		break;
	case GameStates::END_GAME:
		Quit();
		break;
	default:
		break;
	}
}

void 
Game::Draw(BackBuffer& backBuffer)
{
	++m_frameCount;

	backBuffer.Clear();

	switch (m_gameState)
	{
	case GameStates::AUT_SPLASH_SCREEN:
		backBuffer.DrawSprite(*m_pAUTLogoSprite);												// Draw AUT logo
		backBuffer.SetDrawColour(0, 0, 0, GET_SPLASH_ALPHA(m_splashTimer, SPLASH_SHOW_TIME));	// Set cover black colour with varying alpha value (parabola equation)
		backBuffer.DrawRectangle(0, 0, windowWidth, windowHeight);								// Draw cover
		break;
	case GameStates::MIDDLEWARE_SPLASH_SCREEN:
		backBuffer.DrawSprite(*m_pMWLogosSprite);	// Draw middlewares' logos
		backBuffer.SetDrawColour(0, 0, 0, GET_SPLASH_ALPHA(m_splashTimer, SPLASH_SHOW_TIME));
		backBuffer.DrawRectangle(0, 0, windowWidth, windowHeight);
		break;
	case GameStates::MAIN_MENU:
		Hud::GetInstance().DrawMainMenu();
		Hud::GetInstance().DrawInstructions();
		break;
	case GameStates::POWER_UPS:
		Hud::GetInstance().DrawPowerUp(m_pPlayer);
		break;
	case GameStates::IN_GAME:
		m_pMap->Draw(&backBuffer);

		for each (Enemy* enemy in m_pEnemies)
		{
			enemy->Draw(backBuffer);
		}

		for each (Bullet* bullet in m_pBullets)
		{
			bullet->Draw(backBuffer);
		}

		for each (Particle* particle in m_pParticles)
		{
			particle->Draw(backBuffer);
		}

		m_pPlayer->Draw(backBuffer);

		Hud::GetInstance().DrawIngame(m_pPlayer);
		Hud::GetInstance().DrawInstructions();
		break;
	case GameStates::RESULT_SCREEN:
		Hud::GetInstance().DrawResult(m_pPlayer);
		Hud::GetInstance().DrawInstructions();
		break;
	default:
		break;
	}
	
	backBuffer.Present();
}

void 
Game::Quit()
{
	m_looping = false;
}

void Game::SetupNewGame()
{
	m_pPlayer->SetKillCount(0);
	m_pPlayer->SetPosition(windowWidth / 2.f, windowHeight / 2.f);
	m_pPlayer->SetHealth(m_pPlayer->GetMaxHealth());
	m_pPlayer->SetDead(false);
	m_pMap->InitializeNewMap();

	for each (Enemy* e in m_pEnemies)
	{
		delete e;
		e = 0;
	}
	m_pEnemies.clear();
	m_pEnemies.shrink_to_fit();

	for (int i = 0; i < 10; ++i)
	{
		Enemy* enemy = new Enemy(100 + m_pMap->GetMapLevel() * 20);
		enemy->SetHealth(enemy->GetHealth() + m_pMap->GetMapLevel() * 10);
		enemy->SetDamage(enemy->GetDamage() + m_pMap->GetMapLevel() * 4);
		enemy->SetPosition(static_cast<float>(RANDOM(50, 900)), static_cast<float>(RANDOM(50, 900)));

		m_pEnemies.push_back(enemy);
	}

	for each (Bullet* b in m_pBullets)
	{
		delete b;
		b = 0;
	}
	m_pBullets.clear();
	m_pBullets.shrink_to_fit();
}

void Game::SpawnBullet(float dest[2])
{
	if (m_pPlayer->IsAttackable())
	{
		Bullet* bullet = new Bullet();
		float* playerPos = new float[2]{ m_pPlayer->GetPositionX(), m_pPlayer->GetPositionY() };

		bullet->SetupBullet(playerPos, dest);

		m_pBullets.push_back(bullet);

		delete[] playerPos;
		playerPos = 0;
	}
}