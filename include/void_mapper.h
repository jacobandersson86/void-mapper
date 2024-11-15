#ifndef __VOID_MAPPER_H__
#define __VOID_MAPPER_H__

#include <stdint.h>

typedef struct {
    struct {
        uint16_t x;
        uint16_t y;
    } position;
    struct
    {
        uint16_t x;
        uint16_t y;
    } size;
} void_mapper_rectangle_t;

/**
 * @brief Void mapper returns a list of rectangles to fill all the void between the boxes.
 * It will search and find all the empty spaces within the area, returning a list of
 * rectangles that will cover all the void spaces within the area.
 *
 * No dynamic allocation is used, but void mapper needs a buffer for returning the
 * result. In other terms, the same buffer passed as an argument will return the found voids,
 * but note that the length to be used is returned from the function.
 *
 * @param area Area to search
 * @param input Array of the non void areas
 * @param input_length Number of elements of the input array
 * @param buffer For storage of the result
 * @param buffer_length Number of the elements in the buffer
 * @return Number of voids found. (The voids are in the buffer)
 */
uint16_t void_mapper(void_mapper_rectangle_t area, void_mapper_rectangle_t *input, uint16_t input_length, void_mapper_rectangle_t * buffer, uint16_t buffer_length);

#endif /* __VOID_MAPPER_H__ */
