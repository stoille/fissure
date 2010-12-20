// ReadPNG.cpp : Defines the entry pounsigned for the console application.

#include <iostream>

#include "LoadPNG.h"
#include "InitViewer.h"

using namespace std;
using namespace LodePNG;
using namespace Fissure;

//for txtFormat
unsigned init_file();
unsigned close_file();

void movePastBlanks();

char give_file_section1();
unsigned give_file_section2(unsigned * list);
void next_section();

FILE * fp;
char * buff;
int howMany;


//for PNG
unsigned init_decode();
void prunsignedPNG(void);
void prunsignedPixel(unsigned x, unsigned y);
void getPixel(unsigned *, unsigned x, unsigned y);
void waitEnd(void);

Decoder decoder;
vector<unsigned char> buffer, image;
unsigned width, height;
unsigned * pixel;

using namespace std;

unsigned init_file(const string &fileName)
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
unsigned give_file_section2(unsigned * list)//read 5
{
	if(howMany >= 0)
		fscanf(fp, "%d %d %d %d %d", &list[0], &list[1], &list[2], &list[3], &list[4]);
	else
		return 0;

	howMany --;
	return 1;
}

unsigned give_file_section3(unsigned * list)//read 5
{
	if(howMany >= 0)
		fscanf(fp, "%d %d %d %d %d", &list[0], &list[1], &list[2], &list[3], &list[4]);
	else
		return 0;

	howMany --;
	return 1;
}
unsigned give_file_section4(unsigned * list)//read 3
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
unsigned close_file()
{
	return fclose(fp);
}
unsigned init_decode(const string &fileName)
{
	loadFile(buffer, fileName);
	decoder.decode(image, buffer.empty() ? 0 : &buffer[0], (unsigned)buffer.size());
	if(decoder.hasError())
	{
		printf("decode PNG error\n");
		return -1;
	}
	pixel = (unsigned *) malloc(4 * sizeof(unsigned));
	if(pixel == NULL)
		return -1;

	width = decoder.getWidth();
	height = decoder.getHeight();

	return 0;
}
unsigned getPNGWidth()
{
	return width;
}
unsigned getPNGHeight()
{
	return height;
}
void prunsignedPNG()
{
	for(unsigned y = 0; y < height; y ++)
	{
		for(unsigned x = 0; x < width; x ++)
			prunsignedPixel(x, y);
		printf("\n");
	}
}
void getPixel(unsigned * pixel, unsigned x,unsigned y)
{
	unsigned r=0, g=0, b=0, a=0;
	
	r = image[4 * y * width + 4 * x + 0]; //red
	g = image[4 * y * width + 4 * x + 1]; //green
	b = image[4 * y * width + 4 * x + 2]; //blue
	a = image[4 * y * width + 4 * x + 3]; //alpha

	pixel[0] = r;
	pixel[1] = g;
	pixel[2] = b;
	pixel[3] = a;
}
void prunsignedPixel(unsigned x, unsigned y)
{
	unsigned r=0, g=0, b=0, a=0;
	
	r = image[4 * y * width + 4 * x + 0]; //red
	g = image[4 * y * width + 4 * x + 1]; //green
	b = image[4 * y * width + 4 * x + 2]; //blue
	a = image[4 * y * width + 4 * x + 3]; //alpha

	printf("(%d,%d,%d,%d)", r,g,b,a);
}
unsigned pixelFiring(unsigned x, unsigned y)
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
	if(argc != 3)
	{
		cout<<"Syntax usage: Fissure <model filename> <type>"<<endl;
		return -1;
	}
	//our primary viewer and informational object
	InitViewer initViewer;

	char tmp_c =  ' ';
	unsigned * temp_i = new unsigned [5], ret = 0;

	string modelFileName(argv[1]);
	string modelFileType(argv[2]); //text, png
	string firingFileName;
	
	string text("text");
	string png("png");
	
	//initialize our model
	ret = init_file(modelFileName);
	if(ret < 0) return ret;

	//if we are type "text" or "png"
	
	//this is the text choice
	if(modelFileType.compare(text)==0)
	{
		//read the soma types
		cout<<"Section 1:"<<endl;
		next_section();
		cout<<"count:"<<howMany<<endl;
		while((tmp_c = give_file_section1()))
			if( (tmp_c >= 'A' && tmp_c <= 'Z') )
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
		unsigned numFiringCycles = 0;
		unsigned lastFiringCycle = 0;
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
			for(unsigned i = 2; 2 < argc-1; i+=2)
			{
				if(strcmp(argv[i],"-firing") == 0)
				{
					firingFileName = string(argv[i+1]);
					ret = init_decode(firingFileName);
					if(ret < 0) return ret;
				}
			}
		}
	}
	else if(modelFileType.compare(png)==0)//png mode
	{
		cout<<"Section 1 types:"<<endl;//only one type
		initViewer.AddSomaType('A');
		
		cout<<"Section 2 soma location:"<<endl;//add 252x252 grid of somas
		for(int i=0;i<252;i++)
			for(int x=0;x<252;x++)
			{
				initViewer.AddSoma(i*252+x,1,i,x,0);
			}
		
		cout<<"Section 3 synapse location:"<<endl;//interconnect the somas to all 8 neighbors
		int baseI, maxI, baseX, maxX;
		for(int i=0;i<252;i++)
			for(int x=0;x<252;x++)
			{
				if(i==0)//kinda bad
					baseI=0;
				else if(i==251)
					maxI=0;
				else
				{
					baseI=-1;
					maxI=2;
				}
				if(x==0)
					baseX=0;
				else if(x==251)
					maxX=0;
				else
				{
					baseX=-1;
					maxX=2;
				}
				for(int ti=baseI;ti<maxI;ti++)
					for(int tx=baseX;tx<maxX;tx++)
					{
						if(tx==0 || ti ==0)//cannot connect a soma to itself
							continue;
						
						initViewer.AddSynapse(i*252+x,(i+ti)*252 + (x+tx),i,x,-1);
					}
			}
		
		cout<<"Section 4 firing data:"<<endl;
		char fileName[128];
        strncpy(fileName, argv[1], 128);
		ifstream pngStream(fileName);
		char pngName[128];
		
		int timeStep = 0;//dur whoopsie
		while(pngStream.getline(pngName, 128))//loop on the file
		{
			if(pngName[0]=='\0')
				break;
			init_decode(pngName);
			for(int i=0;i<252;i++)//read the png image
			{
				for(int x=0;x<252;x++)
				{
					if(pixelFiring(i,x)==1)
						initViewer.AddFiring(timeStep,i*252 + x,1);
				}
			}
			timeStep++;//will go to the end of the file
		}
		
	}
	
	//start the osg viewer
	initViewer.StartViewer();
}