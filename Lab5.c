
	/*
	** Coding challenge:  calculate even-spacing energy
	*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define SQR(x) ((x)*(x))

/* Window size for tuning */

# define    window 7               // energy window 


int main(int argc, char *argv[])

{
int	        px[100],py[100];
int	        x,y;
int         prev_dist, next_dist, moveto_x, moveto_y;
int         total_contour_pt;
FILE        *fpt;
unsigned char  *image, *initial_contour_img, *sobel_image, *active_contour_image;
char        header[80];
int         COLS, ROWS, BYTES,i,j,r,c,pxx,pyy,r1,c1,max,min,iteration,m,n;
double      *sobelh_filtered, *sobelv_filtered, *sobel_filtered, sum, *win_energy, avg_distance,energy_min;
float       sum1,sum2,factor;
double      internal_energy1[window*window], internal_energy2[window*window], external_energy[window*window],internal_energy1_norm[window*window], internal_energy2_norm[window*window], external_energy_norm[window*window];
int         internal_energy1_max, internal_energy1_min, internal_energy2_max, internal_energy2_min, external_energy_max, external_energy_min;
int         r_min, c_min, gain;

if (argc != 3)
  {
  printf("Usage:  spacing [image] [text]\n");
  exit(0);
  }

/* Read the initial contour points */

if ((fpt=fopen(argv[2],"r")) == NULL)
{
  printf("Unable to open contour file for readinf\n");
  exit(0);
}
int count = 0;
for (total_contour_pt=0;total_contour_pt<100;total_contour_pt++)
{
    i = fscanf(fpt,"%d %d",&px[total_contour_pt],&py[total_contour_pt]);
    if (i != 2)
      break;
}
fclose(fpt);

/* Read the input image */

fpt = fopen(argv[1],"rb");
fscanf(fpt, "%s %d %d %d", header, &COLS, &ROWS, &BYTES);
image = (unsigned char *)calloc(ROWS*COLS, sizeof(unsigned char));
fread(image,1,COLS*ROWS,fpt);
fclose(fpt);

/* Display the image with initial contour points */

initial_contour_img = (unsigned char *)calloc(ROWS*COLS, sizeof(unsigned char));
active_contour_image = (unsigned char *)calloc(ROWS*COLS, sizeof(unsigned char));

/* Making copy of image for displaying contour points */
for (i=0;i<ROWS*COLS;i++)
{
  initial_contour_img[i] = image[i];
}

printf("%d",total_contour_pt);
for (i = 0; i < total_contour_pt; i++)
{
    for (r = -3; r <=3; r++)
    {
        initial_contour_img[(py[i]+r)*COLS + (px[i])] = 0;
    }
    for (c = -3; c <= 3; c++)
    {
        initial_contour_img[(py[i])*COLS + (px[i]+c)] = 0;
    }
}

fpt = fopen("intial_cont_image.ppm","wb");
printf("%d",3457);
fprintf(fpt,"P5 %d %d 255 ",COLS, ROWS);
fwrite(initial_contour_img,COLS*ROWS,sizeof(unsigned char),fpt);
fclose(fpt);

/* Sobel filter */

  int sobel_h[9] = {1, 0, -1, 2, 0, -2, 1, 0, -1};
  int sobel_v[9] = {1, 2, 1, 0, 0, 0, -1, -2, -1};

  sobelh_filtered = (double *)calloc(ROWS*COLS, sizeof(double));
  for (r=0; r<ROWS; r++)
  {
    for (c=0; c<COLS; c++)
    {
      sum = 0.0;

      if (r<1 || r>=(ROWS-1) || c<1 || c>=(COLS-1))
      {
        sobelh_filtered[r*COLS+c] = 0.0;
      }

      else
      {
          for (r1=-1; r1 <=1;r1++)
          {
            for (c1=-1; c1<=1; c1++)
            {
                sum += (image[(r+r1)*COLS+(c+c1)]*sobel_h[(r1+1)*3+(c1+1)]);
            }
          }
          sobelh_filtered[r*COLS+c] = sum;
      }
    }
  }

  sobelv_filtered = (double *)calloc(ROWS*COLS, sizeof(double));

  for (r=0; r<ROWS; r++)
  {
    for (c=0; c<COLS; c++)
    {
      sum =0.0;
      if (r<1 || r>=(ROWS-1) || c<1 || c>=(COLS-1))
      {
        sobelv_filtered[r*COLS+c] = 0.0;
      }

      else
      {
        for (r1=-1; r1 <= 1; r1++)
        {
          for (c1=-1; c1<=1; c1++)
          {
            sum += (image[(r+2)*COLS+(c+c1)]*sobel_v[(r1+1)*3+(c1+1)]);
          }
        }

        sobelv_filtered[r*COLS+c] = sum;
      }
    }
  }

  sobel_filtered = (double *)calloc(ROWS*COLS, sizeof(double));
  for (r=0; r<ROWS; r++)
  {
    for (c=0; c<COLS; c++)
    {
      sobel_filtered[r*COLS+c] = sqrt(pow(sobelh_filtered[r*COLS+c], 2) + pow(sobelv_filtered[r*COLS+c], 2));
    }
  }

        /* Normalizing */
  
  min = 999999999;
  max = 0;

  for (j=0; j < ROWS*COLS; j++)
  {
    if (sobel_filtered[j] > max) 
    {
      max = sobel_filtered[j];
      }
    if (sobel_filtered[j] < min)
    {
      min = sobel_filtered[j];
    }
  }

  gain = 0.0;
  factor = max - min;

  sobel_image = (unsigned char *)calloc(ROWS*COLS, sizeof(unsigned char));
for (r=0; r<ROWS; r++)
{
  for (c=0; c<COLS; c++)
  {
    if (r<1 || c<1)
    {
      sobel_image[r*COLS+c] = 0;
    }

    else{
      gain = ((sobel_filtered[r*COLS+c]-min)*255)/(factor);
      int k = round(gain);
      sobel_image[r*COLS+c] = k;
    }
  }
}
  fpt = fopen("sobel_image.ppm","wb");
  fprintf(fpt, "P5 %d %d 255\n", COLS, ROWS);
  fwrite(sobel_image, sizeof(unsigned char), ROWS*COLS, fpt);
  fclose(fpt);

  /* Active contour algorithm */
  win_energy = (double *)calloc(window*window, sizeof(double));
  /* Run active contour algorithm for 30 interations */

  for (iteration=0; iteration<30; iteration++)
  {

    /* run algorithm for all contour points */
    for (i=0; i<total_contour_pt; i++)
    {
      avg_distance = 0;

      for (j=0; j<total_contour_pt; j++)
      {

        if ((j+1)>=total_contour_pt)
        {
          avg_distance += sqrt(pow(px[j]-px[0],2) + pow(py[j]-py[0],2));
        }

        else
        {
          avg_distance += sqrt(pow(px[j]-px[j+1],2) + pow(py[j]-py[j+1],2));
        }
      }
        printf("%d\n",i);
        avg_distance = avg_distance / total_contour_pt;
        printf("%d\n",avg_distance);

        for (r=-window/2; r<=window/2; r++)
        {
          for (c=-window/2; c<=window/2; c++)
          {
            if((i+1)>=total_contour_pt)
            {
              internal_energy1[(r+(window/2))*window + (c+(window/2))] = (pow(px[0]-(px[i]+c),2) + pow(py[0]-(py[i]+r),2));
            }
            else
            {
              internal_energy1[(r+(window/2))*window + (c+(window/2))] = (pow(px[i+1]-(px[i]+c),2) + pow(py[i+1]-(py[i]+r),2));
            }                                                            

            internal_energy2[(r+(window/2))*window+(c+(window/2))] = pow(sqrt(internal_energy1[(r+(window/2))*window + (c+(window/2))])-avg_distance,2);
            external_energy[(r+(window/2))*window + (c+(window/2))] = pow(sobel_image[(py[i]+r)*COLS + (px[i]+c)], 2);
          }
        }
            internal_energy1_min = internal_energy2_min = external_energy_min = 9999999;
            internal_energy1_max = internal_energy2_max = external_energy_max = -9999999;

            for(n=0; n < window*window; n++)
            {
              if (internal_energy1[n] > internal_energy1_max)
              {
                internal_energy1_max = internal_energy1[n];
              }
              if (internal_energy1[n] < internal_energy1_min)
              {
                internal_energy1_min = internal_energy1[n];
              }

              if (internal_energy2[n] > internal_energy2_max)
              {
                internal_energy2_max  = internal_energy2[n];
              }
              if (internal_energy2[n] < internal_energy2_min)
              {
                internal_energy2_min  = internal_energy2[n];
              }

              if (external_energy[n] > external_energy_max)
              {
                external_energy_max = external_energy[n];
              }
              
              if (external_energy[n] < external_energy_min)
              {
                external_energy_min = external_energy[n];
              }
            }
            for (r = 0; r < window; r++)
            {
              for (c = 0; c < window; c++)
              {
                  internal_energy1_norm[r*window+c] = (internal_energy1[r*window+c]-internal_energy1_min)/(internal_energy1_max-internal_energy1_min);
                  internal_energy2_norm[r*window+c] = (internal_energy2[r*window+c]-internal_energy2_min)/(internal_energy2_max-internal_energy2_min);
                  external_energy_norm[r*window+c] = (external_energy[r*window+c]-external_energy_min)/(external_energy_max-external_energy_min);
              }
            }        

            for (r=0; r < window; r++)
            {
              for (c=0; c<window; c++)
              {
                win_energy[r*window+c] = ( internal_energy1_norm[r*window + c]) + (internal_energy2_norm[r*window + c]) - (external_energy_norm[r*window + c]);
              }
            }
            energy_min = INFINITY;
            r_min = 0;
            c_min = 0;   

            for (r=0; r<window; r++)
            {
              for (c=0; c<window; c++)
              {
                if (win_energy[r*window+c] < energy_min)
                {
                  energy_min = win_energy[r*window+c];
                  r_min = r-(window/2);
                  c_min = c-(window/2);

                }
              }
            } 
            px[i] = px[i] + c_min;
            py[i] = py[i] + r_min;         
    }
  }

    for (i=0; i<ROWS*COLS; i++) 
    {
        active_contour_image[i] = image[i];
    }

    for (i=0; i<total_contour_pt; i++)
    {
        for (r=-3; r<=3; r++)
        {
            active_contour_image[(py[i]+r)*COLS+ (px[i])] = 0;
        }
        for (c=-3; c<=3; c++)
        {
            active_contour_image[(py[i])*COLS+(px[i]+c)] = 0;
        }
    }


/* final contour image and contour text */
    fpt = fopen("active_contour_image.ppm","wb");
    fprintf(fpt,"%s %d %d 255 ",header,COLS, ROWS);
    fwrite(active_contour_image,COLS*ROWS,sizeof(unsigned char),fpt);
    fclose(fpt);

    fpt=fopen("contour_coordinates.txt","wb");
    for (i=0; i<total_contour_pt; i++)
    {
        fprintf(fpt, "%d %d \n", px[i], py[i]);
    }
    fclose(fpt);
}


