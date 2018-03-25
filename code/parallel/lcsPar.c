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

int randomRange(int min, int max){
   return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

int getInput(int* argc, char* argv, char * inputString, char** stringArray, int* MAX_SEQUENCES){
  return 1;
}


int main(int argc,char *argv[]) 
{ 
  int myid, numprocs, i, j, k; 


  if ( argc < 5 ) {
    fprintf( stderr, "\nUse: %s <MAX_SEQUENCES> <MIN_LENGTH_SEQUENCE> <MAX_LENGTH_SEQUENCE> <MAX_SEQ_PER_PROC> \n\n", argv[0] );
    return 0;
  }

  //MPI Initialization
  MPI_Init(&argc,&argv); 
  MPI_Comm_size(MPI_COMM_WORLD,&numprocs); 
  MPI_Comm_rank(MPI_COMM_WORLD,&myid); 
  MPI_Status     status;

  //Timing
  double t1,t2,tucpu,tscpu;

  //Parameters
  int tag = 100;
  int source = -1;
  int count = 0;
  int lcs_value = -1;
  int maxLCSValue = -1;

  int MAX_SEQUENCES;
  sscanf(argv[1],"%d",&MAX_SEQUENCES);

  int min;
  sscanf(argv[2],"%d",&min);
  int max;
  sscanf(argv[3],"%d",&max);

  int MAX_SEQ_PER_PROC = 5;
  sscanf(argv[4],"%d",&MAX_SEQ_PER_PROC);

  char * inputString = (char *)malloc(sizeof(char) * (max+1));
  char* sequence = (char *)malloc(sizeof(char) * (max+1));
  char* sequenceWithMaximumLCS = (char *)malloc(sizeof(char) * (max+1));
  char** stringArray = (char **)malloc(sizeof(char **) * (MAX_SEQ_PER_PROC));
    
  int BUFFER_SIZE = numprocs*MAX_SEQ_PER_PROC*(max+1);
  char* buff = (char *)malloc(sizeof(char) * BUFFER_SIZE);

  int total_seq, position, length;

  if(myid == 0){//Master

    inputString = randstring(randomRange(min, max));
    printf("Master, String 1: %s, (maximum seqs: %d) Buffer size: %d\n", inputString, MAX_SEQUENCES, BUFFER_SIZE);

    //getInput(&argc, &argv, &inputString, &stringArray, &MAX_SEQUENCES);

    i = 0;

    total_seq = MAX_SEQ_PER_PROC;
    length = 0;

    for ( j = 0; j < total_seq; ++j)
    {
        stringArray[j] = (char *)malloc(sizeof(char) * (max+1));
    }

    int slaves = numprocs-1;

    ctimer(&t1,&tucpu,&tscpu);
    
    while(slaves){

      //Receive a msg from any slave
      MPI_Recv(sequence, max+1, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD,  &status);
      //Getting information about the message
      tag = status.MPI_TAG;
      source = status.MPI_SOURCE;
      MPI_Get_count(&status, MPI_CHAR, &count);

      if(tag==SLAVE_READY || tag==SOLUTION_FOUND){

        if(tag==SLAVE_READY){

          if(DEBUG)printf("Master received SLAVE_READY from process %d, sending initial string=%s, with tag %d\n", source, inputString, tag);
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
          // else{
          //   if(DEBUG)printf("Master %d discarding solution from %d, seq=%s\n", myid, source, sequence);
          // }
        }

        if(i < MAX_SEQUENCES){
    
          position = 0;
          MPI_Pack(&total_seq, 1, MPI_INT, buff, BUFFER_SIZE, &position, MPI_COMM_WORLD);

          for ( j = 0; j < total_seq; ++j)
          {
            stringArray[j] = randstring(randomRange(min, max));
            length = strlen(stringArray[j]) + 1;
            //printf("%s, length=%d\n", stringArray[j], strlen(stringArray[j]));
            MPI_Pack(&length, 1, MPI_INT, buff, BUFFER_SIZE, &position, MPI_COMM_WORLD);
            MPI_Pack(stringArray[j], length, MPI_CHAR, buff, BUFFER_SIZE, &position, MPI_COMM_WORLD);
            i++;
          }

          //if(DEBUG)printf("****Master %d prepared PACK to %d\n", myid, source);
          MPI_Send(buff, position, MPI_PACKED, source, FIND_LCS, MPI_COMM_WORLD);

        }else{

          if(DEBUG)printf("****Master %d sending END to %d\n", myid, source);
          //Send ending msg to the slave (tag="END")
          MPI_Send(sequenceWithMaximumLCS, strlen(sequenceWithMaximumLCS), MPI_CHAR, source, END, MPI_COMM_WORLD);
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
    int maxValue = -1;
    char* maxSequence = (char *)malloc(sizeof(char) * (max+1));

    int indexes[MAX_SEQ_PER_PROC];

    if(DEBUG)printf("Slave %d sending ready to process 0 with tag %d\n", myid, SLAVE_READY);
    MPI_Send(maxSequence, strlen(maxSequence), MPI_CHAR, 0, SLAVE_READY, MPI_COMM_WORLD);

    if(DEBUG)printf("Slave %d waiting for the initial sequence from process 0...\n", myid);
    inputString = (char *)malloc(sizeof(char) * (max+1));
    MPI_Recv(inputString, max+1, MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD,  &status);
    MPI_Get_count(&status, MPI_CHAR, &inputLength);

    while(cont){

      position=0;

      MPI_Recv(buff, BUFFER_SIZE, MPI_PACKED, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

      tag = status.MPI_TAG;
      source = status.MPI_SOURCE;
      MPI_Get_count(&status, MPI_CHAR, &sequenceLength);

      if(tag==END){

        printf("Slave %d finishing: sequences completed, from process 0 with tag %d\n", myid, END);
        cont = 0;

      }else{

        if(DEBUG)printf("Slave %d...received a sequence: %s, from process 0 with tag %d\n", myid, inputString, tag);
        
        //Process something

        MPI_Unpack(buff, BUFFER_SIZE, &position, &total_seq, 1, MPI_INT, MPI_COMM_WORLD);

        if(DEBUG)printf("Rank=%d,receiving %d sequences\n", myid, total_seq);

        for ( i = 0; i < total_seq; ++i)
        {
          stringArray[i] = (char *)malloc(sizeof(char) * (max+1));
          MPI_Unpack(buff, BUFFER_SIZE, &position, &length, 1, MPI_INT, MPI_COMM_WORLD);
          MPI_Unpack(buff, BUFFER_SIZE, &position, stringArray[i], length, MPI_CHAR, MPI_COMM_WORLD);
          //if(DEBUG)printf("Rank %d, %s, length=%d, strlen(stringArray[i])=%d\n", myid, stringArray[i], length, strlen(stringArray[i]));         
        }

        lcs_value = -1;

        #pragma omp parallel for reduction(max:maxValue)
        for ( i = 0; i < total_seq; ++i)
        {

          lcs_value=lcs(inputString, stringArray[i], strlen(inputString)+1, strlen(stringArray[i])+1);
          indexes[i] = lcs_value;
          maxValue=lcs_value;

          if(DEBUG)printf("Rank %d found a solution: %s, (with lcs=%d), to process 0\n", myid, stringArray[i], lcs_value);
          
          // if(lcs_value > maxValue){
          //   strcpy(maxSequence, stringArray[i]);
          //   maxValue = lcs_value;
          //   //if(DEBUG)printf("Slave %d found a solution: %s, (with lcs=%d), to process 0\n", myid, maxSequence, maxValue);
          // }

        }

        for ( i = 0; i < total_seq; ++i)
        {
          if(indexes[i] == maxValue){
            strcpy(maxSequence, stringArray[i]);
            if(DEBUG)printf("Slave %d found a solution: %s, (with lcs=%d), to process 0\n", myid, maxSequence, lcs_value);
          
          }
        }

        //Send a msg to master with "SOLUTION FOUND"
        //if(DEBUG)printf("Slave %d sending a solution: %s, (with lcs=%d), to process 0\n", myid, maxSequence, maxValue);
        MPI_Send(maxSequence, strlen(maxSequence)+1, MPI_CHAR, 0, SOLUTION_FOUND, MPI_COMM_WORLD);
        MPI_Send(&maxValue, 1, MPI_INT, 0, SOLUTION_FOUND, MPI_COMM_WORLD);
      
      }
    }
  }

  if(!myid){

    printf("Finishing, sequence=%s, sequence with max LCS=%s, with %d characters\n", inputString, sequenceWithMaximumLCS, maxLCSValue);
    ctimer(&t2,&tucpu,&tscpu);
    printf("out: %d %d %d %d %f\n", MAX_SEQUENCES, min, max, MAX_SEQ_PER_PROC,(float) (t2-t1));
  }

  MPI_Finalize(); 
  return 0; 
} 

