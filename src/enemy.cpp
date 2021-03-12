#include "enemy.h"
#include "graphics.h"
#include "config.h"
#include "util.h"
#include "game.h"

void Enemy::update()
{
	pos_x += 0.09f * speed * graphics::getDeltaTime();	

	if (boss)
	{
		boss_life = std::min<float>(2.0f, boss_life + 0.0005);
	}
}


void Enemy::draw()
{
	graphics::Brush brush;
	graphics::setOrientation(180.0f);
	brush.outline_opacity = 0.0f;
	float w;
	if (!boss)
	{
		brush.texture = std::string(ASSET_PATH) + "bad.png";
		w = size;
	}
	else
	{
		brush.texture = std::string(ASSET_PATH) + "boss.png";
		w = size + size * 0.02f;
	}
	graphics::drawRect(pos_x, pos_y, w, size, brush);
	graphics::resetPose();

	if (game.getDebugMode())
	{
		brush.outline_opacity = 1.0f;
		brush.texture = "";
		brush.fill_color[0] = 1.0f;
		brush.fill_color[1] = 0.3f;
		brush.fill_color[2] = 0.3f;
		brush.fill_opacity = 0.3f;
		brush.gradient = false;

		Disk hull = getCollisionHull();
		graphics::drawDisk(hull.cx, hull.cy, hull.radius, brush);
	}
}


void Enemy::init(int x, int y)
{
	speed = 1.0f;
	pos_x = x;
	pos_y = y;	
}

Enemy::Enemy(const Game& mygame, int x, int y)
	: GameObject(mygame)
{
	init(x, y);
}

Enemy::~Enemy()
{
}

Disk Enemy::getCollisionHull() const
{
	Disk disk;
	disk.cx = pos_x;
	disk.cy = pos_y;
	disk.radius = size / 2.0f;
	return disk;
}

bool Enemy::checkBounce()
{
	if (pos_x <= 10)
	{
		return true;
	}
	if (pos_x >= CANVAS_WIDTH - 10)
	{
		return true;
	}
	return false;
}

bool Enemy::checkBossBounce()
{
	if (pos_x <= CANVAS_WIDTH / 4 - size)
	{
		return true;
	}
	if (pos_x >= CANVAS_WIDTH * 3/4 + size)
	{
		return true;
	}
	return false;
}


bool Enemy::checkReached()
{
	if (pos_y >= CANVAS_HEIGHT - 110)
	{
		return true;
	}
	return false;
}
