// ReadPNG.cpp : Defines the entry point for the console application.
#include <cstdio>
#include <stdlib.h>
#include <istream>
#include "LoadPNG.h"

using namespace std;
using namespace LodePNG;

//for txtFormat
int init_file();
int close_file();

void movePastBlanks();

char give_file_section1();
int give_file_section2(int * list);
void next_section();

FILE * fp;
char * buff;
int howMany;


//for PNG
int init_decode();
void printPNG(void);
void printPixel(int x, int y);
void getPixel(int *, int x, int y);
void waitEnd(void);

Decoder decoder;
vector<unsigned char> buffer, image;
unsigned int width, height;
int * pixel;

int init_file()
{
	fp=fopen("test.txt", "r");
	if(fp == NULL)
		return -1;

	buff = new char[512];

	return 0;
}
void movePastBlanks()
{
	while(1)
	{
		fgets(buff, 512, fp);
		//printf("%s", buff);
		if(strncmp(buff, "#-------------\n", sizeof("#-------------\n")) == 0)
			break;
	}
}
char give_file_section1()//return char
{
	char temp;
	if(howMany >= 0)
		temp = fgets(buff, 512, fp)[0];
	else 
		return 0;

	howMany--;
	return temp;
}
int give_file_section2(int * list)//read 5
{
	if(howMany >= 0)
		fscanf(fp, "%d %d %d %d %d", &list[0], &list[1], &list[2], &list[3], &list[4]);
	else
		return 0;

	howMany --;
	return 1;
}

int give_file_section3(int * list)//read 5
{
	if(howMany >= 0)
		fscanf(fp, "%d %d %d %d %d", &list[0], &list[1], &list[2], &list[3], &list[4]);
	else
		return 0;

	howMany --;
	return 1;
}
int give_file_section4(int * list)//read 3
{
	if(howMany >= 0)
		fscanf(fp, "%d %d %d", &list[0], &list[1], &list[2]);
	else
		return 0;

	howMany --;
	return 1;
}
void next_section()
{
	movePastBlanks();
	fscanf(fp, "%d", &howMany);
}
int close_file()
{
	return fclose(fp);
}

int main(int argc, char* argv[])
{
	int ret = 0;
	int choice = 1;

	char tmp_c =  ' ';
	int * temp_i = new int [5];

	switch(choice)
	{
		case 0://PNG
			ret = init_decode();
			if(ret < 0)
				goto exit;

			printPNG();

			break;

		case 1://TXTFILE
			ret = init_file();
			if(ret < 0)
				goto exit;

			printf("Section 1:\n");
			next_section();
			printf("count:%d\n", howMany);
			
			while((tmp_c = give_file_section1()))
			{//you can read an individual value here
			}
			//note* no last here because this function does not use a *, the char == 0;
			printf("\n");

			printf("Section 2:\n");
			next_section();
			printf("count:%d\n", howMany);

			while(give_file_section2(temp_i))
			{//you can read an individual value here
			}
			printf("last: %d %d %d %d %d\n", temp_i[0], temp_i[1], temp_i[2], temp_i[3], temp_i[4]);
			printf("\n");

			printf("Section 3:\n");
			next_section();
			printf("count:%d\n", howMany);

			while(give_file_section3(temp_i))
			{//you can read an individual value here
			}
			printf("last: %d %d %d %d %d\n", temp_i[0], temp_i[1], temp_i[2], temp_i[3], temp_i[4]);
			printf("\n");

			printf("Section 4:\n");
			next_section();
			printf("count:%d\n", howMany);

			while(give_file_section4(temp_i))
			{//you can read an individual value here
			}
			printf("last: %d %d %d\n", temp_i[0], temp_i[1], temp_i[2]);
			printf("\n");

			ret = close_file();
			if(ret < 0)
				goto exit;
			break;
		default:
			break;
	}

exit:

	waitEnd();
	return 0;
}

int init_decode()
{
	loadFile(buffer, "test.png");
	decoder.decode(image, buffer.empty() ? 0 : &buffer[0], (unsigned)buffer.size());
	if(decoder.hasError())
	{
		printf("decode PNG error\n");
		return -1;
	}
	pixel = (int *) malloc(4 * sizeof(int));
	if(pixel == NULL)
		return -1;

	width = decoder.getWidth();
	height = decoder.getHeight();

	return 0;
}
int getPNGWidth()
{
	return width;
}
int getPNGHeight()
{
	return height;
}
void printPNG()
{
	for(unsigned y = 0; y < height; y ++)
	{
		for(unsigned x = 0; x < width; x ++)
			printPixel(x, y);
		printf("\n");
	}
}
void getPixel(int * pixel, int x,int y)
{
	unsigned int r=0, g=0, b=0, a=0;
	
	r = image[4 * y * width + 4 * x + 0]; //red
	g = image[4 * y * width + 4 * x + 1]; //green
	b = image[4 * y * width + 4 * x + 2]; //blue
	a = image[4 * y * width + 4 * x + 3]; //alpha

	pixel[0] = r;
	pixel[1] = g;
	pixel[2] = b;
	pixel[3] = a;
}
void printPixel(int x, int y)
{
	unsigned int r=0, g=0, b=0, a=0;
	
	r = image[4 * y * width + 4 * x + 0]; //red
	g = image[4 * y * width + 4 * x + 1]; //green
	b = image[4 * y * width + 4 * x + 2]; //blue
	a = image[4 * y * width + 4 * x + 3]; //alpha

	printf("(%d,%d,%d,%d)", r,g,b,a);
}
int pixelFiring(int x, int y)
{
	//if the pixel is black = 0, green = 1
	getPixel(pixel, x, y);
	if(pixel[0] == 0 && pixel[1] == 0 && pixel[2] == 0 && pixel[3] == 255)
		return 0;
	return 1;
}
void waitEnd()
{
	printf("Press Enter to Close\n");
	//cin.get();
}