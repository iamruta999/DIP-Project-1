//NAME: Amruta Kulkarni
//USC ID: 6914970827
//USC Email: arkulkar@usc.edu
//Submission date: 20 Sept 2015

//Problem 1B, part 1
//This code reads a black and white image and converts it into an RGB image using Bilinear Demosaicing
//The Bayer pattern is RGRGRG... and then GBGBGB...
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[])
{
    FILE *file;
    
    int bytesPerPixelCFA = 1; //every pixel has 1 byte in CFA
    int bytesPerPixelbldm = 3; //every pixel has 3 bytes in bilinear demosaiced image
    int width = 424;
    int height = 636;
    
    int widthCFAextended = 426;
    int heightCFAextended = 638;
    
    unsigned char CFAImageData[height][width][bytesPerPixelCFA]; //array size will be 636 * 424 * 1
        
        if (!(file = fopen(argv[1], "rb")))
    {
        cout << "cannot open file: " << argv[1] << endl;
        exit(1);
    }
    
    fread(CFAImageData, sizeof(unsigned char), height*width*bytesPerPixelCFA, file);
    fclose(file);
    
    // Create an extended CFA image data and the data to store bilinear demosaiced pixel values
    unsigned char CFAImageDataExtended[heightCFAextended][widthCFAextended][bytesPerPixelCFA];//array size will be 638 * 426 *1
    unsigned char bldmImageData[height][width][bytesPerPixelbldm];//array size will be 636 * 424 * 1

    // Generate reflections in the extended CFA
    
    for(int row = 0; row < height; row++)
    {
        for(int column = 0;column < width; column++)
            
        {
            CFAImageDataExtended[row+1][column+1][0] = CFAImageData[row][column][0];//copy the CFA data in the central part of the extended image
                                                                                    //i.e from row1-column1 to row636-column424
            
            CFAImageDataExtended[0][column+1][0] = CFAImageData[1][column][0];      //copy row 1 of CFA to row 0 of extended.Column ext. starts from 1
            CFAImageDataExtended[637][column+1][0] = CFAImageData[634][column][0];  //copy row 634 of CFA to row 637 of extended.
            
        }
    }

    for (int row = 0; row < heightCFAextended; row++)
    {
        for(int column = 0; column < widthCFAextended; column++)
        {
            CFAImageDataExtended[row][0][0] = CFAImageDataExtended[row][2][0];      //copy column 2 of new extended to column 0
            CFAImageDataExtended[row][425][0] = CFAImageDataExtended[row][423][0];  //copy column 423 of new extended to column 425
        }
    }
    
    for (int row = 1; row < (heightCFAextended - 1); row++)
    {
        for(int column = 1; column < (widthCFAextended - 1); column++)
        {
            if (row % 2 == 0 && column % 2 == 0)                                    //pixel is blue
            {
                bldmImageData[row - 1][column - 1][0] = (CFAImageDataExtended[row + 1][column + 1][0] + //get red pixels along diagonals
                                                         CFAImageDataExtended[row + 1][column - 1][0] +
                                                         CFAImageDataExtended[row - 1][column + 1][0] +
                                                         CFAImageDataExtended[row - 1][column - 1][0])/4;

                bldmImageData[row - 1][column - 1][1] =  (CFAImageDataExtended[row + 1][column][0] +    //get green pixels at neighboring edges
                                                          CFAImageDataExtended[row - 1][column][0] +
                                                          CFAImageDataExtended[row][column + 1][0] +
                                                          CFAImageDataExtended[row][column - 1][0])/4 ;
                
                bldmImageData[row - 1][column - 1][2] = CFAImageDataExtended[row][column][0];           //blue remains unchanged
                
            }
            
            else if (row % 2 != 0 && column % 2 != 0)                                //pixel is red
            {
                bldmImageData[row - 1][column - 1][0] = CFAImageDataExtended[row][column][0];           //red remains unchanged
                
                bldmImageData[row - 1][column - 1][1] = (CFAImageDataExtended[row + 1][column][0] +     //get green pixels at neighboring edges
                                                        CFAImageDataExtended[row - 1][column][0] +
                                                        CFAImageDataExtended[row][column + 1][0] +
                                                        CFAImageDataExtended[row][column - 1][0])/4 ;
                
                bldmImageData[row - 1][column - 1][2] = (CFAImageDataExtended[row + 1][column + 1][0] + //get blue pixels along diagonals
                                                         CFAImageDataExtended[row + 1][column - 1][0] +
                                                         CFAImageDataExtended[row - 1][column + 1][0] +
                                                         CFAImageDataExtended[row - 1][column - 1][0])/4;

            }
            
            else if (row % 2 == 0 && column % 2 != 0)                              //pixel is green 
            {
                
                bldmImageData[row - 1][column - 1][0] = (CFAImageDataExtended[row + 1][column][0] +     //get red pixels grom vertical edges
                                                         CFAImageDataExtended[row - 1][column][0])/2;
                
                bldmImageData[row - 1][column - 1][1] = CFAImageDataExtended[row][column][0];           //green remains unchanged
                
                bldmImageData[row - 1][column - 1][2] = (CFAImageDataExtended[row][column + 1][0] +     //get blue pixels from horizontal edges
                                                         CFAImageDataExtended[row][column - 1][0])/2;
                
            }
            
            else                                                                   //pixel is green
            {
                    
                    bldmImageData[row - 1][column - 1][0] = (CFAImageDataExtended[row][column + 1][0] +     //get red pixels grom horizontal edges
                                                             CFAImageDataExtended[row][column - 1][0])/2;
                    
                    bldmImageData[row - 1][column - 1][1] = CFAImageDataExtended[row][column][0];           //green remains unchanged
                    
                    bldmImageData[row - 1][column - 1][2] = (CFAImageDataExtended[row + 1][column][0] +     //get blue pixels from vertical edges
                                                             CFAImageDataExtended[row - 1][column][0])/2;

            }
            
        }
    }
    
    //Write the bilinear demosaiced image to a new file
    
    if (!(file=fopen(argv[2],"wb")))
    {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(bldmImageData, sizeof(unsigned char), height*width*bytesPerPixelbldm, file);
	fclose(file);
    
	return 0;

}