#ifndef BUFFER_DT_H
#define BUFFER_DT_H

#include <stdbool.h>

struct buffer {
    size_t length;
    size_t capacity;
    uint8_t *data;
};
typedef struct buffer buffer_dt;

bool buffer_dt_alloc(buffer_dt **out, size_t n);
void buffer_dt_free(buffer_dt *buf);

bool buffer_dt_push(buffer_dt *buf, uint8_t b);
bool buffer_dt_pop(buffer_dt *buf, uint8_t *b);
bool buffer_dt_set(buffer_dt *buf, size_t index, uint8_t b);
bool buffer_dt_memset(buffer_dt *buf, int ch, size_t count, size_t offset);
bool buffer_dt_memcpy(buffer_dt *buf, const void* src, size_t count, size_t offset);

void buffer_dt_print(buffer_dt *buf);
void buffer_dt_print_string(buffer_dt *buf);
void buffer_dt_repr(buffer_dt *buf);

#endif /* BUFFER_DT_H */
