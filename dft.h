/* 
* @license Moon Young Ha <https://github.com/hamoonyoung/fourier_transform>
*
* References:
* 
*/

#ifndef DFT_H_GUARD
#define DFT_H_GUARD

int data_size;
int frequency_range;


static int numoflns (FILE * const file);

double input_data;

double _Complex output_data;

int read_data (FILE * file, double input_data[], int data_size);

void print_output_data(double _Complex output_data[], int size);

void dft(double input_data[], double _Complex output_data[], int freq_range_max, int sample_rate, int data_size);

#endif