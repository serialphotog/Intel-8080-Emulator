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

#include "display/display.h"

#include <GL/gl.h>
#include <stdlib.h>

// Initializes OpenGL
static void initGL(GtkWidget *glarea, gpointer data)
{
	DisplayState *displayState = (DisplayState*) data;
	gtk_gl_area_make_current(GTK_GL_AREA(glarea));

	// TODO: shaders
}

// Renders the view contents
static gboolean render(GtkGLArea *glarea, GdkGLContext *context, gpointer data)
{
	DisplayState *displayState = (DisplayState*) data;
	CPUState *state = displayState->state;

	// Clear the buffer
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// TODO: Draw stuff!

	return TRUE;
}

void initializeWindow(GtkApplication *app, gpointer userData)
{
	GtkWidget *window = gtk_application_window_new(app);
	GtkWidget *glarea;
	CPUState *state;

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