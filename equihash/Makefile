TARGET = equihash
SRC = *.cpp
HEADER = *.h ../util/util.h
CRYPTOPPDIR = ../cryptopp563/
INC = -I. -I$(CRYPTOPPDIR) -I../util/
LDFLAGS = -L$(CRYPTOPPDIR) -lcryptopp -lpthread


$(TARGET): $(SRC) $(HEADER)
	g++ -std=c++11 -o $@ $< $(INC) $(LDFLAGS)

clean:
	rm -f $(TARGET) *~ 
