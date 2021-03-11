#include "enemybullet.h"
#include "graphics.h"
#include "game.h"

void EnemyBullet::update()
{
    by += speed * graphics::getDeltaTime();
}

void EnemyBullet::draw()
{
    graphics::setOrientation(180.0f);
    b.texture = "";
    b.outline_opacity = 0.0f;
    b.fill_color[0] = 0.0f;
    b.fill_color[1] = 1.0f;
    b.fill_color[2] = 0.0f;

    b.fill_opacity = 1.0f;
    b.gradient = false;
    graphics::drawRect(bx, by, size / 10.0f, size, b);
    graphics::resetPose();

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

void EnemyBullet::init(int x, int y)
{
    bx = x;
    by = y;
}

EnemyBullet::EnemyBullet(const Game& mygame, int x, int y)
    : GameObject(mygame)
{
    init(x, y);
}

EnemyBullet::~EnemyBullet()
{
}

Disk EnemyBullet::getCollisionHull() const
{
    Disk disk;
    disk.cx = bx;
    disk.cy = by + size / 2.0f;
    disk.radius = size / 50.0f;
    return disk;
}
