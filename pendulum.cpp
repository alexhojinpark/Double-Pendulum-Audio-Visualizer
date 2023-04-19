#include "SDL.h"
#include<iostream>
#include<string>
#include<cmath>

using namespace std;

// SDL SETUP
#define SCREEN_WIDTH 720
#define SCREEN_HEIGHT 480

// Pendulum position offset
#define OFFSET_X 360
#define OFFSET_Y 200

#define PI 3.14

// Frames
int frameCount, timerFPS, lastFrame, fps;

// Radius of the ball
float r1 = 100;
float r2 = 100;

// Mass of the ball
float m1 = 10;
float m2 = 10;

// Angle
float theta1 = PI / 2;
float theta2 = PI / 2;

// Starting velocity of the ball
float v1 = 0;
float v2 = 0;

float x2, y2;

// Gravity
float g = 1;

// Swaping Value between 2 parameters
void SwapValue(float& a, float& b) {
	float tmp = a;
	a = b;
	b = tmp;
}

// Draws the ball using Bresenham's circle drawing algorithm
void DrawBall(SDL_Renderer* renderer, int x0, int y0, int radius) {
	int x = radius;
	int y = 0;
	int xChange = 1 - (radius * 2);
	int yChange = 0;
	int radiusError = 0;

	// Loop through each pixel on the circle's circumference
	while (x >= y) {
		for (int i = x0 - x; i < x0 + x; i++) {
			SDL_RenderDrawPoint(renderer, i, y0 + y);
			SDL_RenderDrawPoint(renderer, i, y0 - y);
		}
		for (int i = x0 - y; i <= x0 + y; i++) {
			SDL_RenderDrawPoint(renderer, i, y0 + x);
			SDL_RenderDrawPoint(renderer, i, y0 - x);
		}
		y++;
		radiusError += yChange;
		yChange += 2;
		if (((radiusError << 1) + xChange) > 0) {
			x--;
			radiusError += xChange;
			xChange += 2;
		}
	}
}

// Draws a line from (start_x, start_y) to (end_x, end_y)
void DrawString(SDL_Renderer* renderer, float start_x, float start_y, float end_x, float end_y) {
	// Determine whether the line should be drawn from left to right or from top to bottom
	bool step = abs(end_x - start_x) < abs(end_y - start_y);
	if (step) {
		SwapValue(start_x, start_y);
		SwapValue(end_x, end_y);
	}
	if (end_x < start_x) {
		SwapValue(start_x, end_x);
		SwapValue(start_y, end_y);
	}

	// Initialize variables for line drawing
	float error = 0.0;
	float roundError = (float)abs(end_y - start_y) / (end_x - start_x);

	int y = start_y;
	int ystep = (end_y > start_y ? 1 : -1);

	// Iterate over the x coordinates of the line, drawing each pixel along the way
	for (int i = start_x; i < end_x; i++) {
		if (step) {
			SDL_RenderDrawPoint(renderer, y, i);
		}
		else {
			SDL_RenderDrawPoint(renderer, i, y);
		}
		error += roundError;
		// If the error exceeds a certain threshold, move to the next row of pixels
		if (error >= 0.5) {
			y += ystep;
			error -= 1.0;
		}
	}
}


