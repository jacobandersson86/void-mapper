#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "void_mapper.h"

#define RECTANGLE(px, py, sw, sl)               \
        {                                       \
            .position = { .x = px, .y = py},    \
            .size = { .x = sw, .y = sl },       \
        }                                       \

void_mapper_rectangle_t input[1] = { 0 };
uint16_t input_length = sizeof(input[0]) / sizeof(input);

void_mapper_rectangle_t area = {
    .position = { .x = 0, .y = 0},
    .size ={ .x = 100, .y = 200}
};

void_mapper_rectangle_t buffer[10];
uint16_t buffer_length = sizeof(buffer) / sizeof(buffer[0]);

static void assert_rectangle(void_mapper_rectangle_t expected, void_mapper_rectangle_t actual, unsigned int n)
{
    ck_assert_msg(expected.size.x == actual.size.x && expected.size.y == actual.size.y,
                  "n: %i, size mismatch, expected width: %i height: %i, got width: %i height: %i",
                  n,
                  expected.size.x, expected.size.y,
                  actual.size.x, actual.size.y);
    ck_assert_msg(expected.position.x == actual.position.x && expected.position.y == actual.position.y,
                  "n: %i, position mismatch, expected (%i, %i), got (%i, %i)",
                  n,
                  expected.position.x, expected.position.y,
                  actual.position.x, actual.position.y);

}

START_TEST(case_empty_input_size)
{
    input_length = 0;

    uint16_t result = void_mapper(area, input, input_length, buffer, buffer_length);

    ck_assert_mem_eq(&buffer[0], &area, sizeof(area));
    ck_assert_uint_eq(result, 1);
}
END_TEST

START_TEST(case_empty_input_ptr)
 {
    void_mapper_rectangle_t *input = NULL;

    uint16_t result = void_mapper(area, input, input_length, buffer, buffer_length);

    ck_assert_mem_eq(&buffer[0], &area, sizeof(area));
    ck_assert_uint_eq(result, 1);
}
END_TEST

START_TEST(case_empty_buffer_ptr)
{
    void_mapper_rectangle_t *buffer = NULL;

    uint16_t result = void_mapper(area, input, input_length, buffer, buffer_length);

    ck_assert_uint_eq(result, 0);
}
END_TEST

START_TEST(case_empty_buffer_size)
{
    uint16_t buffer_length = 0;

    uint16_t result = void_mapper(area, input, input_length, buffer, buffer_length);

    ck_assert_uint_eq(result, 0);
}
END_TEST

START_TEST(case_buffer_size)
{
    // TODO: Check that the buffer size is big enough. It must be at least (n * 2 + 1)^2 - 1
}
END_TEST

START_TEST(case_one_square_in_the_middle)
{
    void_mapper_rectangle_t square[1] = { { .position = { .x = 20, .y = 20},
                                            .size =     { .x = 10, .y = 10} } };

    uint16_t result = void_mapper(area, square, 1, buffer, buffer_length);


    void_mapper_rectangle_t expected[8] = {
        RECTANGLE(0, 0, 20, 20),    RECTANGLE(20, 0, 10, 20),   RECTANGLE(30, 0, 70, 20),
        RECTANGLE(0, 20, 20, 10),   /* Input was here */        RECTANGLE(30, 20, 70, 10),
        RECTANGLE(0, 30, 20, 170),  RECTANGLE(20, 30, 10, 170), RECTANGLE(30, 30, 70, 170)
    };

    for (unsigned int i = 0; i < sizeof(expected)/sizeof(expected[0]); i ++)
    {
        assert_rectangle(expected[i], buffer[i], i);
    }
    ck_assert_int_eq(result, 8);
}
END_TEST

START_TEST(case_two_squares)
{
    void_mapper_rectangle_t squares[2] = {
        RECTANGLE(20, 20, 10, 10),
        RECTANGLE(40, 40, 5, 5),
    };

    uint16_t result = void_mapper(area, squares, 2, buffer, buffer_length);

    void_mapper_rectangle_t expected[23] = {
        RECTANGLE(0, 0,  20, 20),   RECTANGLE(20, 0, 10, 20),   RECTANGLE(30,  0, 10, 20),  RECTANGLE(40,  0, 5, 20),   RECTANGLE(45,  0, 55, 20),
        RECTANGLE(0, 20, 20, 10),   /* Input here */            RECTANGLE(30, 20, 10, 10),  RECTANGLE(40, 20, 5, 10),   RECTANGLE(45, 20, 55, 10),
        RECTANGLE(0, 30, 20, 10),   RECTANGLE(20, 30, 10, 10),  RECTANGLE(30, 30, 10, 10),  RECTANGLE(40, 30, 5, 10),   RECTANGLE(45, 30, 55, 10),
        RECTANGLE(0, 40, 20, 5),    RECTANGLE(20, 40, 10, 5),   RECTANGLE(30, 40, 10, 5),   /* Input here */            RECTANGLE(45, 40, 55, 5),
        RECTANGLE(0, 45, 20, 155),  RECTANGLE(20, 45, 10, 155), RECTANGLE(30, 45, 10, 155), RECTANGLE(40, 45, 5, 155),  RECTANGLE(45, 45, 55, 155),
    };

    for (unsigned int i = 0; i < sizeof(expected)/sizeof(expected[0]); i ++)
    {
        assert_rectangle(expected[i], buffer[i], i);
    }
    ck_assert_int_eq(result, 23);
}
END_TEST

