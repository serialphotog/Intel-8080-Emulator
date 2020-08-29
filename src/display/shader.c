/*******************************************************************************
 * File: shader.c
 *
 * Purpose:
 *		Handles OpenGL shader programs.
 *
 * Copyright 2020 Adam Thompson <adam@serialphotog.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT .IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 ******************************************************************************/

#include "display/shader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Reads a shader program from disk into a string
static int readShader(const char *path, char **out)
{
	FILE *file = fopen(path, "r");
	if (!file) 
	{
		printf("[ERROR]: Failed to open the shader: %s\n", path);
		return 0;
	}

	// Read the shader code
	fseek(file, 0L, SEEK_END);
	int size = ftell(file);
	fseek(file, 0L, SEEK_SET);
	*out = malloc(size);
	fread(*out, size, 1, file);

	if (ferror(file))
	{
		printf("[ERROR]: Failed to read shader: %s\n", path);
		fclose(file);
		return 0;
	}

	// Make sure the string is null-terminated
	(*out)[size] = '\0';
	fclose(file);
	return 1;
}

GLuint loadShaders(const char *vertexPath, const char *fragmentPath)
{
	GLint result = GL_FALSE;
	int msgSize;
	GLuint vertexID, fragmentID;

	// Vertex shader
	vertexID = glCreateShader(GL_VERTEX_SHADER);
	char *vertexCode;
	if (!readShader(vertexPath, &vertexCode))
		return 0;

	printf("[INFO]: Compiling the vertex shader: %s\n", vertexPath);
	glShaderSource(vertexID, 1, &vertexCode, NULL);
	glCompileShader(vertexID);
	glGetShaderiv(vertexID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertexID, GL_INFO_LOG_LENGTH, &msgSize);

	if (msgSize > 0)
	{
		char msg[msgSize + 1];
		glGetShaderInfoLog(vertexID, msgSize, NULL, msg);
		printf("[OpenGL]: %s\n", msg);
	}

	// Fragment shader
	fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
	char *fragmentCode;
	if (!readShader(fragmentPath, &fragmentCode))
		return 0;

	printf("[INFO]: Compiling the fragment shader: %s\n", fragmentPath);
	glShaderSource(fragmentID, 1, &fragmentCode, NULL);
	glCompileShader(fragmentID);
	glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragmentID, GL_INFO_LOG_LENGTH, &msgSize);

	if (msgSize > 0)
	{
		char msg[msgSize + 1];
		glGetShaderInfoLog(fragmentID, msgSize, NULL, msg);
		printf("[OpenGL]: %s\n", msg);
	}

	// Link the program
	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexID);
	glAttachShader(programID, fragmentID);
	glLinkProgram(programID);

	// Check the program
	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &msgSize);

	if (msgSize > 0)
	{
		char msg[msgSize + 1];
		glGetProgramInfoLog(programID, msgSize, NULL, msg);
		printf("[OpenGL]: %s\n", msg);
	}

	// Clean up
	glDetachShader(programID, vertexID);
	glDetachShader(programID, fragmentID);
	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);

	return programID;
}