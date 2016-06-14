SRC = pebble.cpp
CRYPTOPPDIR = ../../cryptopp563/
INC = -I. -I$(CRYPTOPPDIR) -I../../util/
LDFLAGS = -L$(CRYPTOPPDIR) -lcryptopp -lpthread

TARGET = pebble
$(TARGET): $(SRC) ../../util/util.h
	g++ -O3 -fopenmp -std=c++11 -o $@ $< $(INC) $(LDFLAGS)

clean:
	rm -f $(TARGET) *~
