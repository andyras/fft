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
void writeFT(const char * fileName, double * invec, double * times, int n) {
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
 outputCVectorShift(fileName, out, energies, n);

 // clean up
 delete [] energies;
 return;
}

/* returns the number of numbers in a file.  This way, it doesn't matter if
 * they are one per line or multiple per line.
 */
int Number_of_values (const char * nameOfFile) {
 FILE * inputFile;
 double value;
 int numberOfValues = 0;

 inputFile = fopen(nameOfFile, "r");

 if (inputFile != NULL) {
  while (fscanf(inputFile, "%lf", &value) != EOF) { numberOfValues++; }
  if (numberOfValues == 0 ) {
   fprintf(stderr, "WARNING: input file %s is empty.\n", nameOfFile);
  }
 }
 else {
  std::cerr << "WARNING [" << __FUNCTION__ << "]: " << nameOfFile << " does not exist.\n";
  return -1;
 }
 
 fclose(inputFile);

 return numberOfValues;
}

/* reads in the values from file; returns an array the length of the number of 
 * numbers in the file
 */
void readFTInput(double * times, fftw_complex * in, const char * nameOfFile, int numberOfValues) {
 FILE * inputFile;
 int i = 0;

 inputFile = fopen(nameOfFile,"r");

 if (inputFile != NULL) {
  while (fscanf(inputFile, "%lf %lf %*s", &times[i], &in[i][0]) != EOF && i < numberOfValues) {
#ifdef DEBUG_FFTMANIP
   std::cout << "time " << times[i] << " input " << in[i][0] << "\n";
#endif
   i++;
  }
 }
 else {
  std::cerr << "ERROR [" << __FUNCTION__ << "]: file " << nameOfFile << " does not exist.\n";
 }

 fclose(inputFile);
}

/* This function writes the FT of the input file to a file.
 * It assumes evenly spaced time data (with at least two points).
 */
void writeFTOfFile(const char * inputFile, Parameters p) {
 /* create string for output file name */
 std::string outputFile = inputFile;
 /* find position of last period and insert "FT"*/
 size_t pos = outputFile.rfind(".");
 if (pos == std::string::npos) {
  outputFile.append("FT");
 }
 else {
  outputFile.insert(pos, "FT");
 }
 std::cout << "output file name is " << outputFile << ".\n";
 
 int n = Number_of_values(inputFile);

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
 double dE = 2.0*M_PI/(dt*n);
 // build energies array
 double * energies = new double [n];
 for (int i = 0; i < n; i++) {
  energies[i] = (i-n/2)*dE;
 }

 // do the FFT
 fftw_execute(fftw_fwd);

 // print the result
 outputCVectorShift(outputFile.c_str(), out, energies, n);

 // clean up
 delete [] energies;
 return;
}
