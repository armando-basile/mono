/*
 * atomic.c:  Workarounds for atomic operations for platforms that dont have
 *	      really atomic asm functions in atomic.h
 *
 * Author:
 *	Dick Porter (dick@ximian.com)
 *
 * (C) 2002 Ximian, Inc.
 */

#include <config.h>
#include <glib.h>

#include <mono/utils/atomic.h>

#if defined (WAPI_NO_ATOMIC_ASM) || defined (BROKEN_64BIT_ATOMICS)

#include <pthread.h>

static pthread_mutex_t spin = PTHREAD_MUTEX_INITIALIZER;

#endif

#ifdef WAPI_NO_ATOMIC_ASM

static mono_once_t spin_once=MONO_ONCE_INIT;

static void spin_init(void)
{
	g_warning("Using non-atomic functions!  Expect race conditions when using process-shared handles!");
}

gint32 InterlockedCompareExchange(volatile gint32 *dest, gint32 exch,
				  gint32 comp)
{
	gint32 old;
	int ret;
	
	mono_once(&spin_once, spin_init);
	
	pthread_cleanup_push ((void(*)(void *))pthread_mutex_unlock,
			      (void *)&spin);
	ret = pthread_mutex_lock(&spin);
	g_assert (ret == 0);
	
	old= *dest;
	if(old==comp) {
		*dest=exch;
	}
	
	ret = pthread_mutex_unlock(&spin);
	g_assert (ret == 0);
	
	pthread_cleanup_pop (0);

	return(old);
}

gpointer InterlockedCompareExchangePointer(volatile gpointer *dest,
					   gpointer exch, gpointer comp)
{
	gpointer old;
	int ret;
	
	mono_once(&spin_once, spin_init);
	
	pthread_cleanup_push ((void(*)(void *))pthread_mutex_unlock,
			      (void *)&spin);
	ret = pthread_mutex_lock(&spin);
	g_assert (ret == 0);
	
	old= *dest;
	if(old==comp) {
		*dest=exch;
	}
	
	ret = pthread_mutex_unlock(&spin);
	g_assert (ret == 0);
	
	pthread_cleanup_pop (0);

	return(old);
}

gint32 InterlockedAdd(volatile gint32 *dest, gint32 add)
{
	gint32 ret;
	int thr_ret;
	
	mono_once(&spin_once, spin_init);
	
	pthread_cleanup_push ((void(*)(void *))pthread_mutex_unlock,
			      (void *)&spin);
	thr_ret = pthread_mutex_lock(&spin);
	g_assert (thr_ret == 0);

	*dest += add;
	ret= *dest;
	
	thr_ret = pthread_mutex_unlock(&spin);
	g_assert (thr_ret == 0);
	
	pthread_cleanup_pop (0);
	
	return(ret);
}

gint64 InterlockedAdd64(volatile gint64 *dest, gint64 add)
{
	gint64 ret;
	int thr_ret;
	
	mono_once(&spin_once, spin_init);
	
	pthread_cleanup_push ((void(*)(void *))pthread_mutex_unlock,
			      (void *)&spin);
	thr_ret = pthread_mutex_lock(&spin);
	g_assert (thr_ret == 0);

	*dest += add;
	ret= *dest;
	
	thr_ret = pthread_mutex_unlock(&spin);
	g_assert (thr_ret == 0);
	
	pthread_cleanup_pop (0);
	
	return(ret);
}

gint32 InterlockedIncrement(volatile gint32 *dest)
{
	gint32 ret;
	int thr_ret;
	
	mono_once(&spin_once, spin_init);
	
	pthread_cleanup_push ((void(*)(void *))pthread_mutex_unlock,
			      (void *)&spin);
	thr_ret = pthread_mutex_lock(&spin);
	g_assert (thr_ret == 0);

	(*dest)++;
	ret= *dest;
	
	thr_ret = pthread_mutex_unlock(&spin);
	g_assert (thr_ret == 0);
	
	pthread_cleanup_pop (0);
	
	return(ret);
}

gint64 InterlockedIncrement64(volatile gint64 *dest)
{
	gint64 ret;
	int thr_ret;
	
	mono_once(&spin_once, spin_init);
	
	pthread_cleanup_push ((void(*)(void *))pthread_mutex_unlock,
			      (void *)&spin);
	thr_ret = pthread_mutex_lock(&spin);
	g_assert (thr_ret == 0);

	(*dest)++;
	ret= *dest;
	
	thr_ret = pthread_mutex_unlock(&spin);
	g_assert (thr_ret == 0);
	
	pthread_cleanup_pop (0);
	
	return(ret);
}

gint32 InterlockedDecrement(volatile gint32 *dest)
{
	gint32 ret;
	int thr_ret;
	
	mono_once(&spin_once, spin_init);
	
	pthread_cleanup_push ((void(*)(void *))pthread_mutex_unlock,
			      (void *)&spin);
	thr_ret = pthread_mutex_lock(&spin);
	g_assert (thr_ret == 0);
	
	(*dest)--;
	ret= *dest;
	
	thr_ret = pthread_mutex_unlock(&spin);
	g_assert (thr_ret == 0);
	
	pthread_cleanup_pop (0);
	
	return(ret);
}

gint64 InterlockedDecrement64(volatile gint64 *dest)
{
	gint64 ret;
	int thr_ret;
	
	mono_once(&spin_once, spin_init);
	
	pthread_cleanup_push ((void(*)(void *))pthread_mutex_unlock,
			      (void *)&spin);
	thr_ret = pthread_mutex_lock(&spin);
	g_assert (thr_ret == 0);
	
	(*dest)--;
	ret= *dest;
	
	thr_ret = pthread_mutex_unlock(&spin);
	g_assert (thr_ret == 0);
	
	pthread_cleanup_pop (0);
	
	return(ret);
}

