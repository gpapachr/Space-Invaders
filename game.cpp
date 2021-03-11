#include "game.h"
#include "graphics.h"
#include "config.h"
#include <fstream>
#include <iostream>

bool Game::checkBossDead()
{
	if (boss)
	{
		if (boss->getRemainingLife() <= 0)
		{
			score *= 3.0f;
			return true;
		}
	}
	return false;
}

bool Game::checkEnemiesReached()
{
	for (int i = 0; i < COLUMNS; i++)
	{
		for (int j = 0; j < ROWS; j++)
		{
			if (enemies[i][j])
			{
				if (enemies[i][j]->checkReached())
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool Game::checkNoMoreEnemies()
{
	if (enemies_counter > 0)
	{
		return false;
	}
	return true;
}

bool Game::checkPlayerDead()
{
	if (player->getRemainingLife() <= 0)
	{
		return true;
	}
	return false;
}

void Game::checkPlayerDamage()
{
	if (!player)
	{
		return;
	}
	if (!enemy_fire && !boss_fire)
	{
		return;
	}
	
	if (enemy_fire)
	{
		Disk d1 = enemy_fire->getCollisionHull();
		Disk d2 = player->getCollisionHull();
		float dx = d1.cx - d2.cx;
		float dy = d1.cy - d2.cy;
		if (sqrt(dx * dx + dy * dy) < d1.radius + d2.radius)
		{
			delete enemy_fire;
			enemy_fire = nullptr;
			enemy_bullet_initialized = false;
			player->drainLife(0.15f);
		}
	}
	else if (boss_fire)
	{
		Disk d1 = boss_fire->getCollisionHull();
		Disk d2 = player->getCollisionHull();
		float dx = d1.cx - d2.cx;
		float dy = d1.cy - d2.cy;
		if (sqrt(dx * dx + dy * dy) < d1.radius + d2.radius)
		{
			delete boss_fire;
			boss_fire = nullptr;
			player->drainLife(0.1f);
		}
	}
}

void Game::checkEnemyDamage()
{
	if (!fire)
	{
		return;
	}

	if (!boss_time)
	{
		Disk d1 = fire->getCollisionHull();
		for (int i = 0; i < COLUMNS; i++)
		{
			for (int j = 0; j < ROWS; j++)
			{
				if (enemies[i][j])
				{
					Disk d2 = enemies[i][j]->getCollisionHull();
					float dx = d1.cx - d2.cx;
					float dy = d1.cy - d2.cy;
					if (sqrt(dx * dx + dy * dy) < d1.radius + d2.radius)
					{
						graphics::playSound(std::string(ASSET_PATH) + "shortexplosion.mp3", 0.2f, false);
						score += 10;
						delete enemies[i][j];
						enemies[i][j] = nullptr;
						enemies_counter--;

						for (int i = 0; i < COLUMNS; i++)
						{
							for (int j = 0; j < ROWS; j++)
							{
								if (enemies[i][j] && enemies_counter > 1) { enemies[i][j]->increaseSpeed(); }
							}
						}
						delete fire;
						fire = nullptr;
						bullet_initialized = false;
					}
				}

			}
		}
	}
	else if (boss_time)
	{
		Disk d1 = fire->getCollisionHull();
		Disk d2 = boss->getCollisionHull();
		float dx = d1.cx - d2.cx;
		float dy = d1.cy - d2.cy;
		if (sqrt(dx * dx + dy * dy) < d1.radius + d2.radius)
		{
			graphics::playSound(std::string(ASSET_PATH) + "shortexplosion.mp3", 0.2f, false);
			score += 30;
			boss->drainLife(0.2f);
			delete fire;
			fire = nullptr;
			bullet_initialized = false;
		}
	}
}

void Game::spawnSith()
{
	float init_x = 100; // x for first sith to be spawned

	for (int i = 0; i < COLUMNS; i++)
	{
		init_x += 79.75f; // the gap between 2 sith of the same row
		float init_y = 5; // y for first line of sith to be spawned

		for (int j = 0; j < ROWS; j++)
		{
			init_y += 50; // the gap betwwen 2 rows of sith
			sith = new Enemy(*this, init_x, init_y);
			enemies[i][j] = sith;
			enemies_counter++;
		}
	}
}

void Game::updateStartScreen()
{
	//wait in start screen untill player hits RETURN button
	if (graphics::getKeyState(graphics::SCANCODE_RETURN))
	{
		status = STATUS_PLAYING;
	}
}

void Game::updateLevelScreen()
{
	//INIT PLAYER
	if (!player_initialized && graphics::getGlobalTime() > 1000)
	{
		player = new Player(*this);
		player_initialized = true;

	}

	//UPDATE PLAYER
	if (player)
		player->update();

	//INIT ENEMIES
	if (!enemies_initialized && !boss_time && graphics::getGlobalTime() > 3000)
	{
		spawnSith();
		enemies_initialized = true;
	}

	if (!boss_time)
	{
		//UPDATE ENEMIES
		for (int i = 0; i < COLUMNS; i++)
		{
			for (int j = 0; j < ROWS; j++)
			{
				if (enemies[i][j]) { enemies[i][j]->update(); }
			}
		}
		for (int i = 0; i < COLUMNS; i++)
		{
			for (int j = 0; j < ROWS; j++)
			{
				if (enemies[i][j])
				{
					if (enemies[i][j]->checkBounce()) //when first enemy hit on the walls, every enemy changes direction and row
					{
						for (int i = 0; i < COLUMNS; i++)
						{
							for (int j = 0; j < ROWS; j++)
							{
								if (enemies[i][j]) {
									enemies[i][j]->swapDir(); enemies[i][j]->moveDown(); enemies[i][j]->update();
								}
							}
						}
					}
				}
			}
		}

		//update enemies' bullets
		int chance = rand() % 3; //1 to 3 times a random enemy will fire
		if (chance == 1)
		{
			int i = rand() % COLUMNS;
			int j = rand() % ROWS;

			if (!enemy_bullet_initialized && enemies[i][j] && graphics::getGlobalTime() > 3000)
			{
				enemy_fire = new EnemyBullet(*this, enemies[i][j]->getX(), enemies[i][j]->getY() + 5);
				enemy_bullet_initialized = true;
			}
			else if (enemy_bullet_initialized && graphics::getGlobalTime() > 3000)
			{
				enemy_fire->update();
				if (enemy_fire->getBulletY() > CANVAS_HEIGHT + enemy_fire->getBulletSize())
				{
					delete enemy_fire;
					enemy_fire = nullptr;
					enemy_bullet_initialized = false;
				}
			}
		}
	}
	if (boss_time && !boss_initialized)
	{
		boss = new Enemy(*this, CANVAS_WIDTH / 2, 100);
		boss->isBoss();
		player->itsBossTime();
		boss_initialized = true;
		graphics::stopMusic();
		graphics::playSound(std::string(ASSET_PATH) + "darth_vader.mp3", 0.4f, false);
		graphics::playMusic(std::string(ASSET_PATH) + "boss.mp3", 0.7f, true, 4000);
	}
	if (boss)
	{
		boss->update();
		if (boss->checkBossBounce())
		{
			boss->swapDir();
			boss->moveDown();
			boss->update();
		}
		if (!boss_fire)
		{
			boss_fire = new EnemyBullet(*this, boss->getX(), boss->getY() + 5);
			boss_fire->isBoss();			
		}
		else if (boss_fire)
		{
			boss_fire->update();
			if (boss_fire->getBulletY() > CANVAS_HEIGHT + boss_fire->getBulletSize())
			{
				delete boss_fire;
				boss_fire = nullptr;
			}
		}
	}

	//INIT PLAYER'S BULLET
	if (!bullet_initialized && graphics::getGlobalTime() > 3000 && graphics::getKeyState(graphics::SCANCODE_SPACE))
	{
		fire = new Bullet(*this, player->getPosX(), player->getPosY() - player->getSize());
		bullet_initialized = true;
		graphics::playSound(std::string(ASSET_PATH) + "blaster.mp3", 0.4f, false);

	}

	//UPDATE PLAYER'S BULLET
	if (fire)
	{
		fire->update();
		if (fire->getBulletY() < 0 - fire->getBulletSize()) //if bullet out of upper bounds then delete and re-init it
		{
			delete fire;
			fire = nullptr;
			bullet_initialized = false;
		}
	}

	//INIT SPEED POPUP 
	if (!speed_popup_initialized && graphics::getGlobalTime() > 17000)
	{
		p = new Popups(*this, CANVAS_WIDTH + 40, CANVAS_HEIGHT / 2);
		p->choosePopUp(0);
		speed_popup_initialized = true;
	}

	//INIT HEALTH POPUP
	if (!health_popup_initialized && graphics::getGlobalTime() > 30000)
	{
		p = new Popups(*this, CANVAS_WIDTH + 40, CANVAS_HEIGHT / 2);
		p->choosePopUp(1);
		health_popup_initialized = true;
	}

	//UPDATE SPEED POPUP
	else if (speed_popup_initialized && p && graphics::getGlobalTime() > 17000 && graphics::getGlobalTime() < 29000)
	{
		p->update();
		if (p->getX() <= 0 - p->getSize()) //if out of bounds then delete but dont re-init it
		{
			delete p;
			p = nullptr;
		}
		if (p && fire)
		{
			Disk d1 = p->getCollisionHull();
			Disk d2 = fire->getCollisionHull();
			float dx = d1.cx - d2.cx;
			float dy = d1.cy - d2.cy;

			if (sqrt(dx * dx + dy * dy) < d1.radius + d2.radius) //if player's bullet hit popup then delete both of them and apply power-up
			{
				delete p;
				p = nullptr;

				delete fire;
				fire = nullptr;
				bullet_initialized = false;
				score *= 2;
				player->fast();
			}
		}
	}

	//UPDATE HEALTH POPUP
	else if (health_popup_initialized && p && graphics::getGlobalTime() > 45000)
	{
		p->update();
		if (p->getX() <= 0 - p->getSize())//if out of bounds then delete but dont re-init it
		{
			delete p;
			p = nullptr;
		}
		if (p && fire)
		{
			Disk d1 = p->getCollisionHull();
			Disk d2 = fire->getCollisionHull();
			float dx = d1.cx - d2.cx;
			float dy = d1.cy - d2.cy;

			if (sqrt(dx * dx + dy * dy) < d1.radius + d2.radius)//if player's bullet hit popup then delete both of them and apply power-up
			{
				delete p;
				p = nullptr;

				delete fire;
				fire = nullptr;
				bullet_initialized = false;
				score *= 2;
				player->gainLife(0.2);
			}
		}
	}
	if (graphics::getGlobalTime() > 26000)
	{
		player->normal();
	}

	checkPlayerDamage();  //check if player has been hit
	checkEnemyDamage();   //check if an enemy has been hit
	
	if (checkNoMoreEnemies() && enemies_initialized && !boss_time)//check if first wave of enemies is over
	{
		if (enemy_fire)
		{
			delete enemy_fire;
			enemy_fire = nullptr;
		}
		boss_time = true;
		player->gainLife(1.0f); //heal player
	}

	if (checkBossDead() && boss_initialized) //if boss has been initialised and now its not alive, player wins
	{
		delete boss;
		boss = nullptr;
		writeHighscore();
		victory = true;
		graphics::stopMusic();
		graphics::playSound(std::string(ASSET_PATH) + "victory.mp3", 0.8f, false);
		status = STATUS_END;
	}
	if (checkPlayerDead() || checkEnemiesReached()) //if player died or enemies reached the player, player is defeated
	{
		writeHighscore();
		victory = false;
		graphics::stopMusic();
		graphics::playSound(std::string(ASSET_PATH) + "chewbacca.mp3", 0.8f, false);
		status = STATUS_END;
	}
	cheat_codes(); // check if any cheatcode has been activated
}

void Game::updateEndScreen()
{
	//if the current score is better than the last high score, then go to the new hishscore screen

	//because its possible for a player to achieve highscore even if he has been defeated (when highscore is still 0 or very low)
	//the new highscore screen appears only to winners
	if (highscore < score && victory)
	{
		status = STATUS_RECORD;
	}
}

void Game::updateNewRecordScreen()
{
}

void Game::drawStartScreen()
{
	graphics::Brush br;
	
	//print current HighScore
	highscore = readHighscore();
	char hsc[40];
	sprintf_s(hsc, "High Score : %d", highscore);
	graphics::drawText(CANVAS_WIDTH / 2 - 115, 100, 30, hsc, br);

	//print game logo
	br.texture = std::string(ASSET_PATH) + "final_logo.png";
	br.outline_opacity = 0.0f;
	graphics::drawRect(CANVAS_WIDTH / 2, 250, 500, 200, br);

	//print slogan
	char force[40];
	sprintf_s(force, "may  the  force  be  with  you...");
	graphics::drawText(CANVAS_WIDTH / 4 + 70, CANVAS_HEIGHT / 2 + 20, 20, force, br);

	//print game's instructions
	char info[40];		
	sprintf_s(info, "Press  ENTER  to  start");
	graphics::drawText(CANVAS_WIDTH/4 + 50, CANVAS_HEIGHT/2 + 180 , 30, info, br);

	char hint1[50];
	sprintf_s(hint1, "Use  A,D  or  left,right  arrows  to  move");
	graphics::drawText(CANVAS_WIDTH / 3 - 10, CANVAS_HEIGHT / 2 + 210, 15, hint1, br);

	char hint2[50];
	sprintf_s(hint2, "Use  spacebar  to  fire");
	graphics::drawText(CANVAS_WIDTH / 3 + 60, CANVAS_HEIGHT / 2 + 240, 15, hint2, br);

}

void Game::drawLevelScreen()
{
	graphics::Brush br;
	br.texture = std::string(ASSET_PATH) + "back1.png";
	br.outline_opacity = 0.0f;

	//draw background
	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);

	//draw player
	if (player)
		player->draw();

	//draw enemies	
	for (int i = 0; i < COLUMNS; i++)
	{
		for (int j = 0; j < ROWS; j++)
		{
			if (enemies[i][j]) { enemies[i][j]->draw(); }
		}
	}

	//draw boss
	if (boss)
	{
		boss->draw();
		float boss_life = boss->getRemainingLife();
		graphics::Brush bl;
		bl.outline_opacity = 0.0f;
		bl.fill_color[0] = 1.0f;
		bl.fill_color[1] = 0.2f;
		bl.fill_color[2] = 0.2f;
		bl.texture = "";
		graphics::drawRect(boss->getX() - (2 - boss_life) * 30 / 2, boss->getY() - boss->getSize() / 2 -10, boss_life * 30, 10, bl);

		bl.outline_opacity = 1.0f;
		bl.gradient = false;
		bl.fill_opacity = 0.0f;
		graphics::drawRect(boss->getX(), boss->getY() - boss->getSize() / 2 - 10, 60, 10, bl);
	}

	//draw boss's bullet
	if (boss_fire)
		boss_fire->draw();

	//draw popup
	if (p)
		p->draw();

	//draw enemies' bullets
	if (enemy_fire)
	{
		enemy_fire->draw();
	}

	//draw player's bullet
	if (fire)
		fire->draw();

	//ui info layer
	if (debug_mode == true)
	{
		if (!boss)
		{
			//useful information during debaging for watching the number of remaining enemies
			char ec[sizeof(int)];
			sprintf_s(ec, "%d", enemies_counter);
			graphics::drawText(CANVAS_WIDTH / 2, 50, 40, ec, br);
		}	
	}

	//player's remaining life info
	float player_life = player ? player->getRemainingLife() : 0.0f;

	br.outline_opacity = 0.0f;
	br.fill_color[0] = 1.0f;
	br.fill_color[1] = 0.2f;
	br.fill_color[2] = 0.2f;
	br.texture = "";
	br.fill_secondary_color[0] = 1.0f * (1.0f - player_life) + player_life * 0.2f;
	br.fill_secondary_color[1] = 0.2f * (1.0f - player_life) + player_life * 1.0f;
	br.fill_secondary_color[2] = 0.2f;
	br.gradient = true;
	br.gradient_dir_u = 1.0f;
	br.gradient_dir_v = 0.0f;
	graphics::drawRect(CANVAS_WIDTH - 100 - (1 - player_life) * 120 / 2, 30, player_life * 120, 30, br);

	br.outline_opacity = 1.0f;
	br.gradient = false;
	br.fill_opacity = 0.0f;
	graphics::drawRect(CANVAS_WIDTH - 100, 30, 120, 30, br);

	//score info
	graphics::Brush sb;
	char sc[40];
	sprintf_s(sc, "Score : %d", score);
	graphics::drawText(50, 40, 25, sc, sb);

}

void Game::drawEndScreen()
{
	//print background
	graphics::Brush back;
	back.outline_opacity = 0.0f;
	back.texture = std::string(ASSET_PATH) + "back.png";
	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, back);

	graphics::Brush b;
	if (victory)
	{
		//print this game's score
		char sc[40];
		sprintf_s(sc, "Score : %d", score);
		graphics::drawText(50, 40, 25, sc, b);

		//print victory message
		char victory[40];
		sprintf_s(victory, "VICTORY!");
		graphics::drawText(CANVAS_WIDTH / 3 + 50, CANVAS_HEIGHT / 3 - 50, 40, victory, b);

		b.texture = std::string(ASSET_PATH) + "victory.png";
		b.outline_opacity = 0.0f;
		graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 3 + 70, 150, 150, b);

		char exit[40];
		sprintf_s(exit, "Press  ESC  to  exit");
		graphics::drawText(CANVAS_WIDTH / 3 + 60, CANVAS_HEIGHT / 3 + 250, 20, exit, b);
	}
	else
	{
		//print this game's score
		char sc[40];
		sprintf_s(sc, "Score : %d", score);
		graphics::drawText(50, 40, 25, sc, b);

		//print game over message
		char defeat[40];
		sprintf_s(defeat, "GAME OVER!");
		graphics::drawText(CANVAS_WIDTH / 3 + 25, CANVAS_HEIGHT / 3 - 50, 40, defeat, b);

		b.texture = std::string(ASSET_PATH) + "chewbacca.png";
		b.outline_opacity = 0.0f;
		graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 3 + 70, 150, 150, b);

		char exit[40];
		sprintf_s(exit, "Press  ESC  to  exit");
		graphics::drawText(CANVAS_WIDTH / 3 + 60, CANVAS_HEIGHT / 3 + 250, 20, exit, b);
	}

}

