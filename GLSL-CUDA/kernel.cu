
#include <glad/glad.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <math.h>
#include <stdio.h>
#include <cuda_gl_interop.h>

cudaGraphicsResource* cudaVBO;

float* vertices_dev;
float* Vel_dev;
int* head_dev;
int* lscl_dev;
float* Orts_dev;
float* radius_dev;
float* inter_dev;
int* L_dev;
int* c_dev;


#define gpuErrchk(ans) { gpuAssert((ans), __FILE__, __LINE__); }
inline void gpuAssert(cudaError_t code, const char* file, int line, bool abort = true)
{
	if (code != cudaSuccess)
	{
		fprintf(stderr, "GPUassert: %s %s %d\n", cudaGetErrorString(code), file, line);
		if (abort) exit(code);
	}
}


__global__ void addKernel(float* vel, float* orts, const int* size)
{
	int i = blockIdx.x  * blockDim.x + threadIdx.x;
	if (i < (*size)) {
		float hypot = sqrt(powf(vel[3 * i], 2) + powf(vel[3 * i + 1], 2) + powf(vel[3 * i + 2], 2));
		orts[3 * i] = vel[3 * i] / hypot;
		orts[3 * i + 1] = vel[3 * i + 1] / hypot;
		orts[3 * i + 2] = vel[3 * i + 2] / hypot;
	}
}

__global__ void nullKernel(int* head, const int* L)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	if (i < ((*L) * (*L) * (*L)))
		head[i] = -1;
}



__global__ void head_kernel(int* head, float* vertices, int* lscl, const float* radius, const float* inter, const int* L, const int* size)
{
	
	for (size_t i = 0; i < *size; i++) {
		size_t x1 = ((vertices[3 * i] + (*radius)) / (*inter));
		size_t y1 = ((vertices[3 * i + 1] + (*radius)) / (*inter));
		size_t z1 = ((vertices[3 * i + 2] + (*radius)) / (*inter));

		int c = x1 * ((*L) * (*L)) + y1 * (*L) + z1;

		lscl[i] = head[c];
		head[c] = i;
	}
}


