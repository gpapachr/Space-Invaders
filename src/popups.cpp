#include "popups.h"
#include "graphics.h"
#include "config.h"
#include "game.h"

void Popups::update()
{
    px -= speed * graphics::getDeltaTime();
}

void Popups::draw()
{
    graphics::Brush b1;
    b1.texture = std::string(ASSET_PATH) + "powerup.png";
    b1.outline_opacity = 0.0f;
    graphics::drawRect(px, py, size, size, b1);

    if (game.getDebugMode())
    {
        b1.outline_opacity = 1.0f;
        b1.texture = "";
        b1.fill_color[0] = 0.3f;
        b1.fill_color[1] = 1.0f;
        b1.fill_color[2] = 0.3f;
        b1.fill_opacity = 0.3f;
        b1.gradient = false;

        Disk hull = getCollisionHull();
        graphics::drawDisk(hull.cx, hull.cy, hull.radius, b1);
    }
    b1.texture = "";
        
}

void Popups::init(int x, int y)
{
    px = x;
    py = y;
}

Popups::Popups(const Game& mygame, float x, float y)
    : GameObject(mygame)
{
    init(x,y);
}

Popups::~Popups()
{
}

Disk Popups::getCollisionHull() const
{
    Disk disk;
    disk.cx = px;
    disk.cy = py;
    disk.radius = size / 2.0f;
    return disk;
}
