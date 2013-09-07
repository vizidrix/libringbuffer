#ifndef _VIZ_RINGBUFFER_H_
#define _VIZ_RINGBUFFER_H_

#ifdef __cplusplus
extern "C" {
#endif	
/*
#include <stdarg.h>
#include <stdlib.h>

#include <stdio.h>
#include <time.h>
*/
#include <stdint.h>
#include "lockfree_shared.h"


typedef struct rb_ringbuffer {
	uint64_t			ring_size;								/** < Number of slots allocated per the rules above */
	uint64_t			ring_mask;								/** < Data buffer size - 1; Used to keep data inside the ring */
	uint64_t			buffer_size;							/** < Number of bytes of data for each slot */

	volatile uint64_t	write_head 		____cacheline_aligned;	/** < Index of the next available sequence to be assigned to a writer */
	volatile uint64_t	read_head 		____cacheline_aligned;	/** < Index of the newest sequence which has been released to readers */
	volatile uint64_t	read_tail 		____cacheline_aligned;	/** < Index of the oldest sequence that is still in use by at least one reader */
	
	char *				entries;								/** < Pre-allocated data buffer */
} rb_ringbuffer;

#define RB_SUCCESS 						0 						/** < Successful result */
#define RB_ERROR						(-40600)				/** < Generic error */

#define RB_ALLOC						(RB_ERROR - 100) 		/** < There was an error allocating the buffer */

#define RB_COUNT_ZERO					(RB_ERROR - 200)		/** < Operation requested zero count */
#define RB_COUNT_OVERFLOW				(RB_ERROR - 201)		/** < Operation requested count too big for ring */
#define RB_CLAIM_OVERRUN				(RB_ERROR - 202)		/** < Requested claim set would cause write head to over run the  read tail */
#define RB_PUBLISH_INVALID				(RB_ERROR - 203)		/** < Operation requested for invalid position */
#define RB_PUBLISH_RACE					(RB_ERROR - 204)		/** < Operation had already been completed */
#define RB_PUBLISH_OVERRUN				(RB_ERROR - 205)		/** < Requested publish set would cause  read head to over run the write head */
#define RB_RELEASE_INVALID				(RB_ERROR - 206)		/** < Operation requested for invalid position */
#define RB_RELEASE_RACE					(RB_ERROR - 207)		/** < Operation had already been completed */
#define RB_RELEASE_OVERRUN				(RB_ERROR - 208)		/** < Requested release set would cause  read tail to over run the  read head */

extern int rb_init(rb_ringbuffer ** buffer_ptr, uint64_t data_buffer_size, uint64_t data_size);
extern int rb_free(rb_ringbuffer ** buffer_ptr);

extern int rb_get(rb_ringbuffer * buffer, char ** entry, uint64_t seq_num);

extern int rb_claim(rb_ringbuffer * buffer, uint64_t * seq_num, uint64_t count);
extern int rb_publish(rb_ringbuffer * buffer, uint64_t seq_num, uint64_t count);
extern int rb_release(rb_ringbuffer * buffer, uint64_t seq_num, uint64_t count);

#ifdef __cplusplus
}
#endif

#endif /* _VIZ_RINGBUFFER_H_ */
