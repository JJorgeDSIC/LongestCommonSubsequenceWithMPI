#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "lcsSeq.h"
#include "../utils/ctimer.h"
#include "../utils/generator.h"
#include "../utils/input.h"

int randomRange(int min, int max){
   return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

int main(int argc, char *argv[]) {
	

	char * inputString;
	char * searchString;
	char** stringArray;
	double t1,t2,tucpu,tscpu;
	int i;
	srand(time(0));

	if ( argc ==  4) {

		//fprintf( stderr, "Using Random input, Use: %s <filename> to provide a file\n", argv[0] );
			
		//n = 1000;

		int n;
  		sscanf(argv[1],"%d",&n);

		//stringArray = (char **)malloc(sizeof(char **) * (n));

		int max;
		int min;
		sscanf(argv[2],"%d",&min);
		sscanf(argv[3],"%d",&max);

		if(DEBUG){
			printf("Collection:\n");
		}
/*
		for ( i = 0; i < n; ++i)
		{
			stringArray[i] = randstring(randomRange(min, max));
			if(DEBUG){
				printf("%s\n", stringArray[i]);
			}
		} 
*/
		inputString = randstring(randomRange(min, max));

		if(DEBUG){
			printf("Objective:\n");
			printf("%s\n", inputString);
		}
		ctimer(&t1,&tucpu,&tscpu);

		for ( i = 0; i < n; ++i)
		{
			searchString = randstring(randomRange(min, max));
			lcs( inputString, searchString, strlen(inputString), strlen(searchString));
			free(searchString);
		}	

		ctimer(&t2,&tucpu,&tscpu);

		printf("(random) %d %d %d %f segundos \n", n, min, max, (float) (t2-t1));

	}else if(argc == 2){

		printf("%s", argv[1]);
		//char *filename = "cadenas_seq.txt";
		int n;
		int i;
		int MAX_SEQ = 1000;
		int MAX_LENGTH = 60;

		stringArray = (char **)malloc(sizeof(char **) * (MAX_SEQ));

		for ( i = 0; i < MAX_SEQ; ++i)
		{
			stringArray[i] = (char *)malloc(sizeof(char) * (MAX_LENGTH));
		}

		readFile(argv[1], stringArray, &n);

		printf("Collection:\n");
		for (i = 1; i < n; ++i)
		{
			printf("%s", stringArray[i]);
		} 

		printf("Objective:\n");
		printf("%s", stringArray[0]);

		inputString = stringArray[0];

		ctimer(&t1,&tucpu,&tscpu);

		for ( i = 1; i < n; ++i)
		{
			lcs( inputString, stringArray[i], strlen(inputString), strlen(stringArray[i]));
		}	

		ctimer(&t2,&tucpu,&tscpu);

		printf("(file) %f segundos \n",(float) (t2-t1));
	}else{
		printf("Error\n");
		printf("Use: %s <filename> to provide a file\n", argv[0]);
		printf("Or Use: %s <num_seqs> <min_length> <max_length> to generate random sequences\n", argv[0]);
	}

	




}