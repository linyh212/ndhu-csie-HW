# This program implements a sorting algorithm using the Quick Sort method.
# Quick Sort is a divide-and-conquer algorithm that works as follows:
# 1. Select a pivot element from the array.
# 2. Partition the array such that elements less than or equal to the pivot
#    are on the left, and elements greater than the pivot are on the right.
# 3. Recursively apply the same process to the left and right subarrays.
#    The program reads integers from the user until a 0 is entered, sorts them
#    using Quick Sort, and then prints the sorted array.

.data
array: .space 100          # Reserve space for 25 integers (4 bytes each)
array_size: .word 0        # Variable to store the size of the array
space: .asciiz " "         # Space between numbers

.text
main:
    # Initialize variables
    la $t0, array          # Load the base address of the array into $t0
    li $t1, 0              # Initialize array index to 0

input_loop:
    # Read integers from the user
    li $v0, 5              # syscall: read integer
    syscall
    move $t2, $v0          # Store the read integer in $t2

    beqz $t2, sort_array   # If input is 0, jump to the sorting phase

    sw $t2, 0($t0)         # Store the integer in the array
    addi $t0, $t0, 4       # Move to the next integer position in the array
    addi $t1, $t1, 1       # Increment the array index
    j input_loop           # Repeat the loop

sort_array:
    # Store the size of the array
    sw $t1, array_size     # Save the array size

    # Call quick_sort to sort the array
    la $a0, array          # Pass the base address of the array
    li $a1, 0              # Initialize left index to 0
    lw $t2, array_size     # Load the size of the array
    addi $a2, $t2, -1      # Set the right index to size - 1
    jal quick_sort         # Call the quick_sort function

    # Prepare to print the sorted array
    la $t0, array          # Load the base address of the array
    lw $t1, array_size     # Load the size of the array

print_loop:
    # Print the sorted array
    beqz $t1, exit         # If size is 0, exit the program
    lw $t2, 0($t0)         # Load the current element
    move $a0, $t2          # Move the element to $a0 for printing
    li $v0, 1              # syscall: print integer
    syscall

    # Print a space (except after the last number)
    beq $t1, 1, skip_space # If it's the last number, skip printing space
    li $v0, 4              # syscall: print string
    la $a0, space
    syscall

skip_space:
    addi $t0, $t0, 4       # Move to the next array position
    addi $t1, $t1, -1      # Decrement the size counter
    j print_loop           # Repeat the loop

exit:
    li $v0, 10             # syscall: exit
    syscall

quick_sort:
    # Quick sort function
    bge $a1, $a2, quick_sort_return # If left index >= right index, return

    addi $sp, $sp, -16     # Allocate stack space
    sw $ra, 0($sp)         # Save return address
    sw $a0, 4($sp)         # Save array base address
    sw $a1, 8($sp)         # Save left index
    sw $a2, 12($sp)        # Save right index

    move $t3, $a1          # Left index
    move $t4, $a2          # Right index

    jal partition          # Call the partition function
    move $t5, $v0          # Store the pivot index

    # Sort the left subarray
    lw $a0, 4($sp)         # Load array base address
    lw $a1, 8($sp)         # Load left index
    addi $a2, $t5, -1      # Set right index to pivot - 1
    jal quick_sort         # Recursive call to quick_sort

    # Sort the right subarray
    lw $a0, 4($sp)         # Load array base address
    addi $a1, $t5, 1       # Set left index to pivot + 1
    lw $a2, 12($sp)        # Load right index
    jal quick_sort         # Recursive call to quick_sort

    # Restore stack
    lw $ra, 0($sp)         # Restore return address
    addi $sp, $sp, 16      # Deallocate stack space

quick_sort_return:
    jr $ra                 # Return from quick_sort

partition:
    # Partition function
    mul $t0, $a2, 4        # Calculate the offset for the right index
    add $t1, $a0, $t0      # Get the address of the rightmost element
    lw $t2, 0($t1)         # Load the pivot element

    move $t3, $a1          # Initialize left pointer
    move $t4, $a1          # Initialize scan pointer

partition_loop:
    bgt $t4, $a2, partition_done # If scan pointer > right index, exit loop

    mul $t5, $t4, 4        # Calculate the offset for the scan pointer
    add $t6, $a0, $t5      # Get the address of the scan pointer element
    lw $t7, 0($t6)         # Load the current element

    ble $t7, $t2, do_swap  # If current element <= pivot, swap it
    j skip_swap            # Otherwise, skip the swap

do_swap:
    mul $t8, $t3, 4        # Calculate the offset for the left pointer
    add $t9, $a0, $t8      # Get the address of the left pointer element
    lw $s0, 0($t9)         # Load the left pointer element
    sw $t7, 0($t9)         # Store the current element at the left pointer
    sw $s0, 0($t6)         # Store the left pointer element at the current position
    addi $t3, $t3, 1       # Move the left pointer to the right

skip_swap:
    addi $t4, $t4, 1       # Move the scan pointer to the right
    j partition_loop       # Repeat the loop

partition_done:
    addi $t3, $t3, -1      # Move the left pointer back to the pivot position
    move $v0, $t3          # Return the pivot index
    jr $ra                 # Return from partition