__global__ void main_dev(float* vertices, float *Vel,int* head, int* lscl, float *Orts, const float* radius, const float* inter, const int* size, const int* L)
{


	int xc = blockIdx.x * blockDim.x + threadIdx.x;
	int yc = blockIdx.y * blockDim.y + threadIdx.y;
	int zc = blockIdx.z * blockDim.z + threadIdx.z;
	if (xc < (*L) && yc < (*L) && zc < (*L)) {
		int c = xc * (*L * *L) + yc * (*L) + zc;

		float sx = 0.;
		float sy = 0.;
		float sz = 0.;

		float Hypot, x, y, z, vx, vy, vz, x1, y1, z1, vx1, vy1, vz1;
		float SQRT, dt1, x2, y2, z2, Veln, Velx, Vely, Velz, Velx1, Vely1, Velz1, nx, ny, nz;
		float coef = 0.01;

		for (int xc1 = xc - 1; xc1 <= xc + 1; xc1++)
			for (int yc1 = yc - 1; yc1 <= yc + 1; yc1++)
				for (int zc1 = zc - 1; zc1 <= zc + 1; zc1++) {
					int c1 = ((xc + *L) % (*L)) * (*L * *L) + ((yc1 + *L) % (*L)) * (*L) + ((zc1 + *L) % (*L));
					int i = head[c];
					while (i != -1) {
						sx = 0.;
						sy = 0.;
						sz = 0.;
						int j = head[c1];
						while (j != -1) {
							if (i != j) {
								float dist = sqrt(pow(vertices[3 * (size_t)i] - vertices[3 * (size_t)j], 2) + pow(vertices[3 * (size_t)i + 1] - vertices[3 * (size_t)j + 1], 2) + pow(vertices[3 * (size_t)i + 2] - vertices[3 * (size_t)j + 2], 2));
								if (dist <= (*inter)) {
									sx += Orts[3 * j];
									sy += Orts[3 * j + 1];
									sz += Orts[3 * j + 2];
								}
							}
							j = lscl[j];
						}
					
				
	
						/*
						/*phi = glm::pi<float>() - 2 * glm::pi<float>() * ((float)rand() / (RAND_MAX));
						noise_z = 1. - 2. * ((float)rand() / (RAND_MAX));
						noise_x = sqrt(1 - pow(noise_z, 2)) * cos(phi);
						noise_y = sqrt(1 - pow(noise_z, 2)) * sin(phi);
						*/
						if (abs(sx) < 1e-4 && abs(sy) < 1e-4 && abs(sz) < 1e-4) {
							/*vx = Orts[3 * i];
							vy = Orts[3 * i + 1];
							vz = Orts[3 * i + 2];
							Hypot = sqrt(pow(vx, 2) + pow(vy, 2) + pow(vz, 2));
							Orts[3 * i] = vx / Hypot;
							Orts[3 * i + 1] = vy / Hypot;
							Orts[3 * i + 2] = vz / Hypot;*/
						}
						else {
							Hypot = sqrt(pow(sx, 2) + pow(sy, 2) + pow(sz, 2));
							Orts[3 * i] = sx / Hypot;
							Orts[3 * i + 1] = sy / Hypot;
							Orts[3 * i + 2] = sz / Hypot;
							//Hypot = sqrt(pow(Orts[3 * i], 2) + pow(Orts[3 * i + 1], 2) + pow(Orts[3 * i + 2], 2));
							//Orts[3 * i] = Orts[3 * i] / Hypot;
							//Orts[3 * i + 1] = Orts[3 * i + 1] / Hypot;
							//Orts[3 * i + 2] = Orts[3 * i + 2] / Hypot;
						}


						

						Hypot = sqrt(pow(Vel[3 * i], 2) + pow(Vel[3 * i + 1], 2) + pow(Vel[3 * i + 2], 2));

						x = vertices[3 * (size_t)i];
						y = vertices[3 * (size_t)i + 1];
						z = vertices[3 * (size_t)i + 2];

						vx = Orts[3 * i] * Hypot;
						vy = Orts[3 * i + 1] * Hypot;
						vz = Orts[3 * i + 2] * Hypot;

						x1 = x + vx * coef;
						y1 = y + vy * coef;
						z1 = z + vz * coef;

						if (pow(x1, 2) + pow(y1, 2) + pow(z1, 2) >= pow((*radius), 2))
						{
							SQRT = pow((x * vx + y * vy + z * vz) / (pow(vx, 2) + pow(vy, 2) + pow(vz, 2)), 2) + (pow((*radius), 2) - pow(x, 2) - pow(y, 2) - pow(z, 2)) / (pow(vx, 2) + pow(vy, 2) + pow(vz, 2));
							dt1 = fabs(sqrt(fabs(SQRT)) - (x * vx + y * vy + z * vz) / (pow(vx, 2) + pow(vy, 2) + pow(vz, 2)));

							x2 = x + vx * dt1;
							y2 = y + vy * dt1;
							z2 = z + vz * dt1;

							Velx = vx;
							Vely = vy;
							Velz = vz;

							nx = -x2 / (*radius);
							ny = -y2 / (*radius);
							nz = -z2 / (*radius);

							Veln = Velx * nx + Vely * ny + Velz * nz;
							Velx1 = Velx - 2 * nx * Veln;
							Vely1 = Vely - 2 * ny * Veln;
							Velz1 = Velz - 2 * nz * Veln;

							vx = Velx1;
							vy = Vely1;
							vz = Velz1;

							x = x2 + vx * (coef - dt1);
							y = y2 + vy * (coef - dt1);
							z = z2 + vz * (coef - dt1);

						}
						else {
							x = x1;
							y = y1;
							z = z1;
						}
						vertices[3 * (size_t)i] = x;
						vertices[3 * (size_t)i + 1] = y;
						vertices[3 * (size_t)i + 2] = z;

						Vel[3 * (size_t)i] = vx;
						Vel[3 * (size_t)i + 1] = vy;
						Vel[3 * (size_t)i + 2] = vz;

						i = lscl[i];
					}



				}
	}
}


