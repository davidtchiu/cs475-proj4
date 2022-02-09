/* clkint.s - _clkint */

#include <icu.s>
		.text
count1000:	.word	1000
		.globl	clkint
clkint:
		pushal			# DC: save general purpose registers on stack
		cli			# DC: disable interrupts
		movb	$EOI,%al	# DC: EOI (end-of-interrupt) into least significant 8 bits of %eax (%al)
		outb	%al,$OCW1_2	# DC: write out EOI to I/O port OCW1_2 (clock)

		incl	ctr1000		# DC: ctr1000 is the number of msec since boot
		subw	$1,count1000	# DC: decrement count1000 by 1
		ja	clpreem		# DC: if count1000 > 0 then call clpreem() routine
		incl	clktime		# DC: otherwise, increment clktime (secs since boot)
		movw	$1000,count1000	#       and reset count1000 to 1000

clpreem:	decl	preempt		# decrement preemption counter
		jg	clret		# DC if preempt > 0 --> do nothing and return
		call	resched		# DC Otherwise, preemption --> call resched()
clret:					# return from interrupt
		sti			# DC: enable interrupts
		popal			# DC: pop general purpose regs from stack
		iret			# DC: return
