/** \file dot.h
 ** \brief Defines DirectedGraph and declares its member functions.
 **/
#ifndef DOT_H
	#define DOT_H
	#include <string.h>
	#include "debug.h"

	#ifndef DOT_DEFAULT_SHAPE
		#define DOT_DEFAULT_SHAPE "ellipse"
	#endif
	#ifndef DOT_DEFAULT_STYLE
		#define DOT_DEFAULT_STYLE ""
	#endif
	#ifndef DOT_DEFAULT_PERIPHERIES
		#define DOT_DEFAULT_PERIPHERIES 1
	#endif
	#ifndef DOT_DEFAULT_NAME
		#define DOT_DEFAULT_NAME "G"
	#endif
	#ifndef DOT_DEFAULT_NODE_NAME
		#define DOT_DEFAULT_NODE_NAME(nid) "%c%u", DOT_NODE_SYMBOL, nid
	#endif
	#ifndef DOT_DEFAULT_NODE_LABEL
		#define DOT_DEFAULT_NODE_LABEL(nid) DOT_DEFAULT_NODE_NAME(nid)
	#endif
	#ifndef DOT_DEFAULT_CLUSTER_LABEL
		#define DOT_DEFAULT_CLUSTER_LABEL(cluster) "%u", cluster->id
	#endif
	#ifndef DOT_DEFAULT_EDGE_LABEL
		#define DOT_DEFAULT_EDGE_LABEL(edge) ""
	#endif
	#ifndef DOT_MAX_SHAPE_SIZE
		#define DOT_MAX_SHAPE_SIZE 10
	#endif
	#ifndef DOT_MAX_STYLE_SIZE
		#define DOT_MAX_STYLE_SIZE 10
	#endif
	#ifndef DOT_MAX_LABEL_SIZE
		#define DOT_MAX_LABEL_SIZE 300
	#endif
	#ifndef DOT_MAX_NAME_SIZE
		#define DOT_MAX_NAME_SIZE 30
	#endif
	#ifndef DOT_MAX_NODES
		#define DOT_MAX_NODES 1500
	#endif
	#ifndef DOT_MAX_CLUSTER_NODES
		#define DOT_MAX_CLUSTER_NODES 100
	#endif
	#ifndef DOT_MAX_EDGES
		#define DOT_MAX_EDGES 100
	#endif
	#ifndef DOT_MAX_CLUSTERS
		#define DOT_MAX_CLUSTERS 20
	#endif
	#ifndef DOT_MAX_CLUSTER_CHILDREN
		#define DOT_MAX_CLUSTER_CHILDREN 4
	#endif
	#ifndef DOT_MAX_CLUSTER_SYMBOL
		#define DOT_MAX_CLUSTER_SYMBOL 'c'
	#endif
	#ifndef DOT_NODE_SYMBOL
		#define DOT_NODE_SYMBOL 's'
	#endif
	#ifndef DOT_START_SYMBOL
		#define DOT_START_SYMBOL '{'
	#endif
	#ifndef DOT_END_SYMBOL
		#define DOT_END_SYMBOL '}'
	#endif
	#ifndef DOT_LIST_START_SYMBOL
		#define DOT_LIST_START_SYMBOL '['
	#endif
	#ifndef DOT_LIST_END_SYMBOL
		#define DOT_LIST_END_SYMBOL ']'
	#endif
	#ifndef DOT_QUOTE_SYMBOL
		#define DOT_QUOTE_SYMBOL '"'
	#endif
	#ifndef DOT_STATEMENT_END_SYMBOL
		#define DOT_STATEMENT_END_SYMBOL ';'
	#endif
	#ifndef DOT_EQ_SYMBOL
		#define DOT_EQ_SYMBOL '='
	#endif
	#ifndef DOT_ARROW_SIGN
		#define DOT_ARROW_SIGN "->"
	#endif
	#ifndef DOT_HEADER
		#define DOT_HEADER(G) "digraph %s %c\n\trankdir=LR;\n", G->name, DOT_START_SYMBOL
	#endif
	#ifndef DOT_PROPERTY_LABEL
		#define DOT_PROPERTY_LABEL "label"
	#endif
	#ifndef DOT_PROPERTY_SHAPE
		#define DOT_PROPERTY_SHAPE "shape"
	#endif
	#ifndef DOT_PROPERTY_STYLE
		#define DOT_PROPERTY_STYLE "style"
	#endif
	#ifndef DOT_PROPERTY_PERIPHERIES
		#define DOT_PROPERTY_PERIPHERIES "peripheries"
	#endif
	#ifndef DOT_CLUSTER_HEADER
		#define DOT_CLUSTER_HEADER(cid) \
			"subgraph cluster_%d %c\n",	\
			cid, DOT_START_SYMBOL
	#endif
	#ifndef DOT_CLUSTER_LABEL
		#define DOT_CLUSTER_LABEL(cluster)	\
			"\t%s%c%c%s%c%c\n",				\
			DOT_PROPERTY_LABEL,				\
			DOT_EQ_SYMBOL,					\
			DOT_QUOTE_SYMBOL,				\
			cluster.label,					\
			DOT_QUOTE_SYMBOL,				\
			DOT_STATEMENT_END_SYMBOL
	#endif
	#ifndef DOT_NODE
		#define DOT_NODE(node)										\
			"\t%s %c%s%c%c%s%c %s%c%c%s%c %s%c%c%s%c %s%c%u%c%c\n",	\
			node.name,												\
			DOT_LIST_START_SYMBOL,									\
			DOT_PROPERTY_LABEL,										\
			DOT_EQ_SYMBOL,											\
			DOT_QUOTE_SYMBOL,										\
			node.label,												\
			DOT_QUOTE_SYMBOL,										\
			DOT_PROPERTY_STYLE,										\
			DOT_EQ_SYMBOL,											\
			DOT_QUOTE_SYMBOL,										\
			node.style,												\
			DOT_QUOTE_SYMBOL,										\
			DOT_PROPERTY_SHAPE,										\
			DOT_EQ_SYMBOL,											\
			DOT_QUOTE_SYMBOL,										\
			node.shape,												\
			DOT_QUOTE_SYMBOL,										\
			DOT_PROPERTY_PERIPHERIES,								\
			DOT_EQ_SYMBOL,											\
			node.peripheries,										\
			DOT_LIST_END_SYMBOL,									\
			DOT_STATEMENT_END_SYMBOL
	#endif
	#ifndef DOT_EDGE
		#define DOT_EDGE(from,to,label)		\
			"\t%s%s%s %c%s%c%c%s%c%c%c\n",	\
			from.name,						\
			DOT_ARROW_SIGN,					\
			to.name,						\
			DOT_LIST_START_SYMBOL,			\
			DOT_PROPERTY_LABEL,				\
			DOT_EQ_SYMBOL,					\
			DOT_QUOTE_SYMBOL,				\
			label,							\
			DOT_QUOTE_SYMBOL,				\
			DOT_LIST_END_SYMBOL,			\
			DOT_STATEMENT_END_SYMBOL
	#endif

	typedef int SubGraphId;
	typedef unsigned int NodeId;
	typedef unsigned int EdgeId;

	/** \brief An Edge is a labeled connection between two Node objects.
	 **/
	typedef struct DirectedEdgeBody {
		char label[DOT_MAX_LABEL_SIZE];
		NodeId target;
	} Edge, DirectedEdge;
	#define ASSERT_EDGE(edge)								\
		ASSERT(edge); 										\
		ASSERT(edge->target < DOT_MAX_NODES);				\
		ASSERT(strlen(edge->label) < DOT_MAX_LABEL_SIZE)

	/** \brief A Node is a labeled point in a Graph.
	 **/
	typedef struct NodeBody {
		NodeId id;
		char name[DOT_MAX_NAME_SIZE];
		unsigned int peripheries;
		char style[DOT_MAX_STYLE_SIZE];
		char shape[DOT_MAX_SHAPE_SIZE];
		char label[DOT_MAX_LABEL_SIZE];
		Edge edges[DOT_MAX_EDGES];
		unsigned int nEdges;
	} Node;
	#define ASSERT_NODE(node)									\
		ASSERT(node);											\
		ASSERT(strcmp(node->name,""));							\
		ASSERT(strlen(node->name) < DOT_MAX_NAME_SIZE);			\
		ASSERT(strlen(node->label) < DOT_MAX_LABEL_SIZE);		\
		ASSERT(node->id < DOT_MAX_NODES);						\
		ASSERT(node->nEdges < DOT_MAX_EDGES)

	/** \brief A SubGraph is a cluster in a Graph.
	 **/
	typedef struct SubGraphBody {
		SubGraphId id;
		SubGraphId parentId;
		SubGraphId children[DOT_MAX_CLUSTER_CHILDREN];
		unsigned int nChildren;
		NodeId members[DOT_MAX_CLUSTER_NODES];
		unsigned int size;
		char label[DOT_MAX_LABEL_SIZE];
	} SubGraph;
	#define ASSERT_CLUSTER(cluster)								\
		ASSERT(cluster);										\
		ASSERT(strlen(cluster->label) < DOT_MAX_LABEL_SIZE);	\
		ASSERT(cluster->id < DOT_MAX_CLUSTERS);					\
		ASSERT(cluster->nChildren < DOT_MAX_CLUSTER_CHILDREN)

	/** \brief A Graph is an array of Node objects.
	 **/
	typedef struct DirectedGraphBody {
		char name[DOT_MAX_NAME_SIZE];
		SubGraph clusters[DOT_MAX_CLUSTERS];
		int nClusters;
		Node nodes[DOT_MAX_NODES];
		unsigned int size;
	} Graph, DirectedGraph;
	#define ASSERT_GRAPH(graph) 							\
		ASSERT(graph); 										\
		ASSERT(strcmp(graph->name,""));						\
		ASSERT(strlen(graph->name) < DOT_MAX_NAME_SIZE);	\
		ASSERT(graph->size < DOT_MAX_NODES);				\
		ASSERT(graph->nClusters < DOT_MAX_CLUSTERS)

	char* toLabel_dot(char*, const char*);
	Graph* beautify_dot(Graph*);
	Graph* initialize_dot(Graph*);
	Graph* fromStream_dot(Graph*, FILE*);
	Graph* fromFile_dot(Graph*, const char*);
	SubGraph* insertCluster_dot(Graph*, const SubGraphId);
	Node* insertNode_dot(Graph*, const SubGraphId);
	Edge* insertEdge_dot(Graph*, const NodeId, const NodeId);
	Edge* getEdge_dot(Graph*, const NodeId, const NodeId);
	void toStream_dot(const Graph*, FILE*);
	void toFile_dot(const Graph*, const char*);
#endif
