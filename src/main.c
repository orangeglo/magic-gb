#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <gb/gb.h>
#include <gb/cgb.h>
#include <rand.h>

/*
	- key repeat for life counter?
	- better display of triple digit life
	- poison counter?
	- background music?
*/

#include "../assets/font0.h" //  ../../../../bin/png2asset font0.png -spr8x8 -map -noflip -tile_origin 0
#include "../assets/font_alt0.h"
#include "../assets/font0_v.h"
#include "../assets/font_alt0_v.h"

#include "../assets/font1.h"
#include "../assets/font_alt1.h" 
#include "../assets/font1_v.h"
#include "../assets/font_alt1_v.h" 

#include "../assets/font2.h"
#include "../assets/font_alt2.h" 
#include "../assets/font2_v.h"
#include "../assets/font_alt2_v.h"

#include "../assets/font3.h"
#include "../assets/font_alt3.h" 
#include "../assets/font3_v.h"
#include "../assets/font_alt3_v.h" 

#include "../assets/font4.h"
#include "../assets/font_alt4.h" 
#include "../assets/font4_v.h"
#include "../assets/font_alt4_v.h" 

#include "../assets/font5.h"
#include "../assets/font_alt5.h" 
#include "../assets/font5_v.h"
#include "../assets/font_alt5_v.h" 

#include "../assets/font6.h"
#include "../assets/font_alt6.h" 
#include "../assets/font6_v.h"
#include "../assets/font_alt6_v.h" 

#include "../assets/font7.h"
#include "../assets/font_alt7.h" 
#include "../assets/font7_v.h"
#include "../assets/font_alt7_v.h" 

#include "../assets/font8.h"
#include "../assets/font_alt8.h" 
#include "../assets/font8_v.h"
#include "../assets/font_alt8_v.h" 

#include "../assets/font9.h"
#include "../assets/font_alt9.h" 
#include "../assets/font9_v.h"
#include "../assets/font_alt9_v.h" 

#include "../assets/font10.h"
#include "../assets/font_alt10.h" 
#include "../assets/font10_v.h"
#include "../assets/font_alt10_v.h" 

#include "../assets/font11.h" 
#include "../assets/font_alt11.h" 
#include "../assets/font11_v.h"
#include "../assets/font_alt11_v.h" 

#include "../assets/border2.h"
#include "../assets/watchface.h"

#define FONT_COUNT 12
#define MAX_TILE_COUNT 22

uint8_t* font_tiles[] = {
  font0_tiles, font1_tiles, font2_tiles, font3_tiles, font4_tiles, font5_tiles,
  font6_tiles, font7_tiles, font8_tiles, font9_tiles, font10_tiles, font11_tiles
};

uint8_t* font_tiles_v[] = {
  font0_v_tiles, font1_v_tiles, font2_v_tiles, font3_v_tiles, font4_v_tiles, font5_v_tiles,
  font6_v_tiles, font7_v_tiles, font8_v_tiles, font9_v_tiles, font10_v_tiles, font11_v_tiles
};

uint8_t* font_alt_tiles[] = {
  font_alt0_tiles, font_alt1_tiles, font_alt2_tiles, font_alt3_tiles, font_alt4_tiles, font_alt5_tiles,
  font_alt6_tiles, font_alt7_tiles, font_alt8_tiles, font_alt9_tiles, font_alt10_tiles, font_alt11_tiles
};

uint8_t* font_alt_tiles_v[] = {
  font_alt0_v_tiles, font_alt1_v_tiles, font_alt2_v_tiles, font_alt3_v_tiles, font_alt4_v_tiles, font_alt5_v_tiles,
  font_alt6_v_tiles, font_alt7_v_tiles, font_alt8_v_tiles, font_alt9_v_tiles, font_alt10_v_tiles, font_alt11_v_tiles
};

uint8_t font_tile_counts[] = {
  font0_TILE_COUNT, font1_TILE_COUNT, font2_TILE_COUNT, font3_TILE_COUNT, font4_TILE_COUNT, font5_TILE_COUNT,
  font6_TILE_COUNT, font7_TILE_COUNT, font8_TILE_COUNT, font9_TILE_COUNT, font10_TILE_COUNT, font11_TILE_COUNT
};

