#include "input_subdue.h"


//---------------------------------------------------------------------------
// NAME: ReadInputFile
//
// INPUTS: (Parameters *parameters)
//
// RETURN: (void)
//
// PURPOSE: Reads in the SUBDUE input file, which may consist of
// positive graphs and/or negative graphs, which are collected into
// the positive and negative graph fields of the parameters.  Each
// example in the input file is prefaced by the appropriate token defined
// in subdue.h.  The first graph in the file is assumed positive
// unless the negative token is present.  Each graph is assumed to
// begin at vertex #1 and therefore examples are not connected to one
// another.
//---------------------------------------------------------------------------

void ReadInputFile(Parameters *parameters)
{
   FILE *inputFile = NULL;
   SubdueGraph *graph = NULL;
   SubdueGraph *posGraph= NULL;
   SubdueGraph *negGraph = NULL;
   ULONG posGraphVertexListSize = 0;
   ULONG posGraphEdgeListSize = 0;
   ULONG negGraphVertexListSize = 0;
   ULONG negGraphEdgeListSize = 0;
   ULONG *vertexListSizePtr = NULL;
   ULONG *edgeListSizePtr = NULL;
   LabelList *labelList = NULL;
   ULONG numPosEgs = 0;
   ULONG numNegEgs = 0;
   ULONG *posEgsVertexIndices = NULL;
   ULONG *negEgsVertexIndices = NULL;
   BOOLEAN readingPositive = SUBD_TRUE;
   ULONG vertexOffset = 0;
   BOOLEAN directed = SUBD_TRUE;
   ULONG lineNo = 1;
   char token[TOKEN_LEN];

   labelList = parameters->labelList;
   directed = parameters->directed;

   // Open input file
   inputFile = fopen(parameters->inputFileName,"r");
   if (inputFile == NULL) 
   {
      fprintf(stderr, "Unable to open input file %s.\n",
              parameters->inputFileName);
      exit(1);
   }

   // Parse input file
   while (ReadToken(token, inputFile, &lineNo) != 0) 
   {
      if (strcmp(token, POS_EG_TOKEN) == 0) 
      { // reading positive eg
         if (posGraph == NULL)
            posGraph = AllocateGraph(0,0);
         numPosEgs++;
         vertexOffset = posGraph->numVertices;
         posEgsVertexIndices = AddVertexIndex(posEgsVertexIndices,
                                              numPosEgs, vertexOffset);
         graph = posGraph;
         vertexListSizePtr = & posGraphVertexListSize;
         edgeListSizePtr = & posGraphEdgeListSize;
         readingPositive = SUBD_TRUE;
      }
      else if (strcmp(token, NEG_EG_TOKEN) == 0) 
      { // reading negative eg
         if (negGraph == NULL)
            negGraph = AllocateGraph(0,0);
         numNegEgs++;
         vertexOffset = negGraph->numVertices;
         negEgsVertexIndices = AddVertexIndex(negEgsVertexIndices,
                                              numNegEgs, vertexOffset);
         graph = negGraph;
         vertexListSizePtr = & negGraphVertexListSize;
         edgeListSizePtr = & negGraphEdgeListSize;
         readingPositive = SUBD_FALSE;
      }
      else if (strcmp(token, "v") == 0) 
      {  // read vertex
         if (readingPositive && (posGraph == NULL)) 
         {
            // first graph starts without positive token, so assumed positive
            posGraph = AllocateGraph(0,0);
            numPosEgs++;
            vertexOffset = 0;
            posEgsVertexIndices = AddVertexIndex(posEgsVertexIndices,
                                                numPosEgs, vertexOffset);
            graph = posGraph;
            vertexListSizePtr = & posGraphVertexListSize;
            edgeListSizePtr = & posGraphEdgeListSize;
         }
         ReadVertex(graph, inputFile, labelList, vertexListSizePtr, &lineNo,
                    vertexOffset);
      }
      else if (strcmp(token, "e") == 0)    // read 'e' edge
         ReadEdge(graph, inputFile, labelList, edgeListSizePtr, &lineNo,
                  directed, vertexOffset);

      else if (strcmp(token, "u") == 0)    // read undirected edge
         ReadEdge(graph, inputFile, labelList, edgeListSizePtr, &lineNo,
                  SUBD_FALSE, vertexOffset);

      else if (strcmp(token, "d") == 0)    // read directed edge
         ReadEdge(graph, inputFile, labelList, edgeListSizePtr, &lineNo,
                  SUBD_TRUE, vertexOffset);

      else 
      {
         fclose(inputFile);
         fprintf(stderr, "Unknown token %s in line %lu of input file %s.\n",
                 token, lineNo, parameters->inputFileName);
         exit(1);
      }
   }
   fclose(inputFile);

   // ***** trim vertex, edge and label lists

   parameters->posGraph = posGraph;
   parameters->negGraph = negGraph;
   parameters->labelList = labelList;
   parameters->numPosEgs = numPosEgs;
   parameters->numNegEgs = numNegEgs;
   parameters->posEgsVertexIndices = posEgsVertexIndices;
   parameters->negEgsVertexIndices = negEgsVertexIndices;
}


//---------------------------------------------------------------------------
// NAME: AddVertexIndex
//
// INPUTS: (ULONG *vertexIndices) - array of indices to augment
//         (ULONG n) - size of new array
//         (ULONG index) - index to add to nth element of array
//
// RETURN: (ULONG *) - new vertex index array
//
// PURPOSE: Reallocate the given vertex index array and store the new
// index in the nth element of the array.  This is used to build the
// array of indices into the positive and negative examples graphs.
//---------------------------------------------------------------------------

ULONG *AddVertexIndex(ULONG *vertexIndices, ULONG n, ULONG index)
{
   vertexIndices = (ULONG *) realloc(vertexIndices, sizeof(ULONG) * n);
   if (vertexIndices == NULL)
      OutOfMemoryError((char*)"AddVertexIndex:vertexIndices");
   vertexIndices[n - 1] = index;
   return vertexIndices;
}


//---------------------------------------------------------------------------
// NAME: ReadPredefinedSubsFile
//
// INPUTS: (Parameters *parameters)
//
// RETURN: (void)
//
// PURPOSE: Reads in one or more graphs from the given file and stores
// these on the predefined substructure list in parameters.  Each
// graph is prefaced by the predefined substructure token defined in
// subdue.h.
//
// Right now, these substructures will be used to compress the graph,
// if present, and therefore any labels not present in the input graph
// will be discarded during compression.  If the predefined
// substructures are ever simply put on the discovery queue, then care
// should be taken to not include labels that do not appear in the
// input graph, as this would bias the MDL computation. (*****)
//---------------------------------------------------------------------------

void ReadPredefinedSubsFile(Parameters *parameters)
{
   ULONG numPreSubs = 0;
   SubdueGraph **preSubs = NULL;

   preSubs = ReadSubGraphsFromFile(parameters->psInputFileName,
                                   PREDEF_SUB_TOKEN,
                                   & numPreSubs,
                                   parameters);
   parameters->numPreSubs = numPreSubs;
   parameters->preSubs = preSubs;
}


//---------------------------------------------------------------------------
// NAME: ReadSubGraphsFromFile
//
// INPUTS: (char *fileName) - file containing substructure graphs
//         (char *subToken) - token used to separate sub graphs in file
//         (ULONG *numSubGraphs) - call-by-reference variable returning
//                                 number of substructure graphs read
//         (Parameters *parameters)
//
// RETURN: (SubdueGraph **) - array of substructure graphs
//
// PURPOSE: Reads in one or more graphs from the given file and stores
// them in an array of graphs.  Each graph (except optionally the
// first) is assumed to be prefaced by the given substructure token.
//---------------------------------------------------------------------------

