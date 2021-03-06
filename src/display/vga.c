#include <display/vga.h>
#include "../sys/string.c"
#include "../sys/system.c"
#include "../stdio.c"

static inline uint8_t vga_color_entry(enum vga_color_palette fg_color, enum vga_color_palette bg_color)
{
    return fg_color | bg_color << 4;
}

static inline uint16_t vga_entry(unsigned char c, enum vga_color_palette color)
{
    return (uint16_t) c | (uint16_t) color << 8;
}


void make_vga_terminal(void)
{
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_color_entry(VGA_COLOR_BLACK, VGA_COLOR_WHITE);
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
    //Update position of cursor
    update_cursor_position(terminal_column, terminal_row);
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
    //Check if we should add a new line
    if(NEW_LINE_AT_END == true)
    {
        //Add a new line
        put_terminal_char('\n');
    }
}

void write_string_to_terminal(const char* data)
{
    //Write the string
    write_to_terminal(data, strlen(data));
}

void write_warning_string(char* data, ...)
{
    //Get the old terminal color
    uint8_t old_color = terminal_color;
    //Set the color to cyan font color, white background
    terminal_color = vga_color_entry(VGA_COLOR_CYAN, VGA_COLOR_WHITE);
    //Print the data
    printf(data);
    //Reset back to old color
    terminal_color = old_color;
}

void write_error_string(char* data, ...)
{
    //Get the old terminal color
    uint8_t old_color = terminal_color;
    //Set the color to cyan font color, white background
    terminal_color = vga_color_entry(VGA_COLOR_RED, VGA_COLOR_WHITE);
    //Print the data
    printf(data);
    //Reset back to old color
    terminal_color = old_color;
}

void write_pass_string(char* data,...)
{
    //Get the old terminal color
    uint8_t old_color = terminal_color;
    //Set the color to cyan font color, white background
    terminal_color = vga_color_entry(VGA_COLOR_GREEN, VGA_COLOR_WHITE);
    //Print the data
    printf(data);
    //Reset back to old color
    terminal_color = old_color;
}

void update_cursor_position(int x, int y)
{
    //Set the new position
    uint16_t position = y * WIDTH + x;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t) (position & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t) ((position >> 8) & 0xFF));
}

void write_current_operation(const char* function, const char* text)
{
    printf("[CurrentOperation] => %s:%s", function, text);
}