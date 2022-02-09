	.text
	.globl	test_and_set

/**
 * test_and_set: assume it's called with test_and_set(&lock);
 * @author Jose Rodriguez (LIU - Brooklyn)
 * @param lock pointer to a mutex_t variable
 *
 * TODO - add a comment each line describing its action
 */
test_and_set:
	pushl	%ebx		# preserve contents of the %ebx register, which is about to be modified
	movl	8(%esp), %ebx	# TODO &lock to %ebx
	movl	$1, %eax	# TODO 1 (true) to %eax

	//Swap %eax and lock. Value 1 (TRUE) is copied to lock
	//%eax receives old lock value
	xchg	%eax, (%ebx)	# TODO atomically exchange eax with lock. The atomicity of xchg is what guarantees that at most one thread can be holding the lock

	popl	%ebx		# TODO restore %ebx
	ret			# TODO return value (old value of lock) is already in %eax