SubdueGraph **ReadSubGraphsFromFile(const char *fileName, const char *subToken,
                              ULONG *numSubGraphs, Parameters *parameters)
{
   ULONG numSubs = 0;
   SubdueGraph **subGraphs = NULL;
   FILE *inputFile = NULL;
   SubdueGraph *graph = NULL;
   ULONG vertexListSize = 0;
   ULONG edgeListSize = 0;
   LabelList *labelList = NULL;
   BOOLEAN directed = SUBD_TRUE;
   ULONG vertexOffset = 0;   // Dummy argument to ReadVertex and ReadEdge
   ULONG lineNo = 1;
   char token[TOKEN_LEN];

   labelList = parameters->labelList;
   directed = parameters->directed;

   // Open input file
   inputFile = fopen(fileName,"r");
   if (inputFile == NULL) 
   {
      fprintf(stderr, "Unable to open input file %s.\n", fileName);
      exit(1);
   }

   // Parse input file
   while (ReadToken(token, inputFile, &lineNo) != 0) 
   {
      if (strcmp(token, subToken) == 0) 
      { // new sub-graph
         numSubs++;
         subGraphs = (SubdueGraph **) realloc(subGraphs, (sizeof(SubdueGraph *) * numSubs));
         if (subGraphs == NULL)
            OutOfMemoryError("ReadSubGraphsFromFile:subGraphs");
         subGraphs[numSubs - 1] = AllocateGraph(0, 0);
         graph = subGraphs[numSubs - 1];
         vertexListSize = 0;
         edgeListSize = 0;
      }
      else if (strcmp(token, "v") == 0) 
      {        // read vertex
         if (subGraphs == NULL) 
         {
            // first sub-graph not preceded by subToken
            numSubs++;
            subGraphs = (SubdueGraph **) realloc(subGraphs,
                                           (sizeof(SubdueGraph *) * numSubs));
            if (subGraphs == NULL)
               OutOfMemoryError("ReadSubGraphsFromFile:subGraphs");
            subGraphs[numSubs - 1] = AllocateGraph(0, 0);
            graph = subGraphs[numSubs - 1];
            vertexListSize = 0;
            edgeListSize = 0;
         }
         ReadVertex(graph, inputFile, labelList, &vertexListSize, &lineNo,
                    vertexOffset);
      }
      else if (strcmp(token, "e") == 0)    // read 'e' edge
         ReadEdge(graph, inputFile, labelList, &edgeListSize, &lineNo,
                  directed, vertexOffset);

      else if (strcmp(token, "u") == 0)    // read undirected edge
         ReadEdge(graph, inputFile, labelList, &edgeListSize, &lineNo,
                  SUBD_FALSE, vertexOffset);

      else if (strcmp(token, "d") == 0)    // read directed edge
         ReadEdge(graph, inputFile, labelList, &edgeListSize, &lineNo,
                  SUBD_TRUE, vertexOffset);

      else 
      {
         fclose(inputFile);
         fprintf(stderr, "Unknown token %s in line %lu of input file %s.\n",
                 token, lineNo, fileName);
         exit(1);
      }
   }
   fclose(inputFile);

   // ***** trim vertex, edge and label lists

   *numSubGraphs = numSubs;
   return subGraphs;
}


//---------------------------------------------------------------------------
// NAME:    ReadGraph
//
// INPUTS:  (char *filename) - graph file to read from
//          (LabelList *labelList) - list of labels to be added to from graph
//          (BOOLEAN directed) - TRUE if 'e' edges should be directed
//
// RETURN:  (SubdueGraph *) - graph read from file
//
// PURPOSE: Parses graph file, checking for formatting errors, and builds
// all necessary structures for the graph, which is returned.  labelList
// is destructively changed to hold any new labels.
//---------------------------------------------------------------------------

SubdueGraph *ReadGraph(char *filename, LabelList *labelList, BOOLEAN directed)
{
   SubdueGraph *graph;
   FILE *graphFile;
   ULONG lineNo;             // Line number counter for graph file
   char token[TOKEN_LEN];
   ULONG vertexListSize = 0; // Size of currently-allocated vertex array
   ULONG edgeListSize = 0;   // Size of currently-allocated edge array
   ULONG vertexOffset = 0;   // Dummy argument to ReadVertex and ReadEdge

   // Allocate graph
   graph = AllocateGraph(0,0);

   // Open graph file
   graphFile = fopen(filename,"r");
   if (graphFile == NULL) 
   {
      fprintf(stderr, "Unable to open graph file %s.\n", filename);
      exit(1);
   }

   // Parse graph file
   lineNo = 1;
   while (ReadToken(token, graphFile, &lineNo) != 0) 
   {
      if (strcmp(token, "v") == 0)         // read vertex
         ReadVertex(graph, graphFile, labelList, &vertexListSize, &lineNo,
                    vertexOffset);

      else if (strcmp(token, "e") == 0)    // read 'e' edge
         ReadEdge(graph, graphFile, labelList, &edgeListSize, &lineNo, directed,
                  vertexOffset);

      else if (strcmp(token, "u") == 0)    // read undirected edge
         ReadEdge(graph, graphFile, labelList, &edgeListSize, &lineNo, SUBD_FALSE,
                  vertexOffset);

      else if (strcmp(token, "d") == 0)    // read directed edge
         ReadEdge(graph, graphFile, labelList, &edgeListSize, &lineNo, SUBD_TRUE,
                  vertexOffset);

      else 
      {
         fclose(graphFile);
         FreeGraph(graph);
         fprintf(stderr, "Unknown token %s in line %lu of graph file %s.\n",
                 token, lineNo, filename);
         exit(1);
      }
   }
   fclose(graphFile);

   // ***** trim vertex, edge and label lists

   return graph;
}


//---------------------------------------------------------------------------
// NAME:    ReadVertex
//
// INPUTS:  (SubdueGraph *graph) - pointer to graph being constructed
//          (FILE *fp) - pointer to graph file stream
//          (LabelList *labelList) - list of vertex and edge labels
//          (ULONG *vertexListSize) - pointer to size of graph's allocated
//                                    vertex array
//          (ULONG *pLineNo) - pointer to line counter in calling function
//          (ULONG vertexOffset) - offset to add to vertex numbers
//
// RETURN:  (void)
//
// PURPOSE: Read and check the vertex number and label, store label in
// given label list, and add vertex to graph.  A non-zero vertexOffset
// indicates this vertex is part of a graph beyond the first.
//--------------------------------------------------------------------------

void ReadVertex(SubdueGraph *graph, FILE *fp, LabelList *labelList,
       ULONG *vertexListSize, ULONG *pLineNo, ULONG vertexOffset)
{
   ULONG vertexID;
   ULONG labelIndex;

   // read and check vertex number
   vertexID = ReadInteger(fp, pLineNo) + vertexOffset;
   if (vertexID != (graph->numVertices + 1)) 
   {
      fprintf(stderr, "Error: invalid vertex number at line %lu.\n",
              *pLineNo);
      exit(1);
   }
   // read label
   labelIndex = ReadLabel(fp, labelList, pLineNo);

   AddVertex(graph, labelIndex, vertexListSize);
}


//---------------------------------------------------------------------------
// NAME: AddVertex
//
// INPUTS: (SubdueGraph *graph) - graph to add vertex
//         (ULONG labelIndex) - index into label list of vertex's label
//         (ULONG *vertexListSize) - pointer to size of graph's allocated
//                                   vertex array
//
// RETURN: (void)
//
// PURPOSE: Add vertex information to graph. AddVertex also changes the
// size of the currently-allocated vertex array, which increases by
// LIST_SIZE_INC (instead of just 1) when exceeded.
//---------------------------------------------------------------------------

void AddVertex(SubdueGraph *graph, ULONG labelIndex, ULONG *vertexListSize)
{
   Vertex *newVertexList;
   ULONG numVertices;

   numVertices = graph->numVertices;
   // make sure there is enough room for another vertex
   if (*vertexListSize == graph->numVertices) 
   {
      *vertexListSize += LIST_SIZE_INC;
      newVertexList = (Vertex *) realloc(graph->vertices, 
                                         (sizeof(Vertex) * (*vertexListSize)));
      if (newVertexList == NULL)
         OutOfMemoryError("vertex list");
      graph->vertices = newVertexList;
   }

   // store information in vertex
   graph->vertices[numVertices].label = labelIndex;
   graph->vertices[numVertices].numEdges = 0;
   graph->vertices[numVertices].edges = NULL;
   graph->vertices[numVertices].map = VERTEX_UNMAPPED;
   graph->vertices[numVertices].used = SUBD_FALSE;
   graph->numVertices++;
}


//---------------------------------------------------------------------------
// NAME:    ReadEdge
//
// INPUTS:  (SubdueGraph *graph) - pointer to graph being constructed
//          (FILE *fp) - pointer to graph file stream
//          (LabelList *labelList) - list of vertex and edge labels
//          (ULONG *edgeListSize) - pointer to size of graph's allocated
//                                  edge array
//          (ULONG *pLineNo) - pointer to line counter in calling function
//          (BOOLEAN directed) - TRUE if edge is directed
//          (ULONG vertexOffset) - offset to add to vertex numbers
//
// RETURN:  (void)
//
// PURPOSE: Read and check the vertex numbers and label, store label in
// given label list, and add edge to graph.  A non-zero vertexOffset
// indicates the edge's vertices are part of a graph beyond the first.
//---------------------------------------------------------------------------

