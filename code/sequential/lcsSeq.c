# include <stdlib.h>
# include <stdio.h>


//Funcion Auxiliar para calcular maximos
int max(int a, int b)
{
    return (a > b)? a : b;
}

int lcs( char *X, char *Y, int m, int n )
{

 //------------------------------------------------------------------------------------------------
 // Calcular la Matriz 
 //------------------------------------------------------------------------------------------------
  if(DEBUG){
    printf("X: %s, m=%d\n", X, m);
    printf("Y: %s, n=%d\n", Y, n);
  }
  
  int i,j,rango;
  rango=(n+1)*(m+1);
  int *M = (int *) calloc (rango,sizeof(int));

  for(j=1; j < m; j++){
    for(i=1; i < n; i++){
      //printf("Y: %c, X:%c\n", Y[i-1], X[j-1]);
      if(Y[i-1] == X[j-1]){
        M[i+j*n] = M[(i-1)+(j-1)*n]+1;
      }else {
        M[i+j*n] = max(M[(i)+(j-1)*n],M[(i-1)+(j)*n]);
      }
    }
  }
  
  //Traza para comprobar la matriz
  // if(DEBUG){
  //   for(j=0; j < m; j++){
  //     for(i=0; i < n; i++){
  //       printf("%d   ", M[i+j*n]);
  //     }
  //     printf("\n");
  //   }
  // }
  
 //------------------------------------------------------------------------------------------------
 // Construir la subsecuencia más larga 
 //------------------------------------------------------------------------------------------------
  if(DEBUG){

  i=n;
  j=m;
  int index =  M[(m*n)-1]+1;
  char Result[index];

  while(i > 0 && j > 0){
    
    //Traza para comprobar comportamiento
    
    // printf("Valor de index=%d  \n", index);
    // printf("Valor de j=%d  valor de i=%d \n", j, i);
    // printf("%c%c\n", X[j-1], Y[i-1]);
    // printf("%d  %d \n\n",M[(i)+(j-1)*n],M[(i-1)+(j)*n]);
    

    if( X[j-1] == Y[i-1] ){
      Result[index-1]=X[j-1];
      j=j-1;
      i=i-1;
      index=index-1;
    }
    else if(M[(i)+(j-1)*n] > M[(i-1)+(j)*n]) j=j-1;
    else{
      i=i-1;
    }

  }



  printf("Cadena: %s, El número de elementos en común es: %d\n", Result, M[(n-1)+(m-1)*n]);

  }

 //------------------------------------------------------------------------------------------------
 // Printeando la subsecuencia más larga
 //------------------------------------------------------------------------------------------------
 
  //Impresion de la cadena más larga
  if(DEBUG){
    printf("%d\n", M[(n-1)+(m-1)*n]);
  }

  float value = M[(n-1)+(m-1)*n];

  free(M);

  return value;
}

/**
 * Casos de test con solucion correcta
 * 
 * Caso 1 -- Cadena X = "PAWHEAE"  Cadena Y = "HEAGAWGHEE"  Solución = "AWHEE"
 * Caso 2 -- Cadena X = "CADBRZ"   Cadena Y = "ASBZ"        Solución = "ABZ"
 * Caso 3 -- Cadena X = "AGGTAB"   Cadena Y = "GXTXAYB"     Solución = "GTAB"
 * Caso 4 -- Cadena X = "PERRO"    Cadena Y = "PERRO"       Solucion = "PERRO"
 * Caso 5 -- Cadena X = "ABCDEFG"  Cadena Y = "HIJKLM"      Solución = ""
*/