#include <string>

#include <Engine/Keys.h>
#include <Engine/Input.h>
#include <Engine/InputEvents.h>
#include <Engine/Sprite.h>

#include "Game.h"

/**
*   @brief   Default Constructor.
*   @details Consider setting the game's width and height
             and even seeding the random number generator.
*/
SpaceInvaders::SpaceInvaders()
{
}

/**
*   @brief   Destructor.
*   @details Remove any non-managed memory and callbacks.
*/
SpaceInvaders::~SpaceInvaders()
{
	this->inputs->unregisterCallback(key_callback_id);
	this->inputs->unregisterCallback(mouse_callback_id);


}

/**
*   @brief   Initialises the game.
*   @details The game window is created and all assets required to
			 run the game are loaded. The keyHandler and clickHandler
			 callback should also be set in the initialise function. Brad is gay and likes lolis. 
			 This code is very naazzz. Ur mum gay.
*   @return  True if the game initialised correctly.
*/
bool SpaceInvaders::init()
{
	setupResolution();
	if (!initAPI())
	{
		return false;
	}


	toggleFPS();
	renderer->setWindowTitle("Space Invaders!");

	// input handling functions
	inputs->use_threads = false;

	key_callback_id = inputs->addCallbackFnc(
		ASGE::E_KEY, &SpaceInvaders::keyHandler, this);
	
	mouse_callback_id =inputs->addCallbackFnc(
		ASGE::E_MOUSE_CLICK, &SpaceInvaders::clickHandler, this);


	if (!defender.addSpriteComponent(renderer.get(),
		".\\Resources\\Textures\\spaceshooter\\PNG\\playerShip3_blue.png"))
	{
		defender.addSpriteComponent(renderer.get(),
			".\\Resources\\Textures\\spaceshooter\\PNG\\playerShip3_blue.png");
	}
	defender_sprite = defender.spriteComponent()->getSprite();
	defender_sprite->width(48);
	defender_sprite->height(48);
	defender_sprite->xPos((game_width - defender_sprite->width()) / 2);
	defender_sprite->yPos(game_height - 50);


	initBullets();
	alienSpawn();


	return true;
}

void SpaceInvaders::initBullets()
{
	for (int i = 0; i < bullet_array_size; i++)
	{
		if (!bullets[i].addSpriteComponent(renderer.get(),
			".\\Resources\\Textures\\spaceshooter\\PNG\\Lasers\\laserBlue07.png"))
		{
			bullets[i].addSpriteComponent(renderer.get(),
				".\\Resources\\Textures\\spaceshooter\\PNG\\Lasers\\laserBlue07.png");
		}

		bullets[i].visibility = false;
	}
}

/**
*   @brief   Sets the game window resolution
*   @details This function is designed to create the window size, any 
             aspect ratio scaling factors and safe zones to ensure the 
			 game frames when resolutions are changed in size.
*   @return  void
*/
void SpaceInvaders::setupResolution()
{
	// how will you calculate the game's resolution?
	// will it scale correctly in full screen? what AR will you use?
	// how will the game be framed in native 16:9 resolutions?
	// here are some abritrary values for you to adjust as you see fit
	// https://www.gamasutra.com/blogs/KenanBolukbasi/20171002/306822/Scaling_and_MultiResolution_in_2D_Games.php
	game_width = 1024;
	game_height = 768;
}

