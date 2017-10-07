/**
 * Copies a BMP piece by piece, resizing by a given factor
 */
       
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <string.h>
#include "bmp.h"

void scale(RGBTRIPLE * triple, RGBTRIPLE * tripleN, int w1, int w2, int h1, int h2);

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }
    
    // get resize factor with input checking
    // --- the "errno", "XxEePp" and "tail" checks adapted from cs50.c get_float ---
    char *tail;
    errno = 0;
    float f = strtof(argv[1], &tail);
    if (errno != 0 || *tail != '\0' || strcspn(argv[1], "XxEePp") != strlen(argv[1]) 
        || f <= 0 || f > 100 )
    {
      fprintf(stderr, "Usage: ./copy n infile outfile\n");
      return 1;
    }
    
    // round to one decimal place
    f = roundf(f*10)/10;

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    
    
    BITMAPFILEHEADER bfNew;
    bfNew = bf;
    BITMAPINFOHEADER biNew;
    biNew = bi;
    
    // width and height before and after
    int w1, w2, h1, h2;
    
    // get the old and the new bitmap width and height (rounded, in pixels)
//    biNew.biWidth  = (int) round(bi.biWidth*f);
//    biNew.biHeight = (int) round(bi.biHeight*f);
    biNew.biWidth  = (int) (bi.biWidth*f);
    biNew.biHeight = (int) (bi.biHeight*f);
    w1 = bi.biWidth;
    h1 = abs(bi.biHeight);
    w2 = biNew.biWidth;
    h2 = abs(biNew.biHeight);
    
    // determine padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // set the changed header data and the new padding
    int paddingNew = (4 - (biNew.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    biNew.biSizeImage = ((sizeof(RGBTRIPLE)*w2) + paddingNew)*h2;
    bfNew.bfSize = biNew.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bfNew, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&biNew, sizeof(BITMAPINFOHEADER), 1, outptr);

     // big fat buffers - for the original image..
    RGBTRIPLE* triple;
    triple = malloc( w1 * abs(h1) * sizeof(RGBTRIPLE) );
    
    // and for the new one
    RGBTRIPLE* tripleN;
    tripleN = malloc( w2 * abs(h2) * sizeof(RGBTRIPLE) );
    if (triple == NULL || tripleN == NULL)
    {
		fprintf(stderr, "Not enough memory. Exiting.\n");
		return 5;	
	}

    /**
     * read infile into the first buffer
     */
    int p = 0;
    
    // iterate over infile's scanlines
    for (int i = 0; i < h1; i++)
    {
        
        // iterate over pixels in scanline
        for (int j = 0; j < w1; j++)
        {
            
            // read RGB triple from infile into the buffer
            fread(&triple[p], sizeof(RGBTRIPLE), 1, inptr);
            p++;
        }

        // skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);
    }
    
    // scale the image and load it in the second buffer
    scale(triple, tripleN, w1, w2, abs(h1), abs(h2) );
    
    // the first buffer is no longer needed
	free(triple);

    /**
     * write outfile from the second buffer
     */
     p = 0;
    // iterate over outfile's scanlines
    for (int i = 0; i < h2; i++)
    {
        
        // iterate over pixels in scanline
        for (int j = 0; j < w2; j++)
        {
            
            // write the modified buffer to outfile
            fwrite(&tripleN[p], sizeof(RGBTRIPLE), 1, outptr);
            p++;
        }

        // add the new padding
        for (int k = 0; k < paddingNew; k++)
        {
            fputc(0x00, outptr);
        }
    }

	// free memory from the second buffer
	free(tripleN);
	
    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}


/**
 * 
 * Nearest neibhor scaling algorithm
 * 
 */
void scale(RGBTRIPLE * triple, RGBTRIPLE * tripleN, int w1, int w2, int h1, int h2)
{
    
    // calculate x and y ratio
    float stX = ((float) (w1))/ ((float) (w2));
    float stY = ((float) (h1))/ ((float) (h2));

    // temporary buffers
    int index;

    // array counter for tripleN
    int p = 0;

    // rows
    for (int y2 = 0; y2 < h2; y2++)
    {   
        // columns
        for (int x2 = 0; x2 < w2; x2++)
        {

            // index in the old array for the nearest pixel
            index = (int)( (floor(y2*stY))*w1 + floor (x2*stX) );
            
            // copy the nearest pixel color info to the new array
            tripleN[p] = triple[index];

            // advance position in the new array
            p++;
        }
    }
}