void ReadEdge(SubdueGraph *graph, FILE *fp, LabelList *labelList,
          ULONG *edgeListSize, ULONG *pLineNo, BOOLEAN directed,
          ULONG vertexOffset)
{
   ULONG sourceVertexID;
   ULONG targetVertexID;
   ULONG sourceVertexIndex;
   ULONG targetVertexIndex;
   ULONG labelIndex;

   // read and check vertex numbers
   sourceVertexID = ReadInteger(fp, pLineNo) + vertexOffset;
   if (sourceVertexID > graph->numVertices) 
   {
      fprintf(stderr,
              "Error: reference to undefined vertex number at line %lu.\n", 
              *pLineNo);
      exit(1);
   }
   targetVertexID = ReadInteger(fp, pLineNo) + vertexOffset;
   if (targetVertexID > graph->numVertices) 
   {
      fprintf(stderr,
              "Error: reference to undefined vertex number at line %lu.\n", 
              *pLineNo);
      exit(1);
   }
   sourceVertexIndex = sourceVertexID - 1;
   targetVertexIndex = targetVertexID - 1;

   // read and store label
   labelIndex = ReadLabel(fp, labelList, pLineNo);

   AddEdge(graph, sourceVertexIndex, targetVertexIndex, directed,
           labelIndex, edgeListSize, SUBD_FALSE);
}


//---------------------------------------------------------------------------
// NAME: AddEdge
//
// INPUTS: (SubdueGraph *graph) - graph to add edge to
//         (ULONG sourceVertexIndex) - index of edge's source vertex
//         (ULONG targetVertexIndex) - index of edge's target vertex
//         (BOOLEAN directed) - TRUE is edge is directed
//         (ULONG labelIndex) - index of edge's label in label list
//         (ULONG *edgeListSize) - pointer to size of graph's allocated
//                                 edge array
//         (ULONG spansIncrement)
//
// RETURN: (void)
//
// PURPOSE: Add edge information to graph. AddEdge also changes the
// size of the currently-allocated edge array, which increases by
// LIST_SIZE_INC (instead of just 1) when exceeded.
//---------------------------------------------------------------------------

void AddEdge(SubdueGraph *graph, ULONG sourceVertexIndex, ULONG targetVertexIndex,
             BOOLEAN directed, ULONG labelIndex, ULONG *edgeListSize,
             BOOLEAN spansIncrement)
{
   Edge *newEdgeList;

   // make sure there is enough room for another edge in the graph
   if (*edgeListSize == graph->numEdges) 
   {
      *edgeListSize += LIST_SIZE_INC;
      newEdgeList = (Edge *) realloc(graph->edges,
                                     (sizeof(Edge) * (*edgeListSize)));
      if (newEdgeList == NULL)
         OutOfMemoryError("AddEdge:newEdgeList");
      graph->edges = newEdgeList;
   }

   // add edge to graph
   graph->edges[graph->numEdges].vertex1 = sourceVertexIndex;
   graph->edges[graph->numEdges].vertex2 = targetVertexIndex;
   graph->edges[graph->numEdges].label = labelIndex;
   graph->edges[graph->numEdges].directed = directed;
   graph->edges[graph->numEdges].used = SUBD_FALSE;
   graph->edges[graph->numEdges].spansIncrement = spansIncrement;
   graph->edges[graph->numEdges].validPath = SUBD_TRUE;

   // add index to edge in edge index array of both vertices
   AddEdgeToVertices(graph, graph->numEdges);

   graph->numEdges++;
}


//---------------------------------------------------------------------------
// NAME: StoreEdge
//
// INPUTS: (Edge *overlapEdges) - edge array where edge is stored
//         (ULONG edgeIndex) - index into edge array where edge is stored
//         (ULONG v1) - vertex1 of edge
//         (ULONG v2) - vertex2 of edge
//         (ULONG label) - edge label index
//         (BOOLEAN directed) - edge directedness
//         (BOOLEAN spansIncrement) - edge crossing increment boundary
//
// RETURN: (void)
//
// PURPOSE: Procedure to store an edge in given edge array.
//---------------------------------------------------------------------------

void StoreEdge(Edge *overlapEdges, ULONG edgeIndex,
               ULONG v1, ULONG v2, ULONG label, BOOLEAN directed, 
               BOOLEAN spansIncrement)
{
   overlapEdges[edgeIndex].vertex1 = v1;
   overlapEdges[edgeIndex].vertex2 = v2;
   overlapEdges[edgeIndex].label = label;
   overlapEdges[edgeIndex].directed = directed;
   overlapEdges[edgeIndex].used = SUBD_FALSE;
   overlapEdges[edgeIndex].spansIncrement = spansIncrement;
}


//---------------------------------------------------------------------------
// NAME: AddEdgeToVertices
//
// INPUTS: (SubdueGraph *graph) - graph containing edge and vertices
//         (ULONG edgeIndex) - edge's index into graph edge array
//
// RETURN: (void)
//
// PURPOSE: Add edge index to the edge array of each of the two
// vertices involved in the edge.  If a self-edge, then only add once.
//---------------------------------------------------------------------------

void AddEdgeToVertices(SubdueGraph *graph, ULONG edgeIndex)
{
   ULONG v1, v2;
   Vertex *vertex;
   ULONG *edgeIndices;

   v1 = graph->edges[edgeIndex].vertex1;
   v2 = graph->edges[edgeIndex].vertex2;
   vertex = & graph->vertices[v1];
   edgeIndices = (ULONG *) realloc(vertex->edges,
                                   sizeof(ULONG) * (vertex->numEdges + 1));
   if (edgeIndices == NULL)
      OutOfMemoryError("AddEdgeToVertices:edgeIndices1");
   edgeIndices[vertex->numEdges] = edgeIndex;
   vertex->edges = edgeIndices;
   vertex->numEdges++;

   if (v1 != v2) 
   { // don't add a self edge twice
      vertex = & graph->vertices[v2];
      edgeIndices = (ULONG *) realloc(vertex->edges,
                                      sizeof(ULONG) * (vertex->numEdges + 1));
      if (edgeIndices == NULL)
         OutOfMemoryError("AddEdgeToVertices:edgeIndices2");
      edgeIndices[vertex->numEdges] = edgeIndex;
      vertex->edges = edgeIndices;
      vertex->numEdges++;
   }
}


//---------------------------------------------------------------------------
// NAME:    ReadLabel
//
// INPUTS:  (FILE *fp) - file pointer from which label is read
//          (LabelList *labelList) - list of vertex and edge labels
//          (ULONG *pLineNo) - pointer to line counter in calling function
//
// RETURN:  (ULONG) - index of read label in global label list.
//
// PURPOSE: Reads a label (string or numeric) from the given file and
// stores the label in the given label list if not already there.
// Returns the label's index in the label list.
//---------------------------------------------------------------------------

ULONG ReadLabel(FILE *fp, LabelList *labelList, ULONG *pLineNo)
{
   char token[TOKEN_LEN];
   char *endptr;
   Label label;

   ReadToken(token, fp, pLineNo);
   label.labelType = NUMERIC_LABEL;
   label.labelValue.numericLabel = strtod(token, &endptr);
   if (*endptr != '\0') 
   {
      label.labelType = STRING_LABEL;
      label.labelValue.stringLabel = token;
   }
   return StoreLabel(&label, labelList);
}


//---------------------------------------------------------------------------
// NAME:    ReadInteger
//
// INPUTS:  (FILE *fp) - file pointer from which number is read
//          (ULONG *pLineNo) - pointer to line counter in calling function
//
// RETURN:  (ULONG) - integer read
//
// PURPOSE: Read an unsigned long integer from the given file.
//---------------------------------------------------------------------------

ULONG ReadInteger(FILE *fp, ULONG *pLineNo)
{
   ULONG i;
   char token[TOKEN_LEN];
   char *endptr;

   ReadToken(token, fp, pLineNo);
   i = strtoul(token, &endptr, 10);
   if (*endptr != '\0') 
   {
      fprintf(stderr, "Error: expecting integer in line %lu.\n",
              *pLineNo);
      exit(1);
   }
   return i;
}


//---------------------------------------------------------------------------
// NAME:    ReadToken
//
// INPUTS:  (char *token) - string into which token is copied
//          (FILE *fp) - file pointer from which token is read
//          (ULONG *pLineNo) - pointer to line counter in calling function
//
// RETURN:  (int) - length of token read
//
// PURPOSE: Read the next token from the given file.  A token is
// defined as a string of non-whitespace characters, where whitespace
// includes spaces, tabs, newlines, comments, and EOF.
//---------------------------------------------------------------------------

