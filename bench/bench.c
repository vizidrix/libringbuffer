#include <src/ringbuffer.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#include <locale.h>


#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#include <mach/mach_time.h>
#else
#include <unistd.h>
#endif

#include <../../../github.com/vizidrix/libbenchmark/src/benchmark.h>
#include <../../../github.com/vizidrix/libbenchmark/src/benchmark.c>

void bench_claim_publish_release(B * b) {
	int i;

	rb_ringbuffer * buffer;
	uint64_t seq_num = 0;

	rb_init(&buffer, 4, 1024);

	b_start_timer(b);
	for(i = 0; i < b_count(b); b_sample(b, ++i)) {		
		rb_claim(buffer, &seq_num, 1);
		rb_publish(buffer, seq_num, 1);
		rb_release(buffer, seq_num, 1);
	}
	b_stop_timer(b);

	rb_free(&buffer);
}

void bench_claim_publish_release_x_100(B * b) {
	int i;
	int j;

	rb_ringbuffer * buffer;
	uint64_t seq_num = 0;

	rb_init(&buffer, 4, 1024);

	b_start_timer(b);
	for(i = 0; i < b_count(b); b_sample(b, ++i)) {		
		for(j = 0; j < 100; j++) {
			rb_claim(buffer, &seq_num, 1);
			rb_publish(buffer, seq_num, 1);
			rb_release(buffer, seq_num, 1);
		}
	}
	b_stop_timer(b);

	rb_free(&buffer);
}

void bench_claim_write_publish_check_release(B * b) {
	int i;

	rb_ringbuffer * buffer;
	uint64_t seq_num = 0;
	char * w_entry;
	char * r_entry;
	char data[100];

	for(i = 0; i < 100; i++) {
		data[i] = i;
	}

	rb_init(&buffer, 4, 1024);

	b_start_timer(b);
	for(i = 0; i < b_count(b); b_sample(b, ++i)) {		
		/* Writer */
		rb_claim(buffer, &seq_num, 1);
		rb_get(buffer, &w_entry, seq_num);
		memcpy(w_entry, data, 100);
		rb_publish(buffer, seq_num, 1);
		/* Reader */
		rb_get(buffer, &r_entry, seq_num);
		if(r_entry[20] != 20) {
			printf("**\n**\tError reading buffer\n**\n");
			return;
		}
		rb_release(buffer, seq_num, 1);
	}
	b_stop_timer(b);

	rb_free(&buffer);
}

void bench_claim_write_publish_check_release_x_100(B * b) {
	int i;
	int j;

	rb_ringbuffer * buffer;
	uint64_t seq_num = 0;
	char * w_entry;
	char * r_entry;
	char data[100];

	for(i = 0; i < 100; i++) {
		data[i] = i;
	}

	rb_init(&buffer, 4, 1024);

	b_start_timer(b);
	for(i = 0; i < b_count(b); b_sample(b, ++i)) {		
		for(j = 0; j < 100; j++) {
			/* Writer */
			rb_claim(buffer, &seq_num, 1);
			rb_get(buffer, &w_entry, seq_num);
			memcpy(w_entry, data, 100);
			rb_publish(buffer, seq_num, 1);
			/* Reader */
			rb_get(buffer, &r_entry, seq_num);
			if(r_entry[20] != 20) {
				printf("**\n**\tError reading buffer\n**\n");
				return;
			}
			rb_release(buffer, seq_num, 1);
		}
	}
	b_stop_timer(b);

	rb_free(&buffer);
}

int main() {
	printf("\nlibringbuffer - bench2\n\n");
	
	BENCH(1000, "Claim-Publish-Release", &bench_claim_publish_release);
	BENCH(1000, "Claim-Publish-Release_x_100", &bench_claim_publish_release_x_100);
	BENCH(1000, "Claim-W-Publish-C-Release", &bench_claim_write_publish_check_release);
	BENCH(1000, "Claim-W-Publish-C-Release_x_100", &bench_claim_write_publish_check_release_x_100);

	return 0;
}
