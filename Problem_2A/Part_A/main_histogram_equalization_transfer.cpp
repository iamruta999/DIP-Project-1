//NAME: Amruta Kulkarni
//USC ID: 6914970827
//USC Email: arkulkar@usc.edu
//Submission date: 20 Sept 2015

//Problem 2A, Part 1
//This code performs Transfer function based histogram equalization method
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>

using namespace std;

int main(int argc, char *argv[])
{
 /* Define file pointer and variable */
    FILE *file;
    int bytesPerPixel = 3;
    int size = 512;
    
/* Allocate image data array */
    unsigned char imageData[size][size][bytesPerPixel];
    
/* Read image */
    if (!(file=fopen(argv[1],"rb")))
    {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(imageData, sizeof(unsigned char), size*size*bytesPerPixel, file);
	fclose(file);
    
/* Separate the three color planes */
    
   int red[size][size];
   int green[size][size];
   int blue[size][size];
    
    for (int row = 0; row < 512; row++)
        {
            for (int column = 0; column < 512; column++)
            {
                red[row][column] = imageData[row][column][0];
                
                green[row][column] = imageData[row][column][1];
                
                blue[row][column] = imageData[row][column][2];
                
            }
        }


/* CALCULATE HISTOGRAMS */
    
/* Histogram of red */
    int histRed[256];
    for (int i = 0; i<256; i++)
    {
        histRed[i] = 0;     //assign 0 to the array to avoid garbage values
    }
    
    int redLevel = 0;
    for(int row = 0; row < 512; row++)
    {
        for(int column = 0; column < 512; column++)
        {
            redLevel = red[row][column];
            histRed[redLevel] = histRed[redLevel] + 1;
        }
    }
    
/* Histogram of green */
    int histGreen[256];
    for (int j = 0; j<256; j++)
    {
        histGreen[j] = 0;     //assign 0 to the array to avoid garbage values
    }
    
    int greenLevel = 0;
    for(int row = 0; row < 512; row++)
    {
        for(int column = 0; column < 512; column++)
        {
            greenLevel = green[row][column];
            histGreen[greenLevel] = histGreen[greenLevel] + 1;
        }
    }        
    
/* Histogram of blue */
    int histBlue[256];
    for (int k = 0; k<256; k++)
    {
        histBlue[k] = 0;     //assign 0 to the array to avoid garbage values
    }
    
    int blueLevel = 0;
    for(int row = 0; row < 512; row++)
    {
        for(int column = 0; column < 512; column++)
        {
            blueLevel = blue[row][column];
            histBlue[blueLevel] = histBlue[blueLevel] + 1;
        }
    }
    
    
/* TRANSFER FUNCTION METHOD OF HISTOGRAM EQUALIZATION */
    
    float probRed[256], probGreen[256], probBlue[256];
    for(int i = 0; i<256; i++)
    {
        probRed[i] = (float)histRed[i]/(size*size);
        probGreen[i] = (float)histGreen[i]/(size*size);
        probBlue[i] = (float)histBlue[i]/(size*size);
        
    }
    
    float cdfRed[256];
    cdfRed[0] = probRed[0];
    for( int i = 1; i < 256; i++)
    {
        cdfRed[i] = probRed[i] + cdfRed[i-1];
    }
    
    float cdfGreen[256];
    cdfGreen[0] = probGreen[0];
    for( int i = 1; i < 256; i++)
    {
        cdfGreen[i] = probGreen[i] + cdfGreen[i-1];
    }
    
    float cdfBlue[256];
    cdfBlue[0] = probBlue[0];
    for( int i = 1; i < 256; i++)
    {
        cdfBlue[i] = probBlue[i] + cdfBlue[i-1];
    }
    
    unsigned char transferEqualizedImage[size][size][bytesPerPixel];
    
    for ( int row = 0; row < 512; row++ )
    {
        for( int column = 0; column < 512; column++)
        {
            transferEqualizedImage[row][column][0] = cdfRed[red[row][column]]*255;
            transferEqualizedImage[row][column][1] = cdfGreen[green[row][column]]*255;
            transferEqualizedImage[row][column][2] = cdfBlue[blue[row][column]]*255;
            
        }
    }
    
/* Write data to the final image file */
    
    if (!(file=fopen(argv[2],"wb")))
    {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(transferEqualizedImage, sizeof(unsigned char), size*size*bytesPerPixel, file);
	fclose(file);
    
    return 0;
    
}