int ReadToken(char *token, FILE *fp, ULONG *pLineNo)
{
   char ch;
   int i = 0;

   // skip whitespace and comments
   ch = fgetc(fp);
   while ((ch == SPACE) || (ch == TAB) || (ch == CARRIAGERETURN) ||
    (ch == NEWLINE) || (ch == COMMENT)) 
   {
      if (ch == NEWLINE)
         (*pLineNo)++;
      if (ch == COMMENT) 
      {
         while ((ch != NEWLINE) && (ch != (char)EOF))  // skip to end of line
            ch = fgetc(fp);
         if (ch == NEWLINE)
            (*pLineNo)++;
      }
      if (ch != (char)EOF)
         ch = fgetc(fp);
   }

   // read token
   if (ch == DOUBLEQUOTE) 
   { // read until reaching another double quote
      do 
      {
         token[i++] = ch;
         ch = fgetc(fp);
      } while ((ch != (char)EOF) && (ch != DOUBLEQUOTE));
      if (ch == DOUBLEQUOTE)
         token[i++] = ch;
      ch = fgetc(fp);
   } 
   else 
   { // read until reaching whitespace
      while ((ch != (char)EOF) && (ch != SPACE) && (ch != TAB) &&
             (ch != CARRIAGERETURN) && (ch != NEWLINE) && (ch != COMMENT)) 
      {
         token[i++] = ch;
         ch = fgetc(fp);
      }
   }
   token[i] = '\0';

   // if token ended by NEWLINE, increment lineNo
   if (ch == NEWLINE)
      (*pLineNo)++;

   // if token ended by comment, go ahead and skip comment
   if (ch == COMMENT) 
   {
      while ((ch != NEWLINE) && (ch != (char)EOF))
         ch = fgetc(fp);
      if (ch == NEWLINE)
         (*pLineNo)++;
   }

   return i;
}


//---------------------------------------------------------------------------
// NAME:    AllocateGraph
//
// INPUTS:  (ULONG v) - number of vertices in graph
//          (ULONG e) - number of edges in graph
//
// RETURN:  (SubdueGraph *) - pointer to newly-allocated graph
//
// PURPOSE: Allocate memory for new graph containing v vertices and e
// edges.
//---------------------------------------------------------------------------

SubdueGraph *AllocateGraph(ULONG v, ULONG e)
{
   SubdueGraph *graph;

   graph = (SubdueGraph *) malloc(sizeof(SubdueGraph));
   if (graph == NULL)
      OutOfMemoryError("AllocateGraph:graph");

   graph->numVertices = v;
   graph->numEdges = e;
   graph->vertices = NULL;
   graph->edges = NULL;
   if (v > 0) 
   {
      graph->vertices = (Vertex *) malloc(sizeof(Vertex) * v);
      if (graph->vertices == NULL)
         OutOfMemoryError("AllocateGraph:graph->vertices");
   }
   if (e > 0) 
   {
      graph->edges = (Edge *) malloc(sizeof(Edge) * e);
      if (graph->edges == NULL)
         OutOfMemoryError("AllocateGraph:graph->edges");
   }

   return graph;
}

//---------------------------------------------------------------------------
// NAME:    CopyGraph
//
// INPUTS:  (SubdueGraph *g) - graph to be copied
//
// RETURN:  (SubdueGraph *) - pointer to copy of graph
//
// PURPOSE: Create and return a copy of the given graph.
//---------------------------------------------------------------------------

SubdueGraph *CopyGraph(SubdueGraph *g)
{
   SubdueGraph *gCopy;
   ULONG nv;
   ULONG ne;
   ULONG v;
   ULONG e;
   ULONG numEdges;

   nv = g->numVertices;
   ne = g->numEdges;

   // allocate graph
   gCopy = AllocateGraph(nv, ne);

   // copy vertices; allocate and copy vertex edge arrays
   for (v = 0; v < nv; v++) 
   {
      gCopy->vertices[v].label = g->vertices[v].label;
      gCopy->vertices[v].map = g->vertices[v].map;
      gCopy->vertices[v].used = g->vertices[v].used;
      numEdges = g->vertices[v].numEdges;
      gCopy->vertices[v].numEdges = numEdges;
      gCopy->vertices[v].edges = NULL;
      if (numEdges > 0) 
      {
          gCopy->vertices[v].edges = (ULONG *) malloc(numEdges * sizeof(ULONG));
          if (gCopy->vertices[v].edges == NULL)
             OutOfMemoryError("CopyGraph:edges");
          for (e = 0; e < numEdges; e++)
             gCopy->vertices[v].edges[e] = g->vertices[v].edges[e];
      }
   }

   // copy edges
   for (e = 0; e < ne; e++) 
   {
      gCopy->edges[e].vertex1 = g->edges[e].vertex1;
      gCopy->edges[e].vertex2 = g->edges[e].vertex2;
      gCopy->edges[e].label = g->edges[e].label;
      gCopy->edges[e].directed = g->edges[e].directed;
      gCopy->edges[e].used = g->edges[e].used;
   }

   return gCopy;
}


//---------------------------------------------------------------------------
// NAME:    FreeGraph
//
// INPUTS:  (SubdueGraph *graph) - graph to be freed
//
// RETURN:  void
//
// PURPOSE: Free memory used by given graph, including the vertices array
// and the edges array for each vertex.
//---------------------------------------------------------------------------

void FreeGraph(SubdueGraph *graph)
{
   ULONG v;

   if (graph != NULL) 
   {
      for (v = 0; v < graph->numVertices; v++)
         free(graph->vertices[v].edges);
      free(graph->edges);
      free(graph->vertices);
      free(graph);
   }
}


//---------------------------------------------------------------------------
// NAME:    PrintGraph
//
// INPUTS:  (SubdueGraph *graph) - graph to be printed
//          (LabelList *labelList) - indexed list of vertex and edge labels
//
// RETURN:  void
//
// PURPOSE: Print the vertices and edges of the graph to stdout.
//---------------------------------------------------------------------------

void PrintGraph(SubdueGraph *graph, LabelList *labelList)
{
   ULONG v;
   ULONG e;

   if (graph != NULL) 
   {
      printf("  Graph(%luv,%lue):\n", graph->numVertices, graph->numEdges);
      // print vertices
      for (v = 0; v < graph->numVertices; v++) 
      {
         printf("    ");
         PrintVertex(graph, v, labelList);
      }
      // print edges
      for (e = 0; e < graph->numEdges; e++) 
      {
         printf("    ");
         PrintEdge(graph, e, labelList);
      }
   }
}


//---------------------------------------------------------------------------
// NAME: PrintVertex
//
// INPUTS: (SubdueGraph *graph) - graph containing vertex
//         (ULONG vertexIndex) - index of vertex to print
//         (LabelList *labelList) - labels in graph
//
// RETURN: (void)
//
// PURPOSE: Print a vertex.
//---------------------------------------------------------------------------

void PrintVertex(SubdueGraph *graph, ULONG vertexIndex, LabelList *labelList)
{
   printf("v %lu ", vertexIndex + 1);
   PrintLabel(graph->vertices[vertexIndex].label, labelList);
   printf(" (%c)\n", graph->vertices[vertexIndex].used);
}


//---------------------------------------------------------------------------
// NAME: PrintEdge
//
// INPUTS: (SubdueGraph *graph) - graph containing edge
//         (ULONG edgeIndex) - index of edge to print
//         (LabelList *labelList) - labels in graph
//
// RETURN: (void)
//
// PURPOSE: Print an edge.
//---------------------------------------------------------------------------

void PrintEdge(SubdueGraph *graph, ULONG edgeIndex, LabelList *labelList)
{
   Edge *edge = & graph->edges[edgeIndex];

   if (edge->directed)
      printf("d");
   else 
      printf("u");
   printf(" %lu %lu ", edge->vertex1 + 1, edge->vertex2 + 1);
   PrintLabel(edge->label, labelList);
   printf(" (%c)\n", edge->used);
}


//---------------------------------------------------------------------------
// NAME:    WriteGraphToFile
//
// INPUTS:  (FILE *outFile) - file stream to write graph
//          (SubdueGraph *graph) - graph to be written
//          (LabelList *labelList) - indexed list of vertex and edge labels
//          (ULONG vOffset) - vertex offset for compressed increments
//          (ULONG start) - beginning of vertex range to print
//          (ULONG finish) - end of vertex range to print
//          (BOOLEAN printPS) - flag indicating output is for predefined sub
//
// RETURN:  void
//
// PURPOSE: Write the vertices and edges of the graph to the given
//          file, prefaced by the SUB_TOKEN defined in subdue.h
//          (when WriteSubToken is TRUE).
//---------------------------------------------------------------------------

void WriteGraphToFile(FILE *outFile, SubdueGraph *graph, LabelList *labelList,
                      ULONG vOffset, ULONG start, ULONG finish, BOOLEAN printPS)
{
   ULONG v;
   ULONG e;
   Edge *edge = NULL;

   if (graph != NULL) 
   {
      if (printPS)
         fprintf(outFile, "%s\n", SUB_TOKEN);
      // write vertices
      for (v = start; v < finish; v++)
      {
         fprintf(outFile, "v %lu ", (v + 1 + vOffset - start));
         WriteLabelToFile(outFile, graph->vertices[v].label, labelList, SUBD_FALSE);
         fprintf(outFile, "\n");
      }
      // write edges
      for (e = 0; e < graph->numEdges; e++)
      {
         edge = &graph->edges[e];
         if ((edge->vertex1 >= start) && (edge->vertex1 < finish))
         {
            if (edge->directed)
               fprintf(outFile, "d");
            else
               fprintf(outFile, "u");
            fprintf(outFile, " %lu %lu ",
               (edge->vertex1 + 1 + vOffset - start),
               (edge->vertex2 + 1 + vOffset - start));
            WriteLabelToFile(outFile, edge->label, labelList, SUBD_FALSE);
            fprintf(outFile, "\n");
         }
      }
      if (printPS)
         fprintf(outFile, "\n");
   }
}

