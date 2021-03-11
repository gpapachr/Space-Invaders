#pragma once
#include "enemy.h"

class EnemiesList
{
	Enemy enemies[4][5];
	int enemiesCount = 20;
public:
	void drawEnemies();
	void updateEnemies();
	EnemiesList();
	~EnemiesList();
	void init();
};