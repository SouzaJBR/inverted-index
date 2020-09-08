cmake_minimum_required(VERSION 3.17)
project(trabalho_aed C)

set(CMAKE_C_STANDARD 11)

add_executable(trabalho_aed deps/cJSON/cJSON.c deps/cJSON/cJSON.h main.c structure/trie.c structure/trie.h structure/hash.c structure/hash.h structure/document.c structure/document.h io/filereader.c io/filereader.h structure/document.c)