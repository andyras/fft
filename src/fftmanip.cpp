#include "fftmanip.h"

//#define DEBUG_FFTMANIP

/* This function implements something similar to MATLAB's 'fftshift':
 * it takes the FFT'd vector invec and puts a shifted copy in outvec.*/
void fftshift(fftw_complex * invec, fftw_complex * outvec, int n) {
 // number of elements to shift output array
 int shift = n/2;
 // shifted counter
 int ii;

 // do the shift!
 for (int i = 0; i < n; i++) {
  ii = (i+shift) % n;
  outvec[ii][0] = invec[i][0];
  outvec[ii][1] = invec[i][1];
 }

 return;
}

void fftshift_double(double * invec, double * outvec, int n) {
 // number of elements to shift output array
 int shift = n/2;
 // shifted counter
 int ii;

 // do the shift!
 for (int i = 0; i < n; i++) {
  ii = (i+shift) % n;
  outvec[ii] = invec[i];
  outvec[ii] = invec[i];
 }

 return;
}

/* This function writes the FT of the input time-sampled vector to a file.
 * It assumes evenly spaced time data (with at least two points).
 */
void writeFT(const char * fileName, double * invec, double * times, int n, Parameters p) {
 // compute time spacing
 double dt = times[1] - times[0];
 // compute energy spacing
 double dE = 2.0*M_PI/(dt*n);
 // build energies array
 double * energies = new double [n];
 for (int i = 0; i < n; i++) {
  energies[i] = (i-n/2)*dE;
 }

 // FFTW arrays
 fftw_complex * in;
 fftw_complex * out;
 in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*n);
 out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*n);

 // make FFT plan
 fftw_plan fftw_fwd;
 fftw_fwd = fftw_plan_dft_1d(n, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

 // set up in array
 for (int ii = 0; ii < n; ii++) {
  in[ii][0] = invec[ii];
  in[ii][1] = 0;
 }

 // do the FFT
 fftw_execute(fftw_fwd);

 // print the result
 outputCVectorShift(fileName, out, energies, n, p);

 // clean up
 delete [] energies;
 return;
}

/* returns the number of numbers in a file.  This way, it doesn't matter if
 * they are one per line or multiple per line.
 */
int Number_of_values (const char * fileName) {
 FILE * inputFile;
 double value;
 int numberOfValues = 0;

 inputFile = fopen(fileName, "r");

 if (inputFile != NULL) {
  while (fscanf(inputFile, "%lf", &value) != EOF) { numberOfValues++; }
  if (numberOfValues == 0 ) {
   fprintf(stderr, "WARNING: input file %s is empty.\n", fileName);
  }
 }
 else {
  std::cerr << "WARNING [" << __FUNCTION__ << "]: " << fileName << " does not exist.\n";
  return -1;
 }
 
 fclose(inputFile);

 return numberOfValues;
}

/* Counts the number of lines in a file. */
int countLines(const char * fileName) {
 int n = 0;
 std::string line;
 std::ifstream in(fileName);
 while (std::getline(in, line)) {
  n++;
 }
 if (n == 0 ) {
  std::cerr << "WARNING [" << __FUNCTION__ << "]: input file " << fileName << " is empty.\n";
 }
 return n;
}

/* reads in the values from file; returns an array the length of the number of 
 * numbers in the file
 */
void readFTInput(double * times, fftw_complex * in, const char * fileName, int numberOfValues) {
 int n = 0;		// number of columns in input
 int ii = 0;		// counter
 bool firstLine = true;	// flag for first line
 std::string line;	// each line of input

 /* open input stream */
 std::ifstream inputFile(fileName);

 while (std::getline(inputFile, line)) {
  std::stringstream is(line);
  /* count columns from first line */
  if (firstLine) {
   std::string junk;
   while (is >> junk) {
    n++;
   }
   std::cout << "Number of columns is " << n << "\n";
   firstLine = false;
  }
  else {
  }

  /* read in data */
  is << line;
  if (n == 0) {
   std::cerr << "ERROR reading file: no columns of input.\n";
   return;
  }
  // column 1 is amplitudes, assume integer time spacing
  else if (n == 1) {
   is >> in[ii][0];
   in[ii][1] = 0.0;
   times[ii] = ii;
  }
  // column 1 is time data, column 2 is amplitudes
  else if (n == 2) {
   is >> times[ii];
   is >> in[ii][0];
   in[ii][1] = 0.0;
  }
  // column 1 is time, col 2 is Re(amplitude), col 3 is Im(amplitude), ignore rest
  else {
   is >> times[ii];
   is >> in[ii][0];
   is >> in[ii][1];
  }
  ii++;
 }


 /*
 FILE * inputFile;
 int i = 0;

 inputFile = fopen(fileName,"r");

 if (inputFile != NULL) {
  while (fscanf(inputFile, "%lf %lf %*s", &times[i], &in[i][0]) != EOF && i < numberOfValues) {
#ifdef DEBUG_FFTMANIP
   std::cout << "time " << times[i] << " input " << in[i][0] << "\n";
#endif
   i++;
  }
 }
 else {
  std::cerr << "ERROR [" << __FUNCTION__ << "]: file " << fileName << " does not exist.\n";
 }

 fclose(inputFile);
 */
}

/* This function writes the FT of the input file to a file.
 * It assumes evenly spaced time data (with at least two points).
 */
void writeFTOfFile(const char * inputFile, Parameters p) {
 /* create string for output file name */
 std::string outputFile = inputFile;
 /* find position of last period and insert extension*/
 size_t pos = outputFile.rfind(".");
 if (pos == std::string::npos) {
  outputFile.append(p.outputExt.c_str());
 }
 else {
  outputFile.insert(pos, p.outputExt.c_str());
 }
 if (p.verbose) {
  std::cout << "output file name is " << outputFile << ".\n";
 }
 
 int n = countLines(inputFile);

 double * times = new double [n];

 // FFTW arrays
 fftw_complex * in;
 fftw_complex * out;
 in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*n);
 out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*n);

 // make FFT plan
 fftw_plan fftw_fwd;
 fftw_fwd = fftw_plan_dft_1d(n, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

 // set up in and times arrays
 readFTInput(times, in, inputFile, n);

 // compute time spacing
 double dt = times[1] - times[0];
 // compute energy spacing
 // I think it is right to have (n-1) instead of n...
 double dE = 2.0*M_PI/(dt*(n-1));
 // build energies array
 double * energies = new double [n];
 for (int i = 0; i < n; i++) {
  energies[i] = (i-n/2)*dE;
 }

 // do the FFT
 fftw_execute(fftw_fwd);

 // print the result
 if (p.noshift) {
  outputCVector(outputFile.c_str(), out, energies, n, p);
 }
 else {
  outputCVectorShift(outputFile.c_str(), out, energies, n, p);
 }

 // clean up
 delete [] energies;
 return;
}
