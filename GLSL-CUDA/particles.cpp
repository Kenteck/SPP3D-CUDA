#include "particles.h"

particles::~particles()
{
	kernelTerminate();
}

void particles::Init()
{
	srand(time(NULL));
	for (size_t i = 0; i < c_particles; i++) {
		float u = (float)rand() / (RAND_MAX);
		float v = (float)rand() / (RAND_MAX);
		float theta = u * 2.0 * glm::pi<float>();
		float phi = acos(2.0 * v - 1.0);
		float sinTheta = sin(theta);
		float cosTheta = cos(theta);
		float sinPhi = sin(phi);
		float cosPhi = cos(phi);
		//Random coordinates within sphere
		vertices[3 * i] = cbrt((float)rand() / (RAND_MAX)) * radius * sinPhi * cosTheta;
		vertices[3 * i + 1] = cbrt((float)rand() / (RAND_MAX)) * radius * sinPhi * sinTheta;
		vertices[3 * i + 2] = cbrt((float)rand() / (RAND_MAX)) * radius * cosPhi;
		//Random velocities
		Vel[3 * i] = (float)rand() / (RAND_MAX) * 2 - 1;
		Vel[3 * i + 1] = (float)rand() / (RAND_MAX) * 2 - 1;
		Vel[3 * i + 2] = (float)rand() / (RAND_MAX) * 2 - 1;
	}
	
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, c_particles * 3 * sizeof(float), NULL, GL_STREAM_DRAW);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	cudaInit(VBO, vertices, Vel, head, lscl, Orts, radius, inter, c_particles, L);
}

void particles::Draw()
{
	//glBufferSubData(GL_ARRAY_BUFFER, 0, c_particles * 3 * sizeof(float), vertices);
	kernelUpdate(c_particles, L);
	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0, c_particles);
	glBindVertexArray(0);
	//Move();
}

void particles::Move()
{

	//calc(Vel, Orts, c_particles * 3);
	

	//for (size_t i = 0; i < L3; i++) head[i] = -1;
	//Head_dev(head, vertices, lscl, c_particles, radius, inter, L);

	//main_d(vertices, Vel, head, lscl, Orts, radius, inter, c_particles, L);
	
}