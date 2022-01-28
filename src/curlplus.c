/** \file curlplus.c
 **/
#include "curlplus.h"
#include "timer.h"

DECLARE_SOURCE("CURLPLUS");

static size_t private_toString_curl(void* contents, size_t size, size_t nmemb, void* userp)
{
	DECLARE_FUNCTION(private_toString_curl);

	const char* source;
	char* destination;
	size_t totalsize;

	ASSERT(contents);
	ASSERT(userp);

	source = (const char*)contents;
	destination = *(char**)userp;

	totalsize = size * nmemb;
	destination = (char*)memmove(destination, source, totalsize + 1) + totalsize;
	*(char**)userp = destination;

	return totalsize;
}

CurlResult perform_curl(const CurlRequestType type, CURL* curl, Json* json, const char* url, ...)
{
	DECLARE_FUNCTION(perform_curl);

	unsigned int i;
	va_list args;
	CurlResult result;
	CurlList* list;
	CURLcode retval;
	const Json* fields;
	char buffer[BUFFER_LARGE_SIZE] = "", *str = buffer, **ptr = &str;

	ASSERT_CURLREQUESTTYPE(type);

	if (curl)
		curl_easy_reset(curl);
	else
		curl = curl_easy_init();
	ASSERT(curl);

	ASSERT(url);
	ASSERT(strcmp(url,""));
	ASSERT(strlen(url) < BUFFER_SIZE);

	va_start(args, url);

	list = curl_slist_append(NULL, "Content-Type:application/json;charset=UTF-8");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, private_toString_curl);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)ptr);
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
	curl_easy_setopt(curl, CURLOPT_MAXFILESIZE, BUFFER_LARGE_SIZE);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, CURL_TIMEOUT);
	if (type == CURL_POST) {
		fields = va_arg(args, const Json*);
		ASSERT_JSON(fields);
		str = toString_json(buffer, fields);
		ASSERT(str);
		ASSERT(strcmp(str,""));
		ASSERT(strlen(str) < BUFFER_LARGE_SIZE);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, str);
	} else if (type == CURL_DELETE) {
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
	}

	/* Try to perform curl call N times. */
	for (
		i = 1, retval = curl_easy_perform(curl);
		retval != CURLE_OK && i <= CURL_NTRIES;
		i++, retval = curl_easy_perform(curl)
	);

	unless (retval == CURLE_OK) {
		if (type == CURL_GET) {
			error(MSG_REPORT_VAR("Failed GET Request", "%s", url));
		} else if (type == CURL_POST) {
			error(MSG_REPORT_VAR("Failed POST Request", "%s", url));
		} else {
			error(MSG_REPORT_VAR("Failed DELETE Request", "%s", url));
		}
	}

	curl_slist_free_all(list);
	va_end(args);

	json = fromString_json(json, buffer);
	ASSERT_JSON(json);

	result.curl = curl;
	result.output = json;
	ASSERT_CURLRESULT(result);
	return result;
}
