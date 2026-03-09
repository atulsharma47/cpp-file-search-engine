# C++ File Search Engine

A command-line file search engine written in modern C++.

This project scans files, builds an inverted index, and allows fast keyword searches across directories.

## Features

* Recursive directory scanning
* Word tokenization
* Inverted index
* Persistent index database
* Command line interface
* Example test files

## Project Structure

```
cpp-file-search-engine
│
├── src
│   └── search_engine.cpp
│
├── examples
│   └── hello.cpp
│
├── README.md
├── LICENSE
└── .gitignore
```

## Build

Compile using g++:

```
g++ src/search_engine.cpp -o search_engine
```

## Usage

Build the index:

```
search_engine index <folder>
```

Example:

```
search_engine index .
```

Search for a word:

```
search_engine search hello
```

Example output:

```
examples/hello.cpp:5
```

## Technologies Used

* C++17
* std::filesystem
* unordered_map
* file I/O

## License

MIT License
