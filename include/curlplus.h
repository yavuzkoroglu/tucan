/** \file curlplus.h
 ** \brief Defines some easy CURL manipulation functions
 **/
#ifndef CURLPLUS_H
	#define CURLPLUS_H
	#include <curl/curl.h>
	#include "json.h"

	#ifndef CURL_NTRIES
		#define CURL_NTRIES 3
	#endif
	#ifndef CURL_TIMEOUT
		#define CURL_TIMEOUT 10
	#endif

	typedef enum CurlRequestTypeBody {
		CURL_GET,
		CURL_POST,
		CURL_DELETE
	} CurlRequestType;
	#define ASSERT_CURLREQUESTTYPE(type) \
		ASSERT(type <= CURL_DELETE);

	typedef struct CurlResultBody {
		CURL* curl;
		Json* output;
	} CurlResult;
	typedef struct curl_slist CurlList;
	#define ASSERT_CURLRESULT(result)	\
		ASSERT(result.curl);			\
		ASSERT_JSON(result.output)

	CurlResult perform_curl(const CurlRequestType, CURL*, Json*, const char*, ...);
#endif
