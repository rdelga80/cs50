/**
 * Copies a BMP piece by piece, just because.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "bmp.h"

// parse float input -- sscanf, atof, cs50 get_float
// update outfile's header info -- interpret arrangement, bmp.h
//    -- new bitmap -> new header info, biWidth(no padding), biHeight - biSizeImage (total size)
//          -- bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + padding) * abs(bi.biHeiht)
//          -- bfSize (total size in file, including pixel, padding, and headers)
//              -- bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
// resize horizontally
// remember padding
// resize vertically
// new = bi.biWidth *= f, bi.biHeight *= f, others are dif
// Width = if f is two, then it will duplicate twice ex. 123 -> 112233
// fread() and fwright()
//
// padding - pixed is byte, length of scanline must be multiple of 4 bytes, if the number of pixels isn't multiple of 4, padding (0x00)
// padding = (4 - (bi-biWidth * sizeof(RGBTRIPLE)) % 4) % 4  -- out and infile may be different
//
// skip over infile padding -- fseek
// write outfile padding for each row -- fputc
//
// resize vertically -- figure out which row should be written // seek to that row // draw pixels into outfile // write outfile row's padding


int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }

    // remember factor
    float factor = atof(argv[1]);

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

    // create new headers
    BITMAPFILEHEADER newbf;
    BITMAPINFOHEADER newbi;

    newbi.biPlanes = bi.biPlanes;
    newbi.biBitCount = bi.biBitCount;
    newbi.biCompression = bi.biCompression;
    newbi.biXPelsPerMeter = bi.biXPelsPerMeter;
    newbi.biYPelsPerMeter = bi.biYPelsPerMeter;
    newbi.biClrUsed = bi.biClrUsed;
    newbi.biClrImportant = bi.biClrImportant;

    newbi.biSize = bi.biSize;

    newbf.bfType = bf.bfType;
    newbf.bfReserved1 = bf.bfReserved1;
    newbf.bfReserved2 = bf.bfReserved2;
    newbf.bfOffBits = bf.bfOffBits;

    // Adjust biWidth, bi
    newbi.biWidth = bi.biWidth * factor;
    newbi.biHeight = bi.biHeight * factor;
    int newPadding = (4 - (newbi.biWidth * sizeof(RGBTRIPLE)) % 4 ) % 4;
    newbi.biSizeImage = ((sizeof(RGBTRIPLE) * newbi.biWidth) + newPadding ) * abs(newbi.biHeight);
    newbf.bfSize = newbi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&newbf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&newbi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    // fprintf(stdout, "padding: %i\n", padding);

    // container for repeat row
    BYTE *rpt = malloc(newbi.biWidth * sizeof(RGBTRIPLE));

    if(factor > 1)
    {
        // iterate over infile's scanlines
        for (int i = 0, biHeight = abs(newbi.biHeight); i < (biHeight / factor); i++)
        {
            int rpt_ct = 0;

            // iterate over pixels in scanline
            for (int j = 0; j < bi.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                for (int n = 0; n < (int)roundf(factor); n++)
                {
                    // assign characters into repeat array
                    rpt[rpt_ct] = triple.rgbtBlue;
                    rpt[rpt_ct + 1] = triple.rgbtGreen;
                    rpt[rpt_ct + 2] = triple.rgbtRed;
                    rpt_ct = rpt_ct + sizeof(RGBTRIPLE);

                    // write RGB triple to outfile
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }

            }

            // skip over padding, if any
            fseek(inptr, padding, SEEK_CUR);

            // then add it back (to demonstrate how)
            for (int k = 0; k < newPadding; k++)
            {
                fputc(0x00, outptr);
            }

            // rows duplicate
            for(int n = 0; n < (factor - 1); n++)
            {
                // iterate through new width
                for(int l = 0; l < newbi.biWidth; l++)
                {
                    // add pixel from captured
                    for(int m = 0; m < sizeof(RGBTRIPLE); m++)
                    {
                        int loop = (l * sizeof(RGBTRIPLE)) + m;
                        fwrite(&rpt[loop], 1, 1, outptr);
                    }
                }
                for (int k = 0; k < newPadding; k++)
                {
                    fputc(0x00, outptr);
                }
            }
        }
    }
    // if factor one, just copy
    else if(factor == 1)
    {
        for (int i = 0, biHeight = abs(newbi.biHeight); i < biHeight; i++)
        {

            for (int j = 0; j < bi.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // write RBG triple to outfile
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);

            }

            // skip over padding, if any
            fseek(inptr, padding, SEEK_CUR);

            // then add it back (to demonstrate how)
            for (int k = 0; k < newPadding; k++)
            {
                fputc(0x00, outptr);
            }

        }

    }
    // if less than one, shrink
    else if(factor < 1 && factor > 0)
    {
        // determine number of blocks/row to skip
        int skip = (int)(1 / factor);

        // iterate through height rows
        for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
        {

            // rows not to skip, first and skip rows
            if(i == 0 || i % skip == 0)
            {
                // iterate through width
                for (int j = 0; j < bi.biWidth; j++)
                {
                    if(j == 0 || j % skip == 0)
                    {
                        // temporary storage
                        RGBTRIPLE triple;

                        // read RGB triple from infile
                        fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                        // write RBG triple to outfile
                        fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);

                    }
                    else
                    {
                        // skip over RGB block
                        fseek(inptr, sizeof(RGBTRIPLE), SEEK_CUR);
                    }
                }

                // skip over padding, if any
                fseek(inptr, padding, SEEK_CUR);

                // then add it back (to demonstrate how)
                for (int k = 0; k < newPadding; k++)
                {
                    fputc(0x00, outptr);
                }

            }
            else
            {
                // Skip a full row
                int full = (bi.biWidth * sizeof(RGBTRIPLE)) + padding;
                fseek(inptr, full, SEEK_CUR);
            }

        }
    }

    free(rpt);

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
