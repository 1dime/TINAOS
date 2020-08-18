#include <stdint.h>
#include <stddef.h>
#include <memory.h>

#define WIDTH 80
#define HEIGHT 25

enum vga_color_palette
{
    black = 0,
    white = 15,
};


//Some useful variables
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* buffer;



static inline uint8_t vga_color_entry(enum vga_color_palette fg_color, enum vga_color_palette bg_color)
{
    return fg_color | bg_color << 4;
}

static inline uint16_t vga_entry(unsigned char c, uint8_t color)
{
    return (uint16_t) c | (uint16_t) color << 8;
}


void make_vga_terminal(void)
{
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_color_entry(black, white);
    buffer = (uint16_t*) 0xB8000;
    for(size_t y = 0; y < HEIGHT; y++)
    {
        for(size_t x = 0; x < WIDTH; x++)
        {
            const size_t idx = y * WIDTH + x;
            buffer[idx] = vga_entry(' ', terminal_color);
        }
    }
}

void set_terminal_color(uint8_t color)
{
    terminal_color = color;
}

void put_terminal_entry_at(char c, uint8_t color, size_t x, size_t y)
{
    const size_t idx = y * WIDTH + x;
    buffer[idx] = vga_entry(c, color);
}

void put_terminal_char(char c)
{
    if(c == '\n')
    {
        //Go to next line
        terminal_row += 1;
        terminal_column = 0;
    }
    else
    {
        put_terminal_entry_at(c, terminal_color, terminal_column, terminal_row);
        if(++terminal_column == WIDTH)
        {
            terminal_column = 0;
            if(++terminal_row == HEIGHT)
            {
                terminal_row = 0;
            }
        }
    }

}

void write_to_terminal(const char* data, size_t size)
{
    for(size_t idx = 0; idx < size; idx++)
    {
        put_terminal_char(data[idx]);
    }
}

void write_string_to_terminal(char* data)
{
    write_to_terminal(data, strlen(data));
}