void Game::drawNewRecordScreen()
{
	//print background
	graphics::Brush back;
	back.outline_opacity = 0.0f;
	back.texture = std::string(ASSET_PATH) + "back.png";
	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, back);

	//print highscore message
	graphics::Brush b;
	char rec[40];
	sprintf_s(rec, "NEW HIGHSCORE: %d", score);
	graphics::drawText(CANVAS_WIDTH / 3 - 70, CANVAS_HEIGHT / 3 - 50, 40, rec, b);

	b.texture = std::string(ASSET_PATH) + "highscore.png";
	b.outline_opacity = 0.0f;
	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 3 + 70, 150, 150, b);

	char exit[40];
	sprintf_s(exit, "Press  ESC  to  exit");
	graphics::drawText(CANVAS_WIDTH / 3 + 60, CANVAS_HEIGHT / 3 + 250, 20, exit, b);
}

int Game::readHighscore()
{
	//read last games highscore
	std::ifstream in(std::string(ASSET_PATH) + "high_score.txt");
	if (!in)
	{
		std::cout << "Error in read file opening" << std::endl;
		return 0;
	}
	int h = -1;
	in >> h;
	in.close();
	
	return h;
}

void Game::writeHighscore()
{
	//save new highscore
	std::ofstream out(std::string(ASSET_PATH) + "high_score.txt");
	if (!out)
	{
		std::cout << "Error in write file opening" << std::endl;
		return;
	}
	if (highscore < score)
	{
		out << score;
	}
	else
	{
		out << highscore;
	}
}

