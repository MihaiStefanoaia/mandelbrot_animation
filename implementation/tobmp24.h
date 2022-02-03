#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct color {
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

double toRadians(float deg)
{
	return deg * 3.14159265 / 180;
}

struct color decode(int cod)
{
	struct color ret;
	if (cod == 0)
	{
		ret.r = 0;
		ret.g = 0;
		ret.b = 0;
	}
	else
	{
		ret.r = (0.5 + sin(toRadians((cod * 360 / 255.0f) +   0)) / 2) * 255;
		ret.g = (0.5 + sin(toRadians((cod * 360 / 255.0f) + 120)) / 2) * 255;
		ret.b = (0.5 + sin(toRadians((cod * 360 / 255.0f) + 240)) / 2) * 255;
	}
	return ret;
}



void toBMP24bit(unsigned char* img, int w, int h, char* filename)
{
	FILE* a = fopen(filename, "wb");

	unsigned char* header = (unsigned char*)malloc(14 * sizeof(unsigned char));
	unsigned char* dib_hd = (unsigned char*)malloc(40 * sizeof(unsigned char));
	unsigned char padding = (4 - (w * 3) % 4) % 4;
	
	int filesize = 0;
	filesize += 14;					//bmp header size
	filesize += 40;					//dib header size
	filesize += w * h * 3;			//size of the actual image
	filesize += h * padding;		//padding size

	header[0] = 'B';				//some header, idk exactly what is up with it
	header[1] = 'M';
	
	header[2] = filesize;			//file size
	header[3] = filesize >> 8;
	header[4] = filesize >> 16;
	header[5] = filesize >> 24;
	
	header[6] = 0;					//reserved 1, not used
	header[7] = 0;
	
	header[8] = 0;					//reserved 2, not used
	header[9] = 0;

	header[10] = 14 + 40;			//pixel data offset
	header[11] = 0;
	header[12] = 0;
	header[13] = 0;

	dib_hd[0] = 40;					//header size
	dib_hd[1] = 0;
	dib_hd[2] = 0;
	dib_hd[3] = 0;

	dib_hd[4] = w;					//image width
	dib_hd[5] = w>>8;
	dib_hd[6] = w>>16;
	dib_hd[7] = w>>24;

	dib_hd[8] = h;					//image height
	dib_hd[9] = h >> 8;
	dib_hd[10] = h >> 16;
	dib_hd[11] = h >> 24;

	dib_hd[12] = 1;					//planes
	dib_hd[13] = 0;

	dib_hd[14] = 24;				//bits per pixel 
	dib_hd[15] = 0;

	dib_hd[16] = 0;					//compression (none)
	dib_hd[17] = 0;
	dib_hd[18] = 0;
	dib_hd[19] = 0;

	dib_hd[20] = 0;					//image size with compression
	dib_hd[21] = 0; 
	dib_hd[22] = 0;
	dib_hd[23] = 0;

	dib_hd[24] = 0;					//x pixels per meter
	dib_hd[25] = 0;
	dib_hd[26] = 0;
	dib_hd[27] = 0;

	dib_hd[28] = 0;					//y pixels per meter
	dib_hd[29] = 0;
	dib_hd[30] = 0;
	dib_hd[31] = 0;

	dib_hd[32] = 0;					//total colors - default
	dib_hd[33] = 0;
	dib_hd[34] = 0;
	dib_hd[35] = 0;

	dib_hd[36] = 0;					//important colors - none
	dib_hd[37] = 0;
	dib_hd[38] = 0;
	dib_hd[39] = 0;

	for (int i = 0; i < 14; i++)
		putc(header[i], a);

	for (int i = 0; i < 40; i++)
		putc(dib_hd[i], a);

	for (int i = 0; i < h; i++)
	{
		
		for (int j = 0; j < w; j++)
		{
			int c = img[i * w + j];
			struct color tmp = decode(c);
			putc(tmp.b, a);
			putc(tmp.g, a);
			putc(tmp.r, a); 
		}

		for (int j = 0; j < padding; j++)
			putc(0, a);

	}
	free(header);
	free(dib_hd);
}