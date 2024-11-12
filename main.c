#include "work.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#define PAGE_SIZE (2 * 1024 * 1024) 
#define MAX_PAGES 8
#define OP_FILE "largepages.txt"

void *allocate_hugepage_memory(unsigned long addr) {
    void *ptr = mmap((void *) addr,PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED| MAP_HUGETLB, -1, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        return NULL;
    }
    return ptr;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: main <last 5 digits of your reg. no>\n");
    return EXIT_FAILURE;
  }

  work_init(atoi(argv[1]));
  unsigned long addresses[MAX_PAGES];
  int count = 0;
  FILE *file = fopen(OP_FILE, "r");
  if (file) { 
     while (count < MAX_PAGES && fscanf(file, "%lu", &addresses[count]) == 1) {
        count++;
    }
    fclose(file);
    for (int i = 0; i < MAX_PAGES; i++) {
    printf("Allocating huge page for address: %lu\n", addresses[i]);
    
    void *allocated_memory = allocate_hugepage_memory(addresses[i]);
    if (allocated_memory == NULL) {
        // fprintf(stderr, "Hugepage allocation failed for address: %lu\n", addresses[i]);
        continue;
    }
    // printf("Memory allocated at: %p for address: %lx\n", allocated_memory, addresses[i]);
    }

    if (work_run() == 0) {
      printf("Work completed successfully\n");
    }
  }

  else{
    perror("Could not open largepages.txt");
    return EXIT_FAILURE;
  }

  return 0;
}
