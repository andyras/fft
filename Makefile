# Makefile
# some syntax:
# $@ is the object being built
# $^ is its dependencies

CPP = icpc
CPPFLAGS = -O3
LIBS = -lfftw3

# source directory
SRCDIR = src
# object directory
OBJDIR = obj

### name of the output executable ###
BIN = fft
### objects needed to compile $(BIN)
OBJ = $(OBJDIR)/fft.o $(OBJDIR)/output.o $(OBJDIR)/fftmanip.o
### directories with headers
INCDIR = -I./include

# main executable: this is made by default so should go on top
$(BIN): $(OBJ)
	$(CPP) $(CPPFLAGS) $(INCDIR) $(LIBS) -o $@ $^

$(OBJDIR)/fft.o: $(SRCDIR)/fft.cpp
	$(CPP) $(CPPFLAGS) $(INCDIR) -o $@ -c $^

$(OBJDIR)/output.o: $(SRCDIR)/output.cpp
	$(CPP) $(CPPFLAGS) $(INCDIR) -o $@ -c $^

$(OBJDIR)/fftmanip.o: $(SRCDIR)/fftmanip.cpp
	$(CPP) $(CPPFLAGS) $(INCDIR) -o $@ -c $^

.PHONY: clean

clean:
	rm -f $(OBJDIR)/*.o
	rm -f $(BIN)
