#include <stdio.h>
#include "strbuf.h"

int main()
{
	strbuf s = strbuf_new("prefix: ");

	printf("%s\n", strbuf_cstr(&s));
	printf("strbuf_cstr(&s): %p\n", strbuf_cstr(&s));
	printf("sizeof(strbuf): %zu\n", sizeof(s));
	printf("strbuf_len(s): %zu\n", strbuf_len(s));

	for (int i = 0; i < 257; ++i)
		strbuf_append(&s, "a");

	printf("\n%s\n", strbuf_cstr(&s));
	printf("strbuf_cstr(&s): %p\n", strbuf_cstr(&s));
	printf("sizeof(strbuf): %zu\n", sizeof(s));
	printf("strbuf_len(s): %zu\n", strbuf_len(s));

	strbuf_destroy(s);

	return 0;
}
