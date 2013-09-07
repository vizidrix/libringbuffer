#include <src/ringbuffer.h>
#include <stddef.h>
#include <stdio.h>

void ring_example() {
	rb_ringbuffer * buffer;
	uint64_t seq_num = 0;
	char * entry;

	rb_init(&buffer, 4, 1024);
	rb_claim(buffer, &seq_num, 1);
	rb_get(buffer, &entry, seq_num);
	rb_publish(buffer, seq_num, 1);
	rb_release(buffer, seq_num, 1);
	rb_free(&buffer);
}

int main() {
	printf("\nlibringbuffer - example\n");

	ring_example();

	return 0;
}
