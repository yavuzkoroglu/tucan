/** \file dot.c
 ** \brief Implements DirectedGraph and its member functions.
 **/
#include "dot.h"
#include "stdioplus.h"
#include "stdlibplus.h"
#include "stringplus.h"
#include "until.h"

DECLARE_SOURCE("DOT");

char* toLabel_dot(char* label, const char* rawlabel)
{
	DECLARE_FUNCTION(private_toLabel_dot);

	/* Variable declarations. */
	const char* from;
	char* to;

	/* Checks. */
	ASSERT(rawlabel);

	unless (label)
		SAFE_MALLOC(label, char, BUFFER_SIZE);

	*label = '\0';
	for (from = rawlabel, to = label; (*from); from++, to += strlen(to)) {
		if (*from == DOT_QUOTE_SYMBOL)
			to = fromPattern(to, DOT_MAX_LABEL_SIZE, "\\%c", DOT_QUOTE_SYMBOL);
		else if (*from == '\t')
			;/* Do not parse special tab characters */
		else if (*from == '\n')
			to = fromPattern(to, DOT_MAX_LABEL_SIZE, "\\l");
		else if (*from == '\\')
			to = fromPattern(to, DOT_MAX_LABEL_SIZE, "\\\\");
		else
			to = fromPattern(to, DOT_MAX_LABEL_SIZE, "%c", *from);
		ASSERT(to);
	}

	ASSERT(strlen(label) < DOT_MAX_LABEL_SIZE);
	return label;
}

Graph* beautify_dot(Graph* G)
{
	DECLARE_FUNCTION(beautify_dot);

	char buffer[BUFFER_SIZE], *tmp = buffer;
	SubGraph* cluster;
	Node* node;
	Edge* edge;

	/* Checks. */
	ASSERT_GRAPH(G);

	for (cluster = G->clusters; cluster < G->clusters + G->nClusters; cluster++)
	{
		tmp = fromPattern(buffer, DOT_MAX_LABEL_SIZE, "%s", cluster->label);
		tmp = toLabel_dot(cluster->label, tmp);
		ASSERT(tmp);
		ASSERT(strlen(cluster->label) < DOT_MAX_LABEL_SIZE);
	}

	for (node = G->nodes; node < G->nodes + G->size; node++)
	{
		tmp = fromPattern(buffer, DOT_MAX_LABEL_SIZE, "%s", node->label);
		tmp = toLabel_dot(node->label, tmp);
		ASSERT(tmp);
		ASSERT(strlen(node->label) < DOT_MAX_LABEL_SIZE);

		for (edge = node->edges; edge < node->edges + node->nEdges; edge++) {
			tmp = fromPattern(buffer, DOT_MAX_LABEL_SIZE, "%s", edge->label);
			tmp = toLabel_dot(edge->label, tmp);
			ASSERT(tmp);
			ASSERT(strlen(edge->label) < DOT_MAX_LABEL_SIZE);
		}
	}

	return G;
}

Graph* initialize_dot(Graph* G)
{
	DECLARE_FUNCTION(initialize_dot);

	char* check;

	unless (G)
		SAFE_CALLOC(G, Graph, 1);

 	/* Start with no nodes and no clusters. */
	G->size = 0;
	G->nClusters = 0;

	/* Initialize the default name. */
	check = fromPattern(G->name, DOT_MAX_NAME_SIZE, DOT_DEFAULT_NAME);
	ASSERT(check);
	ASSERT(strcmp(check, ""));

	/* Return the pointer. */
	return G;
}

SubGraph* insertCluster_dot(Graph* G, const SubGraphId parentId)
{
	DECLARE_FUNCTION(insertCluster_dot);

	/* Variable declarations. */
	SubGraphId clusterId;
	SubGraph* cluster;
	SubGraph* parent;
	char* check;

	/* Checks. */
	ASSERT_GRAPH(G);
	ASSERT(parentId < G->nClusters);

	clusterId = G->nClusters++;
	ASSERT_GRAPH(G);

	/* Initialize cluster. */
	cluster = G->clusters + clusterId;
	cluster->id = clusterId;
	cluster->size = 0;
	cluster->nChildren = 0;

	/* Initialize the default label. */
	check = fromPattern
	(
		cluster->label,
		DOT_MAX_LABEL_SIZE,
		DOT_DEFAULT_CLUSTER_LABEL(cluster)
	);
	ASSERT(check);

	/* Connect the parent. */
	cluster->parentId = parentId;
	unless (parentId < 0) {
		parent = G->clusters + parentId;
		ASSERT_CLUSTER(parent);
		parent->children[parent->nChildren++] = clusterId;
		ASSERT_CLUSTER(parent);
		ASSERT_CLUSTER(cluster);
	}

	return cluster;
}

