#include "player.h"
#include "graphics.h"
#include "game.h"


Player::Player(const Game& mygame)
	: GameObject(mygame)
{
}

void Player::update()
{
	
	if (graphics::getKeyState(graphics::SCANCODE_A))
	{
		pos_x -= speed * graphics::getDeltaTime() / 15.f;
	}
	if (graphics::getKeyState(graphics::SCANCODE_D))
	{
		pos_x += speed * graphics::getDeltaTime() / 15.f;
	}
	if (graphics::getKeyState(graphics::SCANCODE_LEFT))
	{
		pos_x -= speed * graphics::getDeltaTime() / 15.f;
	}
	if (graphics::getKeyState(graphics::SCANCODE_RIGHT))
	{
		pos_x += speed * graphics::getDeltaTime() / 15.f;
	}

	if (!boss_time)
	{
		if (pos_x < 0) pos_x = 0;
		if (pos_x > CANVAS_WIDTH) pos_x = CANVAS_WIDTH;
	}
	else if(boss_time)
	{
		if (pos_x < CANVAS_WIDTH / 4 - 60) pos_x = CANVAS_WIDTH / 4 - 60;
		if (pos_x > CANVAS_WIDTH *3/4 + 60) pos_x = CANVAS_WIDTH * 3 / 4 + 60;
	}
	
}

void Player::draw()
{
	float glow = 0.5f + 0.5f * sinf(graphics::getGlobalTime()/10);

	graphics::Brush br;

	br.texture = std::string(ASSET_PATH) + "milleniumfalcon.png";
	br.outline_opacity = 0.0f;

	graphics::drawRect(pos_x, pos_y, size, size, br);

	br.texture= "";
	br.fill_color[0] = 1.0f;
	br.fill_color[1] = 0.5f + glow*0.5f;
	br.fill_color[2] = 0.0f;
	br.fill_secondary_color[0] = 0.3f;
	br.fill_secondary_color[1] = 0.1f;
	br.fill_secondary_color[2] = 0.0f;
	br.fill_opacity = 1.0f;
	br.fill_secondary_opacity = 0.0f;
	br.gradient = true;
	graphics::drawDisk(pos_x, pos_y+20, size/7.0f, br);

	if (game.getDebugMode())
	{
		br.outline_opacity = 1.0f;
		br.texture = "";
		br.fill_color[0] = 0.3f;
		br.fill_color[1] = 1.0f;
		br.fill_color[2] = 0.3f;
		br.fill_opacity = 0.3f;
		br.gradient = false;

		Disk hull = getCollisionHull();
		graphics::drawDisk(hull.cx, hull.cy, hull.radius, br);
	}
}

void Player::init(int x, int y)
{
}

Disk Player::getCollisionHull() const
{
	Disk disk;
	disk.cx = pos_x;
	disk.cy = pos_y-5;
	disk.radius = size / 2.0f;
	return disk;
}
