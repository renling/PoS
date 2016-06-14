TARGET = equihash
SRC = *.cpp
CRYPTOPPDIR = ../cryptopp563/
INC = -I. -I$(CRYPTOPPDIR) -I../util/
LDFLAGS = -L$(CRYPTOPPDIR) -lcryptopp -lpthread


$(TARGET): $(SRC)
	g++ -o $@ $^ $(INC) $(LDFLAGS)

clean:
	rm -f $(TARGET) *~ 
