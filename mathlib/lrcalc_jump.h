#ifndef _JUMP_H

#include <setjmp.h>

/*  Programs using the lrcalc library should set lrcalc_panic_frame
 *  with setjmp(lrcalc_panic_frame).  The lrcalc library will call
 *  longjmp(lrcalc_panic_frame, 1) if an "out of memory" event occurs.
 */
jmp_buf lrcalc_panic_frame;

#endif
