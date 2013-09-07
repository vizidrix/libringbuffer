#include <src/ringbuffer.h>
#include <stddef.h>
#include <stdio.h>
#include <libbenchmark.h>
//#include <../../../github.com/vizidrix/libbenchmark/src/benchmark.h>
//#include <../../../github.com/vizidrix/libbenchmark/src/benchmark.c>

void bench_claim_publish_release(B * b) {
	int i;

	b_start_timer(b);

	for(i = 0; i < b_count(b); b_sample(b, ++i)) {

	}

	b_stop_timer(b);
}

int main() {
	printf("\nlibringbuffer - bench\n\n");
	
	BENCH(1000, "Claim-Publish-Release", &bench_claim_publish_release);

	return 0;
}
