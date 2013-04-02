#include "output.h"
#include <iostream>

/* prints out array of fftw_complex values.  The 'x' array is
 * the x-axis variable: time, energy, &c.
 */
void outputCVector(const char * fileName, fftw_complex * vec, double * x, int len, Parameters p) {
 // make output file
 FILE * output;
 output = fopen(fileName, "w");

 // define offsets for counters; these defaults are for 2-sided transform
 int xOffset = 0;	// offset of x counter relative to amplitude index
 int first = 0;
 int last = len;
 // one-sided FT
 if (!p.twoSided) {
  // unshifted FT
  if (p.noshift) {
   xOffset = len/2;
   last = len-(len/2);
  }
  // shifted FT
  else {
   first = len/2;
  }
 }

 // write to the output
 if (p.complex) {
  for (int ii = first; ii < last; ii++) {
   fprintf(output, "%-.7g %-.7g %-.7g\n", x[ii+xOffset], vec[ii][0], vec[ii][1]);
  }
 }
 else {
  for (int ii = first; ii < last; ii++) {
   fprintf(output, "%-.7g %-.7g\n", x[ii+xOffset], (pow(vec[ii][0],2) + pow(vec[ii][1],2)));
  }
 }

 // clean up the mess
 fclose(output);

 return;
}

/* Wrapper to outputCVector, which fftshifts the output.
 */
void outputCVectorShift(const char * fileName, fftw_complex * vec, double * x, int len, Parameters p) {
 // make a shifted copy of the vector to be printed
 fftw_complex * vec_shift;
 vec_shift = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*len);
 fftshift(vec, vec_shift, len);

 /*
 // make a shifted copy of the x array
 double * x_shift = new double [len];
 fftshift_double(x, x_shift, len);
 */

 // make the output
 outputCVector(fileName, vec_shift, x, len, p);

 // clean up the mess
 fftw_free(vec_shift);
 /*
 delete [] x_shift;
 */

 return;
}
