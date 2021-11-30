#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "entity.h"
#include "hud.h"

class Button
{
public:
	Button();
	~Button();

	void Initialise(Sprite* sprite, ButtonTypes type);
	void Draw(BackBuffer* backBuffer);

	void SetPosition(int x, int y);
	int GetX();
	int GetY();
	int GetWidth();
	int GetHeight();
	ButtonTypes GetType();
private:
	Button(const Button& hud);
	Button& operator=(const Button& Button);

private:
	Sprite* m_pSprite;
	ButtonTypes m_type;
};

#endif // !__BUTTON_H__