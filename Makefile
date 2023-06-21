CXX = gcc
OUTDIR = build
CXXFLAGS = -Wall -o
INCLUDE = -Isrc/include 
LDFLAGS = -lstdc++ -lwebsockets
ifeq ($(OS), Windows_NT)
	TARGET = main.exe
else
	TARGET = main
endif
ifeq ($(OS), Windows_NT)
	RM = rmdir /S /Q
else
	RM = rm -rf
endif

.PHONY: build clean run test

all:
	$(MAKE) -si clean
	$(MAKE) -s build
	$(MAKE) -s run

test:
	$(MAKE) -si clean
	$(MAKE) -s build OUTDIR=test
	$(MAKE) -s run

build:
	mkdir $(OUTDIR)
	$(CXX) $(CXXFLAGS) ./$(OUTDIR)/$(TARGET) ./src/main.cpp $(INCLUDE) $(LDFLAGS)

clean:
	$(RM) $(OUTDIR)

run:
	./$(OUTDIR)/$(TARGET)