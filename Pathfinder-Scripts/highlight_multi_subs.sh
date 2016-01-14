#!/bin/bash

# Highlight in a given graph a given list of subs.
# First paramter: the path to a list of subs (one sub for each line), each of them should be included in the bigger graph
# Second parameter: the bigger graph
# Highlight the corresponding substructures by producing an appropriate PNG.

SCRIPTS="/projets/polemic/pathfinder/scripts"
SUBDUE_PATH="/projets/polemic/prog/subdue-5.2.2/bin"
TMP_FILE=/tmp/highlight.tmp
REPL_TMP_FILE=/tmp/replace.tmp
SEARCH_TMP_FILE=/tmp/search.tmp
FONTSIZE=2
REPLACES=${SCRIPTS}/replace_text.pl
SORT_SUB=${SCRIPTS}/sort_sub.sh
DOT_HOME="/projets/polemic/prog/graphviz-2.30.1/local/"
export PATH=$PATH:"$DOT_HOME/bin"
DOTPROG="$DOT_HOME/bin/dot"
TMP_NEWFILE=/tmp/subdue.newfile.tmp
TMP_INPUTFILE=/tmp/subdue.input.tmp
export GDFONTPATH=/usr/share/fonts/truetype/

if [ $# -lt 3 ]
then
	echo "Used to highlight all subs from <List.lst> into <BigGraph.sub> by producing a PDF file."
	echo "Usage: $0 <List.lst> <BigGraph.sub> <Result (without ext.)>"
else
	LIST="$1"
	BIG="$2"
	RESULT="$3"
	FIRSTSUB=1
	
	# For each element of the list, highlight the corresponding sub
	while read SMALL; do
	    #echo "-- $SMALL"
		
		# Prepare the sub: remove the comments
		cat $SMALL | grep -v "^%" | grep -v "^S" > $SMALL.tmp

		# Highlight the substructure SMALL in BIG
		$SUBDUE_PATH/sgiso -dot ${TMP_NEWFILE} $SMALL.tmp $BIG > $TMP_FILE
		if [ ! -s $TMP_FILE ];
		then
			echo "$SMALL has not been found into $BIG !"
			
			# Cleaning
			rm -f $SMALL.tmp
			rm -f $TMP_FILE $REPL_TMP_FILE $SEARCH_TMP_FILE $TMP_NEWFILE $TMP_INPUTFILE
			exit -1
		fi
		rm -f $SMALL.tmp
		
		# Sort the file
		$SORT_SUB ${TMP_NEWFILE} > ${TMP_NEWFILE}.sort
		mv ${TMP_NEWFILE}.sort ${TMP_NEWFILE}

		if [ $FIRSTSUB -eq 1 ]
		then
			# First sub, do nothing more
			mv ${TMP_NEWFILE} ${TMP_INPUTFILE}
			cp ${TMP_INPUTFILE} ${RESULT}.dot
			FIRSTSUB=0
		else
			# Other subs, compare to the first sub
			
			# Compare the old file and the new file. Any old 'black' lines are removed.
			#diff ${TMP_INPUTFILE} ${TMP_NEWFILE} | grep "^<" | grep black
			lnums=`diff --unchanged-line-format="" --old-line-format="%dn %L" --new-line-format="" ${TMP_INPUTFILE} ${TMP_NEWFILE} | grep "color=black" | cut -f1 -d' '`
			ndiff=0
			for n in $lnums
			do
				#echo "del $n"
				num=$(($n-$ndiff))
				sed "${num}d" ${RESULT}.dot > ${RESULT}.dot.tmp
				mv ${RESULT}.dot.tmp ${RESULT}.dot
				ndiff=$(($ndiff+1))
			done


			# Compare the old file and the new file. Any new 'blue' lines are added.
			#diff ${TMP_INPUTFILE} ${TMP_NEWFILE} | grep "^>" | grep blue
			lnums=`diff --unchanged-line-format="" --old-line-format="" --new-line-format="%dn %L" ${TMP_INPUTFILE} ${TMP_NEWFILE} | grep "color=blue" | cut -f1 -d' '`
			for n in $lnums
			do
				#echo "add $n"
				printf "digraph SubdueGraph {\n" > $SEARCH_TMP_FILE
				printf "digraph SubdueGraph {\n" > $REPL_TMP_FILE
				sed -n "${n}p" ${TMP_NEWFILE} >> $REPL_TMP_FILE
				$REPLACES ${RESULT}.dot $SEARCH_TMP_FILE $REPL_TMP_FILE
			done

			# Problem: lines are added in the beggining, if at the end, same problem: nodes cannot be defined AFTER the edges...
			# Sort the file
			$SORT_SUB ${RESULT}.dot > ${RESULT}.dot.sort
			mv ${RESULT}.dot.sort ${RESULT}.dot

		fi
		
	done < "$LIST"

	# Build the final DOT file
	printf "digraph SubdueGraph {\n" > $SEARCH_TMP_FILE
	printf "digraph SubdueGraph {\n" > $REPL_TMP_FILE
	
	printf "ratio=auto;\n" >> $REPL_TMP_FILE
	printf "size=\"10,10\";\n" >> $REPL_TMP_FILE
	printf "overlap=\"scale\"; \n" >> $REPL_TMP_FILE
	printf "sep=-0.3;\n" >> $REPL_TMP_FILE
	printf "center=true;\n\n" >> $REPL_TMP_FILE
	printf "edge [size=1];\n" >> $REPL_TMP_FILE
	printf "node [shape=circle, margin=0, fixedsize=true, width=1.0, height=1.0,\n" >> $REPL_TMP_FILE
	printf "\tfontsize=8, fillcolor=white, style=filled, color=white];\n" >> $REPL_TMP_FILE
	#printf "node [fontsize=$FONTSIZE];\n" >> $REPL_TMP_FILE

	printf "labelloc=\"top\";\n" >> $REPL_TMP_FILE
	printf "fontsize=100;\n\n" >> $REPL_TMP_FILE
	
	# Add the graph attribute values to the DOT file
	$REPLACES ${RESULT}.dot $SEARCH_TMP_FILE $REPL_TMP_FILE
	
	# Change the color 'blue' to the color 'red'
	printf "=blue" > $SEARCH_TMP_FILE
	printf "=red" > $REPL_TMP_FILE
	$REPLACES ${RESULT}.dot $SEARCH_TMP_FILE $REPL_TMP_FILE
	
	# Change the fillcolor of the highlighted nodes
	printf ",color=red" > $SEARCH_TMP_FILE
	# Note: previous value: penwidth=14
	printf ",color=red,fillcolor=gold2,penwidth=100" > $REPL_TMP_FILE
	$REPLACES ${RESULT}.dot $SEARCH_TMP_FILE $REPL_TMP_FILE

	# Change the graph into a non directed one
	printf "digraph " > $SEARCH_TMP_FILE
	printf "graph " > $REPL_TMP_FILE
	$REPLACES ${RESULT}.dot $SEARCH_TMP_FILE $REPL_TMP_FILE
	printf " -> " > $SEARCH_TMP_FILE
	printf " -- " > $REPL_TMP_FILE
	$REPLACES ${RESULT}.dot $SEARCH_TMP_FILE $REPL_TMP_FILE
			
	# Open graphically the DOT file
	#~/prog/graphviz-2.12/cmd/dotty/dotty.sh ${RESULT}.dot&
	
	# Get a graphical representation of the DOT file corresponding to the Subdue substructure in PDF
	#dot -Nfontname="arialr" -Gratio=fill -Gsize=15,15 -Kneato -Tps2 -o ${RESULT}.ps ${RESULT}.dot 2> /dev/null
	#dot -Nfontname="arialr" -Gratio=fill -Gsize=15,15 -Kneato -Teps -o ${RESULT}.eps ${RESULT}.dot 2> /dev/null
	#ps2pdf14 ${RESULT}.ps ${RESULT}.pdf
	#printf "Created:\t\t${RESULT}.pdf\n"
	printf "Created:\t\t${RESULT}.dot\n"
	$DOTPROG -Gratio=fill -Gsize=15,15 -o ${RESULT}.eps -Teps -Kneato ${RESULT}.dot
	convert ${RESULT}.eps ${RESULT}.png
	printf "Created:\t\t${RESULT}.png\n"
	#acroread ${RESULT}.pdf&
	
	# Cleaning
	rm -f $TMP_FILE $REPL_TMP_FILE $SEARCH_TMP_FILE $TMP_NEWFILE $TMP_INPUTFILE
fi
