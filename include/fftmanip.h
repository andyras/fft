#ifndef __FFTMANIP_H__
#define __FFTMANIP_H__

#include <cmath>
#include <iostream>
#include <string>
#include <fftw3.h>
#include <fstream>

#include "output.h"
#include "fftmanip.h"
#include "typedefs.h"

/* This function implements something similar to MATLAB's 'fftshift':
 * it takes the FFT'd vector invec and puts a shifted copy in outvec.*/
void fftshift(fftw_complex * invec, fftw_complex * outvec, int n);

/* Same as fftshift, but for doubles! */
void fftshift_double(double * invec, double * outvec, int n);

/* This function writes the FT of the input time-sampled vector to a file.
 * It assumes evenly spaced time data (with at least two points).
 */
void writeFT(const char * fileName, double * invec, double * times, int n, Parameters p);

/* This function writes the FT of the input file to a file.
 * It assumes evenly spaced time data (with at least two points).
 */
void writeFTOfFile(const char * fileName, Parameters p);

/* reads in the values from file; returns an array the length of the number of 
 * numbers in the file
 */
void readFTInput(double * times, fftw_complex * in, const char * nameOfFile, int numberOfValues);

/* returns the number of numbers in a file.  This way, it doesn't matter if
 * they are one per line or multiple per line.
 */
int Number_of_values (const char * nameOfFile);

/* Counts the number of lines in a file. */
int countLines(const char * fileName);

#endif
