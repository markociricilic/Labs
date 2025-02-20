/* This files provides address values that exist in the system */

#define SDRAM_BASE            0xC0000000
#define FPGA_ONCHIP_BASE      0xC8000000
#define FPGA_CHAR_BASE        0xC9000000

/* Cyclone V FPGA devices */
#define LEDR_BASE             0xFF200000
#define HEX3_HEX0_BASE        0xFF200020
#define HEX5_HEX4_BASE        0xFF200030
#define SW_BASE               0xFF200040
#define KEY_BASE              0xFF200050
#define TIMER_BASE            0xFF202000
#define PIXEL_BUF_CTRL_BASE   0xFF203020
#define CHAR_BUF_CTRL_BASE    0xFF203030

/* VGA colors */
#define WHITE 0xFFFF
#define YELLOW 0xFFE0
#define RED 0xF800
#define GREEN 0x07E0
#define BLUE 0x001F
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define GREY 0xC618
#define PINK 0xFC18
#define ORANGE 0xFC00
#define BLACK  0x0000

#define ABS(x) (((x) > 0) ? (x) : -(x))

/* Screen size. */
#define RESOLUTION_X 320
#define RESOLUTION_Y 240

/* Constants for animation */
#define BOX_LEN 2
#define NUM_BOXES 8

#define FALSE 0
#define TRUE 1

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// Begin part3.c code for Lab 5

void plot_pixel(int x, int y, short int line_color);
void draw_line(int x0, int y0, int x1, int y1, short int line_color);
void swap(int *i, int *j);
void clear_screen();
void wait_for_vsync();
void box(int x, int y, int color);

volatile int pixel_buffer_start; // global variable

int main(void)
{
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;

    // Declaring other variables
    int N = 8;  
    int color_box[N], x_box[N], y_box[N], dx[N], dy[N];

    // initialize location and direction of rectangles
    short int color[10] = {WHITE, YELLOW, RED, GREEN, BLUE, CYAN, MAGENTA, GREY, PINK, ORANGE};
    int i;
    for (i = 0; i < N; i++) {
        dx[i] = ((rand() % 2) * 2) - 1;     
        dy[i] = ((rand() % 2) * 2) - 1;     

        x_box[i] = rand() % RESOLUTION_X;   // random location within the VGA display
        y_box[i] = rand() % RESOLUTION_Y;	// random location within the VGA display

        color_box[i] = color[rand() % 10];  // random color is chosen from the color array
    }

    /* set front pixel buffer to start of FPGA On-chip memory */
    *(pixel_ctrl_ptr + 1) = 0xC8000000; // first store the address in the back buffer
    /* now, swap the front/back buffers, to set the front buffer location */
    wait_for_vsync();
    /* initialize a pointer to the pixel buffer, used by drawing functions */
    pixel_buffer_start = *pixel_ctrl_ptr;
    clear_screen(); // pixel_buffer_start points to the pixel buffer
    /* set back pixel buffer to start of SDRAM memory */
    *(pixel_ctrl_ptr + 1) = 0xC0000000;
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); // we draw on the back buffer
    clear_screen(); // pixel_buffer_start points to the pixel buffer

    while (1)
    {
        // erases a box
        // box(x_box[i] - dx[i], y_box[i] - dy[i], BLACK);     
        // erases the line to connect the box
        // draw_line(x_box[i] - dx[i], y_box[i] - dy[i], x_box[(i + 1) % N] - dx[(i + 1) % N] * 2, y_box[(i + 1) % N] - dy[(i + 1) % N] * 2, BLACK);
        
        clear_screen(); // clears entire screen (slow but easy)

        // code for drawing the boxes and lines
        // code for updating the locations of boxes
        for (i = 0; i < N; i++) {
                
            // checks the x boundary conditions
            if (x_box[i] == 0 || x_box[i] == RESOLUTION_X - 3)
                dx[i] = -dx[i];

            // checks the y boundary conditions
		    if (y_box[i] == 0 || y_box[i] == RESOLUTION_Y - 3) 
                dy[i] = -dy[i];

            x_box[i] = x_box[i] + dx[i];    // moves x-coordinate of box to new location 
            y_box[i] = y_box[i] + dy[i];    // moves y-coordinate of box to new location 
            
            // draws a box
            box(x_box[i], y_box[i], color[i]);
            // draws the line to connect the box, by "% N" it causes a loop for the lines
            draw_line(x_box[i], y_box[i], x_box[(i + 1) % N], y_box[(i + 1) % N], color[i]);
        }
        wait_for_vsync(); // swap front and back buffers on VGA vertical sync
        pixel_buffer_start = *(pixel_ctrl_ptr + 1); // new back buffer
    }
}

void plot_pixel(int x, int y, short int line_color) 
{
    *(short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = line_color;
}

// converted pseudo-code for the line-drawing algorithm on lab handout to C language
void draw_line(int x0, int y0, int x1, int y1, short int line_color) 
{   
    bool is_steep = abs(y1 - y0) > abs(x1 - x0);

    if (is_steep) {
        swap(&x0, &y0);
        swap(&x1, &y1);
    }
    
    if (x0 > x1) {
        swap(&x0, &x1);
        swap(&y0, &y1);
    }

    int deltax = x1 - x0;	
    int deltay = abs(y1 - y0);
    int error = (-1)*(deltax / 2);

    int y = y0;
    int y_step;

    if (y0 < y1)
        y_step = 1;
    else    
        y_step = -1;

    int x;

    for (x = x0; x <= x1; x++) {
        if (is_steep) 
            plot_pixel(y, x, line_color);
        else 
            plot_pixel(x, y, line_color);

        error = error + deltay;

        if (error >= 0) {
            y = y + y_step;
            error = error - deltax;
        }
    }	
}

// generic swap helper function to be used in draw_line function
void swap(int *i, int *j) 
{
	int temp;

    temp = *i;
	*i = *j;
	*j = temp; 
}

// clears entire screen (easy but slow, should make more efficient in part 2/3)
void clear_screen() 
{
    int x;
    int y;
    for (x = 0; x < 320; x++) {
        for (y = 0; y < 240; y++) {
            short int color = *(short int *)(pixel_buffer_start + (y << 10) + (x << 1));

            // if a colour on the VGA display is not black, make it black (clears screen)
            if (color != BLACK) {
                plot_pixel(x, y, BLACK);
            } 
        }
    }
}

/* checks when the rendering is done in the code as a sign to swap the 
buffer registers, and waits for the swap to happen before returning */
void wait_for_vsync() 
{
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    register int status;

    // tells the VGA controller to begin the swap process 
    *pixel_ctrl_ptr = 1;    // writes a 1 into the S bit of the front buffer register (sets S bit = 1)

    // processor polls the S bit in the status register to determine when the swap actually happens
    status = *(pixel_ctrl_ptr + 3);    // sets status to be the value in the status register (S bit) of the buffer

    // ANDs the S bit with 1 and checks if the result is not 0; i.e. checks if the S bit is 1
    while ((status & 0x01) != 0) {
        status = *(pixel_ctrl_ptr + 3);     // writes a 1 into the S bit of the buffer register (sets S bit = 1)
    }
    // writes a 0 into the S bit (sets S bit = 0), causing swap to happen and can exit
}

// the line drawn is connected to the top-left corner of the box (it is drawn starting in top-left)
void box(int x, int y, int color) 
{
    int i, j;
    for (i = 0; i < 3; i++) 
        for (j = 0; j < 3; j++)
           plot_pixel(x + i, y + j, color);
}