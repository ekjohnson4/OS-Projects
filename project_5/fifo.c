#include <stdio.h>
#include <stdlib.h>

typedef struct pTable {
    int *pages, size, faults;
} PTable;

typedef struct tlb {
    int *pages, *frames, size, hits;
} Tlb;

Tlb *tlb;
PTable *pTable;
signed char **frames;
int head = 0;
int memorySize = 0;

int
getFromTlb(int pNumber) {
    for (int i = 15; i > -1; i--) {
        if (tlb->pages[i] == pNumber) {
            tlb->hits++;
            return tlb->frames[i];
        }
    }
    return -1;
}

void
insertIntoTlb(int pNumber, int fNumber) {
    for (int i = 0; i < 15; i++) {
        tlb->pages[i] = tlb->pages[i + 1];
        tlb->frames[i] = tlb->frames[i + 1];
    }

    tlb->pages[15] = pNumber;
    tlb->frames[15] = fNumber;
}

void
updatepTable(int index) {
    for (int i = 0; i < 256; i++) {
        if (pTable->pages[i] == index) {
            pTable->pages[i] = -1;
        }
    }
}

int
getFrameIndex() {
    int setIndex = head;

    if (frames[setIndex] != 0) {
        free(frames[setIndex]);
    }

    if (memorySize == 128) {
        updatepTable(setIndex);
    }
    head++;
    if (head == memorySize) {
        head = 0;
    }
    return setIndex;
}

int
getFrompTable(int pNumber, int offset) {
    if (pTable->pages[pNumber] != -1) {
        return pTable->pages[pNumber];
    }

    pTable->faults++;

    signed char *data = malloc(sizeof(signed char) * 256);
    FILE *backingStore = fopen("BACKING_STORE.bin", "rb");
    if (fseek(backingStore, pNumber * 256, SEEK_SET) != 0) {
        fprintf(stderr, "Error seeking from backing store.\n");
    }

    if (fread(data, sizeof(signed char), 256, backingStore) == 0) {
        fprintf(stderr, "Error reading from backing.\n");
    }

    int index = getFrameIndex();
    pTable->pages[pNumber] = index;
    frames[index] = data;

    fclose(backingStore);

    return index;
}

void
process(int logicalAddress) {
    int pNumber = (logicalAddress & 0xFFFF) >> 8;
    int offset = (logicalAddress & 0xFF);

    int frameIndex = getFromTlb(pNumber);

    if (frameIndex < 0) {
        frameIndex = getFrompTable(pNumber, offset);
        insertIntoTlb(pNumber, frameIndex);
    }

    printf("Virtual address: %d ", logicalAddress);
    printf("Physical address: %d ", (frameIndex << 8) | offset);
    printf("Value: %d\n", frames[frameIndex][offset]);
}

int
getMemorySize() {
    int size = 256;
    return size;
}

void
initTlb() {
    tlb = malloc(sizeof(Tlb));
    tlb->size = 0;
    tlb->hits = 0;
    tlb->pages = calloc(16, sizeof(int));
    tlb->frames = calloc(16, sizeof(int));
    for (int i = 0; i < 16; i++) {
        tlb->frames[i] = -1;
    }
}

void
initpTable() {
    pTable = malloc(sizeof(pTable));
    pTable->size = 0;
    pTable->faults = 0;
    pTable->pages = malloc(sizeof(int) * 256);
    for (int i = 0; i < 256; i++) {
        pTable->pages[i] = -1;
    }
}

void
printData(int total) {
    printf("Number of Translated Addresses = %d\n", total);
    printf("Page Faults = %d\n", pTable->faults);
    printf("Page Fault Rate = %.3f\n", (double) pTable->faults / total);
    printf("TLB Hits = %d\n", tlb->hits);
    printf("TLB Hit Rate = %.3f\n", (double) tlb->hits / total);
}

void
execute(char *fileName) {
    FILE *addresses = fopen(fileName, "r");

    if (addresses == NULL) {
        fprintf(stderr, "Invalid addresses file.\n");
        exit(1);
    }

    int value;
    int total = 0;
    fscanf(addresses, "%d", &value);
    while (!feof(addresses)) {
        process(value);
        total++;
        fscanf(addresses, "%d", &value);
    }

    fclose(addresses);
    printData(total);
}

void
cleanup() {
    free(pTable->pages);
    free(tlb->pages);
    free(tlb->frames);

    for (int i = 0; i < memorySize; i++) {
        if (frames[i] != 0) {
            free(frames[i]);
        }
    }

    free(frames);
    free(tlb);
    free(pTable);
}

int
main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Please provide addresses file.\n");
        exit(1);
    }

    memorySize = getMemorySize();
    frames = calloc(memorySize, sizeof(signed char*));
    initTlb();
    initpTable();
    execute(argv[1]);
    cleanup();
    return 0;
}
