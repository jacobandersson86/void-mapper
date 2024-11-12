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
    return  a1 >= b0 && a0 <= b1;
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

static void build_vectors(void_mapper_rectangles_t boxes, void_mapper_rectangle_t area, uint16_t *x_vector, uint16_t *y_vector, uint16_t len)
{
    x_vector[0] = area.position.x;
    x_vector[len - 1] = area.position.x + area.size.x;

    y_vector[0] = area.position.y;
    y_vector[len - 1] = area.position.y + area.size.y;

    for (int i = 0;  i < boxes.size; i++)
    {
        x_vector[i * 2 + 1] = boxes.buffer[i].position.x;
        x_vector[i * 2 + 2] = boxes.buffer[i].position.x + boxes.buffer[i].size.x;
        y_vector[i * 2 + 1] = boxes.buffer[i].position.y;
        y_vector[i * 2 + 2] = boxes.buffer[i].position.y + boxes.buffer[i].size.y;
    }
}

void merge(uint16_t *arr, uint16_t start_a, uint16_t end_a, uint16_t start_b, uint16_t end_b)
{
    /* Since a and b are already sorted, take the shortcut if they are already sorted */
    if (arr[end_a] <= arr[start_b])
        return;

    /* Compare the two parts one by one, from start to end */
    while (start_a <= end_a && start_b <= end_b) {

        /* If first element is on the correct spot, keep calm and carry on */
        if (arr[start_a] <= arr[start_b]) {
            start_a++;
            continue;
        }

        /* Otherwise, "start_b" should be at the head */
        uint16_t value = arr[start_b];
        uint16_t index = start_b;

        /* Overwrite "start_b" and shift each element to the right */
        while (index != start_a) {
            arr[index] = arr[index - 1];
            index--;
        }

        /* Finally put the value of "start b" at the head */
        arr[start_a] = value;

        /* Step one step forward */
        start_a++; end_a++; start_b++; end_b++;
    }
}

void merge_sort(uint16_t *arr, uint16_t left, uint16_t right)
{
    /* Recursive divide and conquer, end is reached when left and right is the same*/
    if (left == right)
        return;

    /* Divide the range in 2 */
    uint16_t middle = (left + right) / 2;
    merge_sort(arr, left, middle);
    merge_sort(arr, middle + 1, right);

    /* The two halves are sorted at this point, so just merge them knowing they are in numerical order */
    merge(arr, left, middle, middle + 1, right);
}

static void sort_vector(uint16_t *vector, uint16_t len)
{
    merge_sort(vector, 0, len - 1);
}

static uint16_t remove_duplicates(uint16_t *arr, uint16_t len)
{
    uint16_t head = 0;
    while (head < len) {
        if (arr[head] != arr[head + 1]) {
            head++;
            continue;
        }

        /* Shift everything to the left, overwriting the duplicate */
        uint16_t index = head;
        while (index < len) {
            arr[index] = arr[index + 1];
            index++;
        }

        len--;
    }

    return len;
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

    uint16_t vec_len = boxes.size * 2 + 2;

    // Create, sort and cull vectors
    uint16_t x_vector[vec_len];
    uint16_t y_vector[vec_len];
    build_vectors(boxes, area, x_vector, y_vector, vec_len);

    sort_vector(x_vector, vec_len);
    sort_vector(y_vector, vec_len);

    uint16_t x_len = remove_duplicates(x_vector, vec_len);
    uint16_t y_len = remove_duplicates(y_vector, vec_len);

    // Build all possible rectangles
    uint16_t potential_size = (x_len - 1) * (y_len - 1);
    void_mapper_rectangle_t potential[potential_size];
    x_len--; y_len--; /* Avoid iterating last element, as the loop accesses i + 1 */
    for (uint32_t i = 0; i < x_len; i++) {
        for(uint32_t j = 0; j < y_len; j++) {
            uint32_t potential_index = i + j * (x_len);
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
