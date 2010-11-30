// ReadPNG.cpp : Defines the entry point for the console application.

#include <iostream>

#include "LoadPNG.h"
#include "InitViewer.h"

using namespace std;
using namespace LodePNG;
using namespace Fissure;

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

using namespace std;

int init_file(const string &fileName)
{
	fp=fopen(fileName.c_str(), "r");
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
int init_decode(const string &fileName)
{
	loadFile(buffer, fileName);
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
int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		cout<<"Syntax usage: Fissure <model filename>"<<endl;
		return -1;
	}
	//our primary viewer and informational object
	InitViewer initViewer;

	char tmp_c =  ' ';
	int * temp_i = new int [5], ret = 0;

	string modelFileName(argv[1]);
	string firingFileName;
	
	//initialize our model
	ret = init_file(modelFileName);
	if(ret < 0) return ret;

	//read the soma types
	cout<<"Section 1:"<<endl;
	next_section();
	cout<<"count:"<<howMany<<endl;
	while((tmp_c = give_file_section1()))
		initViewer.AddSomaType(tmp_c);

	//read the soma info
	cout<<"Section 2:"<<endl;
	next_section();
	cout<<"count:"<<howMany<<endl;
	while(give_file_section2(temp_i))
		initViewer.AddSoma(temp_i[0],temp_i[1],temp_i[2],temp_i[3],temp_i[4]);
	cout<<"last:"<<temp_i[0]<<temp_i[1]<<temp_i[2]<<temp_i[3]<<temp_i[4]<<endl;

	//read the synapse info
	cout<<"Section 3:"<<endl;
	next_section();
	cout<<"count:"<<howMany<<endl;
	while(give_file_section3(temp_i))
		initViewer.AddSynapse(temp_i[0],temp_i[1],temp_i[2],temp_i[3],temp_i[4]);
	cout<<"last:"<<temp_i[0]<<temp_i[1]<<temp_i[2]<<temp_i[3]<<temp_i[4]<<endl;

	//read firing data
	int numFiringCycles = 0;
	int lastFiringCycle = 0;
	cout<<"Section 4:"<<endl;
	next_section();
	cout<<"count:"<<howMany<<endl;
	while(give_file_section4(temp_i))
	{
		initViewer.AddFiring(temp_i[0],temp_i[1],temp_i[2]);
		//this assumes they come in order
		if(lastFiringCycle != temp_i[0])
			++numFiringCycles;
		lastFiringCycle = temp_i[0];
	}
	cout<<"last:"<<temp_i[0]<<temp_i[1]<<temp_i[2]<<endl;
	if(close_file() < 0) return -1;
	initViewer.SetNumFiringCycles(numFiringCycles);

	//initialize any extra firing data or options on our model
	if(argc > 1)
	{
		for(int i = 2; 2 < argc-1; i+=2)
		{
			if(strcmp(argv[i],"-firing") == 0)
			{
				firingFileName = string(argv[i+1]);
				ret = init_decode(firingFileName);
				if(ret < 0) return ret;
			}
		}
	}

	//start the osg viewer
	initViewer.StartViewer();
}