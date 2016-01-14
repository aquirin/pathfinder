Toolbox in command line to convert graphs from/to various formats and apply some basic transformations. See checkdb -h

Supported formats (for input and output):
- gSpan (also called the DIMACS format, https://www.cs.ucsb.edu/~xyan/software/gSpan.htm)
- Gaston (http://liacs.leidenuniv.nl/~nijssensgr/gaston/)
- FFSM (chemical databases, input only, http://sourceforge.net/projects/ffsm/)
- Subdue (http://ailab.wsu.edu/subdue/)
- Pajek (http://vlado.fmf.uni-lj.si/pub/networks/pajek/)
- DOT (output only, https://en.wikipedia.org/wiki/DOT_(graph_description_language), http://www.graphviz.org/)
- Prolog (input only, http://www.eecs.wsu.edu/mgd/gdb.html)
- JXplore (proprietary toolbox in Java to visualize the graphs, output only)
- Plain text (output only)
- Human format (better formatted to be read, output only)
Multiple graphs in a single file is supported.


Usage: <checkdb> [options]
AT LEAST ONE OF THEM IS NEEDED (evaluated in the following order):
	-c <type>        Do some checks (all, recognized, tot_recognized, diff_field, all_dfl_weird). See also -w (for all_dfl_weird)
	-m <type>        Modification of non-zero weights (add, sub_from_cst, set, multiply, compare_with). See -w
	-u <filename>    Union with the specified network (must have the same number and values of nodes). See also -o
	-d <filename>    Difference with the specified network (the -d network is considered as the old one) (if -w=0, get only the unsigned difference ABS(SIGN(x)-ABS(SIGN(y), if w=1 get the floating difference). See also -o and -w
	-q <type>        Filter (keep) nodes (all, unisolated)
	-r <type>        Filter (keep) links (all, less, greater, equal, less_equal, greater_equal, sfl, dfl, filter3). See also -w (for less/greater/equal/less_equal/greater_equal) and -o
	-n <type>        Give some statistics (all, num_nodes, num_edges, graph_type, symmetry, num_doubled, num_diff_doubled, links, nodes, central, cycles, show_cycles)
					'central' statistics gives:
						(1) the node minimizing the sum of the distance between a node and all the nodes
						(2) the node minimizing the maximal distance
						(3) the node maximizing the number of inner links
						(4) the node maximizing the sum of links. This is the most stable for normal and pfnet maps
	-o <type>        Output type (none, matrix, edges, arcs, human, dot, plain, gaston, subdue, jxplore, gspan,
								subdue,{directed|undirected|weight|noedgelabel|ijlabel|edgelabel|sorted|unsorted}*
								jxplore,{directed|undirected|weight|noedgelabel|ijlabel|edgelabel}*
								(example: subdue,noedgelabel,undirected - default for subdue: directed,noedgelabel,unsorted)).
OPTIONAL:
	-h               Help
	-f <filename>    Input by a filename instead of standard input
	-w <value>       Value of the 'weird threshold', the 'filter threshold', the 'cst value' or the 'diff type'. See -r, -c, -m or -d
	-s               Use the scored DB (" SCORED_DATABASE ")
	-i <filename>    Use the provided file instead of " DATABASE_NAME " (to desactivate, use "")
	-p               Avoid the computation of the predecessor matrix (could save time a/o memory)
	-t <filename>    Use the given dictionnary to translate the index of the nodes and edges, using their names. Concern only the 'dot', the 'subdue' and the 'gaston' outputs.
	-y <type>        Input type (pajek, subdue, gaston, gspan, ffsm, prolog).
	-v <filename>    Output filename. If not provided, output on stdout. Useful for multiple graphs DB.

Examples:
%s -f file.net -n all                     Give some statistics about the network
%s -f file.net -r less -w 100 -o edges    Return all the links below 100
