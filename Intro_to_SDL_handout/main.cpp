// ----------------------------------------------------------------
// Awesome simple game with SDL
// http://wiki.libsdl.org/APIByCategory
// ----------------------------------------------------------------

#include "stdlib.h"
#include "string.h"

#include "SDL\include\SDL.h"
#include "SDL_image\include\SDL_image.h"
#include "SDL_mixer\include\SDL_mixer.h"
// TODO 1: Add SDL_mixer library
// You need to add the include file (.h)
// The library (.lib) and make sure all .dll
// are in the executable's folder. Everything 
// is in the folder file already.

#pragma comment( lib, "SDL/libx86/SDL2.lib" )
#pragma comment( lib, "SDL/libx86/SDL2main.lib" )
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

// Globals --------------------------------------------------------
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define SCROLL_SPEED 7
#define SHIP_SPEED 10
#define ENEMY_SPEED 10
#define NUM_SHOTS 32
#define SHOT_SPEED 20
#define MAX_KEYS 300

enum KEY_STATE
{
	KEY_IDLE,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

struct projectile
{
	int x, y;
	bool alive;
};

struct globals
{
	SDL_Window* window;
	SDL_Surface* w_surface;
	SDL_Renderer* renderer;
	SDL_Texture* background;
	int background_width;
	SDL_Texture* ship;
	int ship_x;
	int ship_y;
	SDL_Texture* enemy0;
	int enemy0_x;
	int enemy0_y;
	SDL_Texture* enemy_explode_0;
	int enemy_explode_0_x;
	int enemy_explode_0_y;
	SDL_Texture* enemy_explode_1;
	int enemy_explode_1_x;
	int enemy_explode_1_y;
	SDL_Texture* shot;
	projectile shots[NUM_SHOTS];
	int last_shot;
	bool fire, up, down, left, right;
	KEY_STATE* keyboard;
	int hit;


	// TODO 4:
	// Add pointers to store music and the laser fx
	Mix_Music *music;
	Mix_Chunk *sample;

