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
 p.forwardFT = true;
 p.verbose = false;
 p.outputExt = "FT";
 p.printHelp = false;

 bool verbose = false;

 /* process command line options */
 while ((c = getopt(argc, argv, "2bfho:v")) != -1) {
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

 writeFTOfFile(inputFile, p);

 return 0;
}
