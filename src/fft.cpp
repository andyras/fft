#include <cmath>
#include <string>
#include <fftw3.h>
#include <unistd.h>
#include <iostream>

#include "typedefs.h"
#include "output.h"

#define N 10001

int main (int argc, char ** argv) {
 opterr = 0;
 int c;

 /* structure with default parameters */
 Parameters p;
 p.twoSided = false;
 p.backwardFT = false;
 p.complex = false;
 p.forwardFT = true;
 p.verbose = false;
 p.outputExt = "FT";
 p.printHelp = false;
 p.noshift = false;

 /* process command line options */
 while ((c = getopt(argc, argv, "2bcfho:sv")) != -1) {
  switch (c) {
   case '2':
    if (p.verbose) {
     std::cout << "Output is two-sided FT.\n";
    }
    p.twoSided = true;
    break;
   case 'b':
    if (p.verbose) {
     std::cout << "Perform backward FT.\n";
    }
    p.backwardFT = true;
    p.forwardFT = false;
    break;
   case 'c':
    if (p.verbose) {
     std::cout << "Output complex FT.\n";
    }
    p.complex = true;
    break;
   case 'f':
    if (p.verbose) {
     std::cout << "Perform forward FT.\n";
    }
    p.forwardFT = true;
    p.backwardFT = false;
    break;
   case 'h':
    if (p.verbose) {
     std::cout << "Print help.\n";
    }
    p.printHelp = true;
    break;
   case 'v':
    if (p.verbose) {
     std::cout << "Print verbose information.\n";
    }
    p.verbose = true;
    break;
   case 'o':
    p.outputExt = optarg;
    if (p.verbose) {
     std::cout << "Specifying output filename pre-extension: " << p.outputExt << ".\n";
    }
    break;
   case 's':
    p.noshift = true;
    if (p.verbose) {
     std::cout << "Not shifting FT'd output.\n";
    }
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

 if (p.printHelp) {
  std::cout << "\n"
            << "fft: a utility for taking fast Fourier transforms of data.\n"
            << "Usage: fft [-2] [-b] [-f] [-c] [-h] [-o] ext [-s] [-v] inputfile\n"
	    << "\n"
	    << "-2: Output 2-sided FT. Default is 1-sided (positive) values.\n"
	    << "-b: [NOT IMPLEMENTED] Perform backward FT. Default is forward FT.\n"
	    << "-f: Perform forward (time to energy) FT (default).\n"
	    << "-c: Output complex (real and imaginary) amplitude.\n"
	    << "    Real is first amplitude column in the output, imaginary is second.\n"
	    << "    Default is to print the mod of the amplitude.\n"
	    << "-h: Print this help message.\n"
	    << "-o: Specify a filename extension. The default is to append 'FT' to the\n"
	    << "    filename, e.g. input.dat --> inputFT.dat.\n"
	    << "-s: No shifting of transformed output. Default is to shift.\n"
	    << "-v: Print verbose information about what the program is doing.\n"
	    << "\n"
	    << "fft outputs a text file, the first column is the transformed coordinate\n"
	    << "and the other column(s) is(are) the magnitude of the FT.\n"
	    << "\n"
	    << "fft understands input with one, two or three columns (extra columns are\n"
	    << "ignored).  One column is interpreted as a real amplitude and a spacing\n"
	    << "of unity is assumed.  With two columns, the first is the coordinate\n"
	    << "and the second is the real amplitude.  If there is a third column it is\n"
	    << "read in as the imaginary portion of the amplitude.\n"
	    << "\n";
  return 0;
 }

 if (argc == optind) {
  std::cerr << "ERROR: At least one input file must be specified.\n";
 return 1;
 }

 // get input file name
 char * inputFile = NULL;
 // code for get multiple inputs
 for (int ii = optind; ii < argc; ii++) {
  inputFile = argv[ii];
  if (p.verbose) {
   std::cout << "The input file name is " << inputFile << ".\n";
  }
  writeFTOfFile(inputFile, p);
 }

 return 0;
}
