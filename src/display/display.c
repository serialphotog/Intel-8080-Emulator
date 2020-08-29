/*******************************************************************************
 * File: display.c
 *
 * Purpose:
 *		Provides the display architecture for the emulator.
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
#include "display/display.h"

#include <stdlib.h>

// Initializes OpenGL
static void initGL(GtkWidget *glarea, gpointer data)
{
	DisplayState *displayState = (DisplayState*) data;
	gtk_gl_area_make_current(GTK_GL_AREA(glarea));

	glewExperimental = GL_TRUE; 
	glewInit();

	// Initialize the vertex buffer
	glGenVertexArrays(1, &displayState->glVertexBuffer);
	glBindVertexArray(displayState->glVertexBuffer);

	// Load the shaders
	displayState->glProgramID = loadShaders("shader.vertex", "shader.fragment");

	// Textures
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, COLS, ROWS, 0, GL_RGB, GL_UNSIGNED_BYTE, displayState->image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    float verticies[] = {
    	1.0f,  1.0f, 0.0f,   1.0f, 1.0f,   // top right
        1.0f, -1.0f, 0.0f,   0.0f, 1.0f,   // bottom right
       -1.0f, -1.0f, 0.0f,   0.0f, 0.0f,   // bottom left
       -1.0f,  1.0f, 0.0f,   1.0f, 0.0f    // top left
   };

   unsigned int indices[] = {
   		0, 1, 3,
   		1, 2, 3
   };

   glGenBuffers(1, &displayState->glVertexBuffer);
   glBindBuffer(GL_ARRAY_BUFFER, displayState->glVertexBuffer);
   glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
   glEnableVertexAttribArray(1);

   glGenBuffers(1, &displayState->glElementBuffer);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, displayState->glElementBuffer);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

   displayState->glTextureID = textureID;
}

// Renders the view contents
static gboolean render(GtkGLArea *glarea, GdkGLContext *context, gpointer data)
{
	DisplayState *displayState = (DisplayState*) data;
	CPUState *state = displayState->state;

	// Clear the buffer
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(displayState->glProgramID);

	// Draw the contents from the machines video memory
for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < 32; j++) 
		{
			for (int k = 0; k < 8; ++k)
			{
				displayState->image[(i * COLS + j) + k + (j * 8)] = (displayState->state->memory[0x2400 + i * 32 + j] & (1 << k)) == 0 ? 0 : 255;
			}
		}
	}

	glBindTexture(GL_TEXTURE_2D, displayState->glTextureID);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, COLS, ROWS, GL_LUMINANCE, GL_UNSIGNED_BYTE, displayState->image);

	glBindVertexArray(displayState->glVertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, displayState->glElementBuffer);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	return TRUE;
}

void initializeWindow(GtkApplication *app, gpointer userData)
{
	GtkWidget *window = gtk_application_window_new(app);
	GtkWidget *glarea;
	CPUState *state = userData;

	// Initialize the display state
	DisplayState *displayState = (DisplayState*) malloc(sizeof(DisplayState));
	displayState->state = state;

	// Initialize Open GL
	glarea = gtk_gl_area_new();
	gtk_gl_area_set_auto_render((GtkGLArea*) glarea, GL_FALSE);
	g_signal_connect(glarea, "realize", G_CALLBACK(initGL), displayState);
	g_signal_connect(glarea, "render", G_CALLBACK(render), displayState);
	gtk_container_add(GTK_CONTAINER(window), glarea);

	// Window properties
	gtk_window_set_title(GTK_WINDOW(window), "Intel 8080 Emulator"); // TODO: Possibly make this state dependent?
	gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	gtk_widget_show_all(window);

	// Run the main GTK loop
	gtk_main();
}