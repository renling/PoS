SRC = *.cpp
INC = -I. -I./cryptopp563/
LDFLAGS = -lcryptopp -lpthread

all: $(SRC)
	g++ -O3 -fopenmp -std=c++11 -o pebble $(SRC) $(INC) $(LDFLAGS)
