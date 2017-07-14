#pragma once

#include <GLAD/glad.h>

#include <fstream>
#include <string>

class Shader
{
public:
	Shader(const std::string &vsFilename, const std::string &fsFilename)
	{
		unsigned int VertexShader = compileShader(GL_VERTEX_SHADER, vsFilename);
		unsigned int FragmentShader = compileShader(GL_FRAGMENT_SHADER, fsFilename);

		ID = glCreateProgram();
		glAttachShader(ID, VertexShader);
		glAttachShader(ID, FragmentShader);
		glLinkProgram(ID);

		int success;
		glGetProgramiv(ID, GL_LINK_STATUS, &success);

		if (!success)
		{
			char info[512];
			glGetProgramInfoLog(ID, 512, NULL, info);
			std::cout << "Error: Program did not link successfully. InfoLog = " << info << std::endl;
		}

		glDeleteShader(VertexShader);
		glDeleteShader(FragmentShader);
	}

	void use()
	{
		glUseProgram(ID);
	}

	unsigned int getID()
	{
		return ID;
	}

	void setUniform(const std::string& name, int value) { glUniform1i(glGetUniformLocation(ID, name.c_str()), value); }
	void setUniform(const std::string& name, bool value) { glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); }
	void setUniform(const std::string& name, float value) { glUniform1f(glGetUniformLocation(ID, name.c_str()), value); }

private:
	unsigned int compileShader(GLenum type, std::string shadername)
	{
		unsigned int shader;
		shader = glCreateShader(type);

		std::ifstream shaderStream(shadername.c_str());
		std::string shaderCode = std::string(std::istreambuf_iterator<char>(shaderStream), std::istreambuf_iterator<char>());
		const char* shaderCodePtr = shaderCode.c_str();

		glShaderSource(shader, 1, &shaderCodePtr, NULL);
		glCompileShader(shader);

		int success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			char info[512];
			glGetShaderInfoLog(shader, 512, NULL, info);
			std::cout << "Error: Shader did not compile sucessfully. InfoLog = " << info << std::endl;
		}

		return shader;
	}

	unsigned int ID;
};