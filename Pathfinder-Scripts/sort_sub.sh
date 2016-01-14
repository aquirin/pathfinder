#!/bin/bash

###########################
# Sort of a DOT file: print first the nodes, then the edges, and
# both sections are ordered by the values of the node indices.

if [ $# -lt 1 ]
then
	echo "Used to sort <SubStructure.sub> and print it in output."
	echo "Usage: $0 <SubStructure.sub>"
	exit -1
fi

INPUT="$1"

# header
head -n +3 "$INPUT" > ./header.txt.tmp

# footer
tail -n -1 "$INPUT" > ./footer.txt.tmp

# body
tail -n +4 "$INPUT" | head -n -1  > ./body.txt.tmp

# sort body
cat ./header.txt.tmp
cat ./body.txt.tmp | grep -v " -> " | sort -n
cat ./body.txt.tmp | grep " -> " | sort -n
cat ./footer.txt.tmp

# cleaning
rm -f ./header.txt.tmp ./footer.txt.tmp ./body.txt.tmp
