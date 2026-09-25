/*
 *	_psstart.c	- Startup Routine for the Sony PlayStation
 *
 *	Copyright (c) 1996 metrowerks inc. All Rights Reserved.
 *
 */
 
#include <__rts_info_t__.h>

#ifdef __cplusplus
extern "C" void InitHeap();
#else
extern void InitHeap();
#endif

extern void main();

#ifdef __cplusplus
extern "C" void __start(void);
#else
asm void __start(void);
#endif

static void __start_c(void);

SECTION extern unsigned long _gp_addr;

asm void __start(void) 
{

	.set	noreorder
	
	/* Initialize all the registers to 0*/
	move	t0,zero

	.set	reorder

	move	v0,t0
	move	v1,t0
	
	move	a0,t0
	move	a1,t0
	move	a2,t0
	move	a3,t0

	move	t1,t0
	move	t2,t0
	move	t3,t0
	move	t4,t0
	move	t5,t0
	move	t6,t0
	move	t7,t0
	move	t8,t0
	move	t9,t0
	
	move	s0,t0
	move	s1,t0
	move	s2,t0
	move	s3,t0
	move	s4,t0
	move	s5,t0
	move	s6,t0
	move	s7,t0
	move	fp,t0	// s8

	move	k0,t0
	move	k1,t0

	/* Initialize sp										*/
	lw		sp,_stack_addr
	/* round down to even multiple of 16 */
	li		t0, 0xFFFFFFF0
	and		sp,sp,t0		
	
	
	/* Set the global pointer.  _gp_addr is know by the 			*/
	/* linker and the assembler								*/
	lw		gp,_gp_addr	

	/* Initialize the .sbss section to 0					*/
	lw		t0,_fsbss		/* .sbss address				*/
	lw		t1,_sbss_size	/* .sbss size					*/
	b		L1				/* Will bump t0					*/
L2:
	sw		zero,-4(t0)		/* t0 is 4 bytes ahead			*/
	addi	t1,t1,-4		
L1:							
	addiu	t0,t0,4
	bgt		t1,0,L2			/* size is a word size			*/

	
	/* Initialize the .bss section to 0						*/
	lw		t0,_fbss		/* .bss address					*/
	lw		t1,_bss_size	/* .bss size					*/
	b		L3				/* Will bump t0					*/
L4:
	sw		zero,-4(t0)		/* t0 is 4 bytes ahead			*/
	addi	t1,t1,-4
L3:		
	addiu	t0,t0,4
	bgt		t1,0,L4			/* size is a word size			*/

/* #pragma warning Note! Please read the comment on line 100 of _psstart.c! */
/*  The following note applies to anyone upgrading source from
    CodeWarrior for Net Yaroze 1 to CodeWarrior for Net Yaroze 2
    or from CodeWarrior for PlayStation 1,2 to CodeWarrior for PlayStation 3.
    If you are simply building the sample code, you can
    ignore this message.  If you are upgrading your code to
    work with the newest release, please read on.
*/    

/*	According to Sony, InitHeap should be called in main(), not in
	_psstart.c. Thus it has been removed from this function.
	
	InitHeap is now only called in _psstart.c if using C++
	(to allow static initializers to complete)

    If your code worked fine with a previous version of CodeWarrior  and 
    you call malloc() or new() in your program, you MUST add the following
   	code to your main() routine or else your code will crash when using
   	the newest release of CodeWarrior.

	at the top of main.c add
	
	#include <__rts_info_t__.h>

	in your main() function, add
	--------
	// heap starts after the main program on a 16-byte boundary
	unsigned long	lclHeapStart=(_end & 0xFFFFFFF0L)+0x10;

	// heap extends to the bottom of the stack.
	unsigned long lclHeapSize=((_stack_addr-(_stack_size<<10)) &
0xFFFFFFF0L)-lclHeapStart;

	InitHeap((unsigned long *)lclHeapStart, lclHeapSize);
	--------

    You can delete the #pragma warning statement above once
    you fully understand the contents of this comment. Otherwise, just 
    leave it there, it won't hurt anything.
*/
#ifdef __cplusplus

	/* Set the heap size and base address					*/
	lw		a0,_heap_size
	/* We put the heap after the .bss section				*/
	lw		a0,_fbss		/* Base of the .bss section		*/
	lw		a2,_bss_size
	addu	a0,a0,a2		/* calculate end of .bss section */

	addi	a0,a0,15		/* round up to even multiple of 16 */
	addiu	a2,zero,-16
	and		a0,a0,a2

	lw		a1,_stack_addr
	lw		a2,_stack_size
	sll		a2,a2,10
	subu	a1,a1,a2		/* calculate bottom of stack */
	subu	a1,a1,a0		/* find distance from top of .bss to bottom of stack */

	jal		InitHeap		/* call InitHeap to set it up */
	nop
#endif

	/* last but not least, set up for co-pro 2 */
	mfc0	t0,12
	lui		t1,0x4000
	or		t0,t0,t1
	mtc0	t0,12

	/* Push RA from the launcher onto the stack				*/
	addiu	sp, sp, -8
	sw		ra, 8(sp)
	
	/* Call the main program								*/
	
	jal		__start_c
	nop


	breakc	0x8201
	nop
	/* Pull RA from the stack so that we can return			*/
	/* to the launcher										*/
	/* (note: this does not return to the Net Yaroze 		*/
	/* "brick screen". However, if this program were called	*/
	/* from a demo disc, it needs to terminate this way		*/
	
	lw		ra, 8(sp)
	addiu	sp, sp, 8
	jr		ra
	nop
}

static void __start_c(void)
{
	/* The purpose of this routine is to create the fisrt	*/
	/* stack frame.											*/

#ifdef __cplusplus
	/* C++ static initializers.  The symbol __static_init is	*/
	/* defined by the linker.								*/
    void (**si)(void);

	for (si=__static_init; *si; si++) 
		(*si)();
#endif

	main();
}

