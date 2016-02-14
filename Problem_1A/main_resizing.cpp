//NAME: Amruta Kulkarni
//USC ID: 6914970827
//USC Email: arkulkar@usc.edu
//Submission date: 20 Sept 2015

//Problem 1A
// This code reads in the image 'THE STARRY NIGHT' of size 512 * 512 and converts it into an image of size 650 * 650 using bilinear interpolation
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[])

{
    
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel;
	int Size = 256;
	
	// Check for proper syntax
	if (argc < 3)
    {
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Size = 256]" << endl;
		return 0;
	}
	
	// Check if image is grayscale or color
	if (argc < 4)
    {
		BytesPerPixel = 1; // default is grey image
	}
	else
    {
		BytesPerPixel = atoi(argv[3]);
		// Check if size is specified
		if (argc >= 5)
        {
			Size = atoi(argv[4]);
		}
	}
	
	// Allocate image data array
	unsigned char Imagedata[Size][Size][BytesPerPixel];
    
	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb")))
    {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
	fclose(file);
    
    //Bilinear interpolation
    int newHeight = 650;
    int newWidth = 650;
    unsigned char NewImagedata[newHeight][newWidth][BytesPerPixel];
    
    float scaleFactorX = float(511)/float(649); //To get scaled pixel location;
    float scaleFactorY = float(511)/float(649);
    
    for (int alongY = 0; alongY < newWidth; alongY++) //To get through every grid point of the new image to evaluate the intensity value at it

    {
        for (int alongX = 0; alongX < newHeight; alongX++)
        {
     
            float scaledLocationX = alongX * scaleFactorX; //To get the corresponding location in the actual 512*512
            float scaledLocationY = alongY * scaleFactorY;
    
            int intLocationX1 = int(scaledLocationX);
            int intLocationY2 = int(scaledLocationY);
            
            int intLocationX2 = intLocationX1 + 1;//Getting other three points
            int intLocationY1 = intLocationY2 + 1;// x1y2 x2y2
                                                  // x1y1 x2y1
            
            float xdiff = scaledLocationX - intLocationX1;
            float ydiff = scaledLocationY - intLocationY2;
            
            
            for (int rgb = 0; rgb < 3; rgb++)
            {
                NewImagedata[alongX][alongY][rgb] =
         (1-xdiff) * (1-ydiff) * Imagedata[intLocationX1][intLocationY2][rgb] +
         (xdiff)   * (1-ydiff) * Imagedata[intLocationX2][intLocationY2][rgb] +
         (1-xdiff) * (ydiff)   * Imagedata[intLocationX1][intLocationY1][rgb] +
         (xdiff)   * (ydiff)   * Imagedata[intLocationX2][intLocationY1][rgb];
            

            }
    
    
        }
    }
  
    
	// Write image data (filename specified by second argument) from image data matrix
    
	if (!(file=fopen(argv[2],"wb")))
    {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(NewImagedata, sizeof(unsigned char), newHeight*newWidth*BytesPerPixel, file);
	fclose(file);
    
	return 0;
}
