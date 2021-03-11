#pragma once
#include "gameobject.h"
#include "config.h"
#include "graphics.h"

class EnemyBullet : public GameObject, public Collidable
{
	float size = 20;
	float bx, by;
	float speed = 1.5f;
	graphics::Brush b;
public:
	void update() override;
	void draw() override;
	void init(int x, int y) override;
	float getBulletY() { return by; };
	float getBulletSize() { return size; };
	void stop() { speed *= 0;};
	void isBoss() { speed = 1.5f; size = 40; };
	EnemyBullet(const class Game& mygame, int x, int y);
	~EnemyBullet();
	Disk getCollisionHull() const override;
};