/**
*   @brief   Processes any key inputs
*   @details This function is added as a callback to handle the game's
			 keyboard input. For this game, calls to this function
			 are thread safe, so you may alter the game's state as you
			 see fit.
*   @param   data The event data relating to key input.
*   @see     KeyEvent
*   @return  void
*/
void SpaceInvaders::keyHandler(const ASGE::SharedEventData data)
{
	auto key = static_cast<const ASGE::KeyEvent*>(data.get());
	
	if (in_menu == true)
	{
		if (key->key == ASGE::KEYS::KEY_W
			&& key->action == ASGE::KEYS::KEY_RELEASED)
		{
			if (menu_option == 1 && main_menu == true)
			{
				menu_option = 0;
			}
			if (mode_select == true && menu_option > 0)
			{
				menu_option--;
				
			}
		}

		if (key->key == ASGE::KEYS::KEY_S
			&& key->action == ASGE::KEYS::KEY_RELEASED)
		{
			if (menu_option == 0 && main_menu == true)
			{
				menu_option = 1;
			}
			if (mode_select == true && menu_option < 2)
			{
				menu_option++;
				
			}
		}
		
	}
	

	if (key->key == ASGE::KEYS::KEY_ESCAPE)
	{
		signalExit();
	}

	if (key->key == ASGE::KEYS::KEY_ENTER && 
		key->action == ASGE::KEYS::KEY_RELEASED 
		&& main_menu == true)
	{
		if (menu_option == 0)
		{
			mode_select = true;
			main_menu = false;
		}
		else if (menu_option == 1)
		{
			signalExit();
		}
	}
	else if ((key->key == ASGE::KEYS::KEY_ENTER &&
		key->action == ASGE::KEYS::KEY_RELEASED &&
		mode_select == true))
	{
		switch (menu_option)
		{
		case 0:
			mode = 0;
			break;

		case 1:
			mode = 1;
			break;

		case 2:
			mode = 2;
			break;
		}


		mode_select = false;
		in_menu = false;
	}

	if (key->action == ASGE::KEYS::KEY_PRESSED)
	{
		if (key->key == ASGE::KEYS::KEY_A)
		{
			defender.set_vel_x(-1);
			if (key->key == ASGE::KEYS::KEY_SPACE
				&& key->action == ASGE::KEYS::KEY_RELEASED)
			{
				if (number_of_bullets > 0)
				{
					fire();
				}
			}
		}
		if (key->key == ASGE::KEYS::KEY_D)
		{
			defender.set_vel_x(1);
			if (key->key == ASGE::KEYS::KEY_SPACE
				&& key->action == ASGE::KEYS::KEY_RELEASED)
			{
				if (number_of_bullets > 0)
				{
					fire();
				}
			}
		}
	}
	else if (key->action == ASGE::KEYS::KEY_RELEASED)
	{
		defender.set_vel_x(0);
		if (key->key == ASGE::KEYS::KEY_SPACE
			&& key->action == ASGE::KEYS::KEY_RELEASED)
		{
			if (number_of_bullets > 0)
			{
				fire();
			}
		}
	}



}

/**
*   @brief   Processes any click inputs
*   @details This function is added as a callback to handle the game's
		     mouse button input. For this game, calls to this function
             are thread safe, so you may alter the game's state as you
             see fit.
*   @param   data The event data relating to key input.
*   @see     ClickEvent
*   @return  void
*/
void SpaceInvaders::clickHandler(const ASGE::SharedEventData data)
{
	auto click = static_cast<const ASGE::ClickEvent*>(data.get());

	double x_pos, y_pos;
	inputs->getCursorPos(x_pos, y_pos);
}


/**
*   @brief   Updates the scene
*   @details Prepares the renderer subsystem before drawing the
		     current frame. Once the current frame is has finished
		     the buffers are swapped accordingly and the image shown.
*   @return  void
*/
void SpaceInvaders::update(const ASGE::GameTime& us)
{
	//make sure you use delta time in any movement calculations!
	auto dt_sec = us.delta_time.count() / 1000.0;

	if (!in_menu)
	{
		defenderMovement(dt_sec);

		bulletMovement(dt_sec);

		alienMovement(dt_sec);
	}

	
	

}

/**
*   @brief   Renders the scene
*   @details Renders all the game objects to the current frame.
	         Once the current frame is has finished the buffers are
			 swapped accordingly and the image shown.
*   @return  void
*/
void SpaceInvaders::render(const ASGE::GameTime &)
{
	renderer->setFont(0);

	if (in_menu)
	{
		std::string menu_str = "Menu Option: " + std::to_string(menu_option);
		renderer->renderText(menu_str.c_str(),
			20, game_height - 20, ASGE::COLOURS::WHITE);

		if (main_menu)
		{
			renderer->renderText("SPACE INVADERS",
				200, 200, ASGE::COLOURS::WHITE);

			renderer->renderText(menu_option == 0 ? ">PLAY" : "PLAY",
				200, 300, 1.0, ASGE::COLOURS::ORANGE);
			renderer->renderText(menu_option == 1 ? ">QUIT" : "QUIT?",
				200, 350, 1.0, ASGE::COLOURS::ORANGE);
		}
		
		if (mode_select)
		{
			renderer->renderText(menu_option == 0 ? ">GRAVITY" : "GRAVITY",
				200, 300, 1.0, ASGE::COLOURS::ORANGE);
			renderer->renderText(menu_option == 1 ? ">QUADRATIC" : "QUADRATIC",
				200, 350, 1.0, ASGE::COLOURS::ORANGE);
			renderer->renderText(menu_option == 2 ? ">SINE" : "SINE",
				200, 400, 1.0, ASGE::COLOURS::ORANGE);
		}
	}
	else
	{

		renderer->renderSprite(*defender_sprite);

		std::string x_str = "X: " + std::to_string(1-(aliens[0].spriteComponent()->getSprite()->xPos()/game_width));
		renderer->renderText(x_str.c_str(),
			game_width - 60, game_height - 20, ASGE::COLOURS::WHITE);
		std::string y_str = "Y: " + std::to_string(aliens[0].get_vel_y());
		renderer->renderText(y_str.c_str(),
			game_width - 60, game_height - 40, ASGE::COLOURS::WHITE);

		std::string score_str = "Score: " + std::to_string(score);
		renderer->renderText(score_str.c_str(),
			20, game_height - 20, ASGE::COLOURS::WHITE);
		std::string shots_str = "Shots: " + std::to_string(number_of_bullets);
		renderer->renderText(shots_str.c_str(),
			20, game_height - 50, ASGE::COLOURS::WHITE);

		for (int i = 0; i < alien_array_size; i++)
		{
			if (aliens[i].visibility == true)
			{
				renderer->renderSprite(*aliens[i].spriteComponent()->getSprite());
			}
		}

		

		for (int j = 0; j < bullet_array_size; j++)
		{
			if (bullets[j].visibility == true)
			{
				renderer->renderSprite(*bullets[j].spriteComponent()->getSprite());
			}
		}
	}


}

