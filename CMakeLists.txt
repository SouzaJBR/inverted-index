cmake_minimum_required(VERSION 3.15)
project(inverted_index C)

set(CMAKE_C_STANDARD 11)

add_executable(inverted_index deps/cJSON/cJSON.c deps/cJSON/cJSON.h main.c structure/trie.c structure/trie.h structure/hash.c structure/hash.h structure/document.c structure/document.h io/filereader.c io/filereader.h structure/document.c io/parser.c io/parser.h rank.c rank.h structure/vector.c structure/vector.h)
target_link_libraries(inverted_index PUBLIC m ${EXTRA_LIBS})