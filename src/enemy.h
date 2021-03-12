#pragma once
#include "gameobject.h"
#include "graphics.h"
#include "enemybullet.h"


class Enemy : public GameObject, public Collidable
{
	float size = 40;
	float pos_x, pos_y;
	int x, y;
	float speed;
	float boss_life;
	bool boss = false;
	bool explosion = false;

public:
	bool checkReached();
	void update() override;
	void draw() override;
	void init(int x, int y) override;
	Enemy(const class Game& mygame, int x, int y);
	~Enemy();
	Disk getCollisionHull() const override;
	float getX() { return pos_x; };
	float getY() { return pos_y; };
	void swapDir() { speed *= -1; };
	void moveDown() { pos_y += 10; };
	float getSize() { return size; };
	void increaseSpeed() { speed *= 1.05; };
	bool checkBounce();
	void explode() { explosion = true; };
	//Boss
	void isBoss() { size = 60; speed = 5.0f + pos_y/100; boss_life = 2.0f; boss = true; }
	void drainLife(float amount) { boss_life = std::max<float>(0.0f, boss_life - amount); };
	float getRemainingLife() const { return boss_life; };
	bool checkBossBounce();
};