/**
 * Recovers JPEG files from a raw disc image
 */
       
#include <stdio.h>
#include <stdint.h>
       
int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }

    // filename pointers
    char *infile = argv[1];
    char outfile[7];

    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }
    
    // pointer for the output file
    FILE *outptr;

    // counter for the names of the found images
    int n = 0;

    // blocks
    const int block = 512;

    // 'block' bytes buffer
    uint8_t buff[block];
    
    // JPEG found/not found
    int found = 0;

    // read until EOF
    while ( fread(buff, block, 1, inptr) > 0 )
    {
        
        // check the first 4 bytes for header (always block-aligned)
        if ( buff[0] == 0xff && buff[1] == 0xd8 &&
             buff[2] == 0xff && (buff[3]&0xf0) == 0xe0)
        {

            // JPEG found!
            if (found == 1)
            {
                // first close the old image
                fclose(outptr);
            }
            
            // create new name
            sprintf(outfile, "%03i.jpg",n);

            n++;
            found = 1;
            
            // open the new file
            outptr = fopen(outfile, "w");
            if (outptr == NULL)
            {
                fclose(inptr);
                fprintf(stderr, "Could not create %s.\n", outfile);
                return 3;
            }
            
            // write the buffer
            fwrite(&buff, block, 1, outptr);
        }
        
        // no JPEG start found
        else if (found == 1)
           fwrite(&buff, block, 1, outptr);
            
    }

    // close infile
    fclose(inptr);

    // close outfile if open
    if (found == 1)
        fclose(outptr);

    // success
    return 0;
}
