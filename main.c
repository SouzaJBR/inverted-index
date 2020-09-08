#include <stdio.h>
#include <string.h>

#include "io/filereader.h"
#include "deps/cJSON/cJSON.h"

int main() {
    printf("Hello, World!\n");

    struct file_reader* fr = file_reader_create("/home/souzajbr/codez/trabalho-aed/data/News_Category_Dataset_v2.json");

    struct document* doc = file_reader_get_next_document(fr);

    printf("%d -> %s (%s)\n%s\n\n", doc->id, doc->headline, doc->link, doc->shortDescription);

    doc = file_reader_get_next_document(fr);
    printf("%d -> %s\n", doc->id, doc->headline);

    return 0;
}
