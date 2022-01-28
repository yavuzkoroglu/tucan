#include <stdlib.h>
#include "hashtable.h"

DECLARE_SOURCE("TEST_HASHTABLE");

int main(int argc, char* argv[]) {
	DECLARE_FUNCTION(main);

	/* Variable declarations. */
	const Object* age;
	HashTable* ht;

	start_logging();

	say(MSG_REPORT_VAR("sizeof(HashTable)","%luM",sizeof(HashTable)/1024/1024));

	ht = initialize_ht(NULL);
	ASSERT_HASHTABLE(ht);
	say(MSG_REPORT_VAR("sizeof(map)","%luM",sizeof(ht->map)/1024/1024));

	insert_ht(ht, "Yavuz", fromUInt_obj(31));
	insert_ht(ht, "Bade", fromUInt_obj(24));
	insert_ht(ht, "Nazlı", fromUInt_obj(34));
	ASSERT_HASHTABLE(ht);

	age = get_ht(ht, "Nazlı");
	ASSERT(age);
	say(MSG_REPORT_VAR("Nazlı","%u",age->asUInt));

	age = get_ht(ht, "Yavuz");
	ASSERT(age);
	say(MSG_REPORT_VAR("Yavuz","%u",age->asUInt));

	age = get_ht(ht, "Bade");
	ASSERT(age);
	say(MSG_REPORT_VAR("Bade","%u",age->asUInt));

	age = get_ht(ht, "Kıvanç");
	ASSERT(!age);

	stop_logging();

	free(ht);

	return 0;
}

