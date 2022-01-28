#include "xml.h"

DECLARE_SOURCE("TEST_XML");

int main(int argc, char* argv[]) {
	DECLARE_FUNCTION(main);

	/* Variable declaration. */
	const char* inputFile;
	char buffer[BUFFER_LARGE_SIZE], *str = buffer;
	Xml* xml;
	XmlNode* node;
	XmlAttribute* attribute;

	start_logging();

	if (argc > 1) {
		inputFile = argv[1];
	} else {
		inputFile = "tests/example4.xml";
	}
	ASSERT(inputFile);
	ASSERT(strcmp(inputFile,""));
	ASSERT(strlen(inputFile) < BUFFER_SIZE);

	say(MSG_REPORT_VAR("sizeof(Xml)","%luK",sizeof(Xml)/1024));

	say(MSG_REPORT_VAR("XML File", "%s", inputFile));
	xml = fromFile_xml(NULL, inputFile);
	ASSERT_XML(xml);

	for (
		node = xml->tree->nodes + 1;
		node < xml->tree->nodes + xml->tree->size;
		node++
	) {
		ASSERT_XMLNODE(node);

		attribute = findAttr_xmln(node, "clickable");
		unless (attribute)
			continue;
		ASSERT_XMLATTRIBUTE(attribute);
		if (!strcmp(attribute->value, "true")) {
			say(MSG_REPORT_VAR("Clickable TAG", "%s", node->tag));

			str = toXPath_xmln(str, node);
			say(MSG_REPORT_VAR("Clickable XPATH", "%s", str));
		}
	}

	stop_logging();

	free(xml);

	return 0;
}

