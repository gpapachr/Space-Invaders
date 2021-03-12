#pragma once
#include "player.h"
#include "Enemy.h"
#include "config.h"
#include "bullet.h"
#include "enemybullet.h"
#include "popups.h"

class Game
{
	bool victory = false;

	//Score
	int score = 0;
	int highscore;

	//Player
	Player* player = nullptr;
	bool player_initialized = false;

	//Enemies
	Enemy* enemies[COLUMNS][ROWS];
	int enemies_counter;
	bool enemies_initialized = false;
	Enemy* sith = nullptr;
	
	//Boss
	Enemy* boss = nullptr;
	bool boss_initialized = false;
	bool boss_time = false;
	EnemyBullet* boss_fire = nullptr;

	//Enemy Bullet
	EnemyBullet* enemy_fire = nullptr;
	bool enemy_bullet_initialized = false;

	//Player's Bullet	
	Bullet* fire = nullptr;
	bool bullet_initialized = false;

	//Popups
	Popups* p = nullptr;
	bool speed_popup_initialized = false;
	bool health_popup_initialized = false;

	typedef enum { STATUS_START, STATUS_PLAYING ,STATUS_END, STATUS_RECORD } status_t;
	bool debug_mode = false;
	status_t status = STATUS_START;

	bool checkBossDead();
	bool checkEnemiesReached();
	bool checkNoMoreEnemies();
	bool checkPlayerDead();
	void checkPlayerDamage();
	void checkEnemyDamage();
	void spawnSith();
	void updateStartScreen();
	void updateLevelScreen();
	void updateEndScreen();
	void updateNewRecordScreen();
	void drawStartScreen();
	void drawLevelScreen();
	void drawEndScreen();
	void drawNewRecordScreen();
	int readHighscore();
	void writeHighscore();
	void cheat_codes();

public:	
	void setDebugMode(bool d) { debug_mode = d; }
	bool getDebugMode() const { return debug_mode; }
	void update();
	void draw();
	void init();
	Game();													  
	~Game();
};
