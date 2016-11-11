//Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Prototypes
void render(SDL_Renderer *renderer);

bool done = false;

int main(int argc, char* args[]) {
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else {
		//Create window
		window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL) {
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
		else {
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			if (renderer == NULL) {
				printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
			}
			else {
				SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
				render(renderer);
			}
		}
	}

	// Destroy created objects
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	//Quit SDL subsystems
	SDL_Quit();

	return 0;
}

void render(SDL_Renderer *renderer) {
	SDL_Event e;

	double dt = 0;

	Uint64 timeNow = SDL_GetPerformanceCounter();
	Uint64 timeLast = 0;

	// Texture we'll be writing to
	SDL_Texture *texture = SDL_CreateTexture(renderer,
		SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);

	// Pixels of the texture
	Uint32 *pixels = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT];
	SDL_memset(pixels, 0, 640 * 480 * sizeof(Uint32));

	while (!done) {
		timeLast = timeNow;
		timeNow = SDL_GetPerformanceCounter();
		dt = (double)((timeNow - timeLast) * 1000 / SDL_GetPerformanceFrequency());

		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				done = true;
			}
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// Clear pixels
		SDL_memset(pixels, 0, 640 * 480 * sizeof(Uint32));

		for (int x = 0; x < SCREEN_WIDTH; x++) {
			for (int y = 0; y < SCREEN_HEIGHT; y++) {
				int r = x % 255;
				int g = y % 255;
				int b = (x*y) % 255;

				pixels[x + SCREEN_WIDTH*y] = (r << 16) | (g << 8) | (b << 0);
			}
		}

		for (int y = 0; y < SCREEN_HEIGHT; y++) {
			pixels[(SCREEN_WIDTH / 8) + SCREEN_WIDTH*y] = 255 << 16;
		}

		for (int x = 0; x < (int)((dt / 16.0) * (SCREEN_WIDTH / 8.0)); x++) {
			pixels[x + SCREEN_WIDTH*200] = (255 << 16) | (255 << 8) | (255 << 0);
			pixels[x + SCREEN_WIDTH*201] = (255 << 16) | (255 << 8) | (255 << 0);
			pixels[x + SCREEN_WIDTH*202] = (255 << 16) | (255 << 8) | (255 << 0);
		}

		SDL_UpdateTexture(texture, NULL, pixels, 640 * sizeof(Uint32));
		SDL_RenderCopy(renderer, texture, NULL, NULL);

		SDL_RenderPresent(renderer);
	}

	delete[] pixels;
	SDL_DestroyTexture(texture);
}