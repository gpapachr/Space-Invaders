#pragma once
#include "gameobject.h"
#include "config.h"


class Player : public GameObject, public Collidable
{
	float size = 50;
	float speed = 5.0f;
	float pos_x = CANVAS_WIDTH/2, pos_y = CANVAS_HEIGHT-60;
	float life = 1.0f;
	bool boss_time = false;
public:
	
	Player(const class Game& mygame);
	void update() override;
	void draw() override;
	void init(int x, int y) override;
	float getPosY() { return pos_y; }
	float getPosX() { return pos_x; }
	float getSize() { return size; };
	void fast() { speed = 10.0f; }
	void normal() { speed = 5.0f; };
	Disk getCollisionHull() const override;
	float getRemainingLife() const { return life; };
	void stop() { speed *= 0; };
	void drainLife(float amount) { life = std::max<float>(0.0f, life - amount); };
	void gainLife(float amount) { life = std::min<float>(1.0f, life + amount); };
	void itsBossTime() { boss_time = true; };

};
