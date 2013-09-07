#ifndef _VIZ_LOCK_FREE_SHARED_H_
#define _VIZ_LOCK_FREE_SHARED_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __x86_64__
#warning "The program is developed for x86-64 architecture only."
#endif
#if !defined(DCACHE1_LINESIZE) || !DCACHE1_LINESIZE
#ifdef DCACHE1_LINESIZE
#undef DCACHE1_LINESIZE
#endif
#define ____cacheline_aligned	__attribute__((aligned(64)))
#else
#define ____cacheline_aligned	__attribute__((aligned(DCACHE1_LINESIZE)))
#endif

#ifdef __cplusplus
}
#endif


#endif /* _VIZ_LOCK_FREE_SHARED_H_ */
