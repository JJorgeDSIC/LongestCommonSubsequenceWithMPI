# include <stdlib.h>
# include <stdio.h>
# include <mpi.h>

int main(int argc, char *argv[]){  

    int i,j,my_rank,size, position;
    int t=0;
    int n=10;
    int ns [] = {7,10,6,4,6,7,5,5,7,6};
    char s [] = "PAWHEAEHEAGAWGHEECADBRZASBZAGGTABGXTXAYBPERROPERROABCDEFGHIJKLM";
    char buff [500];
    for(i=0; i < n; i++){
        t=t+ns[i];
    }
    /*
    printf("Las palabras son : \n");
    for(i=0; i < n; i++){
        for(j=0; j < ns[i]; j++){
            printf("%c",s[j+t]);
        }
        t=t+ns[i];
        printf("\n");
    }*/

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(my_rank==0){
        position = 0;
        MPI_Pack(&n, 1, MPI_INT,buff, 500, &position, MPI_COMM_WORLD);
        MPI_Pack(ns, n, MPI_INT,buff, 500, &position, MPI_COMM_WORLD);
        MPI_Pack(s, t, MPI_CHAR,buff, 500, &position, MPI_COMM_WORLD);
        MPI_Bcast(buff, position, MPI_PACKED, 0, MPI_COMM_WORLD);
    }
    else{
        MPI_Bcast(buff, position, MPI_PACKED, 0, MPI_COMM_WORLD);
        position = 0;
        MPI_Unpack(buff, 500, &position, &n, 1, MPI_INT, MPI_COMM_WORLD);
        MPI_Unpack(buff, 500, &position, ns, n, MPI_INT, MPI_COMM_WORLD);
        MPI_Unpack(buff, 500, &position, s, t, MPI_CHAR, MPI_COMM_WORLD);
    }
    if(my_rank==1){
       printf("Las palabras son : \n");
        for(i=0; i < n; i++){
            for(j=0; j < ns[i]; j++){
             printf("%c",s[j+t]);
            }
            t=t+ns[i];
            printf("\n");
        }
    }

    MPI_Finalize();

    /**
 * Casos probados con solucion correcta
 * 
 * Caso 1 -- Cadena X = "PAWHEAE"  Cadena Y = "HEAGAWGHEE"  Solución = "AWHEE"
 * Caso 2 -- Cadena X = "CADBRZ"   Cadena Y = "ASBZ"        Solución = "ABZ"
 * Caso 3 -- Cadena X = "AGGTAB"   Cadena Y = "GXTXAYB"     Solución = "GTAB"
 * Caso 4 -- Cadena X = "PERRO"    Cadena Y = "PERRO"       Solucion = "PERRO"
 * Caso 5 -- Cadena X = "ABCDEFG"  Cadena Y = "HIJKLM"      Solución = ""
*/

}