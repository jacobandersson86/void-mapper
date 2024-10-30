#include <stdlib.h>
#include <stdbool.h>
#include "void_mapper.h"

/**
 * @brief Find if to ranges overlap.
 * The ranges are considered to intersect if they share the same
 * number. I.e. if the a range ends at 7 and the b range starts at 7, then
 * they overlap. This function assumes a0 < a1 and b0 < b1
 *
 * @param a0 start of first range
 * @param a1 end of first range
 * @param b0 start of second range
 * @param b1 end of second range
 * @return true if they intersect
 * @return false if they don't
 */
static bool ranges_intersect(int16_t a0, int16_t a1, int16_t b0, int16_t b1);

/**
 * @brief Find if to rectangles intersect. This function returns true if any
 * "pixel" share the same coordinate.
 *
 * @param a First square
 * @param b Second square
 * @return true if the intersect
 * @return false if they don't
 */
static bool rectangles_intersect(void_mapper_rectangle_t a, void_mapper_rectangle_t b);

static bool ranges_intersect(int16_t a0, int16_t a1, int16_t b0, int16_t b1)
{
    return  (a0 <= b0 && a1 >= b1 && a1 <= b1) ||
            (b0 <= a0 && b1 >= a1 && b1 <= a1);
}

static bool rectangles_intersect(void_mapper_rectangle_t a, void_mapper_rectangle_t b)
{
    uint16_t ax0 = a.position.x;
    uint16_t ax1 = a.position.x + a.size.x - 1;
    uint16_t bx0 = b.position.x;
    uint16_t bx1 = b.position.x + b.size.x - 1;
    bool x_intersect = ranges_intersect(ax0, ax1, bx0, bx1);

    uint16_t ay0 = a.position.y;
    uint16_t ay1 = a.position.y + a.size.y - 1;
    uint16_t by0 = b.position.y;
    uint16_t by1 = b.position.y + b.size.y - 1;
    bool y_intersect = ranges_intersect(ay0, ay1, by0, by1);

    return x_intersect && y_intersect;
}

void_mapper_rectangles_t void_mapper(void_mapper_rectangles_t boxes, void_mapper_rectangle_t area,
                                     void_mapper_rectangle_t * buffer, uint16_t size)
{
    if (buffer == NULL || size == 0) {
        return (void_mapper_rectangles_t) {.buffer = NULL, .size = 0};
    }

    if (boxes.buffer == NULL || boxes.size == 0) {
        buffer[0] = area;
        return (void_mapper_rectangles_t) {.buffer = buffer, .size = 1};
    }

    uint16_t vec_len = boxes.size + 3;

    // Find all x and y
    volatile uint16_t x_vector[vec_len];
    uint16_t y_vector[vec_len];
    x_vector[0] = area.position.x;
    x_vector[vec_len - 1] = area.position.x + area.size.x;

    y_vector[0] = area.position.y;
    y_vector[vec_len - 1] = area.position.y + area.size.y;

    uint32_t i = 0;
    for ( ;  i < boxes.size; i++)
    {
        x_vector[i + 1] = boxes.buffer[i].position.x;
        y_vector[i + 1] = boxes.buffer[i].position.y;
    }

    x_vector[vec_len - 2] = boxes.buffer[i - 1].position.x + boxes.buffer[i - 1].size.x;
    y_vector[vec_len - 2] = boxes.buffer[i - 1].position.y + boxes.buffer[i - 1].size.y;

    // Build all possible rectangles
    uint16_t potential_size = (boxes.size * 2 + 1);
    potential_size *= potential_size;

    void_mapper_rectangle_t potential[potential_size];

    vec_len--; /* Avoid iterating last element, as the loop accesses i + 1 */
    for (uint32_t i = 0; i < vec_len; i++) {
        for(uint32_t j = 0; j < vec_len; j++) {
            uint32_t potential_index = i + j * (vec_len);
            potential[potential_index] = (void_mapper_rectangle_t) {
                .position.x = x_vector[i],
                .position.y = y_vector[j],
                .size.x = x_vector[i + 1] - x_vector[i],
                .size.y = y_vector[j + 1] - y_vector[j]
            };
        }
    }

    // Cull out all rectangles that is within the boxes
    int32_t n_found = 0;
    for (int i = 0; i < potential_size; i ++)
    {
        bool save = true;
        for (int j = 0; j < boxes.size; j++)
        {
            save = rectangles_intersect(potential[i], boxes.buffer[j]) == true ? false : save;
        }
        if (save) {
            buffer[n_found++] = potential[i];
        }
    }

    return (void_mapper_rectangles_t) {
        .buffer = buffer,
        .size = n_found
    };
}
