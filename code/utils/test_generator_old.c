#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "generator.h"

int randomRange(int min, int max){
   return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

int main(int argc, char *argv[]) {


	if ( argc < 2 ) {
		fprintf( stderr, "\nUse: %s <length> \n\n", argv[0] );
		return 0;
	}

	int max = 20;
	int min = 10;

	//Testing stuffs...

	for (int i = 0; i < 10; ++i)
	{
		int key = randomRange(min, max);
		printf("%d\n", key);
	}

	size_t length;
	sscanf(argv[1],"%d",&length);

	srand(time(0));

	char *randomString;

	for (int i = 0; i < 10; ++i)
	{
		randomString = randstring(length);
		printf("%s\n", randomString);
	} 


}