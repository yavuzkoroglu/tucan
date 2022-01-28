/** \file xml.c
 ** \brief Implements all the member functions of Xml.
 **/
#include <ctype.h>
#include "until.h"
#include "xml.h"

DECLARE_SOURCE("XML");

char* private_toXmlString(char* xmlstr, const char* str)
{
	DECLARE_FUNCTION(private_toXmlString);

	/* Variable declarations. */
	unsigned int len;
	const char* ptr;
	char* xmlptr;

	/* Check. */
	ASSERT(str);

	unless (xmlstr)
		SAFE_MALLOC(xmlstr, char, BUFFER_SIZE);
	xmlstr[0] = '\0';

	for (ptr = str, xmlptr = xmlstr; (*ptr); ptr++, xmlptr += strlen(xmlptr))
	{
		if (*ptr == XML_NODE_BEGIN_SYMBOL)
			xmlptr = fromPattern(xmlptr, BUFFER_SIZE, "%s", XML_LT);
		else if (*ptr == XML_NODE_END_SYMBOL)
			xmlptr = fromPattern(xmlptr, BUFFER_SIZE, "%s", XML_GT);
		else if (*ptr == XML_QUOTE_SYMBOL)
			xmlptr = fromPattern(xmlptr, BUFFER_SIZE, "%s", XML_QUOTE);
		else if (*ptr == XML_APOSTROPHE_SYMBOL)
			xmlptr = fromPattern(xmlptr, BUFFER_SIZE, "%s", XML_APOSTROPHE);
		else if (*ptr == XML_AMP_SYMBOL)
			xmlptr = fromPattern(xmlptr, BUFFER_SIZE, "%s", XML_AMP);
		else
			xmlptr = fromPattern(xmlptr, BUFFER_SIZE, "%c", *ptr);
		ASSERT(xmlptr);
		ASSERT(strcmp(xmlptr,""));
	}

	len = xmlptr - xmlstr;
	ASSERT(len < BUFFER_SIZE);

	return xmlstr;
}

char* private_fromXmlString(char* str, const char* xmlstr)
{
	DECLARE_FUNCTION(private_fromXmlString);

	/* Variable declarations. */
	unsigned int len;
	unsigned int xml_lt_len;
	unsigned int xml_gt_len;
	unsigned int xml_quote_len;
	unsigned int xml_apostrophe_len;
	unsigned int xml_amp_len;
	char* ptr;
	const char* xmlptr;

	/* Checks. */
	ASSERT(xmlstr);
	ASSERT(strlen(xmlstr) < BUFFER_SIZE);

	unless (str)
		SAFE_MALLOC(str, char, BUFFER_SIZE);
	str[0] = '\0';

	xml_lt_len = strlen(XML_LT);
	xml_gt_len = strlen(XML_GT);
	xml_quote_len = strlen(XML_QUOTE);
	xml_apostrophe_len = strlen(XML_APOSTROPHE);
	xml_amp_len = strlen(XML_AMP);
	for (xmlptr = xmlstr, ptr = str; (*xmlptr); ptr++)
	{
		if (!strncmp(xmlptr, XML_LT, xml_lt_len)) {
			ptr = fromPattern(ptr, BUFFER_SIZE, "%c", XML_NODE_BEGIN_SYMBOL);
			xmlptr += xml_lt_len;
		} else if (!strncmp(xmlptr, XML_GT, xml_gt_len)) {
			ptr = fromPattern(ptr, BUFFER_SIZE, "%c", XML_NODE_END_SYMBOL);
			xmlptr += xml_gt_len;
		} else if (!strncmp(xmlptr, XML_QUOTE, xml_quote_len)) {
			ptr = fromPattern(ptr, BUFFER_SIZE, "%c", XML_QUOTE_SYMBOL);
			xmlptr += xml_quote_len;
		} else if (!strncmp(xmlptr, XML_APOSTROPHE, xml_apostrophe_len)) {
			ptr = fromPattern(ptr, BUFFER_SIZE, "%c", XML_APOSTROPHE_SYMBOL);
			xmlptr += xml_apostrophe_len;
		} else if (!strncmp(xmlptr, XML_AMP, xml_amp_len)) {
			ptr = fromPattern(ptr, BUFFER_SIZE, "%c", XML_AMP_SYMBOL);
			xmlptr += xml_amp_len;
		} else {
			ptr = fromPattern(ptr, BUFFER_SIZE, "%c", *(xmlptr++));
		}
		ASSERT(ptr);
		ASSERT(strcmp(ptr,""));
	}

	len = ptr - str;
	ASSERT(len < BUFFER_SIZE);

	return str;
}

