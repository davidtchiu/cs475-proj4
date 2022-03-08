	.text
	.globl	test_and_set

/**
 * test_and_set: assume it's called with test_and_set(&lock);
 * @param lock pointer to a mutex_t variable
 *
 * TODO - add a comment each line describing its action
 */
test_and_set:
	cli
	pushl	%ebx		# preserve contents of the %ebx register, which is about to be modified
	movl	8(%esp), %ebx	# TODO
	movl	$1, %eax	# TODO
	xchg	%eax, (%ebx)	# TODO
	popl	%ebx		# TODO
	sti
	ret			# TODO