//---------------------------------------------------------------------------
// NAME: OutOfMemoryError
//
// INPUTS: (char *context)
//
// RETURN: (void)
//
// PURPOSE: Print out of memory error with context, and then exit.
//---------------------------------------------------------------------------

void OutOfMemoryError(const char *context)
{
   printf("ERROR: out of memory allocating %s.\n", context);
   exit(1);
}

//---------------------------------------------------------------------------
// NAME: PrintBoolean
//
// INPUTS: (BOOLEAN boolean)
//
// RETURN: (void)
//
// PURPOSE: Print BOOLEAN input as 'TRUE' or 'FALSE'.
//---------------------------------------------------------------------------

void PrintBoolean(BOOLEAN boolean)
{
   if (boolean)
      printf("true\n");
   else 
      printf("false\n");
}


//---------------------------------------------------------------------------
// NAME:    AllocateLabelList
//
// INPUTS:  (void)
//
// RETURN:  (LabelList *)
//
// PURPOSE: Allocate memory for empty label list.
//---------------------------------------------------------------------------

LabelList *AllocateLabelList(void)
{
   LabelList *labelList;
   labelList = (LabelList *) malloc(sizeof(LabelList));
   if (labelList == NULL)
      OutOfMemoryError("AllocateLabelList:labelList");
   labelList->size = 0;
   labelList->numLabels = 0;
   labelList->labels = NULL;
   return labelList;
}


//---------------------------------------------------------------------------
// NAME:    StoreLabel
//
// INPUTS:  (Label *label) - label to be stored (if necessary)
//          (LabelList *labelList) - list to contain label
//
// RETURN:  (ULONG) - label's index in given label list
//
// PURPOSE: Stores the given label, if not already present, in the
// given label list and returns the label's index.  The given label's
// memory can be freed after executing StoreLabel.
//---------------------------------------------------------------------------

ULONG StoreLabel(Label *label, LabelList *labelList)
{
   ULONG labelIndex;
   Label *newLabelList;
   char *stringLabel;

   labelIndex = GetLabelIndex(label, labelList);
   if (labelIndex == labelList->numLabels) 
   { // i.e., label not found
      // make sure there is room for a new label
      if (labelList->size == labelList->numLabels) 
      {
         labelList->size += LIST_SIZE_INC;
         newLabelList = (Label *) realloc(labelList->labels,
                                          (sizeof(Label) * labelList->size));
         if (newLabelList == NULL)
            OutOfMemoryError("StoreLabel:newLabelList");
         labelList->labels = newLabelList;
      }
      // store label
      labelList->labels[labelList->numLabels].labelType = label->labelType;
      switch(label->labelType) 
      {
         case STRING_LABEL:
            stringLabel = (char *) malloc
                          (sizeof(char) * 
                           (strlen(label->labelValue.stringLabel)) + 1);
            if (stringLabel == NULL)
               OutOfMemoryError("StoreLabel:stringLabel");
            strcpy(stringLabel, label->labelValue.stringLabel);
            labelList->labels[labelList->numLabels].labelValue.stringLabel =
               stringLabel;
            break;
         case NUMERIC_LABEL:
            labelList->labels[labelList->numLabels].labelValue.numericLabel =
               label->labelValue.numericLabel;
            break;
         default:
            break;  // error
      }
      labelList->labels[labelList->numLabels].used = SUBD_FALSE;
      labelList->numLabels++;
   }
   return labelIndex;
}


//---------------------------------------------------------------------------
// NAME:    GetLabelIndex
//
// INPUTS:  (Label *label) - label being sought
//          (LabelList *labelList) - list in which to look for label
//
// RETURN: (ULONG) - index of label in label list, or number of labels if
//                   label not found
//
// PURPOSE: Returns the index of the given label in the given label
// list.  If not found, then the index just past the end (i.e., number
// of stored labels) is returned.
//---------------------------------------------------------------------------

ULONG GetLabelIndex(Label *label, LabelList *labelList)
{
   ULONG i = 0;
   ULONG labelIndex = labelList->numLabels;
   BOOLEAN found = SUBD_FALSE;

   while ((i < labelList->numLabels) && (! found)) 
   {
      if (labelList->labels[i].labelType == label->labelType) 
      {
         switch(label->labelType) 
         {
            case STRING_LABEL:
               if (strcmp(labelList->labels[i].labelValue.stringLabel,
                   label->labelValue.stringLabel) == 0) 
               {
                  labelIndex = i;
                  found = SUBD_TRUE;
               }
               break;
            case NUMERIC_LABEL:
               if (labelList->labels[i].labelValue.numericLabel ==
                   label->labelValue.numericLabel) 
               {
                  labelIndex = i;
                  found = SUBD_TRUE;
               }
			   break;
            default:
               break;  // error
         }
      }
      i++;
   }
   return labelIndex;
}


//---------------------------------------------------------------------------
// NAME: SubLabelNumber
//
// INPUTS: (ULONG index) - index of label in labelList
//         (LabelList *labelList) - list of labels
//
// RETURN: (ULONG) - number from substructure label, or zero if 
//                   label is not a valid substructure label
//
// PURPOSE: Checks if label is a valid substructure label of the form
// <SUB_LABEL_STRING>_<#>, where <#> is greater than zero.  If valid,
// then <#> is returned; otherwise, returns zero.
//---------------------------------------------------------------------------

ULONG SubLabelNumber(ULONG index, LabelList *labelList)
{
   char *stringLabel;
   char prefix[TOKEN_LEN];
   char rest[TOKEN_LEN];
   BOOLEAN match;
   int i = 0;
   int labelLength;
   int prefixLength;
   ULONG subNumber;

   match = SUBD_TRUE;
   subNumber = 0;
   if (labelList->labels[index].labelType == STRING_LABEL) 
   { // string label?
      stringLabel = labelList->labels[index].labelValue.stringLabel;
      labelLength = strlen(stringLabel);
      strcpy(prefix, SUB_LABEL_STRING);
      prefixLength = strlen(prefix);
      // check that first part of label matches SUB_LABEL_STRING
      if (labelLength > (prefixLength + 1))
         for (i = 0; ((i < prefixLength) && match); i++)
            if (stringLabel[i] != prefix[i])
               match = SUBD_FALSE;
      if (match && (stringLabel[i] != '_')) // underscore present?
         match = SUBD_FALSE;
      if (match &&                          // rest is a number?
          (sscanf((& stringLabel[i + 1]), "%lu%s", &subNumber, rest) != 1))
         subNumber = 0;
   }
   return subNumber;
}


//---------------------------------------------------------------------------
// NAME: LabelMatchFactor
//
// INPUTS: (ULONG labelIndex1)
//         (ULONG labelIndex2) - indices of labels to match
//         (LabelList *labelList) - list of labels
//
// RETURN: (double) - degree of mis-match [0.0,1.0]
//
// PURPOSE: Return the degree of mis-match between the two labels.  If
// the label indices are equal, then the degree of mis-match is 0.0,
// else 1.0.  Values between 0.0 and 1.0 may be implemented based on
// specifics of labels (e.g., threshold or tolerance matching for
// numeric labels, or semantic distance for string labels).
//---------------------------------------------------------------------------

double LabelMatchFactor(ULONG labelIndex1, ULONG labelIndex2,
          LabelList *labelList)
{
   if (labelIndex1 == labelIndex2)
      return 0.0;

   // ***** May want more elaborate matching scheme here, i.e., the
   // ***** tolerance or threshold matching of numeric labels.

   return 1.0;
}


//---------------------------------------------------------------------------
// NAME:    PrintLabel
//
// INPUTS:  (ULONG index) - index into label list
//          (LabelList *labelList) - list of labels
//
// RETURN:  void
//
// PURPOSE: Print label of given index.
//---------------------------------------------------------------------------

void PrintLabel(ULONG index, LabelList *labelList)
{
   UCHAR labelType;

   labelType = labelList->labels[index].labelType;
   switch(labelType) 
   {
      case STRING_LABEL:
         fprintf(stdout, "%s", labelList->labels[index].labelValue.stringLabel);
         break;
      case NUMERIC_LABEL:
         fprintf(stdout, "%.*g", NUMERIC_OUTPUT_PRECISION,
                 labelList->labels[index].labelValue.numericLabel);
         break;
      default:
         break;
   }
}


//---------------------------------------------------------------------------
// NAME:    PrintLabelList
//
// INPUTS:  (LabelList *labelList) - list of labels
//
// RETURN:  void
//
// PURPOSE: Print labels in given labelList.
//---------------------------------------------------------------------------

