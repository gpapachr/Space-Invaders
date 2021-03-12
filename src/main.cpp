#include "graphics.h"
#include "game.h"
#include "config.h"


void update(float ms)
{
    Game* game = reinterpret_cast<Game *>(graphics::getUserData());
    game->update();
}

void draw()
{
    Game* game = reinterpret_cast<Game*>(graphics::getUserData());
    game->draw();
}

int main()
{
    Game mygame;

    graphics::createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Space Invaders");

    graphics::setUserData(&mygame);

    graphics::setDrawFunction(draw);
    graphics::setUpdateFunction(update);

    graphics::setCanvasSize(CANVAS_WIDTH, CANVAS_HEIGHT);
    graphics::setCanvasScaleMode(graphics::CANVAS_SCALE_FIT);

    mygame.init();
    mygame.setDebugMode(false);

    //start main game message loop
    graphics::startMessageLoop();

    //when game ends or ESC is pressed or X is clicked then destroy everything :D
    graphics::destroyWindow();

    return 0;
}