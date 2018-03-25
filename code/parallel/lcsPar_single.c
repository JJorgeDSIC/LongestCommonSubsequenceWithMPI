#include <mpi.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <math.h> 
#include <limits.h>
#include <omp.h>
#include <string.h>
#include <time.h>
#include "../sequential/lcsSeq.h"
#include "../utils/ctimer.h"
#include "../utils/generator.h"
#include "../utils/input.h"

#define SLAVE_READY 1
#define SOLUTION_FOUND 2
#define FIND_LCS 3
#define END 4
#define MAX_LENGTH_SEQUENCE 20

int randomRange(int min, int max){
   return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

int getInput(int* argc, char* argv, char * inputString, char** stringArray, int* MAX_SEQUENCES){
  return 1;
}


int main(int argc,char *argv[]) 
{ 
  int myid, numprocs, i, k; 

  if ( argc < 4 ) {
    fprintf( stderr, "\nUse: %s <MAX_SEQUENCES> <MIN_LENGTH_SEQUENCE> <MAX_LENGTH_SEQUENCE>\n\n", argv[0] );
    return 0;
  }


  int MAX_SEQUENCES;
  sscanf(argv[1],"%d",&MAX_SEQUENCES);

  int min;
  sscanf(argv[2],"%d",&min);
  int max;
  sscanf(argv[3],"%d",&max);

  MPI_Init(&argc,&argv); 
  MPI_Comm_size(MPI_COMM_WORLD,&numprocs); 
  MPI_Comm_rank(MPI_COMM_WORLD,&myid); 
  MPI_Status     status;

  double t1,t2,tucpu,tscpu;

  int tag = 100;
  int source = -1;
  int count = 0;
  int lcs_value;
  int maxLCSValue = -1;

  char* void_sequence=randstring(randomRange(5, 5));
  char * inputString;
  char* sequence;
  char* sequenceWithMaximumLCS = (char *)malloc(sizeof(char) * (MAX_LENGTH_SEQUENCE+1));

  if(myid == 0){//Master

    inputString = randstring(randomRange(min, max));
    char * anotherString = randstring(randomRange(min, max));
    printf("%s - %s\n", inputString, anotherString);

   
    //getInput(&argc, &argv, &inputString, &stringArray, &MAX_SEQUENCES);

    i = 0;
    int slaves = numprocs-1;

    printf("Master %d starting...MAX_SEQUENCES=%d\n", myid, MAX_SEQUENCES);
    ctimer(&t1,&tucpu,&tscpu);

    while(slaves){

      sequence = (char *)malloc(sizeof(char) * (MAX_LENGTH_SEQUENCE+1));
      //Receive a msg from any slave
      MPI_Recv(sequence, MAX_LENGTH_SEQUENCE, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD,  &status);

      tag = status.MPI_TAG;
      source = status.MPI_SOURCE;
      MPI_Get_count(&status, MPI_CHAR, &count);

      if(tag==SLAVE_READY || tag==SOLUTION_FOUND){

        if(tag==SLAVE_READY){
          if(DEBUG)printf("Master received SLAVE_READY from process %d (content=%s, count=%d), sending initial string=%s, with tag %d\n", source, sequence, count, inputString, tag);
          MPI_Send(inputString, strlen(inputString), MPI_CHAR, source, FIND_LCS, MPI_COMM_WORLD);
        }else{

          MPI_Recv(&lcs_value, 1, MPI_INT, source, SOLUTION_FOUND, MPI_COMM_WORLD,  &status);

          if(maxLCSValue < lcs_value){

            if(DEBUG)printf("Master %d getting solution from %d, seq=%s with greater lcs=%d\n", myid, source, sequence, lcs_value);
            maxLCSValue=lcs_value;

            for (k = 0; k < count; ++k)
            {
              sequenceWithMaximumLCS[k] = sequence[k];
            }
          }

        }

        if(i < MAX_SEQUENCES){
          //Generate random sequence
          char * anotherString = randstring(randomRange(min, max));
          if(DEBUG)printf("Master %d sending %s to %d (%d/%d)\n", myid, anotherString, source,i,MAX_SEQUENCES);
          //Send to a slave with tag "FIND_LCS" to the same slave that sent the msg
          MPI_Send(anotherString, strlen(anotherString), MPI_CHAR, source, FIND_LCS, MPI_COMM_WORLD);
          i++;

        }else{
          //Send ending msg to the slave (tag="END")
          MPI_Send(void_sequence, 5, MPI_CHAR, source, END, MPI_COMM_WORLD);
          slaves--;
        } 
      }
    }

  }else{

    //Slave
    //Send a message that I'm ready (TAG=READY)
    printf("Slave %d starting...\n", myid);
    int cont = 1;
    int inputLength = 0;
    int sequenceLength = 0;

    printf("Slave %d sending ready to process 0 with tag %d\n", myid, SLAVE_READY);
    MPI_Send(void_sequence, 5, MPI_CHAR, 0, SLAVE_READY, MPI_COMM_WORLD);

    printf("Slave %d waiting for the initial sequence from process 0...\n", myid);
    inputString = (char *)malloc(sizeof(char) * (MAX_LENGTH_SEQUENCE+1));
    MPI_Recv(inputString, MAX_LENGTH_SEQUENCE, MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD,  &status);
    MPI_Get_count(&status, MPI_CHAR, &inputLength);

    while(cont){

      char* compSequence = (char *)malloc(sizeof(char) * (MAX_LENGTH_SEQUENCE+1));
      //Receive a msg from master with number n
      MPI_Recv(compSequence, MAX_LENGTH_SEQUENCE, MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD,  &status);
      tag = status.MPI_TAG;
      source = status.MPI_SOURCE;
      MPI_Get_count(&status, MPI_CHAR, &sequenceLength);
      tag = status.MPI_TAG;

      if(tag==END){

        printf("Slave %d finishing: sequences completed, from process 0 with tag %d\n", myid, END);
        cont = 0;

      }else{

        if(DEBUG)printf("Slave %d...received a sequence: %s - %s, from process 0 with tag %d\n", myid, inputString, compSequence, tag);
        //Process something
        lcs_value=lcs(inputString, compSequence, inputLength, sequenceLength);

        //Send a msg to master with "SOLUTION FOUND"
        if(DEBUG)printf("Slave %d sending a solution: %s, (with lcs=%d), to process 0\n", myid, compSequence, lcs_value);
        MPI_Send(compSequence, sequenceLength, MPI_CHAR, 0, SOLUTION_FOUND, MPI_COMM_WORLD);
        MPI_Send(&lcs_value, 1, MPI_INT, 0, SOLUTION_FOUND, MPI_COMM_WORLD);
      
      }
    }
  }

  if(!myid){

    printf("Finishing, sequence=%s, sequence with max LCS=%s, with %d characters\n", inputString, sequenceWithMaximumLCS, maxLCSValue);
    ctimer(&t2,&tucpu,&tscpu);
    printf("out: %d %d %d - %f\n", MAX_SEQUENCES, min, max,(float) (t2-t1));
  }

  MPI_Finalize(); 
  return 0; 
} 
