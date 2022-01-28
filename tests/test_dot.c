#include "dot.h"
#include "repeat.h"

DECLARE_SOURCE("TEST_DOT");

int main(int argc, char* argv[]) {
	DECLARE_FUNCTION(main);

	/* Variable declarations. */
	const char* outputFile;
	NodeId i;
	Graph* G;

	start_logging();

	/* Initialize the Graph. */
	G = initialize_dot(NULL);
	ASSERT_GRAPH(G);
	say(MSG_REPORT_VAR("sizeof(Graph)","%luK",sizeof(Graph)/1024));

	say(MSG_REPORT("Inserting nodes..."));
	repeat4(insertNode_dot(G, -1);)

	say(MSG_REPORT("Inserting edges..."));
	insertEdge_dot(G, 0, 1);
	insertEdge_dot(G, 0, 2);
	insertEdge_dot(G, 1, 2);
	insertEdge_dot(G, 1, 3);

	say(MSG_REPORT("Inserting loops..."));
	for (i = 0; i < G->size; i++)
		insertEdge_dot(G, i, i);

	if (argc > 1) {
		outputFile = argv[1];
	} else {
		outputFile = "tests/deneme.dot";
	}
	ASSERT(outputFile);
	ASSERT(strcmp(outputFile,""));
	ASSERT(strlen(outputFile) < BUFFER_SIZE);
	say(MSG_REPORT_VAR("Writing to File", "%s", outputFile));
	toFile_dot(G, outputFile);

	stop_logging();

	return 0;
}

