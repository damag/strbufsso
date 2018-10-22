/*
 * String buffer with small string optimizations
 *
 * The string is always NUL-terminated so that it can be used with the
 * standard string functions.
 */

#pragma once

#include <stddef.h>
#include <stdint.h>

#define SMALL_BUF_SIZE (sizeof(char *) + sizeof(size_t) * 2 - 1)

typedef union {
	struct {
		char *ptr;
		size_t len;
		size_t alloc : sizeof(size_t) * 8 - 1;
		size_t is_big : 1;
	};
	struct {
		char buf[SMALL_BUF_SIZE];
		uint8_t space_left : 7;
	};
} strbuf;

void strbuf_init(strbuf *s);
strbuf strbuf_newl(const char *src, size_t len);
strbuf strbuf_new(const char *src);
void strbuf_setl(strbuf *s, const char *src, size_t len);
void strbuf_set(strbuf *s, const char *src);
size_t strbuf_len(strbuf s);
void strbuf_appendl(strbuf *s, const char *src, size_t len);
void strbuf_append(strbuf *s, const char *src);
char *strbuf_cstr(const strbuf *s);
void strbuf_destroy(strbuf s);
void strbuf_free(strbuf *s);
