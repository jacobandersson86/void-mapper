#include <stdlib.h>
#include "void_mapper.h"

void_mapper_rectangles_t void_mapper(void_mapper_rectangles_t boxes, void_mapper_rectangle_t area,
                                     void_mapper_rectangle_t * buffer, uint16_t size)
{
    (void) boxes;
    (void) area;
    (void) buffer;
    (void) size;

    return (void_mapper_rectangles_t) {
        .buffer = NULL,
        .size = 0
    };
}
