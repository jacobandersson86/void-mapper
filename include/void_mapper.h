#ifndef __VOID_MAPPER_H__
#define __VOID_MAPPER_H__

#include <stdint.h>

typedef uint16_t void_mapper_uint_t;
typedef int16_t void_mapper_int_t;

typedef struct {
    struct {
        void_mapper_int_t x;
        void_mapper_int_t y;
    } position;
    struct void_mapper
    {
        void_mapper_uint_t x;
        void_mapper_uint_t y;
    } size;
} void_mapper_rectangle_t;

typedef struct {
    void_mapper_rectangle_t * buffer;
    uint16_t size;
} void_mapper_rectangles_t;

/**
 * @brief Void mapper returns a list of rectangles to fill all the void between the boxes.
 * It will search and find all the empty spaces within the area, returning a list of
 * rectangles that will cover all the void spaces within the area.
 *
 * No dynamic allocation is used, but void mapper needs a buffer for returning the
 * result. In other terms, the same buffer passed as an argument will be returned,
 * but note that the size is updated to reflect the resulting number for rectangles.
 *
 * @param boxes The non void areas
 * @param area Area to search
 * @param buffer For storage of the result
 * @param size Size of the buffer
 * @return void_mapper_rectangles_t
 */
void_mapper_rectangles_t void_mapper(void_mapper_rectangles_t boxes, void_mapper_rectangle_t area, void_mapper_rectangle_t * buffer, uint16_t size);

#endif /* __VOID_MAPPER_H__ */
