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
#include "idft.h"

int main (int argc, char ** argv){
	FILE * file ;
	char buffer[256];				// Temporary buffer
	int sample_rate = 0;	
	int length_sec = 1;
	int length_sam = 0;
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

	printf("Length (default: 1 second): ");
	scanf("%s", buffer);
	if(!(length_sec = atoi(buffer)))
		length_sec = 1;

	length_sam = length_sec * sample_rate;

	double _Complex input_data[data_size];
	double output_data[length_sam];

	if (read_data (file, input_data, data_size))
	{
		printf ("failed to read the data\n");
		exit(1);
	}

	idft (input_data, output_data, length_sam, sample_rate, data_size);
	print_output_data (output_data, length_sam);
	
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


void idft(double _Complex input_data[], double output_data[], int length_sam, int sample_rate, int data_size){
	int N = sample_rate;

	for (int k = 1; k <= data_size; k++){
		double _Complex X = 0.0;
		printf ("%dHz\t%f\t%f\n", k, creal(input_data[abs(k)]), cimag(input_data[abs(k)]));
		for (int n = 0 ; n < length_sam; n++){
			if (k == 1){
				output_data[n] = 0;
			}
			double angle = 2.0 * M_PI * n * k/N;
			output_data[n] += (double _Complex) input_data[abs(k)] * cexp(-angle * _Complex_I);
		}
	}
}

void print_output_data(double output_data[], int size){
	FILE * tempGraph = fopen ("idft_output.dat", "w");
	FILE * gnuplotPipe = popen ("gnuplot -persistent", "w");

	char * gnuCommands [5];
	gnuCommands[0] = "set title \"Waves\"";
	gnuCommands[1] = "set xlabel \"x\"";
	gnuCommands[2] = "set ylabel \"s(x)\"";
	gnuCommands[3] = "set grid";
	gnuCommands[4] = "plot 'idft_output.dat' using ($0):1 title 'f(x)' lt 5 lc 7  w lines";

	for(int i=0; i<size; i++){
		fprintf(tempGraph, "%f\n", output_data[i]);
		//printf ("[%d]\t%f\n", i, output_data[i]);
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

int read_data (FILE * file, double _Complex input_data[], int data_size){

	if (file == NULL){
		printf ("No file or data exists.\n");
		return 1;
	}

	char line [200];
	char * buf;
	char * eptr;
	int i = 1;

	while (fgets (line, sizeof(line), file), data_size)
	{
		buf = "";
		buf = strtok (line, "\t");
		if (buf) 
			input_data[i] = (double) strtod(buf, &eptr);
		buf = "";
		buf = strtok (NULL, "\t");
		if (buf) 
			input_data[i] += ((double) strtod(buf, &eptr)) *_Complex_I;
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
