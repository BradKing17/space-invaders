#pragma once
#include <string>
#include "SpriteComponent.h"
#include "Vector2.h"

/**
*  Objects used throughout the game.
*  Provides a nice solid base class for objects in this game world.
*  They currently support only sprite components, but this could easily
*  be extended to include things like rigid bodies or input systems. 
*  @see SpriteComponent
*/
class GameObject
{
public:
	/**
	*  Default constructor.
	*/
	GameObject() = default;
	
	/**
	*  Destructor. Frees dynamic memory.
	*/
	~GameObject();

	/**
	*  Allocates and attaches a sprite component to the object. 
	*  Part of this process will attempt to load a texture file.
	*  If this fails this function will return false and the memory
	*  allocated, freed. 
	*  @param [in] renderer The renderer used to perform the allocations
	*  @param [in] texture_file_name The file path to the the texture to load
	*  @return true if the component is successfully added
	*/
	bool  addSpriteComponent(ASGE::Renderer* renderer, const std::string& texture_file_name);
	
	/**
	*  Returns the sprite componenent.
	*  IT IS HIGHLY RECOMMENDED THAT YOU CHECK THE STATUS OF THE POINTER
	*  IS IS VALID FOR A COMPONENT TO BE NULL AS THEY ARE OPTIONAL!
	*  @return a pointer to the objects sprite component (if any)
	*/
	 SpriteComponent* spriteComponent();

	 int get_vel_x();
	 int get_vel_y();
	 void set_vel_x(int direction);
	 void set_vel_y(int direction);
	 void normalise();



private:

	void freeSpriteComponent();	
	SpriteComponent* sprite_component = nullptr;
	vector2 velocity = { 0,0 };
	

public:
	bool visibility = false;
	int speed = 300;
};