unsigned char* font_maps[] = {
  font0_map, font1_map, font2_map, font3_map, font4_map, font5_map,
  font6_map, font7_map, font8_map, font9_map, font10_map, font11_map
};

unsigned char* font_maps_v[] = {
  font0_v_map, font1_v_map, font2_v_map, font3_v_map, font4_v_map, font5_v_map,
  font6_v_map, font7_v_map, font8_v_map, font9_v_map, font10_v_map, font11_v_map
};

unsigned char* font_maps_alt_v[] = {
  font_alt0_v_map, font_alt1_v_map, font_alt2_v_map, font_alt3_v_map, font_alt4_v_map, font_alt5_v_map,
  font_alt6_v_map, font_alt7_v_map, font_alt8_v_map, font_alt9_v_map, font_alt10_v_map, font_alt11_v_map
};

uint8_t loaded_font_tile_counts[4];
unsigned char* loaded_font_maps[4];
uint8_t digits[4];


uint8_t previousKeys = 0;
uint8_t keys = 0;
#define UPDATE_KEYS() previousKeys = keys; keys = joypad()
#define KEY_PRESSED(K) (keys & (K))
#define KEY_TICKED(K) ((keys & (K)) && !(previousKeys & (K)))

#define MIN(x,y) ((x) > (y) ? (y) : (x))
#define MAX(x,y) ((x) < (y) ? (y) : (x))

#define PLAYER_LEFT 0
#define PLAYER_RIGHT 1

int16_t player_left = 20;
int16_t player_right = 20;
uint8_t selected_player = 0;

#define HORIZONTAL 0
#define VERTICAL 1

uint8_t display_mode = HORIZONTAL;

#define LIFE_COUNTER 0
#define DIE_ROLLER 1

uint8_t screen = LIFE_COUNTER;

const uint16_t d_redo[] = {256, 252, 256, 250, 252, 240}; // if the roll is >= this number, re-roll (evens out distribution)
uint8_t d[] = {4, 6, 8, 10, 12, 20};
uint8_t d_index = 5;
uint8_t d_value = 0;
uint8_t d_rand = 0;

uint8_t eat_start = 0;
uint8_t reset_enabled = 0;

char text_buffer[20];


void set_left_font_bkg_data() {	
	if (player_left) {
		digits[0] = (player_left / 10) % 10;
		digits[1] = player_left % 10;
	} else {
		digits[0] = 10;
		digits[1] = 11;
	}

	for (uint8_t i = 0; i < 2; i++) {
		loaded_font_tile_counts[i] = font_tile_counts[digits[i]];
		if (display_mode == HORIZONTAL) {
	  	set_bkg_data(i * MAX_TILE_COUNT, loaded_font_tile_counts[i], font_alt_tiles[digits[i]]);
			loaded_font_maps[i] = font_maps[digits[i]];
		} else { // VERTICAL
	  	set_bkg_data(i * MAX_TILE_COUNT, loaded_font_tile_counts[i], font_alt_tiles_v[digits[i]]);
			loaded_font_maps[i] = font_maps_alt_v[digits[i]];
		}
	}
}

void set_right_font_bkg_data() {
	if (player_right) {
		digits[2] = (player_right / 10) % 10;
		digits[3] = player_right % 10;
	} else {
		digits[2] = 10;
		digits[3] = 11;
	}

	for (uint8_t i = 2; i < 4; i++) {
		loaded_font_tile_counts[i] = font_tile_counts[digits[i]];
		if (display_mode == HORIZONTAL) {
	  	set_bkg_data(i * MAX_TILE_COUNT, loaded_font_tile_counts[i], font_tiles[digits[i]]);
			loaded_font_maps[i] = font_maps[digits[i]];
		} else { // VERTICAL
	  	set_bkg_data(i * MAX_TILE_COUNT, loaded_font_tile_counts[i], font_tiles_v[digits[i]]);
			loaded_font_maps[i] = font_maps_v[digits[i]];
		}
	}
}

