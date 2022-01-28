#include "curlplus.h"

DECLARE_SOURCE("TEST_CURLPLUS");

char* getBuildVersion(char* version)
{
	DECLARE_FUNCTION(printStatus);

	static const char url[] = "localhost:4723/wd/hub/status";
	CURL* curl = NULL;
	Json* json;
	CurlResult result;
	JsonValue* value;

	json = initialize_json(NULL);
	ASSERT_JSON(json);
	result = perform_curl(CURL_GET, curl, json, url);
	ASSERT_CURLRESULT(result);

	value = locate_json(json,
		"value", JSON_OBJECT,
		"build", JSON_OBJECT,
		"version", JSON_STRING,
		JSON_ELEMENT_END
	);
	ASSERT_JSONVALUE(value);

	version = fromPattern(version, BUFFER_SIZE, "%s", value->data.asStr);
	ASSERT(version);
	ASSERT(strcmp(version,""));

	free(json);

	return version;
}

int main(int argc, char* argv[]) {
	DECLARE_FUNCTION(main);

	char vBuffer[BUFFER_SIZE], *version = vBuffer;

	start_logging();

	say(MSG_REPORT("APPIUM STATUS"));
	version = getBuildVersion(vBuffer);
	ASSERT(version);
	ASSERT(strcmp(version,""));
	ASSERT(strlen(version) < BUFFER_SIZE);
	say(MSG_REPORT_VAR("Build Version", "%s", version));

	stop_logging();

	return 0;
}
