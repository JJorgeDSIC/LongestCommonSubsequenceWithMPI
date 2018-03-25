#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include "generator.h"

int randomRange(int min, int max){
   return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

int main(int argc, char *argv[]) {


	int my_rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);


	if ( argc < 2 ) {
		fprintf( stderr, "\nUse: %s <length> \n\n", argv[0] );
		return 0;
	}
	int i;
	int max = 20;
	int min = 10;

	int MAX_LENGTH = 60;
	int MAX_SEQ = 50;

	char * inputString = (char *)malloc(sizeof(char) * (MAX_LENGTH));
	char** stringArray;

	int total_seq;
	int length;
	int position;
	char buff[MAX_SEQ*MAX_LENGTH];
	stringArray = (char **)malloc(sizeof(char **) * (MAX_SEQ));
	int dest = 1;

	if(my_rank==0){

		total_seq = MAX_SEQ;
        position = 0;
        length = 0;

        MPI_Pack(&total_seq, 1, MPI_INT, buff, MAX_SEQ*MAX_LENGTH, &position, MPI_COMM_WORLD);
        
		for ( i = 0; i < MAX_SEQ; ++i)
		{
			stringArray[i] = (char *)malloc(sizeof(char) * (MAX_LENGTH));
			stringArray[i] = randstring(randomRange(min, max));
			length = strlen(stringArray[i]) + 1;
			printf("%s, length=%d\n", stringArray[i], strlen(stringArray[i]));
			MPI_Pack(&length, 1, MPI_INT, buff, MAX_SEQ*MAX_LENGTH, &position, MPI_COMM_WORLD);
        	MPI_Pack(stringArray[i], length, MPI_CHAR, buff, MAX_SEQ*MAX_LENGTH, &position, MPI_COMM_WORLD);
        
		}

		//MPI_Bcast(buff, position, MPI_PACKED, 0, MPI_COMM_WORLD);
		MPI_Send(buff, position, MPI_PACKED, dest, 0, MPI_COMM_WORLD);
    
    }
    else{
    	
			
    	position = 0;
        //MPI_Bcast(buff, MAX_SEQ*MAX_LENGTH, MPI_PACKED, 0, MPI_COMM_WORLD);
        MPI_Recv(buff, MAX_SEQ*MAX_LENGTH, MPI_PACKED, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        MPI_Unpack(buff, MAX_SEQ*MAX_LENGTH, &position, &total_seq, 1, MPI_INT, MPI_COMM_WORLD);

        printf("Rank=%d,receiving %d sequences\n", my_rank, total_seq);

        for ( i = 0; i < total_seq; ++i)
		{
			stringArray[i] = (char *)malloc(sizeof(char) * (MAX_LENGTH));
			MPI_Unpack(buff, MAX_SEQ*MAX_LENGTH, &position, &length, 1, MPI_INT, MPI_COMM_WORLD);
			MPI_Unpack(buff, MAX_SEQ*MAX_LENGTH, &position, stringArray[i], length, MPI_CHAR, MPI_COMM_WORLD);
			printf("Rank %d, %s, length=%d\n", my_rank, stringArray[i], length);
		}
    }


    MPI_Finalize();




}