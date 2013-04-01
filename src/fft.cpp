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

 writeFTOfFile(inputFile);

 return 0;
}