void set_die_roll_font_bkg_data() {
	digits[0] = d_value / 10;
	digits[1] = d_value % 10;

	for (uint8_t i = 0; i < 2; i++) {
		loaded_font_tile_counts[i] = font_tile_counts[digits[i]];
		if (d_value % 2 == 0) {
	  	set_bkg_data(i * MAX_TILE_COUNT, loaded_font_tile_counts[i], font_tiles[digits[i]]);
		} else {
	  	set_bkg_data(i * MAX_TILE_COUNT, loaded_font_tile_counts[i], font_alt_tiles[digits[i]]);
		}
		loaded_font_maps[i] = font_maps[digits[i]];
	}
}

void clear_screen() {
  fill_bkg_rect(0,0,20,18,0xFF); 
}

void print_at(uint8_t x, uint8_t y, char* str) {
  set_bkg_based_tiles(x, y, strlen(str), 1, str, 0x80 - 32);
}

void draw_text() {
	print_at(1, 1,  "Magic Life Counter");

	if (screen == LIFE_COUNTER) {
		print_at(1, 15, "START  [Life] Roll");
		if (display_mode == HORIZONTAL) {
			print_at(1, 16, "SELECT       [H] V");
		} else { // VERTICAL
			print_at(1, 16, "SELECT       H [V]");
		}

	} else { // DIE_ROLLER
		print_at(1, 15, "START  Life [Roll]");

		switch (d_index) {
			case 0:
				print_at(1, 16, "SELECT        [D4]");
				break;
			case 1:
				print_at(1, 16, "SELECT        [D6]");
				break;
			case 2:
				print_at(1, 16, "SELECT        [D8]");
				break;
			case 3:
				print_at(1, 16, "SELECT       [D10]");
				break;
			case 4:
				print_at(1, 16, "SELECT       [D12]");
				break;
			case 5:
				print_at(1, 16, "SELECT       [D20]");
				break;
		}
	}
}

void draw_life_at(uint8_t life, uint8_t offset, uint8_t x, uint8_t y) {
	if (screen == DIE_ROLLER || display_mode == HORIZONTAL) {
		if (life >= 10 || life == 0) { // configure two digits
			set_bkg_based_tiles(x, y, 4, 6, loaded_font_maps[0 + offset], offset * MAX_TILE_COUNT);
			set_bkg_based_tiles(x + 4, y, 4, 6, loaded_font_maps[1 + offset], (1 + offset) * MAX_TILE_COUNT);
		} else if (life > 0) { // configure one digit
			set_bkg_based_tiles(x + 2, y, 4, 6, loaded_font_maps[1 + offset], (1 + offset) * MAX_TILE_COUNT);
		}

	} else { // VERTICAL
		if (life >= 10 || life == 0) { // configure two digits
			if (offset == 2) { // dumb hack to detect the right player digits, since we need to swap them
				set_bkg_based_tiles(x, y, 6, 4, loaded_font_maps[1 + offset], (1 + offset) * MAX_TILE_COUNT);
				set_bkg_based_tiles(x, y + 4, 6, 4, loaded_font_maps[0 + offset], offset * MAX_TILE_COUNT);
			} else { // left player
				set_bkg_based_tiles(x, y, 6, 4, loaded_font_maps[0 + offset], offset * MAX_TILE_COUNT);
				set_bkg_based_tiles(x, y + 4, 6, 4, loaded_font_maps[1 + offset], (1 + offset) * MAX_TILE_COUNT);
			}
		} else if (life > 0) { // configure one digit
			set_bkg_based_tiles(x, y + 2, 6, 4, loaded_font_maps[1 + offset], (1 + offset) * MAX_TILE_COUNT);
		}
	}
}

void draw_border() {
	if (selected_player == PLAYER_LEFT) {
		set_bkg_based_tiles(0, 5, 10, 8, border2_map, (5 * MAX_TILE_COUNT));
	} else {
		set_bkg_based_tiles(10, 5, 10, 8, border2_map, (5 * MAX_TILE_COUNT));
	}
}

void draw_left() {
	set_left_font_bkg_data();
	if (display_mode == HORIZONTAL) {
	  draw_life_at(player_left, 0, 1, 6);
	} else {
	  draw_life_at(player_left, 0, 2, 5);
	}
}

void draw_right() {
  set_right_font_bkg_data();
  if (display_mode == HORIZONTAL) {
	  draw_life_at(player_right, 2, 11, 6);
	} else { // VERTICAL
	  draw_life_at(player_right, 2, 12, 5);
	}
}