XmlAttribute* initialize_xattr(XmlAttribute* attribute, const char* name, const char* value)
{
	DECLARE_FUNCTION(initialize_xattr);

	/* Variable declaration. */
	char* check;

	/* Checks. */
	ASSERT(name);
	ASSERT(strcmp(name,""));
	ASSERT(strlen(name) < XML_MAX_ATTRIBUTE_NAME_SIZE);
	ASSERT(value);
	ASSERT(strcmp(value,""));
	ASSERT(strlen(value) < BUFFER_SIZE);

	unless (attribute)
		SAFE_MALLOC(attribute, XmlAttribute, 1);

	check = fromPattern(
		attribute->name,
		XML_MAX_ATTRIBUTE_NAME_SIZE,
		"%s",
		name
	);
	ASSERT(check);
	ASSERT(strcmp(check,""));

	check = private_fromXmlString(attribute->value, value);
	ASSERT(check);
	ASSERT(strcmp(check,""));
	ASSERT(strlen(check) < XML_MAX_ATTRIBUTE_VAL_SIZE);

	return attribute;
}

char* toString_xattr(char* str, const XmlAttribute* attribute)
{
	DECLARE_FUNCTION(toString_xattr);

	/* Variable declarations. */
	const char* name;
	char buffer[BUFFER_SIZE], *value = buffer;

	/* Check. */
	ASSERT_XMLATTRIBUTE(attribute);

	name = attribute->name;
	value = private_toXmlString(buffer, attribute->value);
	ASSERT(value);

	str = fromPattern(str, BUFFER_SIZE, XML_ATTRIBUTE(name,value));
	ASSERT(str);
	ASSERT(strcmp(str,""));

	return str;
}

IMPLEMENT_ARRAY_FUNCTIONS_OF(XmlAttribute,XML_MAX_ATTRIBUTES,size,array,xattra,xattr,attribute,attributeId,"","\n","")
IMPLEMENT_LIST_FUNCTIONS_OF(XmlAttribute,XML_MAX_ATTRIBUTES,size,list,array,xattrl,xattr,"","\n","")

XmlNode* initialize_xmln(XmlNode* node, XmlNode* parent, const unsigned int nodeId)
{
	DECLARE_FUNCTION(initialize_xmln);

	/* Variable declarations. */
	unsigned int i;

	unless (node)
		SAFE_MALLOC(node, XmlNode, 1);

	/* NULL parent means there is no parent. */
	node->parent = parent;
	if (parent) {
		/* Register the child to the parent. */
		parent->children[parent->nChildren++] = node;
		ASSERT_XMLNODE(parent);
	}

	/* Empty tag. */
	node->tag[0] = '\0';

	/* Empty contents. */
	for (i = 0; i < XML_MAX_CHILDREN; i++)
		node->content[i][0] = '\0';

	/* No children. */
	node->nChildren = 0;

	/* Store the node index. */
	node->nodeId = nodeId;

	/* Initialize attributes. */
	initialize_xattrl(node->attributes);
	ASSERT_XMLATTRIBUTELIST(node->attributes);

	/* Return the node. */
	return node;
}

