#include "json.h"

DECLARE_SOURCE("TEST_JSON");

int main(int argc, char* argv[]) {
	DECLARE_FUNCTION(main);

	const char testdata[10][BUFFER_SIZE] = {
		"[1,2,3,4,5]",
		"{\"yavuz\":31}",
		"{\"isim\":\"yavuz\",\"soyisim\":\"köroğlu\",\"yaş\":31,\"kilo\":76.5,\"erkek\":true}",
		"{\"birinci\":[]}",
		"{\"array\":[\"Yavuz\",\"Oğuz\"]}",
		"{\"sources\":[{\"address\":\"0.0.0.0\",\"name\":\"localhost\"},{\"address\":\"www.google.com\",\"name\":\"google\"}]}",
		"{}",
		"",
		"[]",
		"{\"\":\"\"}"
	};

	unsigned int i;
	char buffer[BUFFER_LARGE_SIZE], *str = buffer;
	Json *json;
	JsonElement elements[2], *ePtr = elements;
	JsonElement* capabilities = ePtr++;
	JsonElement* alwaysMatch = ePtr++;

	start_logging();

	say(MSG_REPORT_VAR("sizeof(Json)","%luM",sizeof(Json)/1024/1024));

	json = fromBatch_json(
		NULL,
		JSON_ARRAY,
			JSON_INTEGER, 1,
			JSON_INTEGER, 2,
			JSON_INTEGER, 3,
			JSON_INTEGER, 4,
			JSON_INTEGER, 5,
		JSON_ELEMENT_END
	);
	ASSERT_JSON(json);
	str = toString_json(buffer, json);
	ASSERT(str);
	ASSERT(strcmp(str,""));
	say(MSG_REPORT_VAR("JSON", "%s", str));

	json = fromBatch_json(
		json,
		JSON_ARRAY,
			JSON_ARRAY, NULL,
				JSON_INTEGER, 1,
			JSON_ELEMENT_END,
			JSON_ARRAY, NULL,
				JSON_INTEGER, 2,
			JSON_ELEMENT_END,
			JSON_ARRAY, NULL,
				JSON_INTEGER, 3,
			JSON_ELEMENT_END,
			JSON_ARRAY, NULL,
				JSON_INTEGER, 4,
			JSON_ELEMENT_END,
			JSON_ARRAY, NULL,
				JSON_INTEGER, 5,
			JSON_ELEMENT_END,
		JSON_ELEMENT_END
	);
	ASSERT_JSON(json);
	str = toString_json(buffer, json);
	ASSERT(str);
	ASSERT(strcmp(str,""));
	say(MSG_REPORT_VAR("JSON", "%s", str));

	json = fromBatch_json(
		json,
		JSON_ARRAY,
			JSON_OBJECT, NULL,
				JSON_STRING, "isim", "yavuz",
				JSON_INTEGER, "yaş", 31,
			JSON_ELEMENT_END,
			JSON_OBJECT, NULL,
				JSON_STRING, "isim", "fatma",
				JSON_INTEGER, "yaş", 64,
			JSON_ELEMENT_END,
		JSON_ELEMENT_END
	);
	ASSERT_JSON(json);
	str = toString_json(buffer, json);
	ASSERT(str);
	ASSERT(strcmp(str,""));
	say(MSG_REPORT_VAR("JSON", "%s", str));

	json = fromBatch_json(
		json,
		JSON_OBJECT,
			JSON_OBJECT, "capabilities", capabilities,
				JSON_OBJECT, "alwaysMatch", alwaysMatch,
					JSON_STRING, "platformName", "Android",
					JSON_STRING, "automationName", "UIAutomator1",
					JSON_STRING, "appPackage", "org.secuso.privacyfriendlynotes",
				JSON_ELEMENT_END,
			JSON_ELEMENT_END,
		JSON_ELEMENT_END
	);
	ASSERT_JSONELEMENT(capabilities);
	ASSERT_JSONOBJECT(capabilities->entity->asObject);
	ASSERT_JSONELEMENT(alwaysMatch);
	ASSERT_JSONOBJECT(alwaysMatch->entity->asObject);
	alwaysMatch = insert_json(
		json,
		alwaysMatch,
			JSON_STRING, "appActivity", "org.secuso.privacyfriendlynotes.SplashActivity",
			JSON_INTEGER, "newCommandTimeout", 3000L,
		JSON_ELEMENT_END
	);
	ASSERT_JSONELEMENT(alwaysMatch);
	ASSERT_JSONOBJECT(alwaysMatch->entity->asObject);
	ASSERT_JSON(json);
	str = toString_json(buffer, json);
	ASSERT(str);
	ASSERT(strcmp(str,""));
	say(MSG_REPORT_VAR("JSON", "%s", str));

	for (i = 0; i < 10; i++)
	{
		json = fromString_json(json, testdata[i]);
		ASSERT_JSON(json);

		str = toString_json(buffer, json);
		ASSERT(str);
		ASSERT(strcmp(str,""));
		say(MSG_REPORT_VAR("JSON", "%s", str));
	}

	stop_logging();

	free(json);

	return 0;
}


