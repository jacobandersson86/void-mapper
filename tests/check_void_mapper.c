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

void_mapper_rectangle_t box[1] = {0};
void_mapper_rectangles_t input = {
    .buffer = box,
    .size = sizeof(box[0]) / sizeof(box)
};

void_mapper_rectangle_t area = {
    .position = { .x = 0, .y = 0},
    .size ={ .x = 100, .y = 200}
};

void_mapper_rectangle_t buffer[10];
uint16_t buffer_size = sizeof(buffer) / sizeof(buffer[0]);

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

START_TEST(case_buffer_size)
{
    // TODO: Check that the buffer size is big enough. It must be at least (n * 2 + 1)^2 - 1
}
END_TEST

START_TEST(case_one_square_in_the_middle)
{
    void_mapper_rectangle_t square[1] = { { .position = { .x = 20, .y = 20},
                                            .size =     { .x = 10, .y = 10} } };
    void_mapper_rectangles_t input = { .buffer = square, .size = 1 };

    void_mapper_rectangles_t result = void_mapper(input, area, buffer, buffer_size);


    void_mapper_rectangle_t expected[8] = {
        RECTANGLE(0, 0, 20, 20),    RECTANGLE(20, 0, 10, 20),   RECTANGLE(30, 0, 70, 20),
        RECTANGLE(0, 20, 20, 10),   /* Input was here */        RECTANGLE(30, 20, 70, 10),
        RECTANGLE(0, 30, 20, 170),  RECTANGLE(20, 30, 10, 170), RECTANGLE(30, 30, 70, 170)
    };

    for (unsigned int i = 0; i < sizeof(expected)/sizeof(expected[0]); i ++)
    {
        assert_rectangle(expected[i], result.buffer[i], i);
    }
    ck_assert_int_eq(result.size, 8);
}
END_TEST

START_TEST(case_two_squares)
{
    void_mapper_rectangle_t squares[2] = {
        RECTANGLE(20, 20, 10, 10),
        RECTANGLE(40, 40, 5, 5),
    };
    void_mapper_rectangles_t input = { .buffer = squares, .size = 2 };

    void_mapper_rectangles_t result = void_mapper(input, area, buffer, buffer_size);


    void_mapper_rectangle_t expected[23] = {
        RECTANGLE(0, 0,  20, 20),   RECTANGLE(20, 0, 10, 20),   RECTANGLE(30,  0, 10, 20),  RECTANGLE(40,  0, 5, 20),   RECTANGLE(45,  0, 55, 20),
        RECTANGLE(0, 20, 20, 10),   /* Input here */            RECTANGLE(30, 20, 10, 10),  RECTANGLE(40, 20, 5, 10),   RECTANGLE(45, 20, 55, 10),
        RECTANGLE(0, 30, 20, 10),   RECTANGLE(20, 30, 10, 10),  RECTANGLE(30, 30, 10, 10),  RECTANGLE(40, 30, 5, 10),   RECTANGLE(45, 30, 55, 10),
        RECTANGLE(0, 40, 20, 5),    RECTANGLE(20, 40, 10, 5),   RECTANGLE(30, 40, 10, 5),   /* Input here */            RECTANGLE(45, 40, 55, 5),
        RECTANGLE(0, 45, 20, 155),  RECTANGLE(20, 45, 10, 155), RECTANGLE(30, 45, 10, 155), RECTANGLE(40, 45, 5, 155),  RECTANGLE(45, 45, 55, 155),
    };

    for (unsigned int i = 0; i < sizeof(expected)/sizeof(expected[0]); i ++)
    {
        assert_rectangle(expected[i], result.buffer[i], i);
    }
    ck_assert_int_eq(result.size, 23);
}
END_TEST

