/* Spacelad (WIP name) - game where the character goes to planets
to mine resources, build rockets etc   */

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
#include "main.h"


int main() {

	/* Initialising allegro */
	al_init();
	al_init_image_addon();
	al_install_keyboard();
	al_install_mouse();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();

	ALLEGRO_FONT* font = al_load_ttf_font("fixedsys.ttf", 12, 0);
	ALLEGRO_FONT* large_font = al_load_ttf_font("fixedsys.ttf", 60, 0);
	ALLEGRO_COLOR backgroundColour = al_map_rgb(23, 23, 23);
	ALLEGRO_COLOR maskColour = al_map_rgb(0, 162, 232);

	ALLEGRO_TIMER* timer = al_create_timer(SPF);
	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
	ALLEGRO_BITMAP* rightMovement[maxFrames], * leftMovement[maxFrames];
	ALLEGRO_BITMAP* inventory = al_load_bitmap("inventory.png");
	ALLEGRO_BITMAP* inventory_menu = al_load_bitmap("inventory-dropdown.png");
	ALLEGRO_BITMAP* inventory_menu_recipe = al_load_bitmap("inventory-dropdown-2.png");
	ALLEGRO_DISPLAY* display = al_create_display(WIDTH, HEIGHT);
	ALLEGRO_BITMAP* icons = al_load_bitmap("icons-image-sheet.png");

	ALLEGRO_BITMAP* back = al_load_bitmap("back.png");
	al_convert_mask_to_alpha(back, backgroundColour);

	ALLEGRO_BITMAP* mid = al_load_bitmap("mid.png");
	al_convert_mask_to_alpha(mid, backgroundColour);

	ALLEGRO_BITMAP* front = al_load_bitmap("front.png");
	al_convert_mask_to_alpha(front, backgroundColour);

	al_convert_mask_to_alpha(front, al_map_rgb(0, 0, 0));
	al_set_window_title(display, "Spacelad");

	al_convert_mask_to_alpha(icons, maskColour);
	ALLEGRO_BITMAP* playerSprite = al_load_bitmap("astronautImageSheet.png");
	al_convert_mask_to_alpha(playerSprite, backgroundColour);

	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_timer_event_source(timer));


	/* Randomly generate locations to place resources */
	srand(time(NULL));
	iron.generateResourceNodes();
	copper.generateResourceNodes();
	azium.generateResourceNodes();
	wood.generateResourceNodes();

	/* Creating arrays to contain resource sprites */
	ALLEGRO_BITMAP* resources = al_load_bitmap("resources-image-sheet.png");
	al_convert_mask_to_alpha(resources, maskColour);
	ALLEGRO_BITMAP* woodStates = al_load_bitmap("tree-states.png");
	al_convert_mask_to_alpha(woodStates, maskColour);

	/* Resource arrays */

	/* Iron bitmap array */
	for (int i = 0; i < frame; i++) {
		iron.resourceArray[i] = al_create_sub_bitmap(resources, add, 0, 40, 40);
		copper.resourceArray[i] = al_create_sub_bitmap(resources, add, 41, 40, 40);
		azium.resourceArray[i] = al_create_sub_bitmap(resources, add, 81, 40, 40);
		add += 41;
	}
	add = 0;

	/* Wood bitmap array */
	for (int i = 0; i < frame; i++) {
		wood.resourceArray[i] = al_create_sub_bitmap(woodStates, add, 0, 80, 120);
		add += 81;
	}
	add = 0;

	/* Player animation arrays */
	add = 37;
	for (int i = 0; i < maxFrames; i++) {
		rightMovement[i] = al_create_sub_bitmap(playerSprite, add, 0, 31, 64);
		leftMovement[i] = al_create_sub_bitmap(playerSprite, add, 65, 31, 63);
		add += 32;
	}
	add = 0;
	ALLEGRO_BITMAP* idle = al_create_sub_bitmap(playerSprite, 0, 0, 36, 64);



	/* Machines array */
	ALLEGRO_BITMAP* drills = al_load_bitmap("laser-spritesheet.png");
	al_convert_mask_to_alpha(drills, al_map_rgb(0, 255, 0));

	for (int i = 0; i < frame + 1; i++) {
		drill.idleMachineArray[i] = al_create_sub_bitmap(drills, add, 0, 19, 79);
		add += 20;
	}
	add = 80;
	for (int i = 0; i < frame + 1; i++) {
		drill.machineArray[i] = al_create_sub_bitmap(drills, add, 0, 19, 79);
		add += 20;
	}
	drill.machineIcon = al_create_sub_bitmap(icons, 0, 23, drill.iconWidth, drill.iconHeight);


	/* Other allegro variables */
	player.x = 18;
	player.y = 267;




	/* Start of game loop */
	al_start_timer(timer);
	while (isRunning == true) { //beginning of while loop
		copper.resourceSprites();
		iron.resourceSprites();
		azium.resourceSprites();

		invX = planet1.x - al_get_bitmap_width(inventory) / 2;
		invY = planet1.y - al_get_bitmap_height(inventory) / 2;

		planet1.radians = planet1.angle * pi / 180;
		ALLEGRO_EVENT event;
		al_wait_for_event(queue, &event);
		al_clear_to_color(al_map_rgb(0, 0, 0));
		ALLEGRO_KEYBOARD_STATE keyState;
		ALLEGRO_MOUSE_STATE mouseState;
		al_get_mouse_state(&mouseState);


		/* Wood resources are handled differently to others */
		if (wood.grow <= 50) {
			wood.resourceSprite = wood.resourceArray[0];
		}
		else if (wood.grow > 50
			&& wood.grow < 100) {
			wood.resourceSprite = wood.resourceArray[1];
		}
		else if (wood.grow == 200) {
			wood.resourceSprite = wood.resourceArray[2];
		}
		else if (wood.grow == 200) {
			wood.resourceCount = 200;
			wood.grow = 201;
		}

		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			isRunning = false;
		}


		if (event.type == ALLEGRO_EVENT_TIMER) {
			elapsed_time += SPF;
			al_draw_bitmap(back, 0, 0, 0);
			bmpFollowMouseUpdate(mid, 128);
			bmpFollowMouseUpdate(front, 64);

			/* Mouse handling */
			mouseX = al_get_mouse_state_axis(&mouseState, 0);
			mouseY = al_get_mouse_state_axis(&mouseState, 1);
			mouseRadians = (90 * pi / 180) + (atan2(mouseY - planet1.y, mouseX - planet1.x));
			mouseAngle = mouseRadians * 180 / pi;

			/* Keyboard handling */
			al_get_keyboard_state(&keyState);
			if (al_key_down(&keyState, ALLEGRO_KEY_W)) {
				player.moving = false;
			}
			if (al_key_down(&keyState, ALLEGRO_KEY_A)) {
				FRAME_NUM = (int)nframes * fmod(elapsed_time, duration) / duration;
				al_draw_rotated_bitmap(leftMovement[FRAME_NUM], player.x, player.y, planet1.x, planet1.y, planet1.radians, 0);
				planet1.angle -= player.moveSpeed;
				player.moving = true;
			}
			if (al_key_down(&keyState, ALLEGRO_KEY_D)) {
				FRAME_NUM = (int)nframes * fmod(elapsed_time, duration) / duration;
				al_draw_rotated_bitmap(rightMovement[FRAME_NUM], player.x, player.y, planet1.x, planet1.y, planet1.radians, 0);
				planet1.angle += player.moveSpeed;
				player.moving = true;
			}
			if (al_key_down(&keyState, ALLEGRO_KEY_E)) {
				showInventory = true;
			}
			else if (!al_key_down(&keyState, ALLEGRO_KEY_E)) {
				showInventory = false;
			}

			if (iron.down == false && copper.down == false && azium.down == false && wood.down == false) {
				if (mouseState.buttons & 1) {
					leftMousePressed = true;
				}
				else {
					leftMousePressed = false;
				}

				if (mouseState.buttons & 2) {
					rightMousePressed = true;
				}
			}
			else {
				leftMousePressed = true;
			}

			if (mouseState.buttons & 1 && mouseState.buttons & 2) {
				leftMousePressed = false; rightMousePressed = false;
			}

			if (al_key_down(&keyState, ALLEGRO_KEY_LSHIFT)) {
				player.moveSpeed = player.acceleratedMoveSpeed;
			}
			else {
				player.moveSpeed = 1;
			}

			FRAME_NUM = (int)drill.nframes * fmod(elapsed_time, duration) / duration;

			if (iron.down == false && copper.down == false && azium.down == false && wood.down == false) {
				drill.machineSprite = drill.idleMachineArray[FRAME_NUM];
				drillradian = mouseRadians;
			}
			else {
				drill.machineSprite = drill.machineArray[FRAME_NUM];
				drillradian = lastpos;

			}

			if (player.moving == false) {
				al_draw_rotated_bitmap(idle, player.x, player.y, planet1.x, planet1.y, planet1.radians, 0);
			}
			if (drill.hold == true) {
				copper.check();
				azium.check();
				wood.check();
				iron.check();
			}

			frames++; //use for timing

			if (wood.grow >= 200) {
				wood.increasing = false;
			}
			else if (wood.grow <= 200 && wood.resourceCount != 200) {
				wood.increasing = true;
			}

			if (wood.increasing == true && frames == 10) {
				frames = 0;
				wood.grow++;
				
			}

				resourceFrames++;
			
			if (resourceFrames > 300) { //resets every 5 seconds
				resourceFrames = 0;
			}


			/* Draw resource nodes -- could be optimised somehow */
			if (iron.down == true) {
				iron.frame++;
				drillradian = iron.radians + (2.5 * pi / 180);
				if (iron.frame == 15 && iron.resourceCount > 0) {
					iron.resourceCount--;
					iron.resource_player++;
					iron.frame = 0;
				}
			}
			if (copper.down == true) {
				copper.frame++;
				drillradian = copper.radians + (2.5 * pi / 180);
				if (copper.frame == 15 && copper.resourceCount > 0) {
					copper.resourceCount--;
					copper.resource_player++;
					copper.frame = 0;
				}
			}
			if (azium.down == true) {
				azium.frame++;
				drillradian = azium.radians + (2.5 * pi / 180);
				if (azium.frame == 60 && azium.resourceCount > 0) {
					azium.resourceCount--;
					azium.resource_player++;
					azium.frame = 0;
				}
			}
			if (wood.resourceCount == 200) {
				if (wood.down == true) {
					drillradian = wood.radians + (2.5 * pi / 180);
					drill.y = 347;
					wood.frame++;
					drillradian = wood.radians + (2.5 * pi / 180);
					if (wood.frame == 10 && wood.resourceCount > 0) {
						wood.resourceCount--;
						wood.resource_player++;
						wood.frame = 0;
					}
				}
			}
			else {
				drill.y = drill.default_Y;
			}


			iron.checkResources();
			copper.checkResources();
			azium.checkResources();
			wood.checkResources();
			if (drill.hold == true) {
				al_draw_rotated_bitmap(drill.machineSprite, player.x, drill.y, planet1.x, planet1.y, drillradian, 0);
			}

			if (iron.draw == true) {
				al_draw_rotated_bitmap(iron.resourceSprite, player.x, player.y - 35, planet1.x, planet1.y, iron.radians, 0);
			}
			if (copper.draw == true) {
				al_draw_rotated_bitmap(copper.resourceSprite, player.x, player.y - 35, planet1.x, planet1.y, copper.radians, 0);
			}
			if (azium.draw == true) {
				al_draw_rotated_bitmap(azium.resourceSprite, player.x, player.y - 35, planet1.x, planet1.y, azium.radians, 0);
			}
			if (wood.draw == true) {
				al_draw_rotated_bitmap(wood.resourceSprite, player.x + 20, player.y + 39, planet1.x, planet1.y, wood.radians, 0);
			}

			al_draw_filled_circle(planet1.x, planet1.y, planet1.radius, al_map_rgb(0, 255, 0)); //temporary planet for testing
			drill.iconX = invX + 5;
			drill.iconY = invY + 25;
			drill.convert();

			/* Show player's resource values in inventory*/
			if (showInventory == true) {
				iron.convert(); azium.convert();
				copper.convert(); wood.convert();


				al_draw_bitmap(inventory, invX, invY, 0);

				//have to use trial and error to determine text position - very inefficient
				al_draw_text(font, al_map_rgb(0, 0, 0), invX + 160, invY + 13, 0, iron.player_c);
				al_draw_text(font, al_map_rgb(0, 0, 0), invX + 175, invY + 40, 0, copper.player_c);
				al_draw_text(font, al_map_rgb(0, 0, 0), invX + 170, invY + 73, 0, azium.player_c);
				al_draw_text(font, al_map_rgb(0, 0, 0), invX + 163, invY + 102, 0, wood.player_c);

				if (al_create_icon(drill.machineIcon, font, large_font, inventory_menu, inventory_menu_recipe, drill.iconX, drill.iconY, drill.iconWidth, drill.iconHeight, mouseX, mouseY, drill.player_c, leftMousePressed, rightMousePressed) == 1 && drill.machine_player > 0 && drill.hold == false) {
					drill.hold = true;
					drill.machine_player--;
					isTimer = true;
				}
				else if ((al_create_icon(drill.machineIcon, font, large_font, inventory_menu, inventory_menu_recipe, drill.iconX, drill.iconY, drill.iconWidth, drill.iconHeight, mouseX, mouseY, drill.player_c, leftMousePressed, rightMousePressed) == 2)) {
					if (iron.resource_player >= drill.ironCost && copper.resource_player >= drill.copperCost) {
						drill.machine_player++;
						iron.resource_player -= drill.ironCost; copper.resource_player -= drill.copperCost;
						rightMousePressed = false;
					}
				}
			}
			player.moving = false;

			if (iron.down == false && copper.down == false && azium.down == false && wood.down == false) {
				lastpos = mouseRadians;
			}


			
			al_flip_display();
		}

	}
}


