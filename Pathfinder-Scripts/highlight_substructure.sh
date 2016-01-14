#!/bin/bash

# Take in input two graphs (the first one is the smaller one and should be included in the
# second one) and highlight the corresponding substructure in the second one by producing
# an appropriate PDF.

SCRIPTS="/projets/polemic/pathfinder/scripts"
SUBDUE_PATH="/projets/polemic/prog/subdue-5.2.2/bin"
TMP_FILE=/tmp/highlight.tmp
FONTSIZE=2
REPLACES=${SCRIPTS}/replace_text.pl
export GDFONTPATH=/usr/share/fonts/truetype/
DOT_HOME="/projets/polemic/prog/graphviz-2.30.1/local/"
export PATH=$PATH:"$DOT_HOME/bin"
DOTPROG="$DOT_HOME/bin/dot"

if [ $# -lt 2 ]
then
	echo "Used to highlight <SubStructure.sub> into <BigGraph.sub> by producing a PDF file."
	echo "Usage: $0 <SubStructure.sub> <BigGraph.sub>"
else
	SMALL=$1
	BIG=$2
	
	# Find the instances of SMALL in BIG
	$SUBDUE_PATH/sgiso -dot $BIG.dot $SMALL $BIG > $TMP_FILE
	if [ ! -s /tmp/highlight.tmp ];
	then
		echo "$SMALL has not been found into $BIG !"
		exit -1
	fi
	NUM_INSTANCES=`cat $TMP_FILE | grep "instance" | grep "Found" | cut -f2 -d' '`
	
	# Get some statistics about the graphs (size, MDL)
	#$SUBDUE_PATH/mdl $SMALL $BIG > $TMP_FILE
	SIZE_BIG=`cat $TMP_FILE | grep "Size of graph" | cut -f2 -d'=' | cut -f2 -d' '`
	SIZE_SMALL=`cat $TMP_FILE | grep "Size of substructure" | cut -f2 -d'=' | cut -f2 -d' '`
	SIZE_COMPRESSED=`cat $TMP_FILE | grep "Size of compressed graph" | cut -f2 -d'=' | cut -f2 -d' '`
	SIZE_VALUE=`cat $TMP_FILE | grep "Value" | head -n 1 | cut -f2 -d'=' | cut -f2 -d' '`
	MDL_BIG=`cat $TMP_FILE | grep "DL of graph" | cut -f2 -d'=' | cut -f2 -d' '`
	MDL_SMALL=`cat $TMP_FILE | grep "DL of substructure" | cut -f2 -d'=' | cut -f2 -d' '`
	MDL_COMPRESSED=`cat $TMP_FILE | grep "DL of compressed graph" | cut -f2 -d'=' | cut -f2 -d' '`
	MDL_VALUE=`cat $TMP_FILE | grep "Value" | tail -n 1 | cut -f2 -d'=' | cut -f2 -d' '`
	
	# Build the legend of the final PDF
	printf "digraph SubdueGraph {\n" > /tmp/search.tmp
	printf "digraph SubdueGraph {\n" > /tmp/replace.tmp
	
	printf "ratio=auto;\n" >> /tmp/replace.tmp
	printf "size=\"10,10\";\n" >> /tmp/replace.tmp
	printf "overlap=\"scale\"; \n" >> /tmp/replace.tmp
	printf "sep=-0.3;\n" >> /tmp/replace.tmp
	printf "center=true;\n\n" >> /tmp/replace.tmp
	printf "edge [size=1];\n" >> /tmp/replace.tmp
	printf "node [shape=circle, margin=0, fixedsize=true, width=1.0, height=1.0,\n" >> /tmp/replace.tmp
	#printf "\tfontsize=8, fillcolor=white, style=filled, color=white];\n" >> /tmp/replace.tmp
	#printf "node [fontsize=$FONTSIZE];\n" >> /tmp/replace.tmp
	printf "\tfontsize=8, fillcolor=yellowgreen];\n" >> /tmp/replace.tmp
	
	# Legend
	#printf "label=\"HIGHLIGHT Substructure, SCRIPT=$0, BigGraph='$BIG', SubStructure='$SMALL'" >> /tmp/replace.tmp
	#printf "\\\n$NUM_INSTANCES instances, EVAL(size)=$SIZE_VALUE, EVAL(MDL)=$MDL_VALUE" >> /tmp/replace.tmp
	#printf "\\\nStats. for BigGraph: SIZE=$SIZE_BIG, MDL=$MDL_BIG" >> /tmp/replace.tmp
	#printf "\\\nStats. for SubStructure: SIZE=$SIZE_SMALL, MDL=$MDL_SMALL" >> /tmp/replace.tmp
	#printf "\\\nStats. for the graph compressed: SIZE=$SIZE_COMPRESSED, MDL=$MDL_COMPRESSED\";\n" >> /tmp/replace.tmp
	#printf "labelloc=\"top\";\n" >> /tmp/replace.tmp
	printf "fontsize=100;\n\n" >> /tmp/replace.tmp
	
	# Add the legend to the DOT file
	$REPLACES $BIG.dot /tmp/search.tmp /tmp/replace.tmp
	
	# Change the color 'blue' to the color 'red'
	printf "=blue" > /tmp/search.tmp
	printf "=red" > /tmp/replace.tmp
	$REPLACES $BIG.dot /tmp/search.tmp /tmp/replace.tmp
	
	# Change the fillcolor of the highlighted nodes
	printf ",color=red" > /tmp/search.tmp
	# Note: previous value: penwidth=14
	#printf ",color=red,fillcolor=gold2,penwidth=100" > /tmp/replace.tmp
	printf ",color=yellowgreen,shape=box,fontcolor=darkgreen,style=filled,penwidth=20" > /tmp/replace.tmp
	$REPLACES $BIG.dot /tmp/search.tmp /tmp/replace.tmp
	
	#\nnode [fontsize=15,margin=0,fixedsize=true,width=1.7,height=1.7];
	#\nedge [len=0.4,weight=500.2];
	
	
	# Change the graph into a non directed one
	printf "digraph " > /tmp/search.tmp
	printf "graph " > /tmp/replace.tmp
	$REPLACES $BIG.dot /tmp/search.tmp /tmp/replace.tmp
	printf " -> " > /tmp/search.tmp
	printf " -- " > /tmp/replace.tmp
	$REPLACES $BIG.dot /tmp/search.tmp /tmp/replace.tmp
	
	# Open graphically the DOT file
	#~/prog/graphviz-2.12/cmd/dotty/dotty.sh $BIG.dot&
	
	# Get a graphical representation of the DOT file corresponding to the Subdue substructure in PDF
	#dot -Nfontname="arialr" -Gratio=fill -Gsize=15,15 -Kneato -Tps2 -o $BIG.ps $BIG.dot 2> /dev/null
	#dot -Nfontname="arialr" -Gratio=fill -Gsize=15,15 -Kneato -Teps -o $BIG.eps $BIG.dot 2> /dev/null
	#ps2pdf14 $BIG.ps $BIG.pdf
	#printf "Created:\t\t$BIG.pdf\n"
	printf "Created:\t\t$BIG.dot\n"
	$DOTPROG -Gratio=fill -Gsize=15,15 -o $BIG.eps -Teps -Kneato $BIG.dot
	convert $BIG.eps $BIG.png
	printf "Created:\t\t$BIG.png\n"
	#acroread $BIG.pdf&
fi