START_TEST(case_two_squares_reverse_order)
{
    void_mapper_rectangle_t squares[2] = {
        RECTANGLE(40, 40, 5, 5),
        RECTANGLE(20, 20, 10, 10),
    };
    void_mapper_rectangles_t input = { .buffer = squares, .size = 2 };

    void_mapper_rectangles_t result = void_mapper(input, area, buffer, buffer_size);


    void_mapper_rectangle_t expected[23] = {
        RECTANGLE(0, 0,  20, 20),   RECTANGLE(20, 0, 10, 20),   RECTANGLE(30,  0, 10, 20),  RECTANGLE(40,  0, 5, 20),   RECTANGLE(45,  0, 55, 20),
        RECTANGLE(0, 20, 20, 10),   /* Input here */            RECTANGLE(30, 20, 10, 10),  RECTANGLE(40, 20, 5, 10),   RECTANGLE(45, 20, 55, 10),
        RECTANGLE(0, 30, 20, 10),   RECTANGLE(20, 30, 10, 10),  RECTANGLE(30, 30, 10, 10),  RECTANGLE(40, 30, 5, 10),   RECTANGLE(45, 30, 55, 10),
        RECTANGLE(0, 40, 20, 5),    RECTANGLE(20, 40, 10, 5),   RECTANGLE(30, 40, 10, 5),   /* Input here */            RECTANGLE(45, 40, 55, 5),
        RECTANGLE(0, 45, 20, 155),  RECTANGLE(20, 45, 10, 155), RECTANGLE(30, 45, 10, 155), RECTANGLE(40, 45, 5, 155),  RECTANGLE(45, 45, 55, 155),
    };

    for (unsigned int i = 0; i < sizeof(expected)/sizeof(expected[0]); i ++)
    {
        assert_rectangle(expected[i], result.buffer[i], i);
    }
    ck_assert_int_eq(result.size, 23);
}
END_TEST

START_TEST(case_squares_sharing_x_and_y)
{
    void_mapper_rectangle_t squares[3] = {
        RECTANGLE(20, 20, 10, 10),
        RECTANGLE(40, 20, 5, 10),
        RECTANGLE(20, 40, 10, 5),
    };
    void_mapper_rectangles_t input = { .buffer = squares, .size = 3 };

    void_mapper_rectangles_t result = void_mapper(input, area, buffer, buffer_size);

    void_mapper_rectangle_t expected[22] = {
        RECTANGLE(0, 0,  20, 20),   RECTANGLE(20, 0, 10, 20),   RECTANGLE(30,  0, 10, 20),  RECTANGLE(40,  0, 5, 20),   RECTANGLE(45,  0, 55, 20),
        RECTANGLE(0, 20, 20, 10),   /* Input here */            RECTANGLE(30, 20, 10, 10),  /* Input here */            RECTANGLE(45, 20, 55, 10),
        RECTANGLE(0, 30, 20, 10),   RECTANGLE(20, 30, 10, 10),  RECTANGLE(30, 30, 10, 10),  RECTANGLE(40, 30, 5, 10),   RECTANGLE(45, 30, 55, 10),
        RECTANGLE(0, 40, 20, 5),    /* Input here */            RECTANGLE(30, 40, 10, 5),   RECTANGLE(40, 40, 5, 5),    RECTANGLE(45, 40, 55, 5),
        RECTANGLE(0, 45, 20, 155),  RECTANGLE(20, 45, 10, 155), RECTANGLE(30, 45, 10, 155), RECTANGLE(40, 45, 5, 155),  RECTANGLE(45, 45, 55, 155),
    };

    for (unsigned int i = 0; i < sizeof(expected)/sizeof(expected[0]); i ++)
    {
        assert_rectangle(expected[i], result.buffer[i], i);
    }
    ck_assert_int_eq(result.size, 22);
}
END_TEST

START_TEST(case_squares_adjacent_x_and_y)
{
    void_mapper_rectangle_t squares[3] = {
        RECTANGLE(20, 20, 10, 10),
        RECTANGLE(30, 20, 10, 10),
        RECTANGLE(20, 30, 10, 10),
    };
    void_mapper_rectangles_t input = { .buffer = squares, .size = 3 };

    void_mapper_rectangles_t result = void_mapper(input, area, buffer, buffer_size);

    void_mapper_rectangle_t expected[13] = {
        RECTANGLE(0, 0,  20, 20),   RECTANGLE(20, 0, 10, 20),   RECTANGLE(30,  0, 10, 20),  RECTANGLE(40,  0, 60, 20),
        RECTANGLE(0, 20, 20, 10),   /* Input here */            /* Input here */            RECTANGLE(40, 20, 60, 10),
        RECTANGLE(0, 30, 20, 10),   /* Input here */            RECTANGLE(30, 30, 10, 10),  RECTANGLE(40, 30, 60, 10),
        RECTANGLE(0, 40, 20, 160),  RECTANGLE(20, 40, 10, 160), RECTANGLE(30, 40, 10, 160), RECTANGLE(40, 40, 60, 160),
    };

    for (unsigned int i = 0; i < sizeof(expected)/sizeof(expected[0]); i ++)
    {
        assert_rectangle(expected[i], result.buffer[i], i);
    }
    ck_assert_int_eq(result.size, 13);
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