/* Initial resource node generator */
float resources::generateResourceNodes() {
	int angle = (rand() % 360) + 1;
	return angle * pi / 180;
}


/* Returns true if the mouse cursor is over a resource node & the mouse is pressed */
void resources::check() {
	if (lastpos > this->radians && lastpos < this->radians + (5 * pi / 180) && leftMousePressed == true) {
		this->down = true;
	}
	else {
		this->down = false;
	}
}


/* Checks if resource nodes are empty and replaces them if they are */
void resources::checkResources() {
	if (this->resourceCount == 0) {
		drill.hold = false; 
		this->down = false;
		this->draw = false;
		this->grow = 0;
		if (resourceFrames == 300) { //replaces after 5 seconds
			this->radians = (rand() % 270) * pi / 180;
			this->resourceCount = this->startingcount;
			this->draw = true;
			leftMousePressed = false;
			rightMousePressed = false;
			drill.machine_player++;
		}
	}
}


/* Change bitmap sprite according to resource values */
void resources::resourceSprites() {

	if (this->resourceCount > this->startingcount / 2) {
		this->resourceSprite = this->resourceArray[0];
	}
	else if (this->resourceCount <= this->startingcount / 2
		&& this->resourceCount > this->startingcount / 4) {
		this->resourceSprite = this->resourceArray[1];
	}
	else if (this->resourceCount <= this->startingcount / 4
		&& this->resourceCount > 0) {
		this->resourceSprite = this->resourceArray[2];
	}
}


