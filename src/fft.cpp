#include <cmath>
#include <fftw3.h>
#include <unistd.h>
#include <iostream>

#include "output.h"

#define N 10001

int main (int argc, char ** argv) {
opterr = 0;
int c;

bool verbose = false;

/* process command line options */
while ((c = getopt(argc, argv, "2bfho:v")) != -1) {
 switch (c) {
  case '2':
   std::cout << "Output is two-sided FT.\n";
   break;
  case 'b':
   std::cout << "Perform backward FT.\n";
   break;
  case 'f':
   std::cout << "Perform forward FT.\n";
   break;
  case 'h':
   std::cout << "Print help.\n";
   break;
  case 'v':
   std::cout << "Print verbose information.\n";
   verbose = true;
   break;
  case 'o':
   std::cout << "Specify output filename.\n";
   break;
  case '?':
   if (optopt == 'o') {
    fprintf(stderr, "Option -%c requires a filename argument.\n", optopt);
   }
   else if (isprint(optopt)) {
    fprintf(stderr, "Unknown option -%c.\n", optopt);
   }
   else {
    fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
   }
   return 1;
  default:
   continue;
 }
}

if (argc == optind) {
 std::cerr << "ERROR: At least one input file must be specified.\n";
 return 1;
}

/* get input file name */
char * inputFile = NULL;
/* code for getting multiple inputs
 for (int ii = optind; ii < argc; ii++) {
}
*/
inputFile = argv[optind];
if (verbose) {
 std::cout << "The input file name is " << inputFile << ".\n";
}

 // properties of sine function
 double omega = 30.0;	// frequency (time a.u.)
 double phi = 1.0;	// phase

 // time and energy steps (a.u.)
 double dt = 0.02;
 double dE = 2.0*M_PI/dt/N;

 // arrays of times and energies
 double times [N];
 double energies [N];

 // FFT plans
 fftw_plan fftw_fwd;
 fftw_plan fftw_bck;

 // arrays to hold FFT'd quantities
 fftw_complex *in;
 fftw_complex *out;
 in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N);
 out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N);

 // set up FFT plans; should do this before setting up in/out arrays
 fftw_fwd = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
 fftw_bck = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

 // double in array, for test
 double ind [N];

 // set up in array
 for (int i = 0; i < N; i++) {
  in[i][0] = exp(-i*dt)*sin(omega*i*dt + phi) + cos(1.5*omega*i*dt + phi);
  in[i][1] = 0.0;
  ind[i] = in[i][0];
 }

 // set up time and energy arrays
 for (int i = 0; i < N; i++) {
  // time starts at 0
  times[i] = i*dt;
  // energies are centered around 0
  energies[i] = (i-(N/2))*dE;
 }

 // print out initial arrays
 outputCVector("in1.out", in, times, N);
 outputCVector("out1.out", out, energies, N);

 // do the forward FFT
 fftw_execute(fftw_fwd);

 // print the FFT directly
 writeFT("direct.out", ind, times, N);

 // print out in vector, shifted out vector
 outputCVector("in2.out", in, times, N);
 outputCVectorShift("out2.out", out, energies, N);

 // do the backward FFT
 fftw_execute(fftw_bck);

 // clean up the mess
 fftw_destroy_plan(fftw_fwd);
 fftw_destroy_plan(fftw_bck);
 fftw_free(in);
 fftw_free(out);

 return 0;
}