char* toXPath_xmln(char* str, const XmlNode* node)
{
	DECLARE_FUNCTION(toXPath_xmln);

	unsigned int i, n;
	const XmlNode* stack[XML_MAX_NODES];
	const XmlNode* ancestor;
	const XmlNode* child;
	char* ptr;

	ASSERT_XMLNODE(node);

	n = 0;
	stack[n++] = node;
	for (ancestor = node->parent; (ancestor); ancestor = ancestor->parent)
	{
		ASSERT_XMLNODE(ancestor);
		stack[n++] = ancestor;
		ASSERT(n < XML_MAX_NODES);
	}

	/* Insert root. */
	ancestor = stack[--n];
	ptr = str = fromPattern(str, BUFFER_SIZE, "/%s", ancestor->tag);
	ASSERT(str);
	ASSERT(strcmp(str,""));
	ptr += strlen(ptr);
	until (ancestor == node)
	{
		for (i = 0; i < ancestor->nChildren; i++)
		{
			child = ancestor->children[i];
			ASSERT_XMLNODE(child);
			if (child == stack[n-1]) {
				ptr = fromPattern(ptr, BUFFER_SIZE, "/""*[%u]", i+1);
				ASSERT(ptr);
				ASSERT(strcmp(ptr,""));
				ptr += strlen(ptr);
				break;
			}
		}
		ancestor = stack[--n];
	}
	ASSERT(str);
	ASSERT(strcmp(str,""));
	ASSERT(strlen(str) < BUFFER_SIZE);
	return str;
}

char* toString_xmln(char* str, const XmlNode* node)
{
	DECLARE_FUNCTION(toString_xmln);

	/* Variable declarations. */
	unsigned int len, i;
	char* ptr;
	const XmlAttribute* attribute;
	const XmlNode* child;

	/* Checks. */
	ASSERT_XMLNODE(node);

	/* Start the node. */
	ptr = fromPattern(
		str,
		BUFFER_LARGE_SIZE,
		"%c%s",
		XML_NODE_BEGIN_SYMBOL,
		node->tag
	);
	ASSERT(ptr);
	ASSERT(strcmp(ptr,""));
	ptr += strlen(ptr);

	/* Write attributes. */
	for (i = 0; i < node->attributes->size; i++)
	{
		attribute = node->attributes->list[i];
		ASSERT_XMLATTRIBUTE(attribute);

		/* Put a space. */
		*(ptr++) = ' ';
		ptr = toString_xattr(ptr, attribute);
		ASSERT(ptr);
		ASSERT(strcmp(ptr,""));

		ptr += strlen(ptr);
	}

	ptr = fromPattern(ptr, BUFFER_SIZE, "%c", XML_NODE_END_SYMBOL);
	ASSERT(ptr);
	ASSERT(strcmp(ptr,""));
	ptr++;

	/* Write the content. */
	for (i = 0; i < node->nChildren; i++) {
		ptr = private_toXmlString(ptr, node->content[i]);
		ASSERT(ptr);
		ptr += strlen(ptr);

		child = node->children[i];
		ASSERT_XMLNODE(child);

		ptr = toString_xmln(ptr, child);
		ASSERT(ptr);
		ASSERT(strcmp(ptr,""));

		ptr += strlen(ptr);
	}
	ptr = private_toXmlString(ptr, node->content[i]);
	ASSERT(ptr);
	ptr += strlen(ptr);

	/* Finalize the node. */
	ptr = fromPattern(
		ptr,
		BUFFER_SIZE,
		"%c%c%s%c",
		XML_NODE_BEGIN_SYMBOL,
		XML_NODE_STOP_SYMBOL,
		node->tag,
		XML_NODE_END_SYMBOL
	);
	ASSERT(ptr);
	ASSERT(strcmp(ptr,""));

	/* Final check. */
	len = ptr - str;
	ASSERT(len < BUFFER_LARGE_SIZE);

	/* Return the string. */
	return str;
}

char* toContent_xmln(char* str, const XmlNode* node)
{
	DECLARE_FUNCTION(toContent_xmln);

	/* Variable declaration. */
	unsigned int i;
	char* ptr;

	/* Check. */
	ASSERT_XMLNODE(node);

	ptr = fromPattern(str, BUFFER_SIZE, "%s", node->content[0]);
	ASSERT(ptr);
	for (i = 0; i < node->nChildren; i++) {
		ptr += strlen(ptr);
		ptr = toContent_xmln(ptr, node->children[i]);
		ASSERT(ptr);
		ptr += strlen(ptr);
		ptr = fromPattern(ptr, BUFFER_SIZE, "%s", node->content[i+1]);
		ASSERT(ptr);
	}

	ASSERT(strlen(str) < BUFFER_SIZE);
	return str;
}

