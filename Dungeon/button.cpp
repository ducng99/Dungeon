#include "button.h"
#include "backbuffer.h"
#include "sprite.h"
#include "hud.h"

Button::Button()
: m_pSprite(0)
{

}

Button::~Button()
{
	delete m_pSprite;
	m_pSprite = 0;
}

void Button::Initialise(Sprite * sprite, ButtonTypes type)
{
	m_pSprite = sprite;
	m_type = type;
}

void Button::Draw(BackBuffer * backBuffer)
{
	backBuffer->DrawSprite(*m_pSprite);
}

void Button::SetPosition(int x, int y)
{
	m_pSprite->SetX(x);
	m_pSprite->SetY(y);
}

int Button::GetX()
{
	return m_pSprite->GetX();
}

int Button::GetY()
{
	return m_pSprite->GetY();
}

int Button::GetWidth()
{
	return m_pSprite->GetWidth();
}

int Button::GetHeight()
{
	return m_pSprite->GetHeight();
}

ButtonTypes Button::GetType()
{
	return m_type;
}
