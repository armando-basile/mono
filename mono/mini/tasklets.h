#ifndef __MONO_TASKLETS_H__
#define __MONO_TASKLETS_H__

#include "mini.h"

typedef struct {
	MonoLMF *lmf;
	gpointer top_sp;

	/* used to ensure continuations aren't done from another thread/domain */
	gsize thread_id;
	MonoDomain *domain;

	/* stores stack copies that need to be kept alive by the GC */
	MonoGHashTable *keepalive_stacks;

	/* the instruction pointer and stack to return to on Restore */
	gpointer return_ip;
	gpointer return_sp;

	/* the saved stack information */
	int stack_alloc_size;
	int stack_used_size;

	/* pointer to GC memory */
	gpointer saved_stack;
} MonoContinuation;

typedef void (*MonoContinuationRestore) (MonoContinuation *cont, int state, MonoLMF **lmf_addr);

void  mono_tasklets_init    (void) MONO_INTERNAL;
void  mono_tasklets_cleanup (void) MONO_INTERNAL;

MonoContinuationRestore mono_tasklets_arch_restore (void) MONO_INTERNAL;

#endif /* __MONO_TASKLETS_H__ */

