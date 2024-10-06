#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

START_TEST(test_void_mapper)
 {
    ck_assert(true);
 }
 END_TEST

Suite * void_mapper_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Void Mapper");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_void_mapper);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = void_mapper_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
