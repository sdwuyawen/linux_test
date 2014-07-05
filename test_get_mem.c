/*
 * @filename :
 * 		test_get_mem.c
 * 
 */
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

uint32_t get_page_size(void) {
	return (uint32_t) sysconf(_SC_PAGESIZE);
}

uint64_t get_free_memory(void) {
  return (uint64_t) sysconf(_SC_PAGESIZE) * sysconf(_SC_AVPHYS_PAGES);
}


uint64_t get_total_memory(void) {
  return (uint64_t) sysconf(_SC_PAGESIZE) * sysconf(_SC_PHYS_PAGES);
}


int main(int argc, char *argv[]) {
	printf("page size = %u\n", get_page_size());
	printf("free memory = %lluK\n", get_free_memory()/1024);
	printf("total memory = %lluK\n", get_total_memory()/1024);
	
	return 0;
}
 