Node* insertNode_dot(Graph* G, const SubGraphId clusterId)
{
	DECLARE_FUNCTION(insertNode_dot);

	/* Variable declarations. */
	NodeId nid;
	char* check;
	Node* node;
	SubGraph* cluster;

	/* Checks. */
	ASSERT_GRAPH(G);
	ASSERT(clusterId < G->nClusters);

	/* Initialize the node pointer. */
	nid = G->size;
	node = G->nodes + nid;
	node->id = nid;

	/* Set the default perimeter. */
	node->peripheries = DOT_DEFAULT_PERIPHERIES;

	/* Set the default style. */
	check = fromPattern(node->style, DOT_MAX_STYLE_SIZE, "%s", DOT_DEFAULT_STYLE);
	ASSERT(check);

	/* Set the default shape. */
	check = fromPattern(node->shape, DOT_MAX_SHAPE_SIZE, "%s", DOT_DEFAULT_SHAPE);
	ASSERT(check);
	ASSERT(strcmp(check,""));

	/* Initialize the default name. */
	check = fromPattern
	(
		node->name,
		DOT_MAX_NAME_SIZE,
		DOT_DEFAULT_NODE_NAME(nid)
	);
	ASSERT(check);

	/* Initialize the default label. */
	check = fromPattern
	(
		node->label,
		DOT_MAX_LABEL_SIZE,
		DOT_DEFAULT_NODE_LABEL(nid)
	);
	ASSERT(check);

	/* Initialize the number of edges in Node. */
	node->nEdges = 0;

	/* Register the node to the given cluster. */
	unless (clusterId < 0) {
		cluster = G->clusters + clusterId;
		ASSERT_CLUSTER(cluster);
		cluster->members[cluster->size++] = nid;
		ASSERT_CLUSTER(cluster);
	}

	/* Increment the Graph size. */
	G->size++;

	/* Modified stuff: The Graph and the Node. */
	ASSERT_GRAPH(G);
	ASSERT_NODE(node);

	/* Return a pointer to the Node. */
	return node;
}

Edge* insertEdge_dot(Graph* G, const NodeId from, const NodeId to)
{
	DECLARE_FUNCTION(insertEdge_dot);

	/* Variable declarations. */
	char* check;
	Node* source;
	Node* sink;
	Edge* edge;

	/* Checks. */
	ASSERT_GRAPH(G);
	ASSERT(from < G->size);
	ASSERT(to < G->size);

	/* Initialize the source pointer. */
	source = G->nodes + from;
	ASSERT_NODE(source);

	/* Initialize the sink pointer. */
	sink = G->nodes + to;
	ASSERT_NODE(sink);

	/* Initialize the edge pointer. */
	edge = source->edges + source->nEdges;
	ASSERT_EDGE(edge);

	/* Initialize the label. */
	check = fromPattern
	(
		edge->label,
		DOT_MAX_LABEL_SIZE,
		DOT_DEFAULT_EDGE_LABEL(edge)
	);
	ASSERT(check);

	/* Set the target. */
	edge->target = to;

	/* Increment the number of edges. */
	source->nEdges++;

	/* Modified stuff: The source Node and the Edge. */
	ASSERT_NODE(source);
	ASSERT_EDGE(edge);

	/* Return a pointer to the Edge. */
	return edge;
}

Edge* getEdge_dot(Graph* G, const NodeId sourceId, const NodeId sinkId)
{
	DECLARE_FUNCTION(getEdge_dot);

	Node* from;
	const Node* to;
	Edge* edge;

	ASSERT_GRAPH(G);

	unless (sourceId < G->size && sinkId < G->size)
		return NULL;

	from = G->nodes + sourceId;
	ASSERT_NODE(from);
	to = G->nodes + sinkId;
	ASSERT_NODE(to);

	for (edge = from->edges; edge < from->edges + from->nEdges; edge++)
	{
		ASSERT_EDGE(edge);
		if (edge->target == sinkId)
			return edge;
	}

	return NULL;
}

