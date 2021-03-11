#include "bullet.h"
#include "graphics.h"
#include "game.h"

void Bullet::update()
{
   by -= speed * graphics::getDeltaTime();
}

void Bullet::draw()
{
    /*
    b.texture = std::string(ASSET_PATH) + "spaceship.png";
    b.outline_opacity = 0.0f;

    graphics::drawRect(bx, by, size, size, b);
    */ 
    b.texture = "";
    b.outline_opacity = 0.0f;
    b.fill_color[0] = 1.0f;
    b.fill_color[1] = 0.0f;
    b.fill_color[2] = 0.0f;
    b.fill_opacity = 1.0f;
    b.gradient = false;
    graphics::drawRect(bx, by, size / 15.0f, size, b);
 

    if (game.getDebugMode())
    {
        b.outline_opacity = 1.0f;
        b.texture = "";
        b.fill_color[0] = 0.3f;
        b.fill_color[1] = 0.3f;
        b.fill_color[2] = 1.0f;
        b.fill_opacity = 0.3f;
        b.gradient = false;

        Disk hull = getCollisionHull();
        graphics::drawDisk(hull.cx, hull.cy, hull.radius, b);
    }
    
}


void Bullet::init(int x, int y)
{
    bx = x;
    by = y;
}

Bullet::Bullet(const Game& mygame, int x, int y)
    : GameObject(mygame)
{
    init(x, y);
}

Bullet::~Bullet()
{
}

Disk Bullet::getCollisionHull() const
{
    Disk disk;
    disk.cx = bx;
    disk.cy = by - size/2.0f;
    disk.radius = size / 60.0f;
    return disk;
}