gint32 InterlockedExchange(volatile gint32 *dest, gint32 exch)
{
	gint32 ret;
	int thr_ret;
	
	mono_once(&spin_once, spin_init);
	
	pthread_cleanup_push ((void(*)(void *))pthread_mutex_unlock,
			      (void *)&spin);
	thr_ret = pthread_mutex_lock(&spin);
	g_assert (thr_ret == 0);

	ret=*dest;
	*dest=exch;
	
	thr_ret = pthread_mutex_unlock(&spin);
	g_assert (thr_ret == 0);
	
	pthread_cleanup_pop (0);
	
	return(ret);
}

gint64 InterlockedExchange64(volatile gint64 *dest, gint64 exch)
{
	gint64 ret;
	int thr_ret;
	
	mono_once(&spin_once, spin_init);
	
	pthread_cleanup_push ((void(*)(void *))pthread_mutex_unlock,
			      (void *)&spin);
	thr_ret = pthread_mutex_lock(&spin);
	g_assert (thr_ret == 0);

	ret=*dest;
	*dest=exch;
	
	thr_ret = pthread_mutex_unlock(&spin);
	g_assert (thr_ret == 0);
	
	pthread_cleanup_pop (0);
	
	return(ret);
}

gpointer InterlockedExchangePointer(volatile gpointer *dest, gpointer exch)
{
	gpointer ret;
	int thr_ret;
	
	mono_once(&spin_once, spin_init);
	
	pthread_cleanup_push ((void(*)(void *))pthread_mutex_unlock,
			      (void *)&spin);
	thr_ret = pthread_mutex_lock(&spin);
	g_assert (thr_ret == 0);
	
	ret=*dest;
	*dest=exch;
	
	thr_ret = pthread_mutex_unlock(&spin);
	g_assert (thr_ret == 0);
	
	pthread_cleanup_pop (0);
	
	return(ret);
}

gint32 InterlockedExchangeAdd(volatile gint32 *dest, gint32 add)
{
	gint32 ret;
	int thr_ret;
	
	mono_once(&spin_once, spin_init);
	
	pthread_cleanup_push ((void(*)(void *))pthread_mutex_unlock,
			      (void *)&spin);
	thr_ret = pthread_mutex_lock(&spin);
	g_assert (thr_ret == 0);

	ret= *dest;
	*dest+=add;
	
	thr_ret = pthread_mutex_unlock(&spin);
	g_assert (thr_ret == 0);

	pthread_cleanup_pop (0);

	return(ret);
}

#define NEED_64BIT_CMPXCHG 1

#endif

#if defined(BROKEN_64BIT_ATOMICS)

#if defined (TARGET_MACH) && defined (__arm__)

#if !defined (HAVE_ARMV7)
#include <mono/utils/mono-hwcap-arm.h>
#endif

gint64
InterlockedCompareExchange64_asm(volatile gint64 *dest, gint64 exch, gint64 comp) __attribute__ ((naked));

gint64
InterlockedCompareExchange64_asm(volatile gint64 *dest, gint64 exch, gint64 comp)
{
	__asm__ (
	"push {r4, r5, r6, r7}\n"
	"ldr r4, [sp, #16]\n"
	"dmb\n"
"1:\n"
	"ldrexd	r6, r7, [r0]\n"
	"cmp	r7, r4\n"
	"bne 2f\n"
	"cmp	r6, r3\n"
	"bne	2f\n"
	"strexd	r5, r1, r2, [r0]\n"
	"cmp	r5, #0\n"
	"bne	1b\n"
"2:\n"
	"dmb\n"
	"mov	r0, r6\n"
	"mov	r1, r7\n"
	"pop {r4, r5, r6, r7}\n"
	"bx	lr\n"
	);
}

gint64
InterlockedCompareExchange64(volatile gint64 *dest, gint64 exch, gint64 comp)
{
	/* HACK: Because iOS does *not* align 64-bit integers on an 8-byte
	   boundary, we have to fall back to the lock path in the unaligned
	   case. This may change in the future if we align managed longs to
	   8 bytes. */
	if ((size_t) location & 0x7) {
	lock_path:

		gint64 old;

		pthread_mutex_lock (&spin);

		old = *dest;
		if(old == comp)
			*dest = exch;

		pthread_mutex_unlock (&spin);
		return old;
	}

	/* Now for some fun static and dynamic platform detection... */
#if defined (HAVE_ARMV7)
	return InterlockedCompareExchange64_asm (location, exch, comp);
#else
	if (mono_hwcap_arm_is_v7) {
		return InterlockedCompareExchange64_asm (location, exch, comp);
	} else {
		goto lock_path;
	}
#endif
}

#elif defined (TARGET_MACH) && (defined (__i386__) || defined(__x86_64__))

gint64
InterlockedCompareExchange64(volatile gint64 *dest, gint64 exch, gint64 comp)
{
	return __sync_val_compare_and_swap (dest, comp, exch);
}

#else

#define NEED_64BIT_CMPXCHG 1

#endif
#endif

#if defined(NEED_64BIT_CMPXCHG)

gint64
InterlockedCompareExchange64(volatile gint64 *dest, gint64 exch, gint64 comp)
{
	gint64 old;

	pthread_mutex_lock (&spin);

	old = *dest;
	if(old == comp)
		*dest = exch;

	pthread_mutex_unlock (&spin);
	return old;
}

#endif
