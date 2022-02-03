#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "complex_number.h"
#include "tobmp24.h"
#include "tobmp8.h"

#define MAX_ITERATIONS 255

#define IMG_HEIGHT 512
#define IMG_WIDTH  512
#define LIMIT 4.0E12

/*
#define RANGE 0.002536
#define RANGE_Y(R_X,H,W) (R_X * (double)H/W)
#define CENTER_X -0.758060
#define CENTER_Y 0.069731
*/

#define RANGE 2.6
#define RANGE_Y(R_X,H,W) (R_X * (double)H/W)
#define CENTER_X -0.75
#define CENTER_Y 0.0

unsigned char mandelbrot_set(struct complex_number c)
{
	struct complex_number z;
	z.r = 0.0f;
	z.i = 0.0f;
	int it = 0;

	while (it < MAX_ITERATIONS)
	{
		if (fabs(z.i) >= LIMIT || fabs(z.r) >= LIMIT)
			return it;
		z = c_add(c_mul(z, z), c); //z = z * z + c
		it++;
	}

	return it % MAX_ITERATIONS;
}


void render_mandelbrot_frame(int frame_width, int frame_height, long double range, struct complex_number center, int bit_depth, int palette, char* filename)
{
	unsigned char* img;
	img = (unsigned char*)malloc(frame_height* frame_width * sizeof(unsigned char));

	struct complex_number c;
	for (int y = 0; y < frame_height; y++)
	{
		for (int x = 0; x < frame_width; x++)
		{
			
			c.r = center.r - (range / 2) + range * ((double)x / (double)frame_width);
			c.i = center.i - (RANGE_Y(range,frame_height,frame_width) / 2) + RANGE_Y(range,frame_height,frame_width) * ((double)y / (double)frame_height);

			//printf("%f + %fi ",c.r,c.i);

			img[y * frame_width + x] = mandelbrot_set(c);
			if(img[y * frame_width + x] != 0)
			{
				img[y * frame_width + x] = (img[y * frame_width + x] + 0) % 255 + 1;
			}
		}
		//printf("line %d completed\n",y);
	}


	toBMP8bit(img, frame_width, frame_height, filename, palette);
	//toBMP24bit(img, frame_width, frame_height, "reg.bmp");
	//getchar();
	free(img);
}

int main(int argc, char** argv)
{
	struct complex_number center = {.r = CENTER_X, .i = CENTER_Y};
	render_mandelbrot_frame(IMG_WIDTH,IMG_HEIGHT,RANGE,center,8,1,"testing.bmp");
	return 0;
}