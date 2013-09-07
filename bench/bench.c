#include <src/ringbuffer.h>
#include <stddef.h>
#include <stdio.h>
#include <libbenchmark.h>
//#include <../../../github.com/vizidrix/libbenchmark/src/benchmark.h>
//#include <../../../github.com/vizidrix/libbenchmark/src/benchmark.c>

void bench_claim_publish_release(B * b) {
	int i;

	rb_ringbuffer * buffer;
	uint64_t seq_num = 0;
	//char * entry;

	rb_init(&buffer, 4, 1024);

	b_start_timer(b);
	for(i = 0; i < b_count(b); b_sample(b, ++i)) {		
		rb_claim(buffer, &seq_num, 1);
		rb_get(buffer, &entry, seq_num);
		rb_publish(buffer, seq_num, 1);
		rb_release(buffer, seq_num, 1);
	}
	b_stop_timer(b);

	rb_free(&buffer);
}

int main() {
	printf("\nlibringbuffer - bench2\n\n");
	
	BENCH(1000, "Claim-Publish-Release", &bench_claim_publish_release);

	return 0;
}
