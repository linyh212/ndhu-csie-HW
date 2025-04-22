.data
input:
	.word 5
	.word 3
	.word 8
	.word 4
	.word 7
	.word 0
result:
	.space 100
.text
main:
	li  $s0, 0      # $s0 as accumulated sum
	la	$s1, input      # $s1 as memory pointer for input
	la  $s2, result     # $s2 as memory pointer for result
read_n:
	lw  $t0, 0($s1)     # get n from memory
	beq  $t0, $zero, exit   # leave loop if getting 0
	addi  $s1, $s1, 4   # memory pointer to next location

	li  $t1, 0      # initialize F(0)
	li  $t2, 1      # initialize F(1)
	sw  $t1, 0($s2)    # store F(0) in result
	addi  $s2, $s2, 4   # move result pointer to next location
    beq  $t0, 1, read_n   # check if n == 1, if so, read next number
	sw  $t2, 0($s2)   # store F(1) in result
	addi  $s2, $s2, 4   # move result pointer to next location
	subi  $t0, $t0, 2   # n=n-2, adjust n to account for F(0), F(1)
loop:
	beq	 $t0, $zero, read_n    # check if n == 0, if so, read next number
	add  $t3, $t1, $t2   # compute next fib num F(n)
	sw  $t3, 0($s2)   # store F(n) in result
	addi  $s2, $s2, 4   # move result pointer to next location
	move  $t1, $t2   # move F(n-1) to F(n-2)
	move  $t2, $t3   # move F(n) to F(n-1)
	subi  $t0, $t0, 1   # decrement n
	j	loop    #
exit:
	li	$v0, 10		# end of program
	syscall		#
