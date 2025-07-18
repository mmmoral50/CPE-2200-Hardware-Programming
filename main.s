.syntax unified
.cpu cortex-m4
.fpu softvfp
.thumb

// Name: Marlon Morales
// Date: [07/03/25]
// Course: CPE 2200
// Lab 2 - Activity 3: Data Analysis
// Description: This ARM Assembly program processes multiple arrays of grades,computes the minimum, maximum, average, and median for each,
// sorts the arrays in place using bubble sort, and stores the results in memory.

.section .data
.balign 4

// Grade arrays ( -1 is the end of the array)
grades_v1: .word 5, 4, 3, 2, 1, -1
grades_v2: .word 0, 0, 0, -1
grades_v3: .word 0, -1
grades_v4: .word 100, 100, 100, 100, -1
grades_v5: .word 50, 60, 70, 80, 90, 100, -1
grades_v6: .word 0, 86, 37, 47, 14, 94, 69, 25, 54, 10, 1, 24, 91, 82, 5, 41, 5, 50, 48, 60, 39, 51, 58, 58, 72, -1

// This table holds the addresses of the 6 grade arrays above
vector_table: .word grades_v1, grades_v2, grades_v3, grades_v4, grades_v5, grades_v6

// Output: store 4 words per vector (min, max, avg, med)
//.space directive reserves memory in ram for later use
results: .space 4 * 6 * 4     // 4 results x 6 vectors × 4 bytes

.section .text
.global main

main:
    MOV r12, #0                      // r12 = index of the the current vector (starts at 0)
    LDR r11, =vector_table          // r11 = pointer to vector table/Grade
    LDR r10, =results              // r10 = pointer to where the results will be stored

loop_vectors:
    CMP r12, #6                     // compare r12 >= 6 and check if all grade vectors are looped
    BGE done_all                   // if yes, jump to done_all label to end loop (End program)

    // Load current vector base address, r0= loaded vaule, r11 = grades_v#, r12 = index of grade array, #2 logic shift left by 2 bits (r12 * 4bytes)
    LDR r0, [r11, r12, LSL #2]      // Load the address of the current grade array into r0
    MOV r1, #0                     // r1 = count of grades

// Count grades until -1
count_loop:
    LDR r2, [r0, r1, LSL #2]        // Load next grade
    CMP r2, #-1                    // Check if it is at the end of list
    BEQ count_done                // If -1, stop counting
    ADD r1, r1, #1               // else, increment count
    B count_loop

count_done:
    LDR r2, [r0]                   // Load first grade into r2
    MOV r3, r2                     // r3 = min
    MOV r4, r2                     // r4 = max
    MOV r5, r2                     // r5 = sum
    MOV r6, #1                     // index = 1, index count/next grade

// --- Loop through grades to find min, max, and sum ---
minmax_loop:
    CMP r6, r1                     // if (index >= total_grades)
    BGE compute_avg                // then done with loop jump to compute_avg label

    LDR r7, [r0, r6, LSL #2]       // r7 = grades[index]
    CMP r7, r3                     // compare r7 >= r3, if current grade >= min
    BGE skip_min                   // then jump to skip_min label and skip updating min
    MOV r3, r7                     // else if update min = current grade
skip_min:
    CMP r7, r4                     // compare r7 <= r4, if current grade <= max
    BLE skip_max                   // then jump to label skip_max and skip updating max
    MOV r4, r7                     // else if, update max = current grade
skip_max:
    ADD r5, r5, r7                 // sum += grade
    ADD r6, r6, #1                 // increment index by 1
    B minmax_loop                  // repeat loop, jump to minmax_loop label

// Compute average = sum / count
compute_avg:
    SDIV r7, r5, r1                // r7 = average = sum divided by count

// Store min, max, avg
    STR r3, [r10], #4              // store min, then increment result pointer by 4 bytes
    STR r4, [r10], #4              // store max, then increment
    STR r7, [r10], #4              // store avg, then increment

// Bubble Sort (sort grades in place)
    MOV r8, #0                     // r8 is outer loop index i, initialize at 0
sort_outer:
    CMP r8, r1                     // if r8 <= r1, i <= count
    BGE sort_done                  // then jump to label sort_done:
    MOV r9, #1                     // else if inner loop j(r9) = 1

sort_inner:
    CMP r9, r1                     // if r9 <= r1, j <= count
    BGE sort_outer_inc             // then jump to sort_outer_inc label

    SUB r2, r9, #1                 // r2 = j - 1
    LSL r6, r2, #2                 // r6 = (j - 1) * 4 (to get byte offset)
    ADD r6, r0, r6                 // r6 = address of A[j - 1]
    LDR r3, [r6]                   // load A[j - 1] into r3

    LSL r5, r9, #2                 // r5 = j * 4
    ADD r5, r0, r5                 // r5 = address of A[j]
    LDR r4, [r5]                   // load A[j] into r4

    CMP r3, r4                     // compare A[j - 1] and A[j]
    BLE skip_swap                  // if A[j - 1] <= A[j], skip the swap
    STR r4, [r6]                   // store A[j] into A[j - 1]
    STR r3, [r5]                   // store A[j - 1] into A[j]
skip_swap:
    ADD r9, r9, #1                 // j = j + 1
    B sort_inner                   // repeat inner loop

sort_outer_inc:
    ADD r8, r8, #1                 // i = i + 1
    B sort_outer                   // repeat outer loop

// --Sorting is complete, check if count is even or odd--
sort_done:
    AND r2, r1, #1                 // r2 = count mod 2
    CMP r2, #0                    // if r2 <= 0, remainder even = 0, remainder odd = 1
    BEQ even_median               // then jump to even_median loop

// Median: Odd count
odd_median:
    LSR r3, r1, #1                // r3 = count divided by 2 (middle index)
    LDR r4, [r0, r3, LSL #2]      // load median value into r4
    B store_median                // jump to store_median label

// Median: Even count
even_median:
    SUB r3, r1, #1               // r3 = count - 1
    LSR r3, r3, #1               // r3 = (count - 1) / 2 (first middle index)
    LDR r5, [r0, r3, LSL #2]     // load first middle value into r5
    ADD r3, r3, #1               // r3 = second middle index
    LDR r6, [r0, r3, LSL #2]     // load second middle value into r6
    ADD r7, r5, r6               // sum the two middle values
    LSR r4, r7, #1               // r4 = average of two middle values (median)

store_median:
    STR r4, [r10], #4              // store median in memory and increment result pointer

    ADD r12, r12, #1               // index to next grade vector
    B loop_vectors                 // repeat for the next vector

done_all:
    B done_all                     // End loop

