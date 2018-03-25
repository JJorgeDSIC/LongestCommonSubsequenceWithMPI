TEST = test_sequential
LCSSEQ = lcsSeq
RNDSTRING = ../utils/generator
INPUT = ../utils/input
TIMER = ../utils/ctimer

# Optimization & debug flags:
# DEBUG: mensajes de depuracion.
#CFLAGS  =  -O3 -Wall -DDEBUG=0 -fopenmp
CFLAGS  =  -O3 -Wall -DDEBUG=0

CC = gcc

all: clean $(RNDSTRING) $(INPUT) $(TEST) 

$(TEST): $(RNDSTRING) $(INPUT) $(LCSSEQ).o $(TEST).o Makefile 
	$(CC) -o $(TEST) $(TEST).o $(LCSSEQ).o $(RNDSTRING).o $(INPUT).o $(TIMER).c $(CFLAGS) 

$(TEST).o: $(TEST).c Makefile 
	$(CC) -c $(TEST).c $(CFLAGS) 

$(LCSSEQ): $(LCSSEQ).o Makefile 

$(LCSSEQ).o: $(LCSSEQ).c Makefile 
	$(CC) -c $(LCSSEQ).c $(CFLAGS) 

$(RNDSTRING): 
	$(MAKE) -C ../utils

$(INPUT): 
	$(MAKE) -C ../utils

clean:
	- rm -f *.o core $(TEST) $(LCSSEQ) $(RNDSTRING) $(INPUT) *~



