CPP=g++
FLAG=-O3
BIN=fomanl
OBJ=util.o main.o

$(BIN): $(OBJ)
	$(CPP) $(OBJ) -o $@ $(FLAG)

util.o: util.cpp util.h
	$(CPP) $< -c -o $@ $(FLAG)

main.o: fomanl.cpp util.h
	$(CPP) $< -c -o $@ $(FLAG)
