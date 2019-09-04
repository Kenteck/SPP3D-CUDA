#include "Sphere.h"

WireSphere::~WireSphere()
{

}

void WireSphere::Init()
{
	float x, y, z, xy;

	float stackStep = glm::pi<float>() / stack;
	float sectorStep = 2 * glm::pi<float>() / sector;
	float stackAngle, sectorAngle;

	for (int i = 0; i <= stack; i++) {
		stackAngle = glm::pi<float>() / 2 - i * stackStep;
		xy = radius * cosf(stackAngle);
		z = radius * sinf(stackAngle);

		for (int j = 0; j <= sector; j++) {
			sectorAngle = j * sectorStep;

			x = xy * cosf(sectorAngle);
			y = xy * sinf(sectorAngle);
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);
		}
	}

	unsigned int k1, k2;
	for (int i = 0; i < stack; i++) {
		k1 = i * (sector + 1);
		k2 = k1 + sector + 1;

		for (int j = 0; j < sector; j++, k1++, k2++) {
			LineIndices.push_back(k1);
			LineIndices.push_back(k2);
			if (i > 0) {
				LineIndices.push_back(k1);
				LineIndices.push_back(k1 + 1);
			}
		}
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, LineIndices.size() * sizeof(unsigned int), LineIndices.data(), GL_STATIC_DRAW);
}

void WireSphere::Draw()
{

	glBindVertexArray(VAO);
	
	//glDrawArrays(GL_LINE_LOOP, 0, vertices.size());
	//glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());
	glDrawElements(GL_LINES, LineIndices.size(), GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);

}