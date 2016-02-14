//NAME: Amruta Kulkarni
//USC ID: 6914970827
//USC Email: arkulkar@usc.edu
//Submission date: 20 Sept 2015

//Problem 2B, Barn
//This code creates an oil painting effect on the barn image
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
    int width = 380;
    int height = 275;
    
    /* Allocate image data array */
    unsigned char imageData[height][width][bytesPerPixel];
    
    /* Read image */
    if (!(file=fopen(argv[1],"rb")))
    {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(imageData, sizeof(unsigned char), height*width*bytesPerPixel, file);
	fclose(file);
    
    /* Separate R,G,B matrices */
    
    int red[height][width];
    int green[height][width];
    int blue[height][width];
    
    for ( int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            red[row][column] = imageData[row][column][0];
            green[row][column] = imageData[row][column][1];
            blue[row][column] = imageData[row][column][2];
        }
    }
    
    /* Calculate Histograms for each channel */
    
    // For Red
    
    int histRed[256];
    for (int i = 0; i < 256; i++)
    {
        histRed[i] = 0;     //assign 0 to the array to avoid garbage values
    }
    
    int redLevel = 0;
    for(int row = 0; row < height; row++)
    {
        for(int column = 0; column < width; column++)
        {
            redLevel = red[row][column];
            histRed[redLevel] = histRed[redLevel] + 1;
        }
    }
    
    // For Green
    
    int histGreen[256];
    for (int j = 0; j < 256; j++)
    {
        histGreen[j] = 0;
    }
    
    int greenLevel = 0;
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            greenLevel = green[row][column];
            histGreen[greenLevel] = histGreen[greenLevel] + 1;
        }
    }
    
    // For Blue
    
    int histBlue[256];
    for (int k = 0; k < 256; k++)
    {
        histBlue[k] = 0;     //assign 0 to the array to avoid garbage values
    }
    
    int blueLevel = 0;
    for(int row = 0; row < height; row++)
    {
        for(int column = 0; column < width; column++)
        {
            blueLevel = blue[row][column];
            histBlue[blueLevel] = histBlue[blueLevel] + 1;
        }
    }
    
    /* Display the histogram of each channel */
    
    
    /*  for ( int intensity = 0; intensity < 256; intensity ++)
     {
     cout << "intensity = " << intensity << "\tred = " << histRed[intensity] << "\tgreen = " << histGreen[intensity] << "\tblue = " << histBlue[intensity] << "\n";
     
     }*/
    
    /* Quantization Bins and bin boundaries*/
    
    //For Red
    
    int sumRed = 0;
    int redBinBorder[5]; //create an array to store the bin boundaries
    redBinBorder[0] = 0; //minimum intensity level;
    redBinBorder[4] = 255; //maximum intensity level
    int indexRed = 1;
    
    for ( int redLevel = 0; redLevel < 256; redLevel++)
    {
        sumRed = sumRed + histRed[redLevel];
        if ( sumRed >= ((height*width)/4) )
        {
            cout << "redlevel = " << redLevel << "when sumRed is = " << sumRed <<"\n" ;
            sumRed = 0; // To get roughly height*width/4 pixels in each bin
            redBinBorder[indexRed] = redLevel;
            indexRed++;
            
        }
    }
    
    int averageRedBins[4]; //Calculate average of the bins
    int averageIndexR = 0;
    int sumR = 0;
    int indexR = 1;
    int countRed = 0;
    
    for ( int i = 0; i < 257; i++)
    {
        if (i <= redBinBorder[indexR])
        {
            countRed = countRed + 1;
            sumR = sumR + i;
        }
        
        else
        {
            averageRedBins[averageIndexR] = sumR/countRed;
            averageIndexR = averageIndexR + 1;
            indexR = indexR + 1;
            countRed = 0;
            sumR = 0;
            countRed = countRed + 1;
            sumR = sumR + i;
        }
    }
    
    /* cout << averageRedBins[0] << "\t" << averageRedBins[1] << "\t" << averageRedBins[2] << "\t" << averageRedBins[3] << "\t" << endl;*/
    
    // Quantize Red
    
    int quantizedRed[height][width]; //Create a new matrix to hold quantized values
    for ( int row = 0; row < height; row++)
    {
        for ( int column = 0; column < width; column++)
        {
            quantizedRed[row][column] = red[row][column];
        }
    }
    
    
    int redIntensity;
    for ( int row = 0; row < height; row++)
    {
        for ( int column = 0; column < width; column++)
        {
            redIntensity = quantizedRed[row][column];
            
            if (redIntensity <= redBinBorder[1])
                quantizedRed[row][column] = averageRedBins[0];
            
            else if (redIntensity > redBinBorder[1] && redIntensity <= redBinBorder[2])
                quantizedRed[row][column] = averageRedBins[1];
            
            else if (redIntensity > redBinBorder[2] && redIntensity <= redBinBorder[3])
                quantizedRed[row][column] = averageRedBins[2];
            
            else if (redIntensity > redBinBorder[3] && redIntensity <= redBinBorder[4])
                quantizedRed[row][column] = averageRedBins[3];
            
        }
    }
    
    //FOR GREEN
    
    int sumGreen = 0;
    int greenBinBorder[5];//create an array to store the bin boundaries
    greenBinBorder[0] = 0; //minimum intensity level;
    greenBinBorder[4] = 255; //maximum intensity level
    int indexGreen = 1;
    
    for ( int greenLevel = 0; greenLevel < 256; greenLevel++)
    {
        sumGreen = sumGreen + histGreen[greenLevel];
        if ( sumGreen >= ((height*width)/4) )
        {
            cout << "greenlevel = " << greenLevel << "when sumGreen is = " << sumGreen <<"\n" ;
            sumGreen = 0; // To get roughly height*width/4 pixels in each bin
            greenBinBorder[indexGreen] = greenLevel;
            indexGreen++;
            
        }
    }
    
    int averageGreenBins[4]; //Calculate average of the bins
    int averageIndexG = 0;
    int sumG = 0;
    int indexG = 1;
    int countGreen = 0;
    
    for ( int i = 0; i < 257; i++)
    {
        if (i <= greenBinBorder[indexG])
        {
            countGreen = countGreen + 1;
            sumG = sumG + i;
        }
        
        else
        {
            averageGreenBins[averageIndexG] = sumG/countGreen;
            averageIndexG = averageIndexG + 1;
            indexG = indexG + 1;
            countGreen = 0;
            sumG = 0;
            countGreen = countGreen + 1;
            sumG = sumG + i;
        }
    }
    
    /* cout << averageGreenBins[0] << "\t" << averageGreenBins[1] << "\t" << averageGreenBins[2] << "\t" << averageGreenBins[3] << "\t" << endl;*/
    
    // Quantize Green
    
    int quantizedGreen[height][width]; //Create a new matrix to hold quantized values
    for ( int row = 0; row < height; row++)
    {
        for ( int column = 0; column < width; column++)
        {
            quantizedGreen[row][column] = green[row][column];
        }
    }
    
    
    int greenIntensity;
    for ( int row = 0; row < height; row++)
    {
        for ( int column = 0; column < width; column++)
        {
            greenIntensity = quantizedGreen[row][column];
            
            if (greenIntensity <= greenBinBorder[1])
                quantizedGreen[row][column] = averageGreenBins[0];
            
            else if (greenIntensity > greenBinBorder[1] && greenIntensity <= greenBinBorder[2])
                quantizedGreen[row][column] = averageGreenBins[1];
            
            else if (greenIntensity > greenBinBorder[2] && greenIntensity <= greenBinBorder[3])
                quantizedGreen[row][column] = averageGreenBins[2];
            
            else if (greenIntensity > greenBinBorder[3] && greenIntensity <= greenBinBorder[4])
                quantizedGreen[row][column] = averageGreenBins[3];
            
        }
    }
    
    
    
    //FOR BLUE
    
    int sumBlue = 0;
    int blueBinBorder[5]; //create an array to store the bin boundaries
    blueBinBorder[0] = 0; //minimum intensity level;
    blueBinBorder[4] = 255; //maximum intensity level
    int indexBlue = 1;
    
    for ( int blueLevel = 0; blueLevel < 256; blueLevel++)
    {
        sumBlue = sumBlue + histBlue[blueLevel];
        
        if ( sumBlue >= ((height*width)/4) )
        {
            cout << "bluelevel = " << blueLevel << "when sumBlue is = " << sumBlue <<"\n" ;
            sumBlue = 0; // To get roughly height*width/4 pixels in each bin
            blueBinBorder[indexBlue] = blueLevel;
            indexBlue++;
        }
        
    }
    
    int averageBlueBins[4]; //Calculate average of the bins
    int averageIndexB = 0;
    int sumB = 0;
    int indexB = 1;
    int countBlue = 0;
    
    for ( int i = 0; i < 257; i++)
    {
        if (i <= blueBinBorder[indexB])
        {
            countBlue = countBlue + 1;
            sumB = sumB + i;
        }
        
        else
        {
            averageBlueBins[averageIndexB] = sumB/countBlue;
            averageIndexB = averageIndexB + 1;
            indexB = indexB + 1;
            countBlue = 0;
            sumB = 0;
            countBlue = countBlue + 1;
            sumB = sumB + i;
        }
    }
    
    /* cout << averageBlueBins[0] << "\t" << averageBlueBins[1] << "\t" << averageBlueBins[2] << "\t" << averageBlueBins[3] << "\t" << endl;*/
    
    
    // Quantize Blue
    
    int quantizedBlue[height][width]; //Create a new matrix to hold quantized values
    for ( int row = 0; row < height; row++)
    {
        for ( int column = 0; column < width; column++)
        {
            quantizedBlue[row][column] = blue[row][column];
        }
    }
    
    
    int blueIntensity;
    for ( int row = 0; row < height; row++)
    {
        for ( int column = 0; column < width; column++)
        {
            blueIntensity = quantizedBlue[row][column];
            
            if (blueIntensity <= blueBinBorder[1])
                quantizedBlue[row][column] = averageBlueBins[0];
            
            else if (blueIntensity > blueBinBorder[1] && blueIntensity <= blueBinBorder[2])
                quantizedBlue[row][column] = averageBlueBins[1];
            
            else if (blueIntensity > blueBinBorder[2] && blueIntensity <= blueBinBorder[3])
                quantizedBlue[row][column] = averageBlueBins[2];
            
            else if (blueIntensity > blueBinBorder[3] && blueIntensity <= blueBinBorder[4])
                quantizedBlue[row][column] = averageBlueBins[3];
            
        }
    }
    
    /* Final Quantized Image */
    
    unsigned char quantizedImageData[height][width][bytesPerPixel];
    for ( int row = 0; row < height; row++)
    {
        for ( int column = 0; column < width; column++)
        {
            quantizedImageData[row][column][0] = quantizedRed[row][column];
            quantizedImageData[row][column][1] = quantizedGreen[row][column];
            quantizedImageData[row][column][2] = quantizedBlue[row][column];
        }
    }
    
    
    if (!(file=fopen(argv[2],"wb")))
    {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(quantizedImageData, sizeof(unsigned char), height*width*bytesPerPixel, file);
	fclose(file);
    
    /* Filtering for oil Painting effect */
    
    unsigned char oilPaintingImageData[height][width][bytesPerPixel];
    int N = 5;
    int neighborhoodMatrix[N][N][3];
    int center = (N-1)/2;
    int intensity[3];
    int table[N*N][3];
    int maxFrequency = 0;
    int finalValue[3];
    int currentValue[3];
    int currentFrequency = 0;
    
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            for ( int i = 0; i < N; i++)
            {
                for ( int j = 0; j < N; j++)
                {
                    neighborhoodMatrix[i][j][0] = quantizedImageData[row + (i - center)][column + (j - center)][0];
                    neighborhoodMatrix[i][j][1] = quantizedImageData[row + (i - center)][column + (j - center)][1];
                    neighborhoodMatrix[i][j][2] = quantizedImageData[row + (i - center)][column + (j - center)][2];
                }
            }
            
            int tableRow = 0;
            for (int i = 0; i < N; i++)
            {
                for (int j = 0; j < N; j++)
                {
                    intensity[0] = neighborhoodMatrix[i][j][0];
                    intensity[1] = neighborhoodMatrix[i][j][1];
                    intensity[2] = neighborhoodMatrix[i][j][2];
                    
                    
                    for (int column = 0; column < 3; column++)
                    {
                        table[tableRow][column] = intensity[column];
                    }
                    tableRow = tableRow + 1;
                }
            }
            
            for ( int row1 = 0; row1 < ((N*N)-1) ; row1++)
            {
                for ( int row2 = row1 + 1; row2 < (N*N); row2++)
                {
                    if (table[row1][0] == table[row2][0] && table[row1][1] == table[row2][1] && table[row1][2] == table[row2][2])
                    {
                        
                        currentValue[0] = table[row1][0]; currentValue[1] = table[row1][1]; currentValue[2] = table[row1][2];
                        currentFrequency = currentFrequency + 1;
                        
                    }
                }
                
                if (currentFrequency > maxFrequency)
                {
                    finalValue[0] = currentValue[0]; finalValue[1] = currentValue[1]; finalValue[2] = currentValue[2];
                    maxFrequency = currentFrequency;
                    
                }
                
                currentFrequency = 0;
                
            }
            
            maxFrequency = 0;
            
            
            oilPaintingImageData[row][column][0] = finalValue[0];
            oilPaintingImageData[row][column][1] = finalValue[1];
            oilPaintingImageData[row][column][2] = finalValue[2];
            
            
        }
        
    }
    
    
    /*for ( int i = 0; i < N; i++)
     {
     for ( int j = 0; j < N; j++)
     {
     cout << neighborhoodMatrix[i][j][0] << "\t" << neighborhoodMatrix[i][j][1] << "\t" << neighborhoodMatrix[i][j][2] << "\n";
     }
     }*/
    
    
    
    
    if (!(file=fopen(argv[3],"wb")))
    {
		cout << "Cannot open file: " << argv[3] << endl;
		exit(1);
	}
	fwrite(oilPaintingImageData, sizeof(unsigned char), height*width*bytesPerPixel, file);
	fclose(file);
    
    
    return 0;
}