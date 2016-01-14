#!/bin/bash


#####
# DEFINES
#####

SCRIPTS="/projets/polemic/prog/Pathfinder-Scripts/"
PROGSUB="/projets/polemic/prog/subdue-5.2.2/bin/subs2dot"
DOT_HOME="/projets/polemic/prog/local"
export PATH=$PATH:"$DOT_HOME/bin"
PROGDOT="$DOT_HOME/bin/dot"

#####
# FUNCTIONS
#####

# Highlight a sub in another one
highlight(){
	local SMALL_SRC="$1"
	local BIG_SRC="$2"
	# Path+filename
	local BIG_DEST="$3"
	#PROGHIGH="${SCRIPTS}/highlight_multi_subs.sh"
	PROGHIGH="${SCRIPTS}/highlight_substructure.sh"

	# Preparation
	mkdir -p ./tmp
	cp $SMALL_SRC ./tmp/small.sub
	cp $BIG_SRC ./tmp/big.sub
	cat ./tmp/small.sub | grep -v "^%" | grep -v "^S" > ./tmp/small.sub.2
	mv ./tmp/small.sub.2 ./tmp/small.sub

	# Highlight
	$PROGHIGH ./tmp/small.sub ./tmp/big.sub

	# Post-process
	mv ./tmp/big.sub.png $BIG_DEST
}

#####
# Print some subs
#####

