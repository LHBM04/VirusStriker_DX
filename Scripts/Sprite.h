#pragma once

#include "Component.h"
#include "ResourceManager.h"

class Texture {

};

class SpriteRenderer : public Component {
private:
	Texture* m_texture;

public:
	SpriteRenderer(GameObject _owner);
	~SpriteRenderer();

};


