#pragma once
#include <string>
#include <Engine/OGLGame.h>

#include "GameObject.h"
#include "Rect.h"


/**
*  An OpenGL Game based on ASGE.
*/
class SpaceInvaders :
	public ASGE::OGLGame
{
public:
	SpaceInvaders();
	~SpaceInvaders();
	virtual bool init() override;

private:
	void keyHandler(const ASGE::SharedEventData data);
	void clickHandler(const ASGE::SharedEventData data);
	void setupResolution();
	void alienSpawn();
	void defenderMovement(float dt_sec);
	void initBullets();
	void alienMovement(float dt_sec);
	void fire();
	void bulletMovement(float dt_sec);
	

	virtual void update(const ASGE::GameTime &) override;
	virtual void render(const ASGE::GameTime &) override;

	int  key_callback_id = -1;	        /**< Key Input Callback ID. */
	int  mouse_callback_id = -1;        /**< Mouse Input Callback ID. */

	//Game Variables
	bool in_menu = true;
	int state = 0;
	int x_pos = 0;
	int y_pos = 56;
	int score = 0;
	int menu_option = 0;
	bool main_menu = true;
	bool mode_select = false;
	int mode = 0;
	int direction = 1;
	int c = 100;


	//Add your GameObjects
	//Player Ship
	GameObject defender;
	ASGE::Sprite* defender_sprite = nullptr;
	rect defender_box;

	//Bullet Variables
	GameObject bullets[10];
	ASGE::Sprite* bullet_sprite = nullptr;
	rect bullet_box;
	int bullet_array_size = 10;
	int number_of_bullets = bullet_array_size;

	//Alien Ships
	GameObject aliens[56];
	ASGE::Sprite* alien_sprite = nullptr;
	int alien_array_size = 56;
	rect alien_box;
	int number_of_aliens = alien_array_size;


	
};