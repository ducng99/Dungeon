// COMP710 GP 2D Framework 2020

// This include:
#include "sprite.h"

// Local includes:
#include "backbuffer.h"
#include "texture.h"

#include <cstring>

Sprite::Sprite()
: m_pTexture(0)
, m_x(0)
, m_y(0)
, m_textureX(0)
, m_textureY(0)
, m_width(0)
, m_height(0)
{
	strcpy_s(leak_check, "Sprite");
}

Sprite::~Sprite()
{
	m_pTexture = 0;
}

bool 
Sprite::Initialise(Texture& texture, float scale)
{
	m_pTexture = &texture;
	
	m_width = static_cast<int>(roundf(m_pTexture->GetWidth() * scale));
	m_height = static_cast<int>(roundf(m_pTexture->GetHeight() * scale));

	return (true);
}

bool Sprite::Initialise(Texture & texture, int x, int y, int width, int height, float scale)
{
	m_pTexture = &texture;

	m_textureX = x;
	m_textureY = y;
	m_width = static_cast<int>(width * scale);
	m_height = static_cast<int>(height * scale);

	return (true);
}

void 
Sprite::Process(float deltaTime)
{

}

void 
Sprite::Draw(BackBuffer& backbuffer, float angle, bool flip)
{
	backbuffer.DrawSprite(*this, angle, flip);
}

void 
Sprite::SetX(int x)
{
	m_x = x;
}

void 
Sprite::SetY(int y)
{
	m_y = y;
}

int
Sprite::GetX() const
{
	return (m_x);
}

int
Sprite::GetY() const
{
	return (m_y);
}

int Sprite::GetTextureX() const
{
	return this->m_textureX;
}

int Sprite::GetTextureY() const
{
	return this->m_textureY;
}

Texture* 
Sprite::GetTexture()
{
	return (m_pTexture);
}

int 
Sprite::GetWidth() const
{
	return (m_width);
}

int
Sprite::GetHeight() const
{
	return (m_height);
}

void Sprite::SetWidth(int width)
{
	if (width > 0)
	{
		this->m_width = width;
	}
}

void Sprite::SetHeight(int height)
{
	if (height > 0)
	{
		this->m_height = height;
	}
}