	int direction;
	int scroll;
} g;

// ----------------------------------------------------------------
void Start()
{
	g.direction = 0;



	SDL_Init(SDL_INIT_EVERYTHING);

	// Create window --
	g.window = SDL_CreateWindow("Super Awesome Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	g.w_surface = SDL_GetWindowSurface(g.window);

	// Create renderer --
	g.renderer = SDL_CreateRenderer(g.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_SetRenderDrawColor(g.renderer, 0, 0, 0, 255);

	// Load image lib --
	IMG_Init(IMG_INIT_PNG);
	g.background = SDL_CreateTextureFromSurface(g.renderer, IMG_Load("background.png"));
	g.ship = SDL_CreateTextureFromSurface(g.renderer, IMG_Load("ship.png"));
	g.enemy0 = SDL_CreateTextureFromSurface(g.renderer, IMG_Load("enemy0.png"));
	g.enemy_explode_0 = SDL_CreateTextureFromSurface(g.renderer, IMG_Load("enemy_explode_0.png"));
	g.enemy_explode_1 = SDL_CreateTextureFromSurface(g.renderer, IMG_Load("enemy_explode_1.png"));
	g.shot = SDL_CreateTextureFromSurface(g.renderer, IMG_Load("shot.png"));
	SDL_QueryTexture(g.background, NULL, NULL, &g.background_width, NULL);

	Mix_Init(MIX_INIT_OGG);
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);

	// Create mixer --
	// TODO 2:
	// Initialize the audio library 
	// Check https://www.libsdl.org/projects/SDL_mixer/docs/SDL_mixer.html#SEC7

	
	g.music = Mix_LoadMUS("music.ogg");
	Mix_PlayMusic(g.music, -1);

	
	g.sample = Mix_LoadWAV("laser.wav");
	

	// TODO 5:
	// Load music.ogg and play it. Store the pointer to it.
	// Load the laser.wav file and store a pointer to it

	// Init other vars --
	g.scroll = 0;
	g.ship_x = 100;
	g.ship_y = SCREEN_HEIGHT / 2;
	g.enemy0_x = 800;
	g.enemy0_y = 300;
	g.hit = 0;
	g.fire = g.up = g.down = g.left = g.right = false;
	g.last_shot = 0;
	g.keyboard = (KEY_STATE*)malloc(sizeof(KEY_STATE) * MAX_KEYS);
	memset(g.keyboard, KEY_IDLE, MAX_KEYS);
}

// ----------------------------------------------------------------
void Finish()
{
	// TODO 6:
	// Stop and free the music and fx
	Mix_FreeMusic(g.music);
	Mix_FreeChunk(g.sample);


	// TODO 3:
	// Close the audio library

	Mix_CloseAudio();
	Mix_Quit();

	SDL_DestroyTexture(g.background);
	SDL_DestroyTexture(g.ship);
	SDL_DestroyTexture(g.enemy0);
	SDL_DestroyTexture(g.enemy_explode_0);
	SDL_DestroyTexture(g.enemy_explode_1);
	IMG_Quit();
	SDL_DestroyRenderer(g.renderer);
	SDL_DestroyWindow(g.window);
	SDL_Quit();
	free(g.keyboard);
}

// ----------------------------------------------------------------
bool CheckInput()
{
	SDL_PumpEvents();
	const Uint8* keys = SDL_GetKeyboardState(NULL);

	// cyle keyboard keys
	for(int i = 0; i < MAX_KEYS; ++i)
	{	
		if(keys[i] == 1)
			g.keyboard[i] = (g.keyboard[i] == KEY_IDLE) ? KEY_DOWN : KEY_REPEAT;
		else
			g.keyboard[i] = (g.keyboard[i] == KEY_REPEAT || g.keyboard[i] == KEY_DOWN) ? KEY_UP : KEY_IDLE;
	}

	if(g.keyboard[SDL_SCANCODE_ESCAPE] == KEY_DOWN)
		return false;

	g.up = g.keyboard[SDL_SCANCODE_W] == KEY_REPEAT;
	g.down = g.keyboard[SDL_SCANCODE_S] == KEY_REPEAT;
	g.left = g.keyboard[SDL_SCANCODE_A] == KEY_REPEAT;
	g.right = g.keyboard[SDL_SCANCODE_D] == KEY_REPEAT;
	g.fire = g.keyboard[SDL_SCANCODE_SPACE] == KEY_DOWN;

	return true;
}

// ----------------------------------------------------------------
void MoveStuff()
{
	// Calc new ship position
	// TODO 8: move the ship
	// Knowing the booleans up/down/left/right
	// hold true if the player is trying to move in that direction
	// calc player's next position (stored in g.x and g.y)

	if (g.up != NULL){
		if (g.ship_y < 0){
			//g.ship_y = g.ship_y;
		}
		else{
			g.ship_y -= SHIP_SPEED;
		}
	}
	if (g.right != NULL){
		if (g.ship_x > 700){
			//g.ship_x = g.ship_x;
		}
		else{
			g.ship_x += SHIP_SPEED;
		}
	}
	if (g.down != NULL){
		if (g.ship_y > 704){
			//g.ship_y = g.ship_y;
		}
		else{
			g.ship_y += SHIP_SPEED;
		}
	}
	if (g.left != NULL){
		if (g.ship_x < -3){
			//g.ship_x = g.ship_x;
		}
		else{
			g.ship_x -= SHIP_SPEED;
		}
	}
	//enemy move
	if (g.direction == 0){
		g.enemy0_y += ENEMY_SPEED;
		if (g.enemy0_y >= (SCREEN_HEIGHT-100)){
			g.direction = 1;
		}
	}
	else{
		g.enemy0_y -= ENEMY_SPEED;
		if (g.enemy0_y <= 0){
			g.direction = 0;
		}
	}


	if(g.fire)
	{
		Mix_PlayChannel(-1, g.sample, 0);
		g.fire = false;

		if(g.last_shot == NUM_SHOTS)
			g.last_shot = 0;

		g.shots[g.last_shot].alive = true;
		g.shots[g.last_shot].x = g.ship_x + 32;
		g.shots[g.last_shot].y = g.ship_y;
		g.last_shot++;
	}

	for(int i = 0; i < NUM_SHOTS; ++i)
	{
		if(g.shots[i].alive)
		{
			if (g.shots[i].x < SCREEN_WIDTH){
				if (g.shots[i].x > 600 && g.shots[i].x < 650){
					g.hit++;
				}
				g.shots[i].x += SHOT_SPEED;
			}
			else
				g.shots[i].alive = false;
		}
	}
}

// ----------------------------------------------------------------
void Draw()
{
	// Clear screen to black
	SDL_RenderClear(g.renderer);

	SDL_Rect target;//A structure that contains the definition of a rectangle, with the origin at the upper left.
	SDL_Rect target1;
	SDL_Rect enemy0;

	/*
	data fields
	int x - the x location of the rectangle's upper left corner
    int y - the y location of the rectangle's upper left corner
    int w - the width of the rectangle
    int h - the height of the rectangle
	*/

	// Draw the background and scroll --
	// TODO 9: scroll the background
	// you should move target.x over time
	// Remember that you have to draw the
	// background twice to fake repetition

	target.x = 0;
	target.y = 0;
	target.w = g.background_width;
	target.h = SCREEN_HEIGHT;
	g.scroll -= SCROLL_SPEED;
	target.x = g.scroll;

	if (target.x <= -3071){
		g.scroll = 0;
	}

	target1.x = 3072;//Background width
	target1.y = 0;
	target1.w = g.background_width;
	target1.h = SCREEN_HEIGHT;
	target1.x = g.scroll + 3072;

	SDL_RenderCopy(g.renderer, g.background, NULL, &target);//Use this function to copy a portion of the texture to the current rendering target.
	SDL_RenderCopy(g.renderer, g.background, NULL, &target1);

	// Draw the ship --
	target.x = g.ship_x;
	target.y = g.ship_y;
	target.w = 64;
	target.h = 64;
	SDL_RenderCopy(g.renderer, g.ship, NULL, &target);

	// Draw lasers --
	target.w = 64;
	target.h = 64;

	for(int i = 0; i < NUM_SHOTS; ++i)
	{
		if(g.shots[i].alive)
		{
			target.x = g.shots[i].x;
			target.y = g.shots[i].y;
			SDL_RenderCopy(g.renderer, g.shot, NULL, &target);
		}
	}

	//Draw Enemy
	target.x = g.enemy0_x;
	target.y = g.enemy0_y;
	target.w = 200;
	target.h = 200;
	if (g.hit >= 15){
		SDL_RenderCopy(g.renderer, g.enemy_explode_0, NULL, &target);
	}
	else{
		SDL_RenderCopy(g.renderer, g.enemy0, NULL, &target);
	}
	


	// Finally present framebuffer
	SDL_RenderPresent(g.renderer);
}

// ----------------------------------------------------------------
int main(int argc, char* argv[])
{
	Start();

	while(CheckInput())
	{
		MoveStuff();
		Draw();
	}

	Finish();
	return(EXIT_SUCCESS);
}

// TODO 7: Play the laser fx 
// (you need to find where this should happen)