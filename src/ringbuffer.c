#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
/*#include <assert.h>
#include <time.h>*/
/*#include <errno.h>
#include <sched.h>*/
#include <inttypes.h>

#include "ringbuffer.h"
#include "lockfree_helpers.h"

int
rb_init(rb_ringbuffer ** buffer_ptr, uint64_t ring_size, uint64_t buffer_size) {
	/* Normalize the ring size into a 2s complement */
	ring_size = round_up_pow_2_uint64_t(ring_size);
	/* Allocate the buffer struct and room for the buffer */
	//*buffer_ptr = malloc(sizeof(rb_ringbuffer));
	*buffer_ptr = malloc
		(sizeof(rb_ringbuffer) + 
		//(sizeof(char*) * ring_size) + 
		(sizeof(char) * (ring_size * buffer_size)));

	if(!*buffer_ptr) {
		return RB_ALLOC;
	}
	/* Populate the struct */
	(*buffer_ptr)->ring_size = ring_size;
	(*buffer_ptr)->ring_mask = ring_size - 1;
	(*buffer_ptr)->buffer_size = buffer_size;
	(*buffer_ptr)->write_head = 0;
	(*buffer_ptr)->read_head = 0;
	(*buffer_ptr)->read_tail = 0;
	(*buffer_ptr)->entries = (char*)*buffer_ptr+sizeof(rb_ringbuffer);

	uint64_t i, j;
	for(i = 0; i < (*buffer_ptr)->ring_size; i++) {
		for(j = 0; j < (*buffer_ptr)->buffer_size; j++) {
			(*buffer_ptr)->entries[(i*(*buffer_ptr)->buffer_size) + j] = i;
		}
	}

	/*
	printf("&Size:   \t%p\n", (void *)&(*buffer_ptr)->ring_size);
	printf("&Mask:   \t%p\n", (void *)&(*buffer_ptr)->ring_mask);
	printf("&Buffer: \t%p\n", (void *)&(*buffer_ptr)->buffer_size);
	printf("&Write H:\t%p\n", (void *)&(*buffer_ptr)->write_head);
	printf("&Read  H:\t%p\n", (void *)&(*buffer_ptr)->read_head);
	printf("&Read  T:\t%p\n", (void *)&(*buffer_ptr)->read_tail);
	printf("&Entries:\t%p\n", (void *)&(*buffer_ptr)->entries);

	printf("Size:   \t%p\n", (void *)(*buffer_ptr)->ring_size);
	printf("Mask:   \t%p\n", (void *)(*buffer_ptr)->ring_mask);
	printf("Buffer: \t%p\n", (void *)(*buffer_ptr)->buffer_size);
	printf("Write H:\t%p\n", (void *)(*buffer_ptr)->write_head);
	printf("Read  H:\t%p\n", (void *)(*buffer_ptr)->read_head);
	printf("Read  T:\t%p\n", (void *)(*buffer_ptr)->read_tail);
	printf("Entries:\t%p\n", (void *)(*buffer_ptr)->entries);
	*/

	return RB_SUCCESS;
}

int
rb_free(rb_ringbuffer ** buffer_ptr) {
	if(*buffer_ptr != NULL) {
		(*buffer_ptr)=(free(*buffer_ptr),NULL);
	}
	return RB_SUCCESS;
}

int
rb_get(rb_ringbuffer * buffer, char ** entry, uint64_t seq_num) {
	*entry = &buffer->entries[(seq_num & buffer->ring_size)*buffer->buffer_size];
	return RB_SUCCESS;
}

int
rb_claim(rb_ringbuffer * buffer, uint64_t * seq_num, uint64_t count) {
	if(unlikely(count == 0)) { return RB_COUNT_ZERO; }
	if(unlikely(count > buffer->ring_size)) { return RB_COUNT_OVERFLOW; }

	/* Loops until either a slot is claimed or an overrun occurs */
	for(*seq_num = buffer->write_head;
		/* Overrun should only occur in the event of buffer backup */
		unlikely(*seq_num+count >= buffer->read_tail + buffer->ring_size) ||
		/* Compete with other claimants until you win */
		!__sync_bool_compare_and_swap(&buffer->write_head, *seq_num, *seq_num+count);
		/* You lost the update race, move to the new position */
		*seq_num = buffer->write_head) {
		/* If the fail was due to lack of room then bail early */
		if(unlikely(*seq_num+count >= buffer->read_tail + buffer->ring_size)) {
			/* Expect caller to implement backoff on overrun */
			return RB_CLAIM_OVERRUN;
		}
	}
	return RB_SUCCESS;
}

int
rb_publish(rb_ringbuffer * buffer, uint64_t seq_num, uint64_t count) {
	if(unlikely(count == 0)) { return RB_COUNT_ZERO; }
	if(unlikely(count > buffer->ring_size)) { return RB_COUNT_OVERFLOW; }

	/* Should only occur with poorly behaving client */
	if(unlikely(seq_num != buffer->read_head)) {
		return RB_PUBLISH_INVALID;
	}
	/* Make sure publish doesn't overlap into unwritten space */
	if(unlikely(seq_num+count >= buffer->write_head)) {
		return RB_PUBLISH_OVERRUN;
	}
	/* If above have passed then this should be the correct owner and op should proceed */
	if(unlikely(!__sync_bool_compare_and_swap(&buffer->read_head, seq_num, seq_num+count))) {
		return RB_PUBLISH_RACE;
	}
	return RB_SUCCESS;
}

int
rb_release(rb_ringbuffer * buffer, uint64_t seq_num, uint64_t count) {
	if(unlikely(count == 0)) { return RB_COUNT_ZERO; }
	if(unlikely(count > buffer->ring_size)) { return RB_COUNT_OVERFLOW; }

	/* Should only occur with poorly behaving client */
	if(unlikely(seq_num != buffer->read_tail)) {
		return RB_RELEASE_INVALID;
	}
	/* Make sure publish doesn't overlap into unwritten space */
	if(unlikely(seq_num+count >= buffer->read_head)) {
		return RB_RELEASE_OVERRUN;
	}
	/* If above have passed then this should be the correct owner and op should proceed */
	if(unlikely(!__sync_bool_compare_and_swap(&buffer->read_tail, seq_num, seq_num+count))) {
		return RB_RELEASE_RACE;
	}
	return RB_SUCCESS;
}

