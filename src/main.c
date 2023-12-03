#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <gb/gb.h>
#include <gb/cgb.h>
#include <gbdk/console.h>
#include <gbdk/font.h>

#include "../assets/font0.h" //  ../../../../bin/png2asset font0.png -spr8x8 -map -noflip -tile_origin 0
#include "../assets/font_alt0.h" 
#include "../assets/font1.h"
#include "../assets/font_alt1.h" 
#include "../assets/font2.h"
#include "../assets/font_alt2.h" 
#include "../assets/font3.h"
#include "../assets/font_alt3.h" 
#include "../assets/font4.h"
#include "../assets/font_alt4.h" 
#include "../assets/font5.h"
#include "../assets/font_alt5.h" 
#include "../assets/font6.h"
#include "../assets/font_alt6.h" 
#include "../assets/font7.h"
#include "../assets/font_alt7.h" 
#include "../assets/font8.h"
#include "../assets/font_alt8.h" 
#include "../assets/font9.h"
#include "../assets/font_alt9.h" 
#include "../assets/font10.h"
#include "../assets/font_alt10.h" 
#include "../assets/font11.h" 
#include "../assets/font_alt11.h" 
#include "../assets/border2.h"
#include "../assets/watchface.h"

#define FONT_COUNT 12
#define MAX_TILE_COUNT 22

uint8_t* font_tiles[] = {
  font0_tiles, font1_tiles, font2_tiles, font3_tiles, font4_tiles, font5_tiles,
  font6_tiles, font7_tiles, font8_tiles, font9_tiles, font10_tiles, font11_tiles
};

uint8_t* font_alt_tiles[] = {
  font_alt0_tiles, font_alt1_tiles, font_alt2_tiles, font_alt3_tiles, font_alt4_tiles, font_alt5_tiles,
  font_alt6_tiles, font_alt7_tiles, font_alt8_tiles, font_alt9_tiles, font_alt10_tiles, font_alt11_tiles
};

uint8_t font_tile_counts[] = {
  font0_TILE_COUNT, font1_TILE_COUNT, font2_TILE_COUNT, font3_TILE_COUNT, font4_TILE_COUNT, font5_TILE_COUNT,
  font6_TILE_COUNT, font7_TILE_COUNT, font8_TILE_COUNT, font9_TILE_COUNT, font10_TILE_COUNT, font11_TILE_COUNT
};

unsigned char* font_maps[] = {
  font0_map, font1_map, font2_map, font3_map, font4_map, font5_map,
  font6_map, font7_map, font8_map, font9_map, font10_map, font11_map
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

int8_t player_left = 20;
int8_t player_right = 20;
char life_buffer[2];
uint8_t selected_player = 0;


void set_left_font_bkg_data() {	
	if (player_left) {
		digits[0] = player_left / 10;
		digits[1] = player_left % 10;
	} else {
		digits[0] = 10;
		digits[1] = 11;
	}

	for (uint8_t i = 0; i < 2; i++) {
		loaded_font_tile_counts[i] = font_tile_counts[digits[i]];
  	set_bkg_data(i * MAX_TILE_COUNT, loaded_font_tile_counts[i], font_alt_tiles[digits[i]]);
		loaded_font_maps[i] = font_maps[digits[i]];
	}
}

void set_right_font_bkg_data() {
	if (player_right) {
		digits[2] = player_right / 10;
		digits[3] = player_right % 10;
	} else {
		digits[2] = 10;
		digits[3] = 11;
	}

	for (uint8_t i = 2; i < 4; i++) {
		loaded_font_tile_counts[i] = font_tile_counts[digits[i]];
  	set_bkg_data(i * MAX_TILE_COUNT, loaded_font_tile_counts[i], font_tiles[digits[i]]);
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
	print_at(1,1, "Magic Life Counter");
	print_at(1,16,"  L/R        A/B");
}

void draw_life_at(uint8_t life, uint8_t offset, uint8_t x, uint8_t y) {
	if (life >= 10 || life == 0) { // configure two digits
		set_bkg_based_tiles(x, y, 4, 6, loaded_font_maps[0 + offset], offset * MAX_TILE_COUNT);
		set_bkg_based_tiles(x + 4, y, 4, 6, loaded_font_maps[1 + offset], (1 + offset) * MAX_TILE_COUNT);
	} else if (life > 0) { // configure one digit
		set_bkg_based_tiles(x + 2, y, 4, 6, loaded_font_maps[1 + offset], (1 + offset) * MAX_TILE_COUNT);
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
  draw_life_at(player_left, 0, 1, 6);
}

void draw_right() {
  set_right_font_bkg_data();
  draw_life_at(player_right, 2, 11, 6);
}

void update_and_draw() {
	if (!KEY_TICKED(J_A | J_B | J_LEFT | J_RIGHT)) return;
	clear_screen();

	// if (KEY_TICKED(J_LEFT | J_RIGHT)) selected_player = (selected_player + 1) % 2; 

	// if (KEY_TICKED(J_UP)) {
	// 	if (selected_player == PLAYER_LEFT) {
	// 		player_left = MIN(99, player_left + 1);
	// 	} else {
	// 		player_right = MIN(99, player_right + 1);
	// 	}
	// } else if (KEY_TICKED(J_DOWN)){
	// 	if (selected_player == PLAYER_LEFT) {
	// 		player_left = MAX(0, player_left - 1);
	// 	} else {
	// 		player_right = MAX(0, player_right - 1);
	// 	}
	// }

	if (KEY_TICKED(J_RIGHT)) {
		selected_player = PLAYER_LEFT;
		player_left = MIN(99, player_left + 1);
	} else if (KEY_TICKED(J_LEFT)) {
		selected_player = PLAYER_LEFT;
		player_left = MAX(0, player_left - 1);
	}

	if (KEY_TICKED(J_B)) {
		selected_player = PLAYER_RIGHT;
		player_right = MIN(99, player_right + 1);
	} else if (KEY_TICKED(J_A)) {
		selected_player = PLAYER_RIGHT;
		player_right = MAX(0, player_right - 1);
	}

	draw_border();
	draw_left();
	draw_right();
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
	draw_border();
	draw_left();
	draw_right();

	while (1) {
		UPDATE_KEYS();
		update_and_draw();
		wait_vbl_done();
	}
}