void draw_die_roll() {
	set_die_roll_font_bkg_data();
	draw_life_at(d_value, 0, 6, 6);
}

void draw_life_counter_digits() {
	if (display_mode == HORIZONTAL) draw_border();
	draw_left();
	draw_right();
}

void roll_die() {
	do {
		d_rand = rand();
	} while (d_rand >= d_redo[d_index]);
	d_value = (d_rand % d[d_index]) + 1;
}

void update_and_draw() {
	// Reset
	if (KEY_PRESSED(J_START) && KEY_PRESSED(J_SELECT) && KEY_PRESSED(J_A) && KEY_PRESSED(J_B)) {
		if (reset_enabled) {
			reset();
		} else {
			return;
		}
	} else if (!reset_enabled && keys > 0) {
		reset_enabled = 1;
	}


	// Screen Change
	if (eat_start && KEY_TICKED(J_START | J_SELECT)) {
		eat_start = 0;
		draw_text();
		return;
	}
	
	if (KEY_TICKED(J_START)) {
		if (screen == LIFE_COUNTER) {
			screen = DIE_ROLLER;
			initrand(DIV_REG);
			clear_screen();
			draw_text();
			print_at(6, 8, "Press A");
			print_at(6, 9, " To Roll");
		} else {
			screen = LIFE_COUNTER;
			clear_screen();
			draw_text();
			draw_life_counter_digits();
		}
	}
	

  // Update and Display Screen
	if (screen == LIFE_COUNTER) {
		if (KEY_TICKED(J_SELECT)) {
			display_mode = !display_mode;
			clear_screen();
			draw_text();
			draw_life_counter_digits();
		}

		if (display_mode == HORIZONTAL) {
			if (!KEY_TICKED(J_UP | J_DOWN | J_LEFT | J_RIGHT)) return;
		} else { // VERTICAL
			if (!KEY_TICKED(J_A | J_B | J_LEFT | J_RIGHT)) return;
		}

		clear_screen();
		eat_start = 1;

		if (display_mode == HORIZONTAL) {
			if (KEY_TICKED(J_LEFT | J_RIGHT)) selected_player = (selected_player + 1) % 2; 

			if (KEY_TICKED(J_UP)) {
				if (selected_player == PLAYER_LEFT) {
					player_left = MIN(999, player_left + 1);
				} else {
					player_right = MIN(999, player_right + 1);
				}
			} else if (KEY_TICKED(J_DOWN)){
				if (selected_player == PLAYER_LEFT) {
					player_left = MAX(0, player_left - 1);
				} else {
					player_right = MAX(0, player_right - 1);
				}
			}

		} else { // VERTICAL
			if (KEY_TICKED(J_RIGHT)) {
				selected_player = PLAYER_LEFT;
				player_left = MIN(999, player_left + 1);
			} else if (KEY_TICKED(J_LEFT)) {
				selected_player = PLAYER_LEFT;
				player_left = MAX(0, player_left - 1);
			}

			if (KEY_TICKED(J_B)) {
				selected_player = PLAYER_RIGHT;
				player_right = MIN(999, player_right + 1);
			} else if (KEY_TICKED(J_A)) {
				selected_player = PLAYER_RIGHT;
				player_right = MAX(0, player_right - 1);
			}

		}

		draw_life_counter_digits();

	} else { // Die Roller
		if (KEY_TICKED(J_SELECT)) {
			d_index = (d_index + 1) % 6;
			draw_text();
		}

		if (!KEY_TICKED(J_A)) return;

		clear_screen();
		eat_start = 1;

		if (KEY_TICKED(J_A)) {
			roll_die();
			clear_screen();
			delay(500);
			draw_die_roll();
		}
	}
}

void main(void) {
	if (_cpu == CGB_TYPE) set_default_palette();

	SPRITES_8x8;
  SHOW_BKG;
  BGP_REG = 0b00011011;

  set_bkg_data(5 * MAX_TILE_COUNT, border2_TILE_COUNT, border2_tiles);
  set_bkg_data(0x81, watchface_TILE_COUNT, watchface_tiles);

	clear_screen();

	draw_text();
	draw_life_counter_digits();

	while (1) {
		UPDATE_KEYS();
		update_and_draw();
		wait_vbl_done();
	}
}