void SpaceInvaders::alienSpawn()
{
	int alien_row = 1;

	for (int i = 0; i < alien_array_size; i++)

	{
		if (alien_row == 1)
		{
			if (!aliens[i].addSpriteComponent(renderer.get(),
				".\\Resources\\Textures\\spaceshooter\\PNG\\Enemies\\enemyBlack1.png"))
			{
				aliens[i].addSpriteComponent(renderer.get(),
					".\\Resources\\Textures\\spaceshooter\\PNG\\Enemies\\enemyBlack1.png");
			}
		}
		else if (alien_row == 2)
		{
			if (!aliens[i].addSpriteComponent(renderer.get(),
				".\\Resources\\Textures\\spaceshooter\\PNG\\Enemies\\enemyBlue2.png"))
			{
				aliens[i].addSpriteComponent(renderer.get(),
					".\\Resources\\Textures\\spaceshooter\\PNG\\Enemies\\enemyBlue2.png");
			}
		}
		else if (alien_row == 3)
		{
			if (!aliens[i].addSpriteComponent(renderer.get(),
				".\\Resources\\Textures\\spaceshooter\\PNG\\Enemies\\enemyGreen3.png"))
			{
				aliens[i].addSpriteComponent(renderer.get(),
					".\\Resources\\Textures\\spaceshooter\\PNG\\Enemies\\enemyGreen3.png");
			}
		}
		else if (alien_row == 4)
		{
			if (!aliens[i].addSpriteComponent(renderer.get(),
				".\\Resources\\Textures\\spaceshooter\\PNG\\Enemies\\enemyRed4.png"))
			{
				aliens[i].addSpriteComponent(renderer.get(),
					".\\Resources\\Textures\\spaceshooter\\PNG\\Enemies\\enemyRed4.png");
			}
		}
		else if (alien_row == 5)
		{
			if (!aliens[i].addSpriteComponent(renderer.get(),
				".\\Resources\\Textures\\spaceshooter\\PNG\\Enemies\\enemyRed5.png"))
			{
				aliens[i].addSpriteComponent(renderer.get(),
					".\\Resources\\Textures\\spaceshooter\\PNG\\Enemies\\enemyRed5.png");
			}
		}

		alien_sprite = aliens[i].spriteComponent()->getSprite();
		alien_sprite->width(48);
		alien_sprite->height(48);
		alien_sprite->xPos(x_pos);
		alien_sprite->yPos(y_pos);

		x_pos += (alien_sprite->width() + 10);

		if (x_pos + alien_sprite->width() >= (game_width - 200))
		{
			alien_row++;
			x_pos = 0;
			y_pos = alien_row * 56;
		}
		
		aliens[i].visibility = true;
	}
}

void SpaceInvaders::defenderMovement(float dt_sec)
{
	auto defender_pos = defender_sprite->xPos();

	if (defender_sprite->xPos() <= 0)
	{
		defender.set_vel_x(defender.get_vel_x() * -1);
	}
	if (defender_sprite->xPos() + defender_sprite->width() >= game_width)
	{
		defender.set_vel_x(defender.get_vel_x() * -1);
	}

	defender_pos += defender.get_vel_x() * defender.speed* dt_sec;

	defender_sprite->xPos(defender_pos);
}

