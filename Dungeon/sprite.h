// COMP710 GP 2D Framework 2020
#ifndef __SPRITE_H__
#define __SPRITE_H__

// Forward Declarations:
class BackBuffer;
class Texture;

class Sprite
{
	//Member Methods:
public:
	Sprite();
	~Sprite();

	bool Initialise(Texture& texture, float scale = 1.f);
	bool Initialise(Texture & texture, int x, int y, int width, int height, float scale = 1.f);

	void Process(float deltaTime);
	void Draw(BackBuffer& backbuffer, float angle = 0.f, bool flip = false);

	void SetX(int x);
	void SetY(int y);

	int GetX() const;
	int GetY() const;
	int GetTextureX() const;
	int GetTextureY() const;

	int GetWidth() const;
	int GetHeight() const;
	void SetWidth(int width);
	void SetHeight(int height);

	Texture* GetTexture();

protected:

private:
	Sprite(const Sprite& sprite);
	Sprite& operator=(const Sprite& sprite);

	//Member Data:
public:

protected:
	char leak_check[32];
	Texture* m_pTexture;
	int m_x;
	int m_y;
	int m_textureX;
	int m_textureY;

	int m_width;
	int m_height;

private:
};

#endif // __SPRITE_H__
