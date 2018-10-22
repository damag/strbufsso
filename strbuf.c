#define _GNU_SOURCE
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "strbuf.h"

void strbuf_init(strbuf *s)
{
	s->buf[0] = '\0';
	s->space_left = SMALL_BUF_SIZE;
	s->is_big = 0;
}

static void strbuf_alloc_and_set(strbuf *s, const char *src, size_t len)
{
	size_t alloc = 1;
	while (alloc < len + 1)
		alloc *= 2;
	s->ptr = malloc(alloc);
	*(char *)mempcpy(s->ptr, src, len) = '\0';
	s->len = len;
	s->alloc = alloc;
	s->is_big = 1;
}

strbuf strbuf_newl(const char *src, size_t len)
{
	strbuf s;
	if (len <= SMALL_BUF_SIZE) {
		*(char *)mempcpy(&s.buf, src, len) = '\0';
		s.space_left = SMALL_BUF_SIZE - len;
		s.is_big = 0;
	} else {
		strbuf_alloc_and_set(&s, src, len);
	}
	return s;
}

strbuf strbuf_new(const char *src)
{
	return strbuf_newl(src, strlen(src));
}

void strbuf_setl(strbuf *s, const char *src, size_t len)
{
	if (s->is_big) {
		*(char *)mempcpy(s->ptr, src, len) = '\0';
		s->len = len;
	} else if (len <= SMALL_BUF_SIZE) {
		*(char *)mempcpy(&s->buf, src, len) = '\0';
		s->space_left = SMALL_BUF_SIZE - len;
	} else {
		strbuf_alloc_and_set(s, src, len);
	}
}

void strbuf_set(strbuf *s, const char *src)
{
	strbuf_setl(s, src, strlen(src));
}

size_t strbuf_len(strbuf s)
{
	return s.is_big ? s.len : SMALL_BUF_SIZE - s.space_left;
}

static void strbuf_grow(strbuf *s, size_t need)
{
	assert(s->is_big);

	if (s->alloc >= need)
		return;

	size_t newalloc = s->alloc ? s->alloc : 1;
	while (newalloc < need)
		newalloc *= 2;
	s->ptr = realloc(s->ptr, newalloc);
	s->alloc = newalloc;
}

void strbuf_appendl(strbuf *s, const char *src, size_t len)
{
	size_t curlen = strbuf_len(*s);
	size_t newlen = curlen + len;

	if (s->is_big) {
		strbuf_grow(s, newlen + 1);
		*(char *)mempcpy(s->ptr + curlen, src, len) = '\0';
		s->len = newlen;
	} else if (newlen <= SMALL_BUF_SIZE) {
		*(char *)mempcpy(&s->buf[curlen], src, len) = '\0';
		s->space_left = SMALL_BUF_SIZE - newlen;
	} else {
		size_t alloc = 1;
		while (alloc < newlen + 1)
			alloc *= 2;
		char *p = malloc(alloc);
		memcpy(p, &s->buf, curlen);
		*(char *)mempcpy(p + curlen, src, len) = '\0';
		s->ptr = p;
		s->len = newlen;
		s->alloc = alloc;
		s->is_big = 1;
	}
}

void strbuf_append(strbuf *s, const char *src)
{
	strbuf_appendl(s, src, strlen(src));
}

char *strbuf_cstr(const strbuf *s)
{
	return s->is_big ? s->ptr : (char *)&s->buf;
}

void strbuf_destroy(strbuf s)
{
	if (s.is_big)
		free(s.ptr);
}

void strbuf_free(strbuf *s)
{
	strbuf_destroy(*s);
	free(s);
}