void SpaceInvaders::fire()
{
	if (number_of_bullets > 0)
	{
		if (bullets[number_of_bullets - 1].visibility == false)
		{
			bullet_sprite = bullets[number_of_bullets - 1].spriteComponent()->getSprite();
			bullet_sprite->xPos(defender_sprite->xPos() + 22);
			bullet_sprite->yPos(defender_sprite->yPos() - 30);
			bullets[number_of_bullets - 1].visibility = true;
			number_of_bullets--;
		}
	}
}

void SpaceInvaders::bulletMovement(float dt_sec)
{

		for (int j = 0; j < bullet_array_size; j++)
		{
			if (bullets[j].visibility == true)
			{
				bullets[j].set_vel_y(-1);

				ASGE::Sprite* bullet_sprite = bullets[j].spriteComponent()->getSprite();
				bullet_box = bullets[j].spriteComponent()->getBoundingBox();

				auto bullet_pos = bullet_sprite->yPos();

				bullet_pos += bullets[j].get_vel_y() * bullets[j].speed  * dt_sec;

				bullet_sprite->yPos(bullet_pos);

				if (bullet_pos < 0)
				{
					bullets[j].visibility = false;
					number_of_bullets++;
				}
				for (int i = 0; i < alien_array_size; i++)
				{
					alien_box = aliens[i].spriteComponent()->getBoundingBox();

					if (bullet_box.isInside(alien_box) && aliens[i].visibility == true)
					{
						aliens[i].visibility = false;
						bullets[j].visibility = false;	
						number_of_bullets++;
						score += 5000;
						break;
					}
				}
			}
		}
	
}

void SpaceInvaders::alienMovement(float dt_sec)
{
	switch (mode)
	{
	case 0:
	{
		for (int i = 0; i < alien_array_size; i++)
		{


			alien_sprite = aliens[i].spriteComponent()->getSprite();
			float x_pos = alien_sprite->xPos();
			float y_pos = alien_sprite->yPos();

			aliens[i].set_vel_x(1);
			aliens[i].set_vel_y(x_pos / 100);

			if (aliens[13].spriteComponent()->getSprite()->xPos() + aliens[13].spriteComponent()->getSprite()->width() >= game_width)
			{
				direction = -1;
			}
			if (aliens[0].spriteComponent()->getSprite()->xPos() <= 0)
			{
				direction = 1;
			}
			if (direction == -1)
			{
				for (int j = 0; j < alien_array_size; j++)
				{
					aliens[j].set_vel_x(-1);
				}
			}
			else if (direction == 1)
			{
				for (int j = 0; j < alien_array_size; j++)
				{
					aliens[j].set_vel_x(1);
				}
			}
			x_pos += aliens[i].get_vel_x() * aliens[i].speed / 8 * dt_sec;
			y_pos += aliens[i].get_vel_y() * dt_sec;

			alien_sprite->xPos(x_pos);
			alien_sprite->yPos(y_pos);
		}
		break;
	}
	case 1:
	{
		for (int i = 0; i < alien_array_size; i++)
		{
			

			alien_sprite = aliens[i].spriteComponent()->getSprite();
			auto x_pos = alien_sprite->xPos();
			auto y_pos = alien_sprite->yPos();

			float a = x_pos - (game_width / 2);
			aliens[i].set_vel_x(1);
			aliens[i].set_vel_y(a * a);

			

			if (aliens[13].spriteComponent()->getSprite()->xPos() + aliens[13].spriteComponent()->getSprite()->width() >= game_width)
			{
				direction = -1;
			}
			if (aliens[0].spriteComponent()->getSprite()->xPos() <= 0)
			{
				direction = 1;
			}
			if (direction == -1)
			{
				for (int j = 0; j < alien_array_size; j++)
				{
					aliens[j].set_vel_x(-1);
				}
			}
			else if (direction == 1)
			{
				for (int j = 0; j < alien_array_size; j++)
				{
					aliens[j].set_vel_x(1);
				}
			}
			
			

			x_pos += aliens[i].get_vel_x() * aliens[i].speed / 8 * dt_sec;
			y_pos += aliens[i].get_vel_y() * aliens[i].speed / 25 * dt_sec;

			alien_sprite->xPos(x_pos);
			alien_sprite->yPos(y_pos);
		}
		break;
	}

	}
}