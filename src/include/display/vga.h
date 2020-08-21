#ifndef VGA
#define VGA
#include <stdint.h>
#include <stddef.h>
#define WIDTH 80
#define HEIGHT 25

enum vga_color_palette
{
    black = 0,
    green = 2,
    red = 4,
    cyan = 3,
    white = 15,
};


//Some useful variables
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* buffer;



static inline uint8_t vga_color_entry(enum vga_color_palette fg_color, enum vga_color_palette bg_color);

static inline uint16_t vga_entry(unsigned char c, uint8_t color);


void make_vga_terminal(void);
void set_terminal_color(uint8_t color);

void put_terminal_entry_at(char c, uint8_t color, size_t x, size_t y);
void put_terminal_char(char c);

void write_to_terminal(const char* data, size_t size);
void write_string_to_terminal(char* data);

void write_with_color(enum vga_color_palette color, char* data);
void write_warning_string(char* data);
void write_error_string(char* data);
void write_pass_string(char* data);

void update_cursor_position(int x, int y);
#endif