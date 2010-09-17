/*
 * This file contains useful redefinition of bug() macro which uses
 * kernel.resource's own debugging facilities. Include it if you
 * need bug() in your code.
 */

#include <aros/asmcall.h>

#ifdef bug
#undef bug
#endif

AROS_LD2(int, KrnBug,
         AROS_LDA(const char *, format, A0),
         AROS_LDA(va_list, args, A1),
         struct KernelBase *, KernelBase, 12, Kernel);

static inline void _bug(struct KernelBase *KernelBase, const char *format, ...)
{
    va_list args;

    va_start(args, format);

    /* We call the function directly, not using vector table,
       because KernelBase can be NULL here (during very early
       startup) */
    AROS_UFC3(int, AROS_SLIB_ENTRY(KrnBug, Kernel),
	      AROS_UFCA(const char *, format, A0),
	      AROS_UFCA(va_list, args, A1),
	      AROS_UFCA(struct KernelBase *, KernelBase, A6));

    va_end(args);
}

#define bug(...) _bug(KernelBase, __VA_ARGS__)

/*
 * Character output function. All debug output end up there.
 * This function is needed to be implemented for every supported
 * architecture.
 */
int krnPutC(int chr, struct KernelBase *KernelBase);