/* Convert integer values to const char* for text output */
const char* resources::convert() {
	this->player_str = to_string(this->resource_player);
	this->player_c = this->player_str.c_str();
	return this->player_c;
}
const char* machines::convert() {
	this->player_str = to_string(this->machine_player);
	this->player_c = this->player_str.c_str();
	return this->player_c;
}

int al_create_icon(ALLEGRO_BITMAP* icon, ALLEGRO_FONT* font, ALLEGRO_FONT* large_font, ALLEGRO_BITMAP* menu, ALLEGRO_BITMAP* menu2, int x, int y, int w, int h, int posX, int posY, const char* num, bool LMB = false, bool RMB = false) {
	w = al_get_bitmap_width(icon);
	h = al_get_bitmap_height(icon);

	al_draw_bitmap(icon, x, y, 0);
	al_draw_text(font, al_map_rgb(0, 0, 0), x + (w / 1.25), y + h / 2, 0, num);

	if (posX <= (x + w) && posX >= x && (posY <= y + h && posY >= y)) {
		drill.menu(menu, large_font, (x - w / 2), (y - (h * 4.9)));
		al_draw_bitmap(menu2, x + (w * 5), y - (h * 6.9), 0);
		aboveIcon = true;
	}
	else {
		aboveIcon = false;
	}

	if (aboveIcon == true && LMB == true ) {
		return 1;
		RMB = false;
	}
	if (aboveIcon == true && RMB == true) {
		return 2;
		LMB = false;
	}
	else {
		return 0;
	}
}

