#include <iostream>

#ifndef USAGE_H
#define USAGE_H

// Print correct usage to command line
static void usage() {
	printf("Usage: ./tsp inputfile\n");
	printf("where inputfile is in the format:\n");
	printf("   0 x-coord y-coord\n");
	printf("   1 x-coord y-coord\n");
	printf("   .\n   .\n   .\n");
	printf("   n-1 x-coord y-coord\n");
	printf("and n is the number of cities\n");
	printf("\n");
}

#endif