void Game::cheat_codes()
{
	//useful shortcuts to get in specific points of the game while in debugging process
	if (graphics::getKeyState(graphics::SCANCODE_LCTRL) && graphics::getKeyState(graphics::SCANCODE_K)) //ctrl + K to kill all enemies
	{
		for (int i = 0; i < COLUMNS; i++)
		{
			for (int j = 0; j < ROWS; j++)
			{
				if (enemies[i][j])
				{
					delete enemies[i][j];
					enemies[i][j] = nullptr;
					enemies_counter--;
				}
			}
		}
	}
	if (graphics::getKeyState(graphics::SCANCODE_LCTRL) && graphics::getKeyState(graphics::SCANCODE_L)) //ctrl + L for instant player death
	{
		player->drainLife(1.0f);
	}

	if (graphics::getKeyState(graphics::SCANCODE_LCTRL) && graphics::getKeyState(graphics::SCANCODE_R)) //ctrl + R to get out of debug mode
	{
		setDebugMode(false);
	}

	if (graphics::getKeyState(graphics::SCANCODE_LCTRL) && graphics::getKeyState(graphics::SCANCODE_T)) //ctrl + T to get into debug mode
	{
		setDebugMode(true);
	}

	if (graphics::getKeyState(graphics::SCANCODE_LCTRL) && graphics::getKeyState(graphics::SCANCODE_H)) //ctrl + H for instant player heal
	{
		if (player)
		{
			player->gainLife(1.0f);
		}
	}

	
}

void Game::update()
{
	if (status == STATUS_START) 
	{
		updateStartScreen();
	}
	else if(status == STATUS_PLAYING)
	{
		updateLevelScreen();
	}
	else if(status == STATUS_END)
	{
		updateEndScreen();
	}
	else
	{
		updateNewRecordScreen();
	}
}

void Game::draw()
{
	if (status == STATUS_START)
	{
		drawStartScreen();
	}	
	else if (status == STATUS_PLAYING)
	{
		drawLevelScreen();
	}
	else if (status == STATUS_END)
	{
		drawEndScreen();
	}
	else
	{
		drawNewRecordScreen();
	}
}

void Game::init()
{
	graphics::setFont(std::string(ASSET_PATH) + "Death Star.otf");
	graphics::playMusic(std::string(ASSET_PATH) + "main.mp3", 0.5f, true, 4000);
}

Game::Game()
{
}

Game::~Game()
{
	if (player)
	{
		delete player;
		player = nullptr;
	}
}