void PrintLabelList(LabelList *labelList)
{
   ULONG i;

   printf("Label list:\n");
   for (i = 0; i < labelList->numLabels; i++) 
   {
      printf("  labels[%lu] = ", i);
      PrintLabel(i, labelList);
      printf("\n");
   }
}


//---------------------------------------------------------------------------
// NAME: FreeLabelList
//
// INPUTS: (LabelList *labelList)
//
// RETURN: (void)
//
// PURPOSE: Free memory in labelList.
//---------------------------------------------------------------------------

void FreeLabelList(LabelList *labelList)
{
   free(labelList->labels);
   free(labelList);
}


//---------------------------------------------------------------------------
// NAME: WriteLabelToFile
//
// INPUTS: (FILE *outFile) - file to write to
//         (ULONG index) - index of label in label list
//         (LabelList *labelList) - label list
//         (BOOLEAN suppressQuotes) - if TRUE, then delimiting quotes not
//                                    printed
//
// RETURN: (void)
//
// PURPOSE: Write label labelList[index] to given file.  If suppressQuotes
// is TRUE and the label has delimiting end quotes, then do not print them.
//---------------------------------------------------------------------------

void WriteLabelToFile(FILE *outFile, ULONG index, LabelList *labelList,
             BOOLEAN suppressQuotes)
{
   UCHAR labelType;
   char labelStr[TOKEN_LEN];
   int strLength;
   int i;

   labelType = labelList->labels[index].labelType;
   switch(labelType) 
   {
      case STRING_LABEL:
         strcpy(labelStr, labelList->labels[index].labelValue.stringLabel);
         if (suppressQuotes) 
         { // remove delimiting DOUBLEQUOTES if there
            strLength = strlen(labelStr);
            if ((labelStr[0] == DOUBLEQUOTE) &&
                (labelStr[strLength - 1] == DOUBLEQUOTE)) 
            {
               for (i = 0; i < strLength; i++)
                  labelStr[i] = labelStr[i+1];
               labelStr[strLength - 2] = '\0';
            }
         }
         fprintf(outFile, "%s", labelStr);
         break;
      case NUMERIC_LABEL:
         fprintf(outFile, "%.*g", NUMERIC_OUTPUT_PRECISION,
                 labelList->labels[index].labelValue.numericLabel);
         break;
      default:
         break;
   }
}


//---------------------------------------------------------------------------
// NAME:    main
//
// INPUTS:  (int argc) - number of arguments to program
//          (char **argv) - array of strings of arguments to program
//
// RETURN:  (int) - 0 if all is well
//
// PURPOSE: Main SUBDUE function that processes command-line arguments
//          and initiates discovery.
//---------------------------------------------------------------------------

int subdue_main(int argc, char **argv)
{
   struct tms tmsstart, tmsend;
   clock_t startTime, endTime;
   static long clktck = 0;
   Parameters *parameters;
	int cycles;

   clktck = sysconf(_SC_CLK_TCK);
   startTime = times(&tmsstart);
   printf("SUBDUE %s\n\n", SUBDUE_VERSION);
   parameters = GetParameters(argc, argv);
   
   // Statistics
   printf("%lu positive graphs: %lu vertices, %lu edges\n",
      parameters->numPosEgs, parameters->posGraph->numVertices,
      parameters->posGraph->numEdges);
				
   if (parameters->negGraph != NULL) 
   {
      printf("%lu negative graphs: %lu vertices, %lu edges\n",
          parameters->numNegEgs, parameters->negGraph->numVertices,
          parameters->negGraph->numEdges);
   }
   
   // Print the graph
   //PrintGraph(parameters->posGraph, parameters->labelList);

   // Cycle detector
	cycles = CycleDetector(parameters->posGraph, parameters->labelList);
	printf("Number of cycles: %d\n", cycles);

   // Print the graph
   //PrintGraph(parameters->posGraph, parameters->labelList);


   // Free the memory
   FreeParameters(parameters);
   endTime = times(&tmsend);
   printf("\nSUBDUE done (elapsed CPU time = %7.2f seconds).\n",
          (endTime - startTime) / (double) clktck);
   return 0;
}


//---------------------------------------------------------------------------
// NAME: GetParameters
//
// INPUTS: (int argc) - number of command-line arguments
//         (char *argv[]) - array of command-line argument strings
//
// RETURN: (Parameters *)
//
// PURPOSE: Initialize parameters structure and process command-line
//          options.
//---------------------------------------------------------------------------

