/* 
* @license Moon Young Ha <https://github.com/hamoonyoung/fourier_transform>
*
* References:
* http://www.gnuplot.info/
*
*/

#include <math.h>
#include <complex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <errno.h>
#include "dft.h"

int main (int argc, char ** argv){
	FILE * file ;
	char buffer[256];				// Temporary buffer
	int sample_rate = 0;	
	int freq_range_max = 10;
 	int data_size = 0;

	int opt = getopt(argc, argv, "f:");

	switch (opt){
		case 'f':
			file = fopen (optarg, "r");
			data_size = numoflns (file);
			if(!data_size){
				printf ("failed to read the data\n");
				exit(1);
			}
			break;
		default:
			fprintf (stderr, "Usage: %s [-f] input data path\n", argv[0]);
			exit(1);
	}

	printf ("\n%s has %d samples.\n", optarg, data_size);
	printf("\nSample Rate (default 1024): ");
	scanf("%s", buffer);
	if(!(sample_rate = atoi(buffer)))
		sample_rate = 1024;

	printf("Frequency Range (Max, default: 10 Hz): ");
	scanf("%s", buffer);
	if(!(freq_range_max = atoi(buffer)))
		freq_range_max = 10;

	double input_data[data_size];
	double _Complex output_data[freq_range_max];

	if (read_data (file, input_data, data_size))
	{
		printf ("failed to read the data\n");
		exit(1);
	}

	dft (input_data, output_data, freq_range_max, sample_rate, data_size);
	print_output_data (output_data, freq_range_max);

	return 0;
}

static int numoflns (FILE * const file)
{
	int buf_char = EOF;
	int buf_location = 0;
	int lines = 0;
	
	while((buf_char = getc(file)) != EOF)
	{

		if (buf_char == '\n')
		{
			lines++;
			buf_location = 0;

		}else{
			buf_location++;
		}
	}
	if (buf_char != '\n' && lines != 0 && buf_location > 0)
		lines++;
	rewind (file);
	return lines;
}


void dft(double input_data[], double _Complex output_data[], int freq_range_max, int sample_rate, int data_size){
	int N = data_size;
	for (int k = 1; k <= freq_range_max; k++){
		double _Complex X = 0.0;
		
		for (int n = 0 ; n < N; n++){
			double angle = 2.0 * M_PI * n * k/(sample_rate);
			X += (double _Complex) input_data[n] * cexp(angle * _Complex_I);
		}
		//printf ("%dHz\tx:%f\ty:%fi\n", k, creal(X), cimag(X));
		output_data[k] = (double _Complex) X/N;
	}
}


void print_output_data(double _Complex output_data[], int size){
	FILE * tempGraph = fopen ("dft_output.dat", "w");
	FILE * gnuplotPipe = popen ("gnuplot -persistent", "w");

	char * gnuCommands [5];
	gnuCommands[0] = "set title \"Discret Fourier Transform\"";
	gnuCommands[1] = "set xlabel \"Frequency (Hz)\"";
	gnuCommands[2] = "set ylabel \"Amplitude\"";
	gnuCommands[3] = "set grid";
	gnuCommands[4] = "plot 'dft_output.dat' using ($0+1):1 title 'x (real)' lt 1 pt 2 lc 1, 'dft_output.dat' using ($0+1):2 title 'y (imag)' lt 1 pt 4 lc 2";

	for(int i=1; i<=size; i++){
		fprintf(tempGraph, "%f\t%f\n", creal(output_data[i]), cimag(output_data[i]));
		printf ("%dHz\tx:%f\ty:%fi\n", i, creal(output_data[i]), cimag(output_data[i]));
	}

	// print via gnuplot
	for (int i=0; i < 5; i++)
	{
		fprintf(gnuplotPipe, "%s\n", gnuCommands[i]);
	}
	fflush(gnuplotPipe);
	fclose(tempGraph);

	printf ("printing data done.\n");
}

int read_data (FILE * file, double input_data[], int data_size){

	if (file == NULL){
		printf ("No file or data exists.\n");
		return 1;
	}

	char line [200];
	char * buf;
	char * eptr;
	int i = 0;

	while (fgets (line, sizeof(line), file), data_size)
	{
		buf = "";
		buf = strtok (line, "\t");
		if (buf) 
			input_data[i] = (double) strtod(buf, &eptr);
		buf = "";
		buf = strtok (NULL, "\n");
		i++;

		if(i>data_size)
			break;
	}
	printf ("%d entries have been read.\n", (i-1));
	fclose(file);
	return 0;
}