extern void cudaInit(unsigned int VBO, float* vertices, float* Vel, int* head, int* lscl, float* Orts, const float radius, const float inter, const int size, const int L)
{
	cudaGraphicsGLRegisterBuffer(&cudaVBO, VBO, cudaGraphicsRegisterFlagsWriteDiscard);

	cudaMalloc(&vertices_dev, size * sizeof(float) * 3);
	cudaGraphicsMapResources(1, &cudaVBO, NULL);
	cudaGraphicsResourceGetMappedPointer((void**)&vertices_dev, NULL, cudaVBO);
	cudaMemcpy(vertices_dev, vertices, size * sizeof(float) * 3, cudaMemcpyHostToDevice);
	
	cudaMalloc(&Vel_dev, size * sizeof(float) * 3);
	cudaMemcpy(Vel_dev, Vel, size * sizeof(float) * 3, cudaMemcpyHostToDevice);

	cudaMalloc(&head_dev, (L * L * L * L) * sizeof(int));
	cudaMemcpy(head_dev, head, (L * L * L) * sizeof(int), cudaMemcpyHostToDevice);

	cudaMalloc(&lscl_dev, size * sizeof(int));
	cudaMemcpy(lscl_dev, lscl, size * sizeof(int), cudaMemcpyHostToDevice);

	cudaMalloc(&Orts_dev, size * sizeof(float) * 3);
	cudaMemcpy(Orts_dev, Orts, size * sizeof(float) * 3, cudaMemcpyHostToDevice);

	cudaMalloc(&radius_dev, sizeof(float));
	cudaMemcpy(radius_dev, &radius, sizeof(float), cudaMemcpyHostToDevice);

	cudaMalloc(&inter_dev, sizeof(float));
	cudaMemcpy(inter_dev, &inter, sizeof(float), cudaMemcpyHostToDevice);
	
	cudaMalloc(&L_dev, sizeof(int));
	cudaMemcpy(L_dev, &L, sizeof(int), cudaMemcpyHostToDevice);
	
	cudaMalloc(&c_dev, sizeof(int));
	cudaMemcpy(c_dev, &size, sizeof(int), cudaMemcpyHostToDevice);
	
}

extern void kernelUpdate(const int size, const int L)
{
	
	
	dim3 threadIn(512);
	dim3 blockIn(size / threadIn.x + 1);
	addKernel << <blockIn, threadIn >>> (Vel_dev, Orts_dev, c_dev);
	gpuErrchk(cudaPeekAtLastError());
	dim3 threadNull(512);
	int L3 = L * L * L;
	dim3 BlockNull(L3 / 512 + 1);
	nullKernel << <BlockNull, threadNull >> > (head_dev, L_dev);
	gpuErrchk(cudaPeekAtLastError());
	head_kernel << <1, 1 >> > (head_dev, vertices_dev, lscl_dev, radius_dev, inter_dev, L_dev, c_dev);
	gpuErrchk(cudaDeviceSynchronize());
	gpuErrchk(cudaPeekAtLastError());
	dim3 thread(8, 8, 8);
	dim3 blocks(L / thread.x + 1, L / thread.y + 1, L / thread.z + 1);
	main_dev << <blocks, thread >> > (vertices_dev, Vel_dev, head_dev, lscl_dev, Orts_dev, radius_dev, inter_dev, c_dev, L_dev);
	
	gpuErrchk(cudaPeekAtLastError());
	
	
	
}

extern void kernelTerminate()
{
	cudaGraphicsUnmapResources(1, &cudaVBO, NULL);
	cudaFree(vertices_dev);
	cudaFree(Vel_dev);
	cudaFree(Orts_dev);
	cudaFree(head_dev);
	cudaFree(lscl_dev);
	cudaFree(radius_dev);
	cudaFree(inter_dev);
	cudaFree(L_dev);
	cudaFree(c_dev);
}