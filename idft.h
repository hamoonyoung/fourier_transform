/* 
* @license Moon Young Ha <https://github.com/hamoonyoung/fourier_transform>
*
* References:
* 
*/

#ifndef IDFT_H_GUARD
#define IDFT_H_GUARD

static int numoflns (FILE * const file);

int read_data (FILE * const file, double _Complex input_data[], int data_size);

void print_output_data(double output_data[], int size);

void idft(double _Complex input_data[], double output_data[], int length_sam, int points, int data_size);

#endif