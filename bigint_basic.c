#include "bigint.h"

#include <stdlib.h>
#include <ctype.h>

/* Allocates and initializes a new digit struct. Also updates
   neighbouring digits and the bigint pointers to point to the new
   digit. This function is intended to be used as a helper function
   for other bigint functions, and is not intended to be used outside
   of the bigint library itself.
   
   Parameters:
   - value: digit value of the new digit, must be between 0 and
     base-1.
   - next: digit to the right of the new digit. If set, will also be
     updated to point to new digit. If NULL, will cause bigint->last
     to be updated instead.
   - prev: digit to the left of the new digit. If set, will also be
     updated to point to new digit. If NULL, will cause bigint->first
     to be updated instead.

   Returns: a pointer to the newly allocated digit.
 */
digit_t *bigint_add_digit(bigint_t *bigint, baseint_t value, digit_t *next, digit_t *prev) {

  // YOUR CODE HERE
  // So it just adds a bigint
  digit_t *digit = malloc(sizeof(digit_t));
  digit->value = value;

  // The place where the digit is added is determined by next and prev
  if (next == NULL) {
    bigint->last = digit;
  }
  else {
    next->prev = digit;
  }

  if (prev == NULL) {
    bigint->first = digit;
  }
  else {
      prev->next = digit;
  }


  // Isn't there a repetitive action here? Fixed
  digit->next = next;
  digit->prev = prev; 


  return digit;
}


// next->prev = digit;
// You have to fix the problem where it is added in the end and in the start
// Maybe fixed?










/* Allocates and initializes a new big integer struct that has a
   single digit.
   
   Parameters:
   - sign: the sign of the new number (positive, negative, zero).
   - base: the base at which the number is represented. Assumed to be
     between MIN_BASE and MAX_BASE.
   - digit_value: the value of the single digit. Must be between
     0 and base-1.
   
   Returns: a pointer to the newly allocated big integer.
 */
bigint_t *new_basic_bigint(sign_t sign, baseint_t base, baseint_t digit_value) {
  
  // YOUR CODE HERE

  // Initialize and allocate the big integer
  bigint_t *p = malloc(sizeof(bigint_t));

  // Assign a sign
    p->sign = sign;

  // Assigning the base
  p->base = base;


  // Adding the single digit
  digit_t *digit = malloc(sizeof(digit_t));

  // Assigning the digit value
  digit->value = digit_value;

  // Points to the most and least significant digit of the big integer
  p->first = digit;
  p->last = digit;    // Does this point back?

  // If most and least significant digit, then they point to null
  digit->next = NULL;
  digit->prev = NULL;

  // Returns the pointer of the bigInt
  return p;
}


// Need a proper picture of this linkedlist with the variables of the bigint and digit
// I guess maybe fixed













/* Ensures that the big integer data structure is clean. Removes
   leading zeros, and updates the sign to SIGN_ZERO if there is only a
   single digit with the value zero.

   This function is expected to be called by most (if not all) library
   functions that return a `bigint` object before it is returned.

   Parameter: bigint: the big integer to be cleaned up.

   Returns: the same bigint passed as a parameter.
 */
bigint_t *cleanup_bigint(bigint_t *bigint) {

  // YOUR CODE HERE
  // 1. Traverse through the linkedlist from left to right
  // 2. As long as you see the value 0, keep putting the head to the right
  // 3. Free the digit objects one by one
  // 4. Do this until you reach a non-zero value
  // 5. If there exists a single digit only in the end, and it is 0, set the sign as SIGN_ZERO

  // Digit points to the first digit
  digit_t *temp = bigint->first;

  while (temp->next != NULL) {
    if (temp->value == 0) {
      bigint->first = temp->next;
      free(temp);
      temp = bigint->first;
    }
    else {
      break;
    }

  }
  // prev needs to be set to Null according to the prof
  // fix the 0 issue

  if (bigint->first->value == 0) {
  bigint->sign = SIGN_ZERO;
}

  temp->prev = NULL;
  return bigint;


}










/* Frees a big integer and all its digits.
   
   Parameter: bigint: the big integer to be freed.
 */
void bigint_free(bigint_t *bigint) {
  
  // YOUR CODE HERE
  // Same procedure as one of our worksheets I believe
  // 1. Create a new digit pointer
  // 2. The head moves forwards while the digit pointer stays behind
  // 3. The digit pointer frees up the memory space
  // 4. The digit pointer is assigned to the head pointer
  // 5. This procedure is continued till you reach the null

  // The real question is what is the head pointer, is it the first pointer?

  digit_t *cur = bigint->first;
  digit_t *next = NULL;

  while (cur != NULL) {           // While cur hasn't reached null yet
    next = cur->next;
    free(cur);
    cur = next;
  }

  free(bigint);
}

/* Counts the number of digits in a big integer.

   Parameters: bigint: the big integer to be evaluated.
   
   Returns: a positive integer representing the number of digits in
   the big integer. The sign is not counted as a digit. A value of
   zero is considered to have one digit.
 */
uintmax_t bigint_num_digits(bigint_t *bigint) {

  // YOUR CODE HERE

  // Do I need to call the above function to clean up the big int first, then actually count
  // The number of digits?
  // Count the number of digits by traversing through
  // I can assume the leading zeros are removed


  int count = 0;
  digit_t *p = bigint->first;
  while (p != NULL) {
    count++;
    p = p->next;
  }


  return count;
}








/* Creates a new copy of a big integer. The copy will represent the
   same number in the same base, with new digits.

   Parameters: src: the big integer to be copied.
   
   Returns: a newly allocated big integer representing the same number
   as src in the same base.
 */
bigint_t *bigint_dup(bigint_t *src) {

  // YOUR CODE HERE
  bigint_t *new_bigint = malloc(sizeof(bigint_t));
  new_bigint->first = NULL;
  new_bigint->sign = src->sign;
  new_bigint->base = src->base;

  digit_t *old_pointer = src->first;
  digit_t *prev = NULL;

  while (old_pointer != NULL) {
    digit_t *digit = malloc(sizeof(digit_t));
    digit->value = old_pointer->value;
    digit->next = NULL;
    new_bigint->last = digit;

    if (prev) {
      digit->prev = prev;
      prev->next = digit;
    }
    else {
      new_bigint->first = digit;
      digit->prev = NULL;
    }

    prev = digit;
    old_pointer = old_pointer->next;

  }

  return new_bigint;

  // I believe we already did this in a worksheet
}
