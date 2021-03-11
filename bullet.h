#pragma once
#include "gameobject.h"
#include "config.h"
#include "graphics.h"

class Bullet : public GameObject, public Collidable
{
	float size = 40;
	float bx, by;
	float speed = 1.0f;
	graphics::Brush b;
public:
	void update() override;
	void draw() override;
	void init(int x, int y) override;
	float getBulletY() { return by; };
	float getBulletSize() { return size; };
	void isBossTime() { speed = 2.0f; }
	Bullet(const class Game& mygame, int x, int y);
	~Bullet();
	Disk getCollisionHull() const override;
};