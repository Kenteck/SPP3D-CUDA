#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <vector>


class WireSphere
{
	const float radius;
	const int stack;
	const int sector;
	unsigned int VAO, VBO, EBO;
	std::vector<float> vertices;
	std::vector<int> indices;
	std::vector<unsigned int> LineIndices;

public:
	WireSphere(float radius, int stack, int sector) : radius(radius), stack(stack), sector(sector) {}
	~WireSphere();
	void Init();

	void Draw();
};