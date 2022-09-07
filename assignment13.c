
	/*
	** This program reads bridge.ppm, a 512 x 512 PPM image.
	** It smooths it using a standard 3x3 mean filter.
	** The program also demonstrates how to time a piece of code.
	**
	** To compile, must link using -lrt  (man clock_gettime() function).
	*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main()

{
FILE		*fpt;
unsigned char	*image;
unsigned char	*smoothed;
char		header[320];
int		ROWS,COLS,BYTES;
int		sum,r,c,r2,c2;
float *smoothed1;
int i =0;
int p1 = 0;
int p2 = 0;
int retain_row;
struct timespec	tp1,tp2;

	/* read image */
if ((fpt=fopen("bridge.ppm","rb")) == NULL)
  {
  printf("Unable to open bridge.ppm for reading\n");
  exit(0);
  }
fscanf(fpt,"%s %d %d %d",header,&COLS,&ROWS,&BYTES);
if (strcmp(header,"P5") != 0  ||  BYTES != 255)
  {
  printf("Not a greyscale 8-bit PPM image\n");
  exit(0);
  }
image=(unsigned char *)calloc(ROWS*COLS,sizeof(unsigned char));
header[0]=fgetc(fpt);	/* read white-space character that separates header */
fread(image,1,COLS*ROWS,fpt);
fclose(fpt);

	/* allocate memory for smoothed version of image */
smoothed1 = (float *)calloc(ROWS * COLS, sizeof(float));
smoothed=(unsigned char *)calloc(ROWS*COLS,sizeof(unsigned char));

	/* query timer */
clock_gettime(CLOCK_REALTIME,&tp1);
printf("%ld %ld\n",(long int)tp1.tv_sec,tp1.tv_nsec);

	/* smooth image, skipping the border points */

// Column Part

for (r=0; r<ROWS; r++)
{
    sum=0;
    for (c=0; c < COLS; c++)
	{
		if (c < 7)
		{
			i = (r * COLS) + c;
			sum += image[i];
			if (c==6)
			{
				i = (r * COLS) +3;
				smoothed1[i] = sum;
			}
		}
		
		else
		{
			p2  = image[(r * COLS) + c];
			p1 = image[(r * COLS) + (c-7)];
			sum = p2 + (sum - p1);
			i = (r * COLS) + (c-3);
			smoothed1[i] = sum;
		}
	}
   }


// ROW

for (c = 0; c < COLS; c++)
{
	sum =0;
	retain_row = 3;
	for (r=0; r < ROWS; r++)
	{
		if (r < 7)
		{
			i = (r * COLS) + c;
			sum += smoothed1[i];
			if (r ==6)
			{
				i = (retain_row * COLS) + c;
				smoothed[i] = sum/49;
				retain_row++;
			}
		}

		else
		{
			p2 = smoothed1[(r * COLS) + c];
			p1 = smoothed1[((r -7) * COLS) + c];
			sum = p2 + (sum - p1);
			i = (retain_row * COLS) + c;
			smoothed[i] = sum/49;
			retain_row++;
		}
	}

}



/* query timer */
clock_gettime(CLOCK_REALTIME,&tp2);
printf("%ld %ld\n",(long int)tp2.tv_sec,tp2.tv_nsec);

	/* report how long it took to smooth */
printf("%ld\n",tp2.tv_nsec-tp1.tv_nsec);

	/* write out smoothed image to see result */
fpt=fopen("smoothed13.ppm","w");
fprintf(fpt,"P5 %d %d 255\n",COLS,ROWS);
fwrite(smoothed,COLS*ROWS,1,fpt);
fclose(fpt);
}