void toStream_dot(const Graph* G, FILE* stream)
{
	DECLARE_FUNCTION(toStream_dot);

	/* Variable declarations. */
	SubGraphId cid, rootcid, pid;
	SubGraphId stack[DOT_MAX_CLUSTERS];
	NodeId nid;
	EdgeId eid;
	unsigned int size, i;
	char isNodeProcessed[DOT_MAX_NODES] = {0};

	/* Checks. */
	ASSERT_GRAPH(G);
	ASSERT(stream);

	/* Write the header and put the start symbol. */
	fprintf(stream, DOT_HEADER(G));

	for (rootcid = 0; rootcid < G->nClusters; rootcid++)
	{
		/* Must be a root cluster. */
		unless (G->clusters[rootcid].parentId < 0)
			continue;

		size = 0;
		stack[size++] = rootcid;
		while (size) {
			cid = stack[--size];
			ASSERT(cid < G->nClusters);

			/* Print appropriate tabs. */
			for (pid = cid; pid >= 0; pid = G->clusters[pid].parentId)
				fprintf(stream, "%c", '\t');
			fprintf(stream, DOT_CLUSTER_HEADER(cid));

			/* Print appropriate tabs. */
			for (pid = cid; pid >= 0; pid = G->clusters[pid].parentId)
				fprintf(stream, "%c", '\t');
			fprintf(stream, DOT_CLUSTER_LABEL(G->clusters[cid]));

			/* Declare member nodes. */
			for (i = 0; i < G->clusters[cid].size; i++)
			{
				nid = G->clusters[cid].members[i];
				ASSERT(nid < G->size);
				isNodeProcessed[nid] = 1;

				/* Print appropriate tabs. */
				for (pid = cid; pid >= 0; pid = G->clusters[pid].parentId)
					fprintf(stream, "%c", '\t');
				fprintf(stream, DOT_NODE(G->nodes[nid]));
			}

			/* Push children to stack. */
			for (i = G->clusters[cid].nChildren; i > 0;)
				stack[size++] = G->clusters[cid].children[--i];

			/* Continue if this cluster has children. */
			if (G->clusters[cid].nChildren)
				continue;

			/* Print appropriate tabs. */
			for (pid = cid; pid >= 0; pid = G->clusters[pid].parentId)
				fprintf(stream, "%c", '\t');
			fprintf(stream, "%c\n", DOT_END_SYMBOL);

			pid = G->clusters[cid].parentId;
			until (pid < 0 || G->clusters[pid].children[G->clusters[pid].nChildren - 1] != cid) {
				/* The parent exists and the child is the last child. */
				cid = pid;

				/* Print appropriate tabs. */
				for (; pid >= 0; pid = G->clusters[pid].parentId)
					fprintf(stream, "%c", '\t');
				fprintf(stream, "%c\n", DOT_END_SYMBOL);
			}
		}
	}

	/* Declare every non-member. */
	for (nid = 0; nid < G->size; nid++)
	{
		/* The node must not be processed before. */
		if (isNodeProcessed[nid])
			continue;

		fprintf(stream, DOT_NODE(G->nodes[nid]));
	}

	/* Declare every edge. */
	for (nid = 0; nid < G->size; nid++) {
		for (eid = 0; eid < G->nodes[nid].nEdges; eid++) {
			fprintf(
				stream,
				DOT_EDGE(
					G->nodes[nid],
					G->nodes[G->nodes[nid].edges[eid].target],
					G->nodes[nid].edges[eid].label
				)
			);
		}
	}

	/* Put the end symbol. */
	fprintf(stream, "%c\n", DOT_END_SYMBOL);

	/* Flush the stream. */
	fflush(stream);
}

void toFile_dot(const Graph* G, const char* filename)
{
	DECLARE_FUNCTION(toFile_dot);

	/* Variable declaration. */
	FILE* fp;

	/* Checks. */
	ASSERT_GRAPH(G);
	ASSERT(filename);
	ASSERT(strcmp(filename, ""));

	/* Open the file for writing. */
	SAFE_FOPEN(fp, filename, "w");

	/* Write the graph to the file stream. */
	toStream_dot(G, fp);

	/* Close the file. */
	fclose(fp);
}
