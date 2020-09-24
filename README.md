Universidade Federal de Juiz de Fora

Programa de Pós-Graduação em Ciência da Computação


# Algorithms and Data Structures

João Victor de Souza @souzajbr

This project implements a simple search engine using Trie and Hash as an inverted index. It allows the user to input a document collection and loads it to an index, with a simple search console where the user can type up to two search terms, retrieving the top-20 most relevant documents. 

## Implementation

This project was implemented using the C language, using a linked-list internally to store a document-term list with an inverted index (loaded with a trie or a hash table) to access it. For relevancy, was calculated a TF-IDF for the found documents.

A search term needs to start with a letter. It will be normalized to a lowercase character (a-z) or a digit (0-9), with proper accentuation handling for brazilian portuguese. Also, a simple stop-words list avoids indexing poor search terms.

## How to compile

The simplest way to compile the project is by using CMake to generate the build files.

On Ubuntu 20.04:

```sh
cd inverted-index

sudo apt install cmake build-essential # CMake, make e o GCC
mkdir build # create the build folder
cd build
cmake .. # generate build files
make # compile
```

When the build is done, the executable `./inverted-index` will be available on *build* folder.

Tested with CMake 3.16.3, make 4.2.1 e GCC 9.3

## How to execute

To execute, just execute the `./inverted-index` file created on *build* folder inside a terminal window, with a valid document collection path.

On `data` folder, there are some sample files that can be used. These files are the expected input format. The main dataset, with 200k+ documents, was extracted from [rmisra's Huffpost News dataset](https://www.kaggle.com/rmisra/news-category-dataset/data#). 

Example:

```sh
./inverted_index ../data/news_5k.json
```

After reading the file, you need to choose a structure to create the index, using the console window. 