Parameters *GetParameters(int argc, char *argv[])
{
   Parameters *parameters;
   int i;
   double doubleArg;
   ULONG ulongArg;
   //BOOLEAN limitSet = SUBD_FALSE;
   FILE *outputFile;

   parameters = (Parameters *) malloc(sizeof(Parameters));
   if (parameters == NULL)
      OutOfMemoryError("parameters");

   // initialize default parameter settings
   parameters->directed = SUBD_TRUE;
   parameters->limit = 0;
   parameters->numBestSubs = 3;
   parameters->beamWidth = 4;
   parameters->valueBased = SUBD_FALSE;
   parameters->prune = SUBD_FALSE;
   strcpy(parameters->outFileName, "none");
   parameters->outputToFile = SUBD_FALSE;
   parameters->outputLevel = 2;
   parameters->allowInstanceOverlap = SUBD_FALSE;
   parameters->threshold = 0.0;
   parameters->evalMethod = EVAL_MDL;
   parameters->iterations = 1;
   strcpy(parameters->psInputFileName, "none");
   parameters->predefinedSubs = SUBD_FALSE;
   parameters->minVertices = 1;
   parameters->maxVertices = 0; // i.e., infinity
   parameters->recursion = SUBD_FALSE;
   parameters->variables = SUBD_FALSE;
   parameters->relations = SUBD_FALSE;
   parameters->incremental = SUBD_FALSE;
   parameters->compress = SUBD_FALSE;

   if (argc < 2)
   {
      fprintf(stderr, "input graph file name must be supplied\n");
      exit(1);
   }

   // process command-line options
   i = 1;
   while (i < (argc - 1))
   {
      if (strcmp(argv[i], "-beam") == 0) 
      {
         i++;
         sscanf(argv[i], "%lu", &ulongArg);
         if (ulongArg == 0) 
         {
            fprintf(stderr, "%s: beam must be greater than zero\n", argv[0]);
            exit(1);
         }
         parameters->beamWidth = ulongArg;
      }
      else if (strcmp(argv[i], "-compress") == 0)
      {
         parameters->compress = SUBD_TRUE;
      }
      else if (strcmp(argv[i], "-eval") == 0) 
      {
         i++;
         sscanf(argv[i], "%lu", &ulongArg);
         if ((ulongArg < 1) || (ulongArg > 3)) 
         {
            fprintf(stderr, "%s: eval must be 1-3\n", argv[0]);
            exit(1);
         }
         parameters->evalMethod = ulongArg;
      } 
      else if (strcmp(argv[i], "-inc") == 0)
      {
         parameters->incremental = SUBD_TRUE;
      }
      else if (strcmp(argv[i], "-iterations") == 0) 
      {
         i++;
         sscanf(argv[i], "%lu", &ulongArg);
         parameters->iterations = ulongArg;
      } 
      else if (strcmp(argv[i], "-limit") == 0) 
      {
         i++;
         sscanf(argv[i], "%lu", &ulongArg);
         if (ulongArg == 0) 
         {
            fprintf(stderr, "%s: limit must be greater than zero\n", argv[0]);
            exit(1);
         }
         parameters->limit = ulongArg;
         //limitSet = SUBD_TRUE;
      }
      else if (strcmp(argv[i], "-maxsize") == 0) 
      {
         i++;
         sscanf(argv[i], "%lu", &ulongArg);
         if (ulongArg == 0) 
         {
            fprintf(stderr, "%s: maxsize must be greater than zero\n", argv[0]);
            exit(1);
         }
         parameters->maxVertices = ulongArg;
      }
      else if (strcmp(argv[i], "-minsize") == 0) 
      {
         i++;
         sscanf(argv[i], "%lu", &ulongArg);
         if (ulongArg == 0) 
         {
            fprintf(stderr, "%s: minsize must be greater than zero\n", argv[0]);
            exit(1);
         }
         parameters->minVertices = ulongArg;
      }
      else if (strcmp(argv[i], "-nsubs") == 0) 
      {
         i++;
         sscanf(argv[i], "%lu", &ulongArg);
         if (ulongArg == 0) 
         {
            fprintf(stderr, "%s: nsubs must be greater than zero\n", argv[0]);
            exit(1);
         }
         parameters->numBestSubs = ulongArg;
      }
      else if (strcmp(argv[i], "-out") == 0) 
      {
         i++;
         strcpy(parameters->outFileName, argv[i]);
         parameters->outputToFile = SUBD_TRUE;
      }
      else if (strcmp(argv[i], "-output") == 0) 
      {
         i++;
         sscanf(argv[i], "%lu", &ulongArg);
         if ((ulongArg < 1) || (ulongArg > 5)) 
         {
            fprintf(stderr, "%s: output must be 1-5\n", argv[0]);
            exit(1);
         }
         parameters->outputLevel = ulongArg;
      }
      else if (strcmp(argv[i], "-overlap") == 0) 
      {
         parameters->allowInstanceOverlap = SUBD_TRUE;
      }
      else if (strcmp(argv[i], "-prune") == 0) 
      {
         parameters->prune = SUBD_TRUE;
      }
      else if (strcmp(argv[i], "-ps") == 0) 
      {
         i++;
         strcpy(parameters->psInputFileName, argv[i]);
         parameters->predefinedSubs = SUBD_TRUE;
      }
      else if (strcmp(argv[i], "-recursion") == 0) 
      {
         parameters->recursion = SUBD_TRUE;
      }
      else if (strcmp(argv[i], "-relations") == 0) 
      {
         parameters->relations = SUBD_TRUE;
         parameters->variables = SUBD_TRUE; // relations must involve variables
      }
      else if (strcmp(argv[i], "-threshold") == 0) 
      {
         i++;
         sscanf(argv[i], "%lf", &doubleArg);
         if ((doubleArg < (double) 0.0) || (doubleArg > (double) 1.0))
         {
            fprintf(stderr, "%s: threshold must be 0.0-1.0\n", argv[0]);
            exit(1);
         }
         parameters->threshold = doubleArg;
      }
      else if (strcmp(argv[i], "-undirected") == 0) 
      {
         parameters->directed = SUBD_FALSE;
      }
      else if (strcmp(argv[i], "-valuebased") == 0) 
      {
         parameters->valueBased = SUBD_TRUE;
      }
      else if (strcmp(argv[i], "-variables") == 0) 
      {
         parameters->variables = SUBD_TRUE;
      }
      else 
      {
         fprintf(stderr, "%s: unknown option %s\n", argv[0], argv[i]);
         exit(1);
      }
      i++;
   }

   if (parameters->iterations == 0)
      parameters->iterations = MAX_UNSIGNED_LONG; // infinity

   // initialize log2Factorial[0..1]
   parameters->log2Factorial = (double *) malloc(2 * sizeof(double));
   if (parameters->log2Factorial == NULL)
      OutOfMemoryError("GetParameters:parameters->log2Factorial");
   parameters->log2FactorialSize = 2;
   parameters->log2Factorial[0] = 0; // lg(0!)
   parameters->log2Factorial[1] = 0; // lg(1!)

   // read graphs from input file
   strcpy(parameters->inputFileName, argv[argc - 1]);
   parameters->labelList = AllocateLabelList();
   parameters->posGraph = NULL;
   parameters->negGraph = NULL;
   parameters->numPosEgs = 0;
   parameters->numNegEgs = 0;
   parameters->posEgsVertexIndices = NULL;
   parameters->negEgsVertexIndices = NULL;

   if (parameters->incremental)
   {
      if (parameters->predefinedSubs)
      {
         fprintf(stderr, "Cannot process predefined examples incrementally");
         exit(1);
      }
  
      if (parameters->evalMethod == EVAL_MDL)
      {
         fprintf(stderr, "Incremental SUBDUE does not support EVAL_MDL, ");
         fprintf(stderr, "switching to EVAL_SIZE\n");
         parameters->evalMethod = EVAL_SIZE;
      }

      if ((parameters->evalMethod == EVAL_SIZE) && (parameters->compress))
      {
         fprintf(stderr, "Incremental SUBDUE does not support compression, ");
         fprintf(stderr, "with EVAL_SIZE, turning compression off\n");
         parameters->compress = SUBD_FALSE;
      }

      if (parameters->iterations > 1)
      {
         fprintf(stderr,
            "Incremental SUBDUE only one iteration, setting to 1\n");
         parameters->iterations = 1;
      }
   }
   else 
   {
      ReadInputFile(parameters);
      if (parameters->evalMethod == EVAL_MDL)
      {
         parameters->posGraphDL = 0;
         if (parameters->negGraph != NULL)
         {
            parameters->negGraphDL = 0;
         }
      }
   }

   // read predefined substructures
   parameters->numPreSubs = 0;
   if (parameters->predefinedSubs)
      ReadPredefinedSubsFile(parameters);

   parameters->incrementList = (IncrementList*)malloc(sizeof(IncrementList));
   parameters->incrementList->head = NULL;

   if (parameters->incremental)
   {
      parameters->vertexList = (InstanceVertexList*)malloc(sizeof(InstanceVertexList));
      parameters->vertexList->avlTreeList = (AvlTreeList*)malloc(sizeof(AvlTreeList));
      parameters->vertexList->avlTreeList->head = NULL;
   }

   // create output file, if given
   if (parameters->outputToFile) 
   {
      outputFile = fopen(parameters->outFileName, "w");
      if (outputFile == NULL) 
      {
         printf("ERROR: unable to write to output file %s\n",
                parameters->outFileName);
         exit(1);
      }
      fclose(outputFile);
   }  

   return parameters;
}


//---------------------------------------------------------------------------
// NAME: PrintParameters
//
// INPUTS: (Parameters *parameters)
//
// RETURN: (void)
//
// PURPOSE: Print selected parameters.
//---------------------------------------------------------------------------

void PrintParameters(Parameters *parameters)
{
   printf("Parameters:\n");
   printf("  Input file..................... %s\n",parameters->inputFileName);
   printf("  Predefined substructure file... %s\n",parameters->psInputFileName);
   printf("  Output file.................... %s\n",parameters->outFileName);
   printf("  Beam width..................... %lu\n",parameters->beamWidth);
   printf("  Compress....................... ");
   PrintBoolean(parameters->compress);
   printf("  Evaluation method.............. ");
   switch(parameters->evalMethod) 
   {
      case 1: printf("MDL\n"); break;
      case 2: printf("size\n"); break;
      case 3: printf("setcover\n"); break;
   }
   printf("  'e' edges directed............. ");
   PrintBoolean(parameters->directed);
   printf("  Incremental.................... ");
   PrintBoolean(parameters->incremental);
   printf("  Iterations..................... ");
   if (parameters->iterations == 0)
      printf("infinite\n");
   else 
      printf("%lu\n", parameters->iterations);
   printf("  Limit.......................... %lu\n", parameters->limit);
   printf("  Minimum size of substructures.. %lu\n", parameters->minVertices);
   printf("  Maximum size of substructures.. %lu\n", parameters->maxVertices);
   printf("  Number of best substructures... %lu\n", parameters->numBestSubs);
   printf("  Output level................... %lu\n", parameters->outputLevel);
   printf("  Allow overlapping instances.... ");
   PrintBoolean(parameters->allowInstanceOverlap);
   printf("  Prune.......................... ");
   PrintBoolean(parameters->prune);
   printf("  Threshold...................... %lf\n", parameters->threshold);
   printf("  Value-based queue.............. ");
   PrintBoolean(parameters->valueBased);
   printf("  Recursion...................... ");
   PrintBoolean(parameters->recursion);
   printf("\n");

   printf("Read %lu total positive graphs\n", parameters->numPosEgs);
   if (parameters->numNegEgs > 0) 
      printf("Read %lu total negative graphs\n", parameters->numNegEgs);
   if (parameters->numPreSubs > 0)
      printf("Read %lu predefined substructures\n", parameters->numPreSubs);
   printf("\n");
}


//---------------------------------------------------------------------------
// NAME: FreeParameters
//
// INPUTS: (Parameters *parameters)
//
// RETURN: (void)
//
// PURPOSE: Free memory allocated for parameters.  Note that the
//          predefined substructures are de-allocated as soon as they are
//          processed, and not here.
//---------------------------------------------------------------------------

void FreeParameters(Parameters *parameters)
{
   FreeGraph(parameters->posGraph);
   FreeGraph(parameters->negGraph);
   FreeLabelList(parameters->labelList);
   free(parameters->posEgsVertexIndices);
   free(parameters->negEgsVertexIndices);
   free(parameters->log2Factorial);
   free(parameters);
}


//---------------------------------------------------------------------------
// NAME: CycleDetector
//
// INPUTS: (SubdueGraph *graph)
//
// RETURN: (int) - The number of cycles
//
// PURPOSE: Return the number of cycles in the given graph.
//
//          Algorithm: loop/cycle detector
//          
//            Put all the nodes in White (W)
//            Put one node (any) in Grey (G)
//            While a grey node S exists, do
//              For each link (S,S'), do
//                If Color(S')=G
//                  PRINT("Problem with S")
//                  Color(S')=B
//                Else If Color(S')=W
//                  Color(S')=G
//                End If
//              End For
//              Color(S)=B
//            End While
//            
//          Return the minimum number of loop detected (that is NOT the real number of cycles, but depend on
//          how they are crossed by the algorithm. If 0, there is not loop in the graph.
//---------------------------------------------------------------------------

