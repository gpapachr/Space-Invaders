#include "game.h"
#include "EnemiesList.h"


void EnemiesList::drawEnemies()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			enemies[i][j].draw();
		}
	}
}

void EnemiesList::updateEnemies()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			enemies[i][j].update();
		}
	}
}

EnemiesList::EnemiesList()
{
	init();
}

EnemiesList::~EnemiesList()
{

}

void EnemiesList::init()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			enemies[i][j].init();
		}
	}
}

