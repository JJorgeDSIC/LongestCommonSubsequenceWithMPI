TEST = test_parallel
LCSPAR = lcsPar
LCSPARSINGLE = lcsPar_single
LCSSEQ = ../sequential/lcsSeq
RNDSTRING = ../utils/generator
INPUT = ../utils/input
TIMER = ../utils/ctimer

# Optimization & debug flags:
# DEBUG: mensajes de depuracion.
CFLAGS  =  -O3 -Wall -DDEBUG=0 -fopenmp

CC = mpicc

#all: clean $(RNDSTRING) $(INPUT) $(LCSSEQ) $(LCSPAR) $(TEST) 
all: clean $(RNDSTRING) $(INPUT) $(LCSSEQ) $(LCSPAR) $(LCSPARSINGLE)

#$(TEST): $(RNDSTRING) $(INPUT) $(LCSPAR).o $(LCSSEQ).o $(TEST).o Makefile 
#	$(CC) -o $(TEST) $(TEST).o $(LCSPAR).o $(LCSSEQ).o $(RNDSTRING).o $(INPUT).o $(TIMER).c $(CFLAGS) 

#$(TEST).o: $(TEST).c Makefile 
#	$(CC) -c $(TEST).c $(CFLAGS) 

$(LCSPAR): $(LCSPAR).o Makefile 
	$(CC) -o $(LCSPAR) $(LCSPAR).o $(LCSSEQ).o $(RNDSTRING).o $(INPUT).o $(TIMER).c $(CFLAGS) 

$(LCSPAR).o: $(LCSPAR).c Makefile 
	$(CC) -c $(LCSPAR).c $(CFLAGS) 

$(LCSPARSINGLE): $(LCSPARSINGLE).o Makefile 
	$(CC) -o $(LCSPARSINGLE) $(LCSPARSINGLE).o $(LCSSEQ).o $(RNDSTRING).o $(INPUT).o $(TIMER).c $(CFLAGS) 

$(LCSPARSINGLE).o: $(LCSPARSINGLE).c Makefile 
	$(CC) -c $(LCSPARSINGLE).c $(CFLAGS) 

$(RNDSTRING): 
	$(MAKE) -C ../utils

$(INPUT): 
	$(MAKE) -C ../utils

$(LCSSEQ):
	$(MAKE) -C ../sequential

clean:
	- rm -f *.o core $(TEST) $(LCSPAR) $(LCSPARSINGLE) $(RNDSTRING) $(INPUT) *~