XmlAttribute* findAttr_xmln(XmlNode* node, const char* name)
{
	DECLARE_FUNCTION(findAttr_xmln);

	unsigned int i;
	XmlAttribute* attribute;

	ASSERT_XMLNODE(node);
	ASSERT(name);
	ASSERT(strcmp(name,""));
	ASSERT(strlen(name) < XML_MAX_ATTRIBUTE_NAME_SIZE);

	for (i = 0; i < node->attributes->size; i++)
	{
		attribute = node->attributes->list[i];
		ASSERT_XMLATTRIBUTE(attribute);

		if (!strcmp(attribute->name, name))
			return attribute;
	}

	return NULL;
}

const XmlAttribute* findConstAttr_xmln(const XmlNode* node, const char* name)
{
	DECLARE_FUNCTION(findConstAttr_xmln);

	unsigned int i;
	const XmlAttribute* attribute;

	ASSERT_XMLNODE(node);
	ASSERT(name);
	ASSERT(strcmp(name,""));
	ASSERT(strlen(name) < XML_MAX_ATTRIBUTE_NAME_SIZE);

	for (i = 0; i < node->attributes->size; i++)
	{
		attribute = node->attributes->list[i];
		ASSERT_XMLATTRIBUTE(attribute);

		if (!strcmp(attribute->name, name))
			return attribute;
	}

	return NULL;
}

IMPLEMENT_ARRAY_FUNCTIONS_OF(XmlNode,XML_MAX_NODES,size,nodes,xmlna,xmln,node,nodeId,"","\n","")
IMPLEMENT_LIST_FUNCTIONS_OF(XmlNode,XML_MAX_NODES,size,nodes,nodes,xmlnl,xmln,"","\n","")

Xml* initialize_xml(Xml* xml)
{
	DECLARE_FUNCTION(initialize_xml);

	/* Variable declaration. */
	char* meta;
	XmlNodeArray* nodes;
	XmlAttributeArray* attributes;

	unless (xml)
		SAFE_MALLOC(xml, Xml, 1);

	meta = fromPattern(xml->meta, XML_META_MAX_SIZE, "%s", XML_DEFAULT_META);
	ASSERT(meta);
	ASSERT(strcmp(meta,""));

	nodes = initialize_xmlna(xml->tree);
	ASSERT_XMLNODEARRAY(nodes);

	attributes = initialize_xattra(xml->attributes);
	ASSERT_XMLATTRIBUTEARRAY(xml->attributes);

	ASSERT_XML(xml);
	return xml;
}

