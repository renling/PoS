TARGET = pebble
SRC = pebble.cpp
CRYPTOPPDIR = ../../cryptopp563/
INC = -I. -I$(CRYPTOPPDIR) 
LDFLAGS = -L$(CRYPTOPPDIR) -lcryptopp -lpthread

$(TARGET): $(SRC) util.h
	g++ -O3 -fopenmp -std=c++11 -o $@ $< $(INC) $(LDFLAGS)

clean:
	rm -f $(TARGET) *~
