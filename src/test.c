/* vim: noet ts=4 sw=4
*/
#include <stdio.h>
#include <string.h>
#include "simple_sparsehash.h"

#define begin_tests() int test_return_val = 0;\
					  int tests_failed = 0;\
					  int tests_run = 0;
#define run_test(test) printf("%s: ", #test);\
	test_return_val = test();\
	if (!test_return_val) {\
		tests_failed++;\
		printf("%c[%dmFailed.%c[%dm\n", 0x1B, 31, 0x1B, 0);\
	} else {\
		tests_run++;\
		printf("%c[%dmPassed.%c[%dm\n", 0x1B, 32, 0x1B, 0);\
	}
#define finish_tests() printf("\n-----\nTests passed: (%i/%i)\n", tests_run,\
							  tests_run + tests_failed);
#define assert(x) if (!x) {\
		printf("failed on line %i ", __LINE__);\
		return 0;\
	}


int test_cannot_set_bigger_elements() {
	struct sparse_array *arr = NULL;
	const uint64_t test_num = 666;
	arr = sparse_array_init(sizeof(char));
	assert(arr);

	assert(sparse_array_set(arr, 0, &test_num, sizeof(test_num)) == 0);

	assert(sparse_array_free(arr));
	return 1;

}

int test_array_set() {
	int i;
	struct sparse_array *arr = NULL;
	arr = sparse_array_init(sizeof(int));
	assert(arr);

	for (i = 0; i < GROUP_SIZE; i++) {
		size_t siz = 0;
		assert(sparse_array_set(arr, i, &i, sizeof(i)));
		assert(sparse_array_get(arr, i, &siz) == i);
		assert(siz == sizeof(int));
	}

	assert(sparse_array_free(arr));
	return 1;
}

int test_array_set_overwrites_old_values() {
	struct sparse_array *arr = NULL;
	const int test_num = 666;
	const int test_num2 = 1024;
	arr = sparse_array_init(sizeof(int));
	assert(arr);

	assert(sparse_array_set(arr, 0, &test_num, sizeof(test_num)));
	assert(sparse_array_set(arr, 0, &test_num2, sizeof(test_num2)));

	assert(sparse_array_get(arr, 0, NULL) == 1024);

	assert(sparse_array_free(arr));
	return 1;
}

int test_array_get() {
	struct sparse_array *arr = NULL;
	const int test_num = 666;
	size_t item_size = 0;
	arr = sparse_array_init(sizeof(int));
	assert(arr);

	assert(sparse_array_set(arr, 0, &test_num, sizeof(test_num)));
	assert(sparse_array_get(arr, 0, &item_size) == 666);
	assert(item_size == sizeof(int));

	assert(sparse_array_free(arr));
	return 1;
}

int test_dict_set() {
	struct sparse_dict *dict = NULL;
	dict = sparse_dict_init();
	assert(dict);

	assert(sparse_dict_set(dict, "key", strlen("key"), "value", strlen("value")));

	assert(sparse_dict_free(dict));
	return 1;
}

int test_dict_get() {
	struct sparse_dict *dict = NULL;
	const char *value = NULL;

	dict = sparse_dict_init();
	assert(dict);

	assert(sparse_dict_set(dict, "key", strlen("key"), "value", strlen("value")));

	value = sparse_dict_get(dict, "key", strlen("key"));
	assert(strncmp(value, "value", strlen("value")) == 0);

	assert(sparse_dict_free(dict));
	return 1;
}

int main(int argc, char *argv[]) {
	begin_tests();
	run_test(test_cannot_set_bigger_elements);
	run_test(test_array_set);
	run_test(test_array_get);
	run_test(test_array_set_overwrites_old_values);
	run_test(test_dict_set);
	run_test(test_dict_get);
	finish_tests();

	return 0;
}