START_TEST(case_two_squares_reverse_order)
{
    void_mapper_rectangle_t squares[2] = {
        RECTANGLE(40, 40, 5, 5),
        RECTANGLE(20, 20, 10, 10),
    };

    uint16_t result = void_mapper(area, squares, 2, buffer, buffer_length);

    void_mapper_rectangle_t expected[23] = {
        RECTANGLE(0, 0,  20, 20),   RECTANGLE(20, 0, 10, 20),   RECTANGLE(30,  0, 10, 20),  RECTANGLE(40,  0, 5, 20),   RECTANGLE(45,  0, 55, 20),
        RECTANGLE(0, 20, 20, 10),   /* Input here */            RECTANGLE(30, 20, 10, 10),  RECTANGLE(40, 20, 5, 10),   RECTANGLE(45, 20, 55, 10),
        RECTANGLE(0, 30, 20, 10),   RECTANGLE(20, 30, 10, 10),  RECTANGLE(30, 30, 10, 10),  RECTANGLE(40, 30, 5, 10),   RECTANGLE(45, 30, 55, 10),
        RECTANGLE(0, 40, 20, 5),    RECTANGLE(20, 40, 10, 5),   RECTANGLE(30, 40, 10, 5),   /* Input here */            RECTANGLE(45, 40, 55, 5),
        RECTANGLE(0, 45, 20, 155),  RECTANGLE(20, 45, 10, 155), RECTANGLE(30, 45, 10, 155), RECTANGLE(40, 45, 5, 155),  RECTANGLE(45, 45, 55, 155),
    };

    for (unsigned int i = 0; i < sizeof(expected)/sizeof(expected[0]); i ++)
    {
        assert_rectangle(expected[i], buffer[i], i);
    }
    ck_assert_int_eq(result, 23);
}
END_TEST

START_TEST(case_squares_sharing_x_and_y)
{
    void_mapper_rectangle_t squares[3] = {
        RECTANGLE(20, 20, 10, 10),
        RECTANGLE(40, 20, 5, 10),
        RECTANGLE(20, 40, 10, 5),
    };

    uint16_t result = void_mapper(area, squares, 3, buffer, buffer_length);

    void_mapper_rectangle_t expected[22] = {
        RECTANGLE(0, 0,  20, 20),   RECTANGLE(20, 0, 10, 20),   RECTANGLE(30,  0, 10, 20),  RECTANGLE(40,  0, 5, 20),   RECTANGLE(45,  0, 55, 20),
        RECTANGLE(0, 20, 20, 10),   /* Input here */            RECTANGLE(30, 20, 10, 10),  /* Input here */            RECTANGLE(45, 20, 55, 10),
        RECTANGLE(0, 30, 20, 10),   RECTANGLE(20, 30, 10, 10),  RECTANGLE(30, 30, 10, 10),  RECTANGLE(40, 30, 5, 10),   RECTANGLE(45, 30, 55, 10),
        RECTANGLE(0, 40, 20, 5),    /* Input here */            RECTANGLE(30, 40, 10, 5),   RECTANGLE(40, 40, 5, 5),    RECTANGLE(45, 40, 55, 5),
        RECTANGLE(0, 45, 20, 155),  RECTANGLE(20, 45, 10, 155), RECTANGLE(30, 45, 10, 155), RECTANGLE(40, 45, 5, 155),  RECTANGLE(45, 45, 55, 155),
    };

    for (unsigned int i = 0; i < sizeof(expected)/sizeof(expected[0]); i ++)
    {
        assert_rectangle(expected[i], buffer[i], i);
    }
    ck_assert_int_eq(result, 22);
}
END_TEST

