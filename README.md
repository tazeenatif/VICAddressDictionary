# VICAddressDictionary
A C implementation of a dictionary abstract data type using a linked list to store and retrieve Victorian address records from a CSV dataset. The program supports key-based lookup on the EZI_ADD field; outputs matching records in a formatted form and reports detailed comparison statistics including bit, node and string comparisons.
# Victorian Address Dictionary (Stage 1)

## Overview

This project implements a **dictionary abstract data type** in C using a **linked list** to store Victorian address records read from a CSV file. the dictionary supports lookup by key (`EZI_ADD`) and outputs all matching records along with detailed comparison statistics.

## Features

- Reads a CSV dataset of Victorian addresses
- Stores each record as a node in a linked list
- Supports lookup by address key (`EZI_ADD`)
- Handles **non-unique keys** (returns all matches)
- Outputs:
  - Matching records to an output file
  - Number of records found to `stdout`
  - Comparison statistics:
    - Bit comparisons
    - Node comparisons
    - String comparisons
- Robust CSV parsing (quoted fields and escaped quotes)
- Modular design suitable for reuse with alternative data structures

---

## File Structure

├── Makefile
├── main.c
├── dictionary.c
├── dictionary.h
├── csv.c
├── csv.h
├── bitstring.c
├── bitstring.h
├── record.h
└── README.md

## File Descriptions

- **main.c**  
  Program entry point. Handles command-line arguments, reads input data, processes search queries, and manages input/output.

- **dictionary.c / dictionary.h**  
  Linked-list dictionary implementation, including insertion, search, and formatted output of records.

- **csv.c / csv.h**  
  CSV parsing utilities supporting quoted fields and escaped quotes.

- **bitstring.c / bitstring.h**  
  Bit-level string comparison utilities used to count bit comparisons during key lookup.

- **record.h**  
  Defines the `Record` structure used to store a single row of CSV data.

- **Makefile**  
  Builds the executable `dict1`.

## Compilation

To compile the program, run:
```sh
make

This will produce an executable called:
dict1

To clean build files:
make clean

**## Usage**

The program takes three command-line arguments:
./dict1 1 input.csv output.txt

Arguments
1. Stage number
Must be 1 for this implementation (lookup stage).

2. Input CSV file
Dataset containing Victorian address records.

3. Output file
File where matching records will be written.

## Input Format
The CSV file must contain a header row
The key field EZI_ADD is assumed to be present
Search queries are read from stdin, one per line

Example:
./dict1 1 data.csv results.txt < queries.txt

**## Output**
- Output File
    Each query is written to the output file
    All matching records are printed in labeled format
    If no matches are found, NOTFOUND is written

    Example record format:
    --> HEADER1: value1 || HEADER2: value2 || ... ||

- Standard Output
    For each query, the program prints:
    <query> --> <number> records found - comparisons: b<bit> n<node> s<string>
