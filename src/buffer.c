#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buffer.h"

void buffer_dt_repr(buffer_dt *buf);

bool buffer_dt_alloc(buffer_dt **out, size_t n) {
    *out = malloc(sizeof(buffer_dt));
    if (*out == NULL) return false;

    (*out)->capacity = n;
    (*out)->length = 0;

    (*out)->data = malloc(n * sizeof(uint8_t));
    if ((*out)->data == NULL) return false;
    return true;
}

void buffer_dt_free(buffer_dt *buf) {
    free(buf->data);
    free(buf);
}

bool buffer_dt_push(buffer_dt *buf, uint8_t b)
{
    if (buf->length >= buf->capacity)
        return false;
    *(buf->data + buf->length++) = b;
    return true;
}

bool buffer_dt_pop(buffer_dt *buf, uint8_t *b)
{
    if (buf->length == 0)
        return false;
    b = buf->data[--(buf->length)];
    return true;
}

bool buffer_dt_set(buffer_dt *buf, size_t index, uint8_t b)
{
    if (index >= buf->length)
        return false;
    *(buf->data + index) = b;
    return true;
}

bool buffer_dt_memset(buffer_dt *buf, int ch, size_t count, size_t offset)
{
    if ((buf->length + count) >= buf->capacity)
        return false;
    memset(buf->data + offset, ch, count);
    buf->length += count;
    return true;
}

bool buffer_dt_memcpy(buffer_dt *buf, const void* src, size_t count, size_t offset)
{
    if ((buf->length + count) >= buf->capacity)
        return false;
    memcpy(buf->data + offset, src, count);
    buf->length += count;
    return true;
}

void buffer_dt_print(buffer_dt *buf)
{
    for (int i = 0; i < buf->length; i++)
        printf("%x", buf->data[i]);
    printf("\n");
}

void buffer_dt_print_string(buffer_dt *buf)
{
    for (int i = 0; i < buf->length; i++)
        printf("%c", buf->data[i]);
}

void buffer_dt_repr(buffer_dt *buf)
{
    printf("buffer_dt(%d, %d, ...)\n", buf->capacity, buf->length);
    buffer_dt_print(buf);
}
