//NAME: Amruta Kulkarni
//USC ID: 6914970827
//USC Email: arkulkar@usc.edu
//Submission date: 20 Sept 2015

//Problem 2A, Part 2
//This code performs Cumulative-probability based histogram equalization method
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
    
    
    /* CUMULATIVE-PROBABILITY(BUCKET FILLING) METHOD OF EQUALIZATION */
    
    // For Red channel
    
    int equalizedRed[size][size];
    
    for ( int row = 0; row < 512; row++)
    {
        for (int column = 0; column < 512; column++)
        {
            equalizedRed[row][column] = red[row][column];
        }
    }
    
    int elementRed;
    int newIntensityRed = 0;
    int pixelCountRed = 0;
    
    for (int intensity = 0; intensity < 256; intensity++)
    {
        for ( int row = 0; row < 512; row++)
        {
            for (int column = 0; column < 512; column++)
            {
                elementRed = red[row][column];
                
                if (elementRed == intensity)
                {
                    if(pixelCountRed % 1024 !=0 || pixelCountRed == 0)
                    {
                        equalizedRed[row][column] = newIntensityRed;
                    }
                    
                    else if(pixelCountRed % 1024 == 0 && pixelCountRed != 0)
                    {
                        newIntensityRed = newIntensityRed + 1;
                        equalizedRed[row][column] = newIntensityRed;
                    }
                    pixelCountRed = pixelCountRed + 1;
                }
            }
        }
        
    }
    
    
    // For Green channel
    int equalizedGreen[size][size];
    
    for ( int row = 0; row < 512; row++)
    {
        for (int column = 0; column < 512; column++)
        {
            equalizedGreen[row][column] = green[row][column];
        }
    }
    
    int elementGreen;
    int newIntensityGreen = 0;
    int pixelCountGreen = 0;
    
    for (int intensity = 0; intensity < 256; intensity++)
    {
        for ( int row = 0; row < 512; row++)
        {
            for (int column = 0; column < 512; column++)
            {
                elementGreen = green[row][column];
                
                if (elementGreen == intensity)
                {
                    if(pixelCountGreen % 1024 !=0 || pixelCountGreen == 0)
                    {
                        equalizedGreen[row][column] = newIntensityGreen;
                    }
                    
                    else if(pixelCountGreen % 1024 == 0 && pixelCountGreen != 0)
                    {
                        newIntensityGreen = newIntensityGreen + 1;
                        equalizedGreen[row][column] = newIntensityGreen;
                    }
                    pixelCountGreen = pixelCountGreen + 1;
                }
            }
        }
        
    }
    
    
    // For Blue channel
    int equalizedBlue[size][size];
    
    for ( int row = 0; row < 512; row++)
    {
        for (int column = 0; column < 512; column++)
        {
            equalizedBlue[row][column] = blue[row][column];
        }
    }
    
    int elementBlue;
    int newIntensityBlue = 0;
    int pixelCountBlue = 0;
    
    for (int intensity = 0; intensity < 256; intensity++)
    {
        for ( int row = 0; row < 512; row++)
        {
            for (int column = 0; column < 512; column++)
            {
                elementBlue = blue[row][column];
                
                if (elementBlue == intensity)
                {
                    if(pixelCountBlue % 1024 !=0 || pixelCountBlue == 0)
                    {
                        equalizedBlue[row][column] = newIntensityBlue;
                    }
                    
                    else if(pixelCountBlue % 1024 == 0 && pixelCountBlue != 0)
                    {
                        newIntensityBlue = newIntensityBlue + 1;
                        equalizedBlue[row][column] = newIntensityBlue;
                    }
                    pixelCountBlue = pixelCountBlue + 1;
                }
            }
        }
        
    }
    
    
    /* Creating ouput image data */
    
    unsigned char cdf_Equalized_Image[size][size][bytesPerPixel];
    
    for ( int row = 0; row < 512; row++)
    {
        for( int column = 0; column < 512; column++)
        {
            cdf_Equalized_Image[row][column][0] = equalizedRed[row][column];
            cdf_Equalized_Image[row][column][1] = equalizedGreen[row][column];
            cdf_Equalized_Image[row][column][2] = equalizedBlue[row][column];
        }
    }
    
    if (!(file=fopen(argv[2],"wb")))
    {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(cdf_Equalized_Image, sizeof(unsigned char), size*size*bytesPerPixel, file);
	fclose(file);
    
    
    return 0;
    
}
