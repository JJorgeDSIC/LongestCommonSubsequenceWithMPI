GEN = generator
INP = input
TEST = test_generator
# Optimization & debug flags:
# DEBUG: mensajes de depuracion.
#CFLAGS  =  -O3 -Wall -DDEBUG=0 -fopenmp
CFLAGS  =  -O3 -DDEBUG=0

CC = gcc

all: clean $(GEN) $(INP)

$(GEN): $(GEN).o $(INP) Makefile 
	
$(GEN).o: $(GEN).c Makefile 
	$(CC) -c $(GEN).c $(CFLAGS) 

$(INP): $(INP).o Makefile 
	
$(INP).o: $(INP).c Makefile 
	$(CC) -c $(INP).c $(CFLAGS) 

# $(TEST): $(GEN) $(TEST).o Makefile
# 	$(CC) -c $(TEST).c $(CFLAGS)

# $(TEST): $(TEST).o Makefile
# 	$(CC) -o $(TEST) $(TEST).o $(GEN).o
	
clean:
	- rm -f *.o core $(GEN) $(INP) *~



