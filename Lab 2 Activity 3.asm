.data
grades:     .word 5, 4, 3, 2, 1, -1       // Replace this with other test vectors for demo
min_val:    .word 0                      // To store minimum grade
max_val:    .word 0                      // To store maximum grade
avg_val:    .word 0                      // To store average grade
med_val:    .word 0                      // To store median grade

.text
.global _start

_start:
    LDR r0, =grades                      // r0 = address of grades array
    BL  count_elements                   // r1 = number of grades before -1

    LDR r0, =grades
    MOV r1, r1                           // r1 = count
    BL  find_min_max_sum_avg            // find min, max, sum, and average

    LDR r0, =grades
    MOV r1, r1                           // r1 = count
    BL  bubble_sort                     // sort the array for median

    LDR r0, =grades
    MOV r1, r1                           // r1 = count
    BL  find_median                     // compute median

    B   .                                // infinite loop to end

// --------------------------
// Count elements before -1
// --------------------------
count_elements:
    MOV r1, #0                          // r1 = count = 0
count_loop:
    LDR r2, [r0, r1, LSL #2]            // r2 = grades[r1]
    CMP r2, #-1                         // check for end marker
    BEQ count_done                      // if -1, end
    ADD r1, r1, #1                      // count++
    B   count_loop
count_done:
    BX  lr                              // return with count in r1

// --------------------------
// Find min, max, sum, avg
// --------------------------
find_min_max_sum_avg:
    LDR r2, [r0]                        // r2 = first grade
    MOV r3, r2                          // r3 = min
    MOV r4, r2                          // r4 = max
    MOV r5, r2                          // r5 = sum
    MOV r6, #1                          // r6 = index = 1

minmax_loop:
    CMP r6, r1
    BGE store_avg

    LDR r7, [r0, r6, LSL #2]            // r7 = grades[r6]
    CMP r7, r3
    MOVLT r3, r7                        // if grade < min, update min
    CMP r7, r4
    MOVGT r4, r7                        // if grade > max, update max
    ADD r5, r5, r7                      // sum += grade
    ADD r6, r6, #1                      // index++
    B   minmax_loop

store_avg:
    SDIV r8, r5, r1                     // avg = sum / count
    LDR r9, =min_val
    STR r3, [r9]                        // store min
    LDR r9, =max_val
    STR r4, [r9]                        // store max
    LDR r9, =avg_val
    STR r8, [r9]                        // store average
    BX  lr

// --------------------------
// Bubble sort: Ascending
// --------------------------
bubble_sort:
    MOV r2, #0                          // k = 0
outer_loop:
    CMP r2, r1
    SUB r10, r1, #1
    BGE end_sort                        // if k >= n-1, end

    MOV r3, #0                          // i = 0
inner_loop:
    SUB r5, r1, r2
    SUB r5, r5, #1                      // n - k - 1
    CMP r3, r5
    BGE next_outer

    LSL r6, r3, #2
    ADD r6, r0, r6                      // &A[i]
    LDR r7, [r6]                        // A[i]

    ADD r9, r3, #1
    LSL r9, r9, #2
    ADD r9, r0, r9                      // &A[i+1]
    LDR r8, [r9]                        // A[i+1]

    CMP r7, r8
    BLE skip_swap

    STR r8, [r6]                        // A[i] = A[i+1]
    STR r7, [r9]                        // A[i+1] = A[i]

skip_swap:
    ADD r3, r3, #1
    B   inner_loop

next_outer:
    ADD r2, r2, #1
    B   outer_loop
end_sort:
    BX  lr

// --------------------------
// Compute median
// --------------------------
find_median:
    MOV r2, r1                          // r2 = count
    AND r3, r2, #1                      // r3 = count % 2
    CMP r3, #0
    BEQ even_median                     // if even, go to even_median

odd_median:
    LSR r3, r2, #1                      // r3 = count / 2
    LDR r4, [r0, r3, LSL #2]            // median = A[count / 2]
    LDR r5, =med_val
    STR r4, [r5]
    BX  lr

even_median:
    SUB r3, r2, #1
    LSR r3, r3, #1                      // r3 = (count - 1) / 2
    LDR r4, [r0, r3, LSL #2]            // A[mid1]
    ADD r3, r3, #1
    LDR r6, [r0, r3, LSL #2]            // A[mid2]
    ADD r7, r4, r6
    LSR r7, r7, #1                      // median = (mid1 + mid2) / 2
    LDR r5, =med_val
    STR r7, [r5]
    BX  lr