Xml* fromString_xml(Xml* xml, const char* xmlstr)
{
	DECLARE_FUNCTION(fromString_xml);

	/* Variable declarations. */
	unsigned int i, len;
	char xmlContent[BUFFER_SIZE];
	char xmlValue[BUFFER_SIZE];
	char begin_quote;
	const char* ptr;
	XmlNodeIndexPair n;
	XmlAttributeIndexPair a;
	XmlNode* parent;

	/* Checks. */
	ASSERT(xmlstr);
	ASSERT(strcmp(xmlstr,""));
	ASSERT(strlen(xmlstr) < BUFFER_LARGE_SIZE);

	xml = initialize_xml(xml);
	ASSERT_XML(xml);

	/* Skip spaces. */
	for (ptr = xmlstr; (*ptr) && isspace(*ptr); ptr++);
	ASSERT(strcmp(ptr,""));

	/* Must begin with XML_NODE_BEGIN. */
	unless (*ptr == XML_NODE_BEGIN_SYMBOL)
		error(MSG_REPORT("Expected 'XML_NODE_BEGIN'"));

	/* Skip the XML_NODE_BEGIN_SYMBOL. */
	ptr++;
	ASSERT(strcmp(ptr,""));

	/* Skip all the meta nodes. */
	until (*ptr != XML_META_SYMBOL) {
		xml->meta[0] = XML_NODE_BEGIN_SYMBOL;

		/* Skip everything until XML_NODE_END. */
		for (i = 1; (*ptr) && *ptr != XML_NODE_END_SYMBOL; i++, ptr++)
			xml->meta[i] = *ptr;
		ASSERT(strcmp(ptr,""));
		xml->meta[i++] = *ptr;
		ASSERT(strcmp(ptr,""));
		xml->meta[i] = '\0';
		ASSERT(strlen(xml->meta) < XML_META_MAX_SIZE);

		/* Skip the node end. */
		ptr++;
		ASSERT(strcmp(ptr,""));

		/* Skip spaces. */
    	for (; (*ptr) && isspace(*ptr); ptr++);
		ASSERT(strcmp(ptr,""));

		/* Must begin with XML_NODE_BEGIN. */
		unless (*ptr == XML_NODE_BEGIN_SYMBOL)
			error(MSG_REPORT("Expected 'XML_NODE_BEGIN'"));

		/* Skip the XML_NODE_BEGIN_SYMBOL. */
		ptr++;
		ASSERT(strcmp(ptr,""));
	}

	n.node = xml->tree->nodes;
	parent = NULL;
	while (n.node) {
		/* Allocate a node from the Xml tree. */
		n = getNew_xmlna(xml->tree);
		ASSERT_XMLNODEARRAY(xml->tree);

		/* Initialize the node. */
		n.node = initialize_xmln(n.node, parent, n.nodeId);
		ASSERT(n.node);

		/* Fill the tag information. */
		i = 0;
		while ((*ptr) && !isspace(*ptr) && *ptr != XML_NODE_END_SYMBOL && *ptr != XML_NODE_STOP_SYMBOL)
			n.node->tag[i++] = *(ptr++);
		n.node->tag[i] = '\0';
		ASSERT(i < XML_MAX_TAG_SIZE);
		ASSERT(strcmp(ptr,""));

		/* Skip spaces. */
		for (; (*ptr) && isspace(*ptr); ptr++);
		ASSERT(strcmp(ptr,""));

		/* Fill the attributes. */
		until (*ptr == XML_NODE_STOP_SYMBOL || *ptr == XML_NODE_END_SYMBOL) {
			a = getNew_xattra(xml->attributes);
			ASSERT_XMLATTRIBUTEARRAY(xml->attributes);

			/* Fill the attribute name. */
			i = 0;
			while ((*ptr) && *ptr != XML_EQUAL_SYMBOL)
				a.attribute->name[i++] = *(ptr++);
			a.attribute->name[i] = '\0';
			ASSERT(i < XML_MAX_ATTRIBUTE_NAME_SIZE);
			ASSERT(strcmp(ptr,""));

			/* Skip the equal sign. */
			ptr++;
			ASSERT(strcmp(ptr,""));

			/* Must begin with a quote. */
			begin_quote = *ptr;
			unless (begin_quote == XML_QUOTE_SYMBOL || begin_quote == XML_APOSTROPHE_SYMBOL)
				error(MSG_REPORT("Expected 'QUOTE'"));

			/* Skip the quote. */
			ptr++;
			ASSERT(strcmp(ptr,""));

			/* Fill the attribute value. */
			i = 0;
			while ((*ptr) && *ptr != begin_quote)
				xmlValue[i++] = *(ptr++);
			xmlValue[i] = '\0';
			ASSERT(strcmp(ptr,""));
			private_fromXmlString(a.attribute->value, xmlValue);
			ASSERT_XMLATTRIBUTE(a.attribute);
			addXmlAttribute_xattrl(n.node->attributes, a.attribute);
			ASSERT_XMLATTRIBUTELIST(n.node->attributes);

			/* Skip the quote. */
			ptr++;
			ASSERT(strcmp(ptr,""));

			/* Skip spaces. */
    		for (; (*ptr) && isspace(*ptr); ptr++);
			ASSERT(strcmp(ptr,""));
		}

		if (*ptr == XML_NODE_STOP_SYMBOL) {
			/* Skip the stop symbol. */
			ptr++;
			ASSERT(strcmp(ptr,""));

			/* Go back a step. */
			n.node = parent;
			unless (n.node)
				break;
			parent = n.node->parent;
		}

		ASSERT(strcmp(ptr,""));
		unless(*ptr == XML_NODE_END_SYMBOL)
			error(MSG_REPORT("Expected 'XML_NODE_END'"));

		/* Skip the end symbol. */
		ptr++;
		ASSERT(strcmp(ptr,""));

		while (n.node) {
			/* Fill the content. */
			i = 0;
			while ((*ptr) && *ptr != XML_NODE_BEGIN_SYMBOL)
				xmlContent[i++] = *(ptr++);
			xmlContent[i] = '\0';
			ASSERT(strcmp(ptr,""));
			/* Append the content string! */
			private_fromXmlString(n.node->content[n.node->nChildren], xmlContent);

			/* Skip the node begin symbol. */
			ptr++;
			ASSERT(strcmp(ptr,""));

			unless (*ptr == XML_NODE_STOP_SYMBOL)
				break;

			/* Skip the node stop. */
			ptr++;
			ASSERT(strcmp(ptr,""));

			len = strlen(n.node->tag);
			unless (!strncmp(n.node->tag, ptr, len))
				error(MSG_REPORT("Tag Mismatch"));

			/* Skip the end_tag. */
			ptr += len;
			ASSERT(strcmp(ptr,""));

			/* Skip spaces. */
			for (; (*ptr) && isspace(*ptr); ptr++);
			ASSERT(strcmp(ptr,""));

			/* Must encounter an end symbol. */
			unless (*ptr == XML_NODE_END_SYMBOL)
				error(MSG_REPORT("Expected 'XML_NODE_END'"));

			/* Skip the end symbol. */
			ptr++;

			/* No children. */
			/* Go back a step. */
			n.node = parent;
			unless (n.node)
				break;
			parent = n.node->parent;

			ASSERT(strcmp(ptr,""));
		}

		if (n.node)
			parent = n.node;
	}

	/* Skip spaces. */
	for (; (*ptr) && isspace(*ptr); ptr++);

	if (*ptr)
		warning(MSG_REPORT(ptr));
	return xml;
}

