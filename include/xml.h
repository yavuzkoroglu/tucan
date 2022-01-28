/** \file xml.h
 ** \brief Declares all the Xml related stuff.
 **/
#ifndef XML_H
	#define XML_H
	#include "list.h"

	#ifndef XML_MAX_TAG_SIZE
		#define XML_MAX_TAG_SIZE 100
	#endif
	#ifndef XML_MAX_CONTENT_SIZE
		#define XML_MAX_CONTENT_SIZE 1
	#endif
	#ifndef XML_MAX_CHILDREN
		#define XML_MAX_CHILDREN 200
	#endif
	#ifndef XML_MAX_NODES
		#define XML_MAX_NODES 512
	#endif
	#ifndef XML_MAX_ATTRIBUTES
		#define XML_MAX_ATTRIBUTES 4096
	#endif
	#ifndef XMLNODE_MAX_ATTRIBUTES
		#define XMLNODE_MAX_ATTRIBUTES 32
	#endif
	#ifndef XML_MAX_ATTRIBUTE_NAME_SIZE
		#define XML_MAX_ATTRIBUTE_NAME_SIZE 32
	#endif
	#ifndef XML_MAX_ATTRIBUTE_VAL_SIZE
		#define XML_MAX_ATTRIBUTE_VAL_SIZE 256
	#endif
	#ifndef XML_META_MAX_SIZE
		#define XML_META_MAX_SIZE 250
	#endif
	#ifndef XML_EQUAL_SYMBOL
		#define XML_EQUAL_SYMBOL '='
	#endif
	#ifndef XML_QUOTE_SYMBOL
		#define XML_QUOTE_SYMBOL '"'
	#endif
	#ifndef XML_QUOTE
		#define XML_QUOTE "&quot;"
	#endif
	#ifndef XML_APOSTROPHE_SYMBOL
		#define XML_APOSTROPHE_SYMBOL '\''
	#endif
	#ifndef XML_APOSTROPHE
		#define XML_APOSTROPHE "&aops;"
	#endif
	#ifndef XML_QUOTE_SYMBOLS
		#define XML_QUOTE_SYMBOLS {XML_QUOTE_SYMBOL,XML_APOSTROPHE_SYMBOL,'\0'}
	#endif
	#ifndef XML_DEFAULT_META
		#define XML_DEFAULT_META "<?xml version='1.0' encoding='UTF-8' standalone='yes'?>"
	#endif
	#ifndef XML_META_SYMBOL
		#define XML_META_SYMBOL '?'
	#endif
	#ifndef XML_NODE_BEGIN_SYMBOL
		#define XML_NODE_BEGIN_SYMBOL '<'
	#endif
	#ifndef XML_GT
		#define XML_GT "&gt;"
	#endif
	#ifndef XML_NODE_STOP_SYMBOL
		#define XML_NODE_STOP_SYMBOL '/'
	#endif
	#ifndef XML_NODE_END_SYMBOL
		#define XML_NODE_END_SYMBOL '>'
	#endif
	#ifndef XML_LT
		#define XML_LT "&lt;"
	#endif
	#ifndef XML_AMP_SYMBOL
		#define XML_AMP_SYMBOL '&'
	#endif
	#ifndef XML_AMP
		#define XML_AMP "&amp;"
	#endif
	#ifndef XML_ATTRIBUTE
		#define XML_ATTRIBUTE(name, value)	\
			"%s%c%c%s%c",					\
			name,							\
			XML_EQUAL_SYMBOL,				\
			XML_QUOTE_SYMBOL,				\
			value,							\
			XML_QUOTE_SYMBOL
	#endif

	typedef struct XmlAttributeBody {
		char name[XML_MAX_ATTRIBUTE_NAME_SIZE];
		char value[XML_MAX_ATTRIBUTE_VAL_SIZE];
	} XmlAttribute;
	#define ASSERT_XMLATTRIBUTE(attribute)								\
		ASSERT(attribute);												\
		ASSERT(strcmp(attribute->name,""));								\
		ASSERT(strlen(attribute->name) < XML_MAX_ATTRIBUTE_NAME_SIZE);	\
		ASSERT(strlen(attribute->value) < XML_MAX_ATTRIBUTE_VAL_SIZE)

	XmlAttribute* initialize_xattr(XmlAttribute*, const char*, const char*);
	char* toString_xattr(char*, const XmlAttribute*);

	DECLARE_ARRAY_OF(XmlAttribute,XML_MAX_ATTRIBUTES,size,array,xattra,attribute,attributeId);
	DECLARE_LIST_OF(XmlAttribute,XML_MAX_ATTRIBUTES,size,list,xattrl);

	#define ASSERT_XMLATTRIBUTEARRAY(array)				\
		ASSERT_ARRAY(array,size,XML_MAX_ATTRIBUTES)
	#define ASSERT_XMLATTRIBUTELIST(list)				\
		ASSERT_LIST(list,size,XMLNODE_MAX_ATTRIBUTES)

	typedef struct XmlNodeBody {
		struct XmlNodeBody* parent;
		struct XmlNodeBody* children[XML_MAX_CHILDREN];
		unsigned int nChildren;
		unsigned int nodeId;
		char tag[XML_MAX_TAG_SIZE];
		char content[XML_MAX_CHILDREN+1][XML_MAX_CONTENT_SIZE];
		XmlAttributeList attributes[1];
	} XmlNode;
	#define ASSERT_XMLNODE(node)						\
		ASSERT(node);									\
		ASSERT(strcmp(node->tag,""));					\
		ASSERT(strlen(node->tag) < XML_MAX_TAG_SIZE);	\
		ASSERT(node->nChildren < XML_MAX_CHILDREN);		\
		ASSERT_XMLATTRIBUTELIST(node->attributes)

	XmlNode* initialize_xmln(XmlNode*, XmlNode*, const unsigned int);
	char* toXPath_xmln(char*, const XmlNode*);
	char* toString_xmln(char*, const XmlNode*);
	char* toContent_xmln(char*, const XmlNode*);
	XmlAttribute* findAttr_xmln(XmlNode*, const char*);
	const XmlAttribute* findConstAttr_xmln(const XmlNode*, const char*);

	DECLARE_ARRAY_OF(XmlNode,XML_MAX_NODES,size,nodes,xmlna,node,nodeId);
	DECLARE_LIST_OF(XmlNode,XML_MAX_NODES,size,nodes,xmlnl);

	#define ASSERT_XMLNODEARRAY(array)	\
		ASSERT_ARRAY(array,size,XML_MAX_NODES)
	#define ASSERT_XMLNODELIST(list)	\
		ASSERT_LIST(list,size,XML_MAX_NODES)

	typedef struct XmlBody {
		char meta[XML_META_MAX_SIZE];
		XmlAttributeArray attributes[1];
		XmlNodeArray tree[1];
	} Xml;
	#define ASSERT_XML(xml)								\
		ASSERT(xml);									\
		ASSERT(strlen(xml->meta) < XML_META_MAX_SIZE);	\
		ASSERT_XMLATTRIBUTEARRAY(xml->attributes);		\
		ASSERT_XMLNODEARRAY(xml->tree)

	Xml* initialize_xml(Xml*);
	Xml* fromString_xml(Xml*, const char*);
	Xml* fromStream_xml(Xml*, FILE*);
	Xml* fromFile_xml(Xml*, const char*);
	char* toString_xml(char*, const Xml*);
#endif
