#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "void_mapper.h"

void_mapper_rectangle_t box[1] = {0};
void_mapper_rectangles_t input = {
    .buffer = box,
    .size = sizeof(box[0]) / sizeof(box)
};

void_mapper_rectangle_t area = {
    .position = { .x = 0, .y = 0},
    .size ={100, 200}
};

void_mapper_rectangle_t buffer[10];
uint16_t buffer_size = sizeof(buffer) / sizeof(buffer[0]);

START_TEST(case_empty_input_size)
{
    input.size = 0;

    void_mapper_rectangles_t result = void_mapper(input, area, buffer, buffer_size);

    ck_assert_mem_eq(&result.buffer[0], &area, sizeof(area));
    ck_assert_uint_eq(result.size, 1);
}
END_TEST

START_TEST(case_empty_input_ptr)
 {
    input.buffer = NULL;

    void_mapper_rectangles_t result = void_mapper(input, area, buffer, buffer_size);

    ck_assert_mem_eq(&result.buffer[0], &area, sizeof(area));
    ck_assert_uint_eq(result.size, 1);
}
END_TEST

START_TEST(case_empty_buffer_ptr)
{
    void_mapper_rectangle_t *buffer = NULL;

    void_mapper_rectangles_t result = void_mapper(input, area, buffer, buffer_size);

    ck_assert_ptr_eq(result.buffer, NULL);
    ck_assert_uint_eq(result.size, 0);
}
END_TEST

START_TEST(case_empty_buffer_size)
{
    uint16_t buffer_size = 0;

    void_mapper_rectangles_t result = void_mapper(input, area, buffer, buffer_size);

    ck_assert_ptr_eq(result.buffer, NULL);
    ck_assert_uint_eq(result.size, 0);
}
END_TEST

Suite * void_mapper_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Void Mapper");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, case_empty_input_ptr);
    tcase_add_test(tc_core, case_empty_input_size);
    tcase_add_test(tc_core, case_empty_buffer_ptr);
    tcase_add_test(tc_core, case_empty_buffer_size);
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
