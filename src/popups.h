#pragma once
#include "gameobject.h"

class Popups : public GameObject, public Collidable
{
	float size = 40;
	float px, py;
	float speed = 0.2f;
	int mode;
	
public:
	void choosePopUp(int p) { mode = p; };
	void update() override;
	void draw() override;
	void init(int x, int y) override;
	float getX() { return px; };
	float getSize() { return size; };
	Popups(const class Game& mygame, float x, float y);
	~Popups();
	Disk getCollisionHull() const;
};
