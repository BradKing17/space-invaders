#include <Engine\Renderer.h>
#include "GameObject.h"

GameObject::~GameObject()
{
	freeSpriteComponent();
}

bool GameObject::addSpriteComponent(
	ASGE::Renderer* renderer, const std::string& texture_file_name)
{
	freeSpriteComponent();

	sprite_component = new SpriteComponent();
	if (sprite_component->loadSprite(renderer, texture_file_name))
	{
		return true;
	}

	freeSpriteComponent();
	return false;
}

void  GameObject::freeSpriteComponent()
{
	delete sprite_component;
	sprite_component = nullptr;
}


SpriteComponent* GameObject::spriteComponent() 
{
	return sprite_component;
}

int GameObject::get_vel_x()
{
	return velocity.x;
}

int GameObject::get_vel_y()
{
	return velocity.y;
}

void GameObject::set_vel_x(int direction_x)
{
	velocity.x = direction_x;
}

void GameObject::set_vel_y(int direction_y)
{
	velocity.y = direction_y;

}
