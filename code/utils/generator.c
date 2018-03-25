#include <string.h>
#include <time.h>

//Code from: https://codereview.stackexchange.com/questions/29198/random-string-generator-in-c
char *randstring(size_t length) {

    static char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";        
    char *randomString = NULL;
    int n;
    if (length) {
        randomString = malloc(sizeof(char) * (length +1));

        if (randomString) {            
            for ( n = 0;n < length;n++) {            
                int key = rand() % (int)(sizeof(charset) -1);
                randomString[n] = charset[key];
            }

            randomString[length] = '\0';
        }
    }

    return randomString;
}