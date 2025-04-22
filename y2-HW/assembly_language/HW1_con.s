.text
main:
	li 	$v0, 5		#input n
	syscall			#
	move 	$s0, $v0	#

	li 	$t1, 0		#initialized F(0)
	li 	$t2, 1		#initialized F(1)
	
	li 	$v0, 1		#print F(0)
	move	$a0, $t1	#
	syscall			#
	
	li	$v0, 4		#print new line
	la	$a0, newline	#
	syscall			#

	li	$t3, 2		#check if n >= 2
	blt	$s0, $t3, exit	#
	
print_f1:
	li	$v0, 1		#print F(1)
	move	$a0, $t2	#
	syscall			#

	li	$v0, 4		#print new line
	la	$a0, newline	#
	syscall			#

	addi	$s0, $s0, -2	#adjust n to account for F(0), F(1)
	blez	$s0, exit	#exit if n < 0
loop:
	add	$t4, $t1, $t2	#compute next fib num F(n)
	move	$t1, $t2	#move F(n-1) to F(n-2)
	move	$t2, $t4	#move F(n) to F(n-1)
	
	li	$v0, 1		#print F(n)
	move	$a0, $t4		#
	syscall			#
	
	li	$v0, 4		#print new line
	la	$a0, newline	#
	syscall			#
	
	addi	$s0, $s0, -1	#decrement n
	bgtz	$s0, loop	#loop if n >0
exit:
	li	$v0, 10		#exit
	syscall			#
.data
newline:	.asciiz "\n"	#newline
