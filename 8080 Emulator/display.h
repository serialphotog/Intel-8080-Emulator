#pragma once

#include "cpu.h"

#include <SFML/Graphics.h>
#include <SFML/System/Clock.h>

/**
 * Stores the current state of the display
*/
typedef struct DisplayState {
	sfWindow *renderWindow; // The window to render the display to
	sfClock *frameClock;
} DisplayState;

/**
 * Initializes and returns the display state
*/
DisplayState* initDisplay();

/**
 * Runs the display
*/
void runDisplay(DisplayState *state);

/**
 * Destroys the display object
*/
void destroyDisplay(DisplayState *state);