// Implements a double pendulum using the given physics equations
// and renders it on the SDL_Renderer passed as argument.
void DoublePen(SDL_Renderer* renderer) {
	// Calculate the angular acceleration of the a1 and a2 pendulum
	float a1 =
		((-g * (2 * m1 + m2) * sin(theta1)) + (-m2 * g * sin(theta1 - 2 * theta2)) + (-2 * sin(theta1 - theta2) * m2) * (pow(v2, 2) * r2 + pow(v1, 2) * r1 * cos(theta1 - theta2)))
		/ (r1 * (2 * m1 + m2 - m2 * cos(2 * theta1 - 2 * theta2)));

	float a2 =
		((2 * sin(theta1 - theta2)) * (((pow(v1, 2) * r1 * (m1 + m2)) + (g * (m1 + m2) * cos(theta1)) + (pow(v2, 2) * r2 * m2 * cos(theta1 - theta2))))
			/ (r2 * (2 * m1 + m2 - m2 * cos(2 * theta1 - 2 * theta2))));

	// Calculate the position of the first pendulum based on its angular displacement and add offset
	float x1 = r1 * sin(theta1) + OFFSET_X;
	float y1 = r1 * cos(theta1) + OFFSET_Y;

	// Calculate the position of the second pendulum
	// based on its angular displacement relative to the first pendulum
	x2 = x1 + r2 * sin(theta2);
	y2 = y1 + r2 * cos(theta2);

	// Render the strings, balls and line segments representing the pendulum on the renderer
	DrawString(renderer, OFFSET_X, OFFSET_Y, x1, y1);
	DrawBall(renderer, x1, y1, m1);
	DrawString(renderer, x1, y1, x2, y2);
	DrawBall(renderer, x2, y2, m2);

	// Update the angular velocity and displacement of the pendulums based on their acceleration
	v1 += a1;
	v2 += a2;
	theta1 += v1;
	theta2 += v2;
}

/*
	This is a callback function that is used with SDL_AudioSpec to generate audio samples.
	The function takes in a pointer to some user data, an audio buffer stream, and the length of the buffer.
	It generates an audio waveform based on some parameters and fills the stream with the generated samples.
	The waveform is a square wave, sawtooth wave, sine wave or cosine wave, depending on the chosen line position.
	The amplitude, sampling frequency, and frequency range are also defined in the function.
*/
void Func_Callback(void* userdata, Uint8* stream, int len)
{
	const double A = 50.0; // amplitude
	const double Fs = 44100.0; // sampling frequency
	const double f_min = 100.0; // minimum frequency
	const double f_max = 1000.0; // maximum frequency

	for (int i = 0; i < len; i++)
	{
		double t = (double)i / Fs; // current time
		double f = f_min + (f_max - f_min) * (double)x2 / SCREEN_WIDTH; // frequency depends on x2 position
		
		double s = (sin(2 * M_PI * f * t) > 0) ? A : -A; // square wave
		// double s = A * (2 * (t * f - floor(0.5 + t * f))); // sawtooth wave
		// double s = A * sin(2 * M_PI * f * t); // sine wave
		// double s = A * cos(2 * M_PI * f * t); // cosine wave

		stream[i] = (Uint8)(s + 128); // convert to unsigned 8-bit audio samples
	}
}

int main(int argc, char *argv[])
{	
	SDL_Init(SDL_INIT_EVERYTHING);
	
	SDL_AudioSpec as;

	as.freq = 22000;
	as.format = AUDIO_U8;
	as.samples = 1024;
	as.callback = Func_Callback;
	as.userdata = NULL;
	as.channels = 1;

	if (SDL_OpenAudio(&as, NULL) < 0) {
		cout << "Unable to open audio" << '\n';
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow("Double Pendulum Audio Visualizer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_Texture* my_layer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetTextureBlendMode(my_layer, SDL_BLENDMODE_BLEND);

	bool running = true;
	static int lastTime = 0;

	while (running) {
		lastFrame = SDL_GetTicks();
		if (lastFrame >= (lastTime + 1000)) {
			lastTime = lastFrame;
			fps = frameCount;
			frameCount = 0;
			// cout << fps << endl; // Print FPS
		}

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type)
			{
			case SDL_QUIT:
				running = false;
				break;

			default:
				break;
			}
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		DoublePen(renderer);

		SDL_SetRenderTarget(renderer, my_layer);
		SDL_RenderDrawPoint(renderer, x2, y2);

		SDL_SetRenderTarget(renderer, NULL);
		SDL_RenderCopy(renderer, my_layer, NULL, NULL);
		SDL_RenderPresent(renderer);

		frameCount++;
		timerFPS = SDL_GetTicks() - lastFrame;
		if (timerFPS < (1000 / 60)) {
			SDL_Delay((1000 / 60) - timerFPS);
		}
		SDL_PauseAudio(0);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_QUIT;

	return 0;
}
