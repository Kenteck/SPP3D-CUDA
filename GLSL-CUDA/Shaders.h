#pragma once
//////////////////////////////////////////////////////Sphere////////////////////////////////////////////////
const char* VertexShaderSourceSphere = "#version 400 core\n"
///////////////////data position/////////////////////////////
"layout (location = 0) in vec3 aPos;\n"
/////////////////used data//////////////////////////////////////
//"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"gl_Position = projection * view  * vec4(aPos, 1.0);\n"
"}\0";

const char* FragmentShaderSourceSphere = "#version 400 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
"}\0";
//////////////////////////////////////////////////////Particles////////////////////////////////////////////////
const char* VertexShaderSourceParticle = "#version 400 core\n"
///////////////////data position/////////////////////////////
"layout (location = 0) in vec3 aPos;\n"
/////////////////used data//////////////////////////////////////
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"gl_Position = projection * view  * vec4(aPos, 1.0);\n"
"gl_PointSize = max(1000.0 / gl_Position.w, 1.0);\n"
"}\0";

const char* FragmentShaderSourceParticle = "#version 400 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
"}\0";


////////////////////////////////////////////////Sphere///////////////////////////////////////////////////
unsigned int InitSphereShader()
{
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &VertexShaderSourceSphere, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR VERTEX SHADER SPHERE COMPILATION: " << infoLog << std::endl;
	}


	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &FragmentShaderSourceSphere, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR FRAGMENT SHADER SPHERE COMPILATION: " << infoLog << std::endl;
	}

	///////////////////////////////////////////shader program///////////////////////////////////////////
	unsigned int ShaderProgram;
	ShaderProgram = glCreateProgram();
	glAttachShader(ShaderProgram, vertexShader);
	glAttachShader(ShaderProgram, fragmentShader);
	glLinkProgram(ShaderProgram);

	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(ShaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR SHADER PROGRAM LINK: " << infoLog << std::endl;
	}


	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return ShaderProgram;
}


////////////////////////////////////////////////Particles///////////////////////////////////////////////////
unsigned int InitParticleShader()
{
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &VertexShaderSourceParticle, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR VERTEX SHADER PARTICLE COMPILATION: " << infoLog << std::endl;
	}


	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &FragmentShaderSourceParticle, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR FRAGMENT SHADER PARTICLE COMPILATION: " << infoLog << std::endl;
	}

	///////////////////////////////////////////shader program///////////////////////////////////////////
	unsigned int ShaderProgram;
	ShaderProgram = glCreateProgram();
	glAttachShader(ShaderProgram, vertexShader);
	glAttachShader(ShaderProgram, fragmentShader);
	glLinkProgram(ShaderProgram);

	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(ShaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR SHADER PROGRAM LINK: " << infoLog << std::endl;
	}


	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return ShaderProgram;
}