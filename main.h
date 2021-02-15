#pragma once
#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <vector>
#include <math.h>
#include <cmath>
#include <random>
#include <string>
#include <ctime>

extern "C" {
	const int WIDTH = 1080;
	const int HEIGHT = 720;

	constexpr auto pi = 3.1459;
	using namespace std;
	const int frame = 4;
	double mouseRadians;
	double mouseAngle;
	bool leftMousePressed = false;
	bool rightMousePressed = false;
	bool hold = false;
	int inventoryTIMER;
	bool inventorytimer;

	/* Animation related variables */
	int FRAME_NUM;
	const int maxFrames = 7;
	int nframes = 6;
	const double FPS = 60.0;
	const double SPF = 1.0 / FPS;
	double elapsed_time;
	double duration = 0.5;
	int frames = 0;
	int frametimer = 0; bool isTimer = false;
	int resourceFrames = 0;
	float drillradian;
	float lastpos;
	bool aboveIcon = false;

	bool isRunning = true;
	int mouseX, mouseY;
	bool showInventory = false;
	int invX, invY;
	int add = 0;

	/* Creating planets */
	class planets {
	public:
		int radius, x, y, angle;
		float radians;
		bool moving = false;
		planets(int aRadius, int aX, int aY, int aAngle, bool isMoving, float aRadians = 0) {
			radius = aRadius;
			x = aX; y = aY;
			angle = aAngle;
			moving = isMoving = false;
			radians = aRadians;
		}
	};
	planets planet1(204, 500, 300, 0, false, 0);

	/* Generating resources */
	class resources {
	public:
		ALLEGRO_BITMAP* resourceSprite;
		ALLEGRO_BITMAP* resourceArray[frame] = { NULL };
		int angle = 0; unsigned int startingcount;
		float radians = 0;
		unsigned int resourceCount;
		bool draw = true; bool increasing = false; int grow = 0;
		bool above = false;
		bool down = false;
		int resource_player = 0;
		int frame = 0;
		const char* player_c;

		resources(ALLEGRO_BITMAP* bmp, int radian, int aPlayer, int resource, ALLEGRO_BITMAP* icon = NULL) {
			resourceSprite = bmp;
			radians = radian * pi / 180;
			startingcount = resource;
			resourceCount = startingcount;

			resource_player = aPlayer;
		}
		void resourceSprites();
		float generateResourceNodes();
		void checkResources();
		const char* convert();
		void check();

	};
	/* Resource Objects */
	resources iron(NULL, 30, 200, 100, NULL);
	resources copper(NULL, 180, 100, 100, NULL);
	resources azium(NULL, 97, 0, 25, NULL);
	resources wood(NULL, 260, 0, 1, NULL);

	/* Creating machines */
	class machines {
	public:
		ALLEGRO_BITMAP* idleMachineArray[frame + 1] = { NULL };
		ALLEGRO_BITMAP* machineArray[frame + 1] = { NULL };
		ALLEGRO_BITMAP* machineSprite = NULL;
		ALLEGRO_BITMAP* machineIcon = NULL;
		float radians = 0;
		bool hold = false, place = false, draw = false;
		int machine_player = 0; //number of this machine the player has.
		int ironCost = 0, copperCost = 0, aziumCost = 0, woodCost = 0;
		int nframes = 4;
		int frames = 0;
		int y = 267;
		const int default_Y = 267;
		int iconX = 0, iconY = 0, iconWidth = 0, iconHeight = 0; //values for inventory items
		const char* player_c;

		machines(int iron, int copper, int azium, int wood, ALLEGRO_BITMAP* icon, int w, int h, int aPlayer = 0) { //assigns "costs" for each object of resources
			ironCost = iron;
			copperCost = copper;
			aziumCost = azium;
			woodCost = wood;
			machineIcon = icon;

			iconWidth = w;
			iconHeight = h;

			machine_player = aPlayer;
		}
		const char* convert();
		void menu(ALLEGRO_BITMAP* bmp, ALLEGRO_FONT* font, int x, int y);
		void recipe(ALLEGRO_FONT* font, int x, int y);
	};
	machines drill(200, 100, 0, 0, NULL, 24, 21, 0);

	/* Declaring player variables */
	struct playerValues {
		int x = 18, y = 267; //player starting pos
		int moveSpeed = 1;
		const int acceleratedMoveSpeed = 10;
		bool moving = false;
	}player;

	struct background {
		float x;
		float y;
		float velX;
		float velY;
		int dirX;
		int dirY;

		int width;
		int height;

		ALLEGRO_BITMAP* image;
	}BG;

	int al_create_icon(ALLEGRO_BITMAP* icon, ALLEGRO_FONT* font, ALLEGRO_FONT* large_font, ALLEGRO_BITMAP* menu, ALLEGRO_BITMAP* menu2, int x, int y, int w, int h, int posX, int posY, const char* num, bool LMB, bool RMB);
	//huge function for inventory handling & crafting
}
	void bmpFollowMouseUpdate(ALLEGRO_BITMAP* bmp, int factor);
	void bmpFollowMouseUpdate(ALLEGRO_BITMAP* bmp, int factor, int xoffset, int yoffset);


