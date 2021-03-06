#!/bin/bash
NUMSEQ=$1
MIN=$2
MAX=$3
SEQSPERNODE=$4
THREADS=$5
echo "Creating scripts..."
for (( i = 2; i <= 6; i++)); do
	OUTPUTSCRIPT=task_"$i"_"$NUMSEQ"_"$MIN"_"$MAX"_"$SEQSPERNODE"_"$THREADS".sh

	echo "#!/bin/sh
#PBS -l nodes=$i,walltime=00:60:00
#PBS -q mcpd
#PBS -d .
#PBS -o output_'$i'_'$NUMSEQ'_'$MIN'_'$MAX'_'$SEQSPERNODE'_'$THREADS'.txt
#PBS -e error_'$i'_'$NUMSEQ'_'$MIN'_'$MAX'_'$SEQSPERNODE'_'$THREADS'.txt

OMP_NUM_THREADS=$THREADS mpiexec lcsPar $NUMSEQ $MIN $MAX $SEQSPERNODE" > $OUTPUTSCRIPT
	qsub $OUTPUTSCRIPT
done