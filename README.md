huffman-compressor

Huffman Compression Tool

This repository contains an implementation of file compression and decompression using the Huffman coding algorithm.

Project Structure

project-root/
├── Makefile
├── README.md
└── src/
    ├── compress.cpp
    ├── uncompress.cpp
    ├── HCTree.hpp
    ├── HCTree.cpp
    ├── Helper.hpp
    └── Helper.cpp

	•	Makefile: Build scripts for both compress and uncompress executables. Located in the project root so you can simply run make.
	•	src/: All C++ source and header files live here.

Prerequisites
	•	A C++17–capable compiler (e.g., g++)
	•	make

Building

From the project root:

make          # builds both `compress` and `uncompress`

To rebuild with profiling support (gprof):

make clean
make CXXFLAGS+=" -pg"

Usage

Compress a file

./compress <input_file> <output_file>

	•	<input_file>: path to the original file (≤10 MB)
	•	<output_file>: path to write the compressed .huff file

Uncompress a file

./uncompress <compressed_file> <recovered_file>

	•	<compressed_file>: path to a .huff file produced by compress
	•	<recovered_file>: path to write the reconstructed original

How It Works
	1.	Frequency Counting: Read every byte of the input and count occurrences.
	2.	Build Huffman Tree (HCTree): Use a min-heap to build a binary tree where more frequent bytes get shorter codes.
	3.	Header Serialization: Write the tree structure as a sequence of bits plus leaf symbols so that uncompress can rebuild the same tree.
	4.	Encoding: Translate each byte into its Huffman bit sequence and stream bits out.
	5.	Decoding (uncompress): Read the header bits to reconstruct the tree, then read the body bit-by-bit to traverse the tree and recover each byte.

Profiling & Debugging

gprof (GNU Profiler)
	1.	Compile with profiling flags:

make clean
make CXXFLAGS+=" -pg"


	2.	Run your program to generate gmon.out:

./compress input.bin output.huff


	3.	Analyze:

gprof compress gmon.out > profile.txt
less profile.txt



gdb (GNU Debugger)
	1.	Compile without optimizations for easier debugging (already -g in CXXFLAGS).
	2.	Start:

gdb ./compress


	3.	Run:

(gdb) run input.bin output.huff


	4.	Inspect backtrace on crash:

(gdb) backtrace


	5.	Set breakpoints, inspect variables, and step through code.

Credit

Helper functions and initial project template were adapted from CSE 100: Introduction to Data Structures, UC San Diego.