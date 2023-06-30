CXX = gcc
OUTDIR = build
CXXFLAGS = -Wall -O3 -o
INCLUDE = -Isrc/include
LDFLAGS = -lstdc++ -lwebsockets
ifeq ($(OS), Windows_NT)
	TARGET = main.exe
	cp -r lib/* $(OUTDIR)
else
	TARGET = main
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
	cp -r ui $(OUTDIR)
	cp -r data $(OUTDIR)
	mkdir $(OUTDIR)/logs
	$(CXX) $(CXXFLAGS) ./$(OUTDIR)/$(TARGET) src/main.cpp src/config.cpp src/uigen.cpp src/exec.cpp src/misc.cpp src/wrapper/String.cpp $(INCLUDE) $(LDFLAGS)

clean:
	rm -rf $(OUTDIR)

run:
	cd $(OUTDIR) && ./$(TARGET)