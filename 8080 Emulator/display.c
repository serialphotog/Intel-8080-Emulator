#include "display.h"

#include <stdlib.h>

// Initializes the display state
DisplayState* initDisplay()
{
	DisplayState *state = calloc(1, sizeof(DisplayState));

	// Initialize the render window
	sfVideoMode mode = { 800, 600 };
	state->renderWindow = sfWindow_create(mode, "Intel 8080 Emulator", sfResize | sfClose, NULL);
	sfWindow_setFramerateLimit(state->renderWindow, 120);

	// Initialize the frame clock
	state->frameClock = sfClock_create();

	return state;
}

// Runs the display
void runDisplay(DisplayState *state)
{
	while (sfWindow_isOpen(state->renderWindow)) 
	{
		sfEvent event;
		while (sfWindow_pollEvent(state->renderWindow, &event)) 
		{
			// Handle the close window event
			if (event.type == sfEvtClosed)
			{
				sfWindow_close(state->renderWindow);
			}
		}
	}
}

// Destroy the display
void destroyDisplay(DisplayState *state)
{
	sfClock_destroy(state->frameClock);
	sfWindow_destroy(state->renderWindow);
}