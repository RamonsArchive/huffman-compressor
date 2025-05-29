CXX=g++
CXXFLAG =-Wall -Wmaybe-uninitialized -pedantic -Werror -g -O0 -std=c++17
OUTFILES=compress uncompress 

all: $(OUTFILES)

compress: src/compress.cpp src/HCTree.cpp src/Helper.cpp src/HCTree.hpp src/Helper.hpp
	$(CXX) $(CXXFLAGS) -o compress src/compress.cpp src/HCTree.cpp src/Helper.cpp

uncompress: src/uncompress.cpp src/HCTree.cpp src/Helper.cpp src/HCTree.hpp src/Helper.hpp
	$(CXX) $(CXXFLAGS) -o uncompress src/uncompress.cpp src/HCTree.cpp src/Helper.cpp


gprof: src/compress.cpp src/uncompress.cpp src/HCTree.cpp src/Helper.cpp src/HCTree.hpp src/Helper.hpp
	make clean
	$(CXX) $(CXXFLAGS) -pg -o compress src/compress.cpp src/Helper.cpp src/HCTree.cpp
	$(CXX) $(CXXFLAGS) -pg -o uncompress src/uncompress.cpp src/Helper.cpp src/HCTree.cpp

clean:
	rm -f $(OUTFILES)

