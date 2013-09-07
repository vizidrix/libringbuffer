#ifndef _VIZ_LOCK_FREE_HELPERS_H_
#define _VIZ_LOCK_FREE_HELPERS_H_

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
 *
 *		Helper Methods
 *
 ****************************************************************************/

#define likely(condition) __builtin_expect(!!(condition), 1)
#define unlikely(condition) __builtin_expect(!!(condition), 0)

#define sleepns(nsec) 														\
	if (0 < nsec && nsec < 9999999L) {										\
		struct timespec ts;													\
		ts.tv_sec = 0;														\
		ts.tv_nsec = nsec;													\
		nanosleep(&ts, (struct timespec *) NULL);							\
	}
	
uint64_t round_up_pow_2_uint64_t(uint64_t v) {
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v |= v >> 32;
	v++;
	return v;
}

/* Compiler barrier to avoid rearranging of calls */
#define BARRIER() { asm volatile("" ::: "memory"); }

/****************************************************************************
 *
 *		/ Helper Methods
 *
 ****************************************************************************/


#ifdef __cplusplus
}
#endif


#endif /* _VIZ_LOCK_FREE_HELPERS_H_ */
