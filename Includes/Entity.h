#pragma once
#include "texture.h"
#include "TextureFactory.h"
#include "Camera.h"
#include "Shader.h"
#include "Behaviour.h"
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <string>

//forward declaration
class DrawRoutine;
class Renderer;
class GeometryLoader;

class Entity : public std::enable_shared_from_this<Entity> {
public:
	//entity constructor using default draw
	Entity(glm::vec3 pos, glm::vec3 rot, glm::vec3 scl, std::string modelName, std::string textureName);

	//entity constructor using custom draw
	Entity(glm::vec3 pos, glm::vec3 rot, glm::vec3 scl);

	void draw(std::shared_ptr<Camera>,Renderer* r);
	void setDrawRoutine(std::unique_ptr<DrawRoutine> newDrawRoutine);
	~Entity();
	void addBehaviour(std::unique_ptr<Behaviour>);
	void update(float deltaTime);
	void start(Renderer* r);
	void preInitialise(Renderer* r, std::shared_ptr<GeometryLoader> loader);

	//note rotations need to be in radians
	glm::vec3 position, rotation, scale;

private:
	std::unique_ptr<DrawRoutine> drawRoutine;
	std::vector<std::unique_ptr<Behaviour>> behaviours;
};