# Convert a SUB (Subdue format) into a PDF.
printsub(){
	local MYSUB="$1"
	#FONTSIZE=15 # 40
	VISID=0
	DOTSIZE="15,15"
	if [ "$#" -eq 2 ]; then
		VISID=$2	# Visual ID
	fi
	
	# Preparation
	cat $MYSUB | grep -v "^%" | grep -v "^S" > $MYSUB.tmp

	# Convert the substructure in a DOT file
	$PROGSUB $MYSUB.tmp $MYSUB.dot > /dev/null
	cp $MYSUB.dot $MYSUB.dot.cpy

	# Modify some parameters of the DOT file (fontsize, legend)
	
	##################################################
	# Original command line, typical maps, for large maps
	if [ $VISID -eq 4 ]; then
		DOTSIZE="7,7"
		sed "s/compound=true;/compound=true;\nnode [fontsize=15,margin=0,fixedsize=true,width=0.1,height=0.1];\nratio=fill;\nsize=\"15,15\";\npad=\"1.5,1.5\";\nsep=-0.2;\ncenter=true;\n/g" $MYSUB.dot > $MYSUB.dot2; mv $MYSUB.dot2 $MYSUB.dot
	fi
	##################################################
		
	##################################################
	# To increase readability of small graphs (<10 nodes) (old)
	#sed "s/compound=true;/compound=true;\nnode [fontsize=40,margin=0,fixedsize=true,width=0.1,height=0.1,nodesep=0.05];\nedge[len=0.05,weight=1000.2];\nratio=fill;\nsize=\"15,15\";\npad=\"1.5,1.5\";\nsep=-0.2;\ncenter=true;\nnodesep=0.0;\n/g" $MYSUB.dot > $MYSUB.dot2; mv $MYSUB.dot2 $MYSUB.dot
	##################################################
	
	##################################################
	if [ $VISID -eq 1 ]; then
		# To increase readability of small graphs (<10 nodes) (new)
		sed "s/compound=true;/compound=true;\nnode [shape=box,fontsize=15,margin=0,fixedsize=true,width=1.7,height=1.7,color=yellowgreen,style=filled,fontcolor=darkgreen];\nedge[len=0.4,weight=500.2];\nratio=fill;\nsize=\"15,15\";\npad=\"1.5,1.5\";\nsep=-0.2;\ncenter=true;\nnodesep=0.0;\n/g" $MYSUB.dot > $MYSUB.dot2; mv $MYSUB.dot2 $MYSUB.dot
		# node: 
		sed "s/^\( \+[0-9]\+ \[.\+\),color=black,fontcolor=black/\1/" $MYSUB.dot > $MYSUB.dot2; mv $MYSUB.dot2 $MYSUB.dot
	fi
	##################################################
	
	##################################################
	if [ $VISID -eq 2 ]; then
		# To increase readability of small graphs (<10 nodes) (subdue)
		sed "s/compound=true;/compound=true;\nnode [shape=box,fontsize=15,margin=0,fixedsize=true,width=1.0,height=1.0,color=yellowgreen,style=filled,fontcolor=darkgreen];\nedge[len=0.4,weight=500.2];\nratio=fill;\nsize=\"15,15\";\npad=\"1.5,1.5\";\nsep=-0.2;\ncenter=true;\nnodesep=0.0;\n/g" $MYSUB.dot > $MYSUB.dot2; mv $MYSUB.dot2 $MYSUB.dot
		# node: 
		sed "s/^\( \+[0-9]\+ \[.\+\),color=black,fontcolor=black/\1/" $MYSUB.dot > $MYSUB.dot2; mv $MYSUB.dot2 $MYSUB.dot
	fi
	##################################################
	
	##################################################
	if [ $VISID -eq 3 ]; then
		# To increase readability of small graphs (<10 nodes) (continent)
		sed "s/compound=true;/compound=true;\nnode [shape=box,fontsize=15,margin=0,width=1.0,height=1.0,color=yellowgreen,style=filled,fontcolor=darkgreen];\nedge[len=0.2,weight=500.2];\nratio=fill;\npad=\".3,.3\";\nsep=0.3;\ncenter=true;\nnodesep=0.0;\n/g" $MYSUB.dot > $MYSUB.dot2; mv $MYSUB.dot2 $MYSUB.dot
		# node: 
		sed "s/^\( \+[0-9]\+ \[.\+\),color=black,fontcolor=black/\1/" $MYSUB.dot > $MYSUB.dot2; mv $MYSUB.dot2 $MYSUB.dot
	fi
	##################################################
	
	
	##################################################
	# For US-2005 map (old)
	#sed "s/compound=true;/compound=true;\nnode [fontsize=15,margin=0,fixedsize=true,width=0.5,height=0.5,color=yellowgreen,style=filled,fontcolor=darkgreen];\nratio=fill;\nsize=\"15,15\";\npad=\"1.5,1.5\";\nsep=-0.2;\ncenter=true;\n/g" $MYSUB.dot > $MYSUB.dot2; mv $MYSUB.dot2 $MYSUB.dot
	##################################################
	
	##################################################
	# For US-2005 map (new)
	#sed "s/compound=true;/compound=true;\nnode [shape=box,fontsize=15,color=yellowgreen,style=filled,fontcolor=darkgreen];\nedge[len=2];\n/g" $MYSUB.dot > $MYSUB.dot2; mv $MYSUB.dot2 $MYSUB.dot
	# Process all lines, only for the nodes. Remove the color of the nodes.
	#sed "s/^\( \+[0-9]\+ \[.\+\),color=black,fontcolor=black/\1/" $MYSUB.dot > $MYSUB.dot2; mv $MYSUB.dot2 $MYSUB.dot
	##################################################

	# Get a graphical representation of the DOT file corresponding to the Subdue substructure
	$PROGDOT -Gratio=fill -Gsize=$DOTSIZE -Kneato -Teps -o $MYSUB.eps $MYSUB.dot 2> /dev/null
	#convert $MYSUB.eps $MYSUB.pdf
	#epstopdf --res=600 $MYSUB.eps
	#epstool --copy --bbox $MYSUB.eps --output $MYSUB.eps
	epstopdf -hires $MYSUB.eps
	echo "Creation of: $MYSUB.pdf ..."
	
	#bmp canon cmap cmapx cmapx_np dot eps fig gtk gv ico imap imap_np ismap jpe jpeg jpg pdf plain plain-ext png ps ps2 svg svgz tif tiff tk vml vmlz x11 xdot xlib

	# For PDF
	rm $MYSUB.tmp $MYSUB.dot $MYSUB.dot.cpy $MYSUB.eps
	
	# For EPS
	#rm $MYSUB.tmp $MYSUB.dot $MYSUB.dot.cpy $MYSUB.pdf
}
#printsub "$mysub"

# Convert the SUB number N into a PDF.
printNsub() {
	local N="$1"
	local FILE="$2"
	local OUTPUT="$3"
	VISID=0
	if [ "$#" -eq 4 ]; then
		VISID=$4	# Visual ID
	fi
	
	MIN="`cat "$COMFILE" | grep -n "($N)" | cut -f1 -d':'`"
	MAX="`cat "$COMFILE" | grep -n "($(($N+1)))" | cut -f1 -d':'`"
	if [ -e $MAX ]
	then
		MAX="`wc -l "$COMFILE" | cut -f1 -d' '`"
	else
		MAX=$(($MAX-1))
	fi
	
	cat "$COMFILE" | sed -n ${MIN},${MAX}p > ./mysub.tmp
	printsub ./mysub.tmp $VISID
	rm ./mysub.tmp
	mv ./mysub.tmp.pdf "$OUTPUT"
	echo "Creation of: $OUTPUT ..."
}