void machines::menu(ALLEGRO_BITMAP* bmp, ALLEGRO_FONT* font, int x, int y) {
	al_draw_bitmap(bmp, x, y, 0);
	al_draw_text(font, al_map_rgb(0,0,0), x + 10, y + 5, 0, this->player_c);
}

void bmpFollowMouseUpdate(ALLEGRO_BITMAP* bmp, int factor) {                                      //Draws bitmap to follow the mouse
	int tempx;
	int tempy;

	tempx = ((mouseX + (WIDTH / 2))) / factor;                                                          //Smaller factor gives greater range
	tempy = ((mouseY + (HEIGHT / 2))) / factor;
	tempx -= (WIDTH / factor);
	tempy -= (HEIGHT / factor);
	al_draw_bitmap(bmp, tempx, tempy, 0);
}

void bmpFollowMouseUpdate(ALLEGRO_BITMAP* bmp, int factor, int xoffset, int yoffset) {                  //Overloaded function, allows for image offset
	int tempx;
	int tempy;

	tempx = ((mouseX + (WIDTH / 2))) / factor;                                                          //Smaller factor gives greater range
	tempy = ((mouseY + (HEIGHT / 2))) / factor;
	tempx -= (WIDTH / factor);
	tempy -= (HEIGHT / factor);
	tempx += xoffset;                                                                                   //Adds X offset
	tempy += yoffset;                                                                                   //Adss Y offset
	al_draw_bitmap(bmp, tempx, tempy, 0);
}