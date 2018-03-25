#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int readFile(char* filename, char** stringArray, int *nRead){

  FILE *file;
  int n;
  char str[60];

  file = fopen(filename, "r");
  if(file == NULL) {
      perror("Error opening file");
      return -1;
   }

  if(fgets(str,60,file)!=NULL){
    n = atoi(str);
  }

  *nRead = n;

  //stringArray = (char **)malloc(sizeof(char **) * (n));

  int i;
  for (i=0; i<n; i++){
    if(fgets(str,60,file)!=NULL){
      strcpy(stringArray[i],str);
      if(DEBUG){
        printf("%s", stringArray[i]);
      }
    }
  }

  fclose(file);

  return 1;
}