START_TEST(case_squares_adjacent_x_and_y)
{
    void_mapper_rectangle_t squares[3] = {
        RECTANGLE(20, 20, 10, 10),
        RECTANGLE(30, 20, 10, 10),
        RECTANGLE(20, 30, 10, 10),
    };

    uint16_t result = void_mapper(area, squares, 3, buffer, buffer_length);

    void_mapper_rectangle_t expected[13] = {
        RECTANGLE(0, 0,  20, 20),   RECTANGLE(20, 0, 10, 20),   RECTANGLE(30,  0, 10, 20),  RECTANGLE(40,  0, 60, 20),
        RECTANGLE(0, 20, 20, 10),   /* Input here */            /* Input here */            RECTANGLE(40, 20, 60, 10),
        RECTANGLE(0, 30, 20, 10),   /* Input here */            RECTANGLE(30, 30, 10, 10),  RECTANGLE(40, 30, 60, 10),
        RECTANGLE(0, 40, 20, 160),  RECTANGLE(20, 40, 10, 160), RECTANGLE(30, 40, 10, 160), RECTANGLE(40, 40, 60, 160),
    };

    for (unsigned int i = 0; i < sizeof(expected)/sizeof(expected[0]); i ++)
    {
        assert_rectangle(expected[i], buffer[i], i);
    }
    ck_assert_int_eq(result, sizeof(expected)/sizeof(expected[0]));
}
END_TEST

START_TEST(case_different_sized_sides)
{
    void_mapper_rectangle_t area = RECTANGLE(90, 84, 118, 68);

    void_mapper_rectangle_t squares[2] = {
        RECTANGLE(115, 84, 51, 68), /* < -- Box 0 */
        RECTANGLE(166, 89, 16, 16)  /* < -- Box 1 */
    };

    uint16_t result = void_mapper(area, squares, 2, buffer, buffer_length);

    void_mapper_rectangle_t expected[] = {
        RECTANGLE(90, 84, 25, 5),   /* -------------- */ RECTANGLE(166, 84, 16, 5),   RECTANGLE(182, 84, 26, 5),
        RECTANGLE(90, 89, 25, 16),  /* ¦ Box 0 here ¦ */      /* Box 1 here */        RECTANGLE(182, 89, 26, 16),
        RECTANGLE(90, 105, 25, 47), /* -------------- */ RECTANGLE(166, 105, 16, 47), RECTANGLE(182, 105, 26, 47)
    };

    for (unsigned int i = 0; i < sizeof(expected)/sizeof(expected[0]); i ++)
    {
        assert_rectangle(expected[i], buffer[i], i);
    }
    ck_assert_int_eq(result, sizeof(expected)/sizeof(expected[0]));
}
END_TEST

START_TEST(case_sort_vectors)
{
    /**
     * This test is from a real-life issue where the vectors where incorrectly
     * sorted, causing an underflow later on when subtractions were maid.
     *
     * The test will catch any underflows.
     */
    void_mapper_rectangle_t area = RECTANGLE(0, 0, 320, 240);

    void_mapper_rectangle_t squares[] = {
        { { 10,  90}, { 60,  60} },
        { {244,  16}, { 60, 101} },
        { {244, 123}, { 60, 101} },
        { { 60,  35}, {162,  27} },
        { { 90,  84}, { 51,  68} },
        { {141,  84}, { 51,  68} },
        { {192,  89}, { 16,  16} },
        { { 45, 178}, {162,  27} },
        { {177, 178}, { 11,  27} },
        { {188, 178}, { 11,  27} },
        { {199, 178}, {  8,  27} }
    };
    uint16_t result = void_mapper(area, squares, sizeof(squares)/sizeof(squares[0]), buffer, buffer_length);

    for (int i = 0; i < result; i ++) {
        ck_assert_int_le(buffer[i].position.x, 320);
        ck_assert_int_le(buffer[i].position.x + buffer[i].size.x, 320);
        ck_assert_int_le(buffer[i].position.y, 240);
        ck_assert_int_le(buffer[i].position.y + buffer[i].size.y, 240);
    }
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
    tcase_add_test(tc_core, case_one_square_in_the_middle);
    tcase_add_test(tc_core, case_two_squares);
    tcase_add_test(tc_core, case_two_squares_reverse_order);
    tcase_add_test(tc_core, case_squares_sharing_x_and_y);
    tcase_add_test(tc_core, case_squares_adjacent_x_and_y);
    tcase_add_test(tc_core, case_different_sized_sides);
    tcase_add_test(tc_core, case_sort_vectors);
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

    srunner_set_fork_status(sr, CK_NOFORK);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
