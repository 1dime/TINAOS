#ifndef VGA
#define VGA
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static const int WIDTH = 80;
static const int HEIGHT = 40;
bool NEW_LINE_AT_END = true;

enum vga_color_palette
{
    VGA_COLOR_BLACK,
    VGA_COLOR_BLUE,
	VGA_COLOR_GREEN,
	VGA_COLOR_CYAN,
	VGA_COLOR_RED,
	VGA_COLOR_MAGENTA,
	VGA_COLOR_BROWN,
	VGA_COLOR_LGREY,
	VGA_COLOR_DGREY,
	VGA_COLOR_LBLUE,
	VGA_COLOR_LGREEN,
	VGA_COLOR_LCYAN,
	VGA_COLOR_LRED,
	VGA_COLOR_LMAGENTA,
	VGA_COLOR_LBROWN,
	VGA_COLOR_WHITE
};

//Some useful variables
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* buffer;



static inline uint8_t vga_color_entry(enum vga_color_palette fg_color, enum vga_color_palette bg_color);

static inline uint16_t vga_entry(unsigned char c, enum vga_color_palette color);


void make_vga_terminal(void);
void set_terminal_color(uint8_t color);

void put_terminal_entry_at(char c, uint8_t color, size_t x, size_t y);
void put_terminal_char(char c);

void write_to_terminal(const char* data, size_t size);
void write_string_to_terminal(const char* data);

void write_with_color(enum vga_color_palette color, const char* data);
void write_warning_string(char* data, ...);
void write_error_string(char* data, ...);
void write_pass_string(char* data,...);

void update_cursor_position(int x, int y);
#endif