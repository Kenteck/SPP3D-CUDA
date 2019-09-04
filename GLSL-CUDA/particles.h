#pragma once
#include <glad\glad.h>
#include <vector>
#include <glm/gtc/constants.hpp>
#include <time.h>
#include <iostream>

#include "cuda_runtime.h"
#include "device_launch_parameters.h"

//extern void calc(float* vel, float* ort, const int size);
//extern void Head_dev(int* head, float* vertices, int* lscl, const int size, const float radius, const float inter, const int L);
//extern void main_d(float* vertices, float *Vel, int* head, int* lscl, float*Orts, const float radius, const float inter, const int size, const int L);
extern void cudaInit(unsigned int VBO, float* vertices, float* Vel, int* head, int* lscl, float* Orts, const float radius, const float inter, const int size, const int L);
extern void kernelUpdate(const int size, const int L);
extern void kernelTerminate();
class particles
{
	const size_t c_particles;
	unsigned int VAO, VBO;
	float* vertices = new float[c_particles * 3];
	float* Vel = new float[c_particles * 3];
	const float radius;
	void Move();
	float coef = 0.05;
	const float inter;
	const int L = (2 * radius) / inter ;
	const int L2 = pow(L, 2);
	const int L3 = pow(L, 3);
	float *Orts = new float[c_particles * 3];
	int *lscl = new int[c_particles];
	int *head = new int[L3 + L];
	const float noise_amp = 0;

 public:
	particles(int number, int radius, float inter) : c_particles(number), radius(radius), inter(inter) {}
	~particles();
	void Init();
	void Draw();
};