Xml* fromStream_xml(Xml* xml, FILE* stream)
{
	DECLARE_FUNCTION(fromStream_xml);

	/* Variable declarations. */
	int check;
	unsigned long len, size;
	char buffer[BUFFER_LARGE_SIZE];

	/* Check. */
	ASSERT(stream);

	/* Calculate file length. */
	check = fseek(stream, 0, SEEK_END);
	ASSERT(!check);

	len = ftell(stream);
	ASSERT(len < BUFFER_LARGE_SIZE);

	/* Rewind everything. */
	check = fseek(stream, 0, SEEK_SET);
	ASSERT(!check);

	/* Read the file. */
	size = fread(buffer, 1, len, stream);
	ASSERT(size==len);
	buffer[len] = '\0';

	return fromString_xml(xml, buffer);
}

Xml* fromFile_xml(Xml* xml, const char* filename)
{
	DECLARE_FUNCTION(fromFile_xml);

	/* Variable declaration. */
	FILE* fp;

	/* Checks. */
	ASSERT(filename);
	ASSERT(strcmp(filename,""));
	ASSERT(strlen(filename) < BUFFER_SIZE);

	SAFE_FOPEN(fp,filename,"r");
		xml = fromStream_xml(xml, fp);
	fclose(fp);

	ASSERT_XML(xml);
	return xml;
}

char* toString_xml(char* xmlstr, const Xml* xml)
{
	DECLARE_FUNCTION(toString_xml);

	ASSERT_XML(xml);

	xmlstr = fromPattern(xmlstr, BUFFER_LARGE_SIZE, "%s", xml->meta);
	ASSERT(xmlstr);

	if (xml->tree->size)
		toString_xmln(xmlstr + strlen(xmlstr), xml->tree->nodes);

	return xmlstr;
}