int CycleDetector(SubdueGraph *graph, LabelList *labelList)
{
	ULONG v, noden, node2n;
	ULONG edgen;
	Vertex *node, *node2;
	Edge *edge;
	int done = 0, cycle = 0;

   for (v = 0; v < graph->numVertices; v++) {
		node = & graph->vertices[v];
		node->used = 'W';
	}
	node = & graph->vertices[0];
	node->used = 'G';
	noden = 0;
	
	while(done!=1) {
		done = 1;
		// Look for a grey node S
		for (v = 0; v < graph->numVertices; v++) {
			node = & graph->vertices[v];
			if(node->used == 'G') {
				noden = v;
				done = 0;
				break;
			}
		}
		if(done==1) {
			// Look for a white node S
			for (v = 0; v < graph->numVertices; v++) {
				node = & graph->vertices[v];
				if(node->used == 'W') {
					node->used = 'G';
					noden = v;
					done = 0;
					break;
				}
			}
		}
		
		// Look for each edge S-S'
		for (v = 0; v < node->numEdges; v++) {
			edgen = node->edges[v];
			edge = & graph->edges[edgen];
			if(edge->vertex1 == noden)
				node2n = edge->vertex2;	// S'
			else
				node2n = edge->vertex1;	// S'
			node2 = & graph->vertices[node2n];
			if(node2->used == 'G') {
				// A cycle has been detected between 'S' and 'S''
				printf("Cycle detected: ");
				PrintEdge(graph, edgen, labelList);
				node2->used = 'B';
				cycle++;
			} if(node2->used == 'W') {
				node2->used = 'G';
			}
		}
		node->used = 'B';
	}
	
	return cycle;
}


// Return a String without \"
void clear_quotes(char* src, char* dst) {
	while(*src != 0) {
		if(*src != '\"') {
			*dst = *src;
			dst++;
		}
		src++;
	}
	*dst = 0;
}

/**************************************************\
Open a graph DB in the Subdue format.
Notes:
- Manage properly multiple graphs in the DB.
- Does not keep the order of the graphs (read the
  positive graphs then the negative graphs).
\*************************************************/
vector<Graph*> open_subdue_file(GLOBAL* gP, char* filename) {

	int argc;
	char* argv[2];
   Parameters *parameters;
   Graph* G;
   vector<Graph*> DB;
   int index;
   char buffer[100];
   char buffer2[100];
   UCHAR labelType;
   unsigned int i, ng;
	
	argc = 2;
	argv[0] = strdup("checkdb");
	argv[1] = strdup(filename);

   parameters = GetParameters(argc, argv);
   
   // Statistics
   //printf("%lu positive graphs: %lu vertices, %lu edges\n",
   //   parameters->numPosEgs, parameters->posGraph->numVertices,
   //   parameters->posGraph->numEdges);
				
   //if (parameters->negGraph != NULL) 
   //{
   //   printf("%lu negative graphs: %lu vertices, %lu edges\n",
   //       parameters->numNegEgs, parameters->negGraph->numVertices,
   //       parameters->negGraph->numEdges);
   //}
   
   // Print the graph
   //PrintGraph(parameters->posGraph, parameters->labelList);
   
   ////////////////////
   // POSITIVE GRAPHS
   ////////////////////

   // Loop for all the positive graphs
   unsigned int num_pos_graphs = parameters->numPosEgs;
   
   /*printf("[");
   for(i=0; i<num_pos_graphs; i++)
     printf(" %ld", parameters->posEgsVertexIndices[i]);
   printf("]\n");*/
   
   for(ng=0; ng<num_pos_graphs; ng++) {
   
	  // Allocation of a graph
	  G = new Graph(1);
	  G->set_class(1);
  
	  // Total number of nodes
	  int num_nodes = parameters->posGraph->numVertices;
	  
	  // Node start and end
	  unsigned int node_start = parameters->posEgsVertexIndices[ng];
	  unsigned int node_end = ((ng+1)<num_pos_graphs) ? parameters->posEgsVertexIndices[ng+1]-1 : num_nodes-1;
  
	  // Read the name of each node
	  for (unsigned i=node_start; i<=node_end;i++) {
		index = parameters->posGraph->vertices[i].label;
		labelType = parameters->labelList->labels[index].labelType;
		switch(labelType) 
		{
			case STRING_LABEL:
		    	 sprintf(buffer, "%s", parameters->labelList->labels[index].labelValue.stringLabel);
		         break;
		    case NUMERIC_LABEL:
		         sprintf(buffer, "%.*g", NUMERIC_OUTPUT_PRECISION,
		                 parameters->labelList->labels[index].labelValue.numericLabel);
		         break;
		    default:
		         break;
		}
		clear_quotes(buffer, buffer2);
		G->add_node(buffer2);

	  }
	
	  // Total number of edges
	  unsigned int num_edges = parameters->posGraph->numEdges;
  
	  // Read the weights
	  for (i = 0; i < num_edges; i++) 
	  {
	  	Edge *edge = & parameters->posGraph->edges[i];
		if((edge->vertex1<node_start) || (edge->vertex2<node_start))
			continue;
		if((edge->vertex1>node_end) || (edge->vertex2>node_end))
			break;
						
		index = edge->label;
	  	labelType = parameters->labelList->labels[index].labelType;
	  	switch(labelType) 
		{
	  	  	case STRING_LABEL:
	  	  	  	sprintf(buffer, "%s", parameters->labelList->labels[index].labelValue.stringLabel);
	  	  	  	break;
	  	  	case NUMERIC_LABEL:
	  	  	  	sprintf(buffer, "%.*g", NUMERIC_OUTPUT_PRECISION,
	  	  	  	  	parameters->labelList->labels[index].labelValue.numericLabel);
	  	  	  	break;
	  	  	default:
	  	  	  	break;
	  	}
		clear_quotes(buffer, buffer2);
		int offset = node_start;
		G->add_edge(edge->vertex1-offset, edge->vertex2-offset, 1.0, buffer2, edge->directed);
	  }
	  
	  //G->print();
	  
	  DB.push_back(G);
	  
	} // end for(ng)
	  
   ////////////////////
   // NEGATIVE GRAPHS
   ////////////////////

   // Loop for all the negative graphs
   unsigned int num_neg_graphs = parameters->numNegEgs;

   for(ng=0; ng<num_neg_graphs; ng++) {

	  // Allocation of a graph
	  G = new Graph(1);
	  G->set_class(0);

	  // Total number of nodes
	  int num_nodes = parameters->negGraph->numVertices;

	  // Node start and end
	  unsigned int node_start = parameters->negEgsVertexIndices[ng];
	  unsigned int node_end = ((ng+1)<num_neg_graphs) ? parameters->negEgsVertexIndices[ng+1]-1 : num_nodes-1;

	  // Read the name of each node
	  for (i=node_start; i<=node_end;i++) {
		index = parameters->negGraph->vertices[i].label;
		labelType = parameters->labelList->labels[index].labelType;
		switch(labelType)
		{
			case STRING_LABEL:
		    	 sprintf(buffer, "%s", parameters->labelList->labels[index].labelValue.stringLabel);
		         break;
		    case NUMERIC_LABEL:
		         sprintf(buffer, "%.*g", NUMERIC_OUTPUT_PRECISION,
		                 parameters->labelList->labels[index].labelValue.numericLabel);
		         break;
		    default:
		         break;
		}
		clear_quotes(buffer, buffer2);
		G->add_node(buffer2);

	  }

	  // Total number of edges
	  unsigned int num_edges = parameters->negGraph->numEdges;

	  // Read the weights
	  for (i = 0; i < num_edges; i++)
	  {
	  	Edge *edge = & parameters->negGraph->edges[i];
		if((edge->vertex1<node_start) || (edge->vertex2<node_start))
			continue;
		if((edge->vertex1>node_end) || (edge->vertex2>node_end))
			break;

		index = edge->label;
	  	labelType = parameters->labelList->labels[index].labelType;
	  	switch(labelType)
		{
	  	  	case STRING_LABEL:
	  	  	  	sprintf(buffer, "%s", parameters->labelList->labels[index].labelValue.stringLabel);
	  	  	  	break;
	  	  	case NUMERIC_LABEL:
	  	  	  	sprintf(buffer, "%.*g", NUMERIC_OUTPUT_PRECISION,
	  	  	  	  	parameters->labelList->labels[index].labelValue.numericLabel);
	  	  	  	break;
	  	  	default:
	  	  	  	break;
	  	}
		clear_quotes(buffer, buffer2);
		int offset = node_start;
		G->add_edge(edge->vertex1-offset, edge->vertex2-offset, 1.0, buffer2, edge->directed);
	  }

	  //G->print();

	  DB.push_back(G);

	} // end for(ng)

  // Free
   free(argv[0]);
   free(argv[1]);

	return DB;
}
