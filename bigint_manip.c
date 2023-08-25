#include "bigint.h"
#include <stdlib.h>
#include <ctype.h>

/* Increments the value of a bigint object in-place by one.

   Parameters:
   - bigint: the big integer to be incremented.
   
   Returns: the original bigint, with its value incremented by one.
 */
bigint_t *bigint_increment(bigint_t *bigint) {

  // YOUR CODE HERE
  // bigint to int
  // increment by one
  // int to bigint

  //sign_t oursign = bigint->sign;
  //baseint_t ourbase = bigint->base;
  //intmax_t num = bigint_to_int(bigint);
  //num = num * oursign + 1;
  //return int_to_bigint(num, ourbase);

  digit_t *p = bigint->last;
  

  // This is for negative numbers
  // This is right
  if (bigint->sign == SIGN_NEGATIVE) {
    while (p != NULL) {
      if (p->value == 0) {
          p->value = bigint->base - 1;
          p = p->prev;
      }
      else {
        p->value--;
        p = NULL;
      }
    }

  }


  // This is for positve numbers and zero
  // This is wrong
  // Cases 0, 10, 9, 19
  // The important part is taking into account the number 9
  while (p != NULL) {
    p->value = p->value + 1;

    if (p->value == bigint->base) {
      p->value = 0;

      if (p->prev == NULL) {
        bigint_add_digit(bigint, 1, p, NULL);
        p = NULL;
        break;
      }
      p = p->prev;
    }
    else {
      p = NULL;
    }
    
  }

  // In case the the number was 0, and increment to 1.
  if (bigint->sign == SIGN_ZERO) {
    bigint->sign = SIGN_POSITIVE;
  }

  bigint = cleanup_bigint(bigint);

  return bigint;
}







/* Increments the value of a bigint object in-place by one.

   Parameters:
   - bigint: the big integer to be incremented.
   
   Returns: the original bigint, with its value incremented by one.
 */
bigint_t *bigint_decrement(bigint_t *bigint) {

  // YOUR CODE HERE
  //sign_t oursign = bigint->sign;
  //baseint_t ourbase = bigint->base;
  //intmax_t num = bigint_to_int(bigint);
  //num = num * oursign - 1;
  //return int_to_bigint(num, ourbase);

  digit_t *p = bigint->last;

  // Handles the special case when bigint is 0
  if (bigint_num_digits(bigint) == 1 && bigint->first->value == 0) {
      bigint->sign = SIGN_NEGATIVE;
      bigint->first->value = 1;
      return bigint;
  }

  // In case the number is negative, decrement
  // This part is wrong
  // Cases -10, -9, -19
  if (bigint->sign == SIGN_NEGATIVE) {

    while (p != NULL) {
      p->value = p->value + 1;

      if (p->value == bigint->base) {
        p->value = 0;

        if (p->prev == NULL) {
          bigint_add_digit(bigint, 1, p, NULL);
          p = NULL;
          break;
        }

        p = p->prev;
      }
      else {
        p = NULL;
      }
    }
  }



  // In case the number is positive, decrement
  // This part is right
  // cases 11, 9, 10, 100, 1
  while (p != NULL) {
    if (p->value == 0) {
        p->value = bigint->base - 1;
        p = p->prev;
    }
    else {
      p->value--;
      break;
    }
  }


  // Fixes 09, 099 leading zeroes
  bigint = cleanup_bigint(bigint);

  // Handle the special case when bigint turns 0 from positive
  if (bigint_num_digits(bigint) == 1 && bigint->first->value == 0) {
      bigint->sign = SIGN_ZERO;
      return bigint;
  }

  return bigint;
}


// Does this care about the base at all?
// What if it is base 2?




















/* Divides a big integer by an unsigned integer divisor. This function
   can be used, among other things, as part of the computation
   required to convert a big integer into a new base (see
   bigint_change_base).

   Parameters:
   - dividend: the big integer corresponding to the dividend. This
     object remains unmodified.
   - divisor: the number that the dividend will be divided by. Cannot
     be zero.
   - remainder: if not NULL, the remainder of the division will be
     stored into *remainder.

   Returns: a newly allocated big integer corresponding to the result
   of the division of dividend by divisor.
 */
bigint_t *bigint_divide_by_uint(const bigint_t *dividend, baseint_t divisor,
				baseint_t *remainder) {

  // YOUR CODE HERE
  // I think you need to do long division
  // Start from left to right
  // You probably need to create a new bigint, and put the newly created digits in it
  // Each time you read a number from the left side, if the divisor is bigger than the number, you add the number to the right side digit to it
  bigint_t *new_bigint = malloc(sizeof(bigint_t));

  digit_t *p = dividend->first;

  int remain = 0;
  int result = 0;

  new_bigint->sign = dividend->sign;
  new_bigint->base = dividend->base;

  digit_t *temp = NULL;

  // Initializing
  int val = p->value; // 327 775 227 / 7

  // 940 / 4
  while (p != NULL) {

    remain = val % divisor; // 3
    result = val / divisor; // 2

    digit_t *digit = malloc(sizeof(digit_t));

    if (temp) {
      digit->next = NULL;
      digit->prev = temp;
      temp->next = digit;
      new_bigint->last = digit; // not nuLL?
    }
    else {
      digit->next = NULL;
      new_bigint->last = digit;
      new_bigint->first = digit;
      digit->prev = NULL;
    }

    temp = digit;

    // assigning the value 0 to the newly created digit
    digit->value = result;

    p = p->next; // 3 -> 2

    if (p != NULL) {
      val = remain * dividend->base + p->value; // 16 I think I have to change * 10 to * base
    }
    else {
      *remainder = remain;
    }

      // 56 / 2 = 28

      // 940 
      // 4
      // 2
      // 14

  }

  new_bigint = cleanup_bigint(new_bigint);

  return new_bigint;

}








// Tested with 25 / 7
// Tested with 940 / 4
// 1101 / 10



/* Creates a copy of a big integer, replacing the base with a new
   base. The copy will represent the same number but with a different
   base, performing the appropriate computations as required. Calling
   this function with a new_base that is the same as the bigint
   object's old base is equivalent to calling bigint_dup(old).

   Parameters:
   - old: the big integer to be copied.
   - base: the base at which the new number is to be represented. Must
     be between MIN_BASE and MAX_BASE.
   
   Returns: a newly allocated big integer representing the same number
   as src in a different base.
 */
bigint_t *bigint_change_base(bigint_t *old, baseint_t new_base) {


  /



// So apparently what I have to do is constantly divide the number we are given and put the
// remainder from right to left
 // Creating the new bigint that is going to store the remainders of the divisions
  baseint_t remainder = 0;




  bigint_t *p = NULL;
  bigint_t *remover = NULL;

  if (old->base == new_base) {
    return bigint_dup(old);
  }
      bigint_t *newbigint = malloc(sizeof(bigint_t));

    newbigint->base = new_base;
  newbigint->sign = old->sign;

  // For the first digit, conversion to the new base, from right to left are added
  p = bigint_divide_by_uint(old, new_base, &remainder);
  //free(remover);

  digit_t *temp = NULL;
  digit_t *digit = bigint_add_digit(newbigint, remainder, temp, NULL);

  // conversion to the new base, from right to left are added
  while (p->first->value != 0 || bigint_num_digits(p) != 1) {
    temp = digit;
    
    remover = p;

    p = bigint_divide_by_uint(p, new_base, &remainder);
    bigint_free(remover);

    digit = bigint_add_digit(newbigint, remainder, temp, NULL);
  }

  bigint_free(p);

  newbigint = cleanup_bigint(newbigint);

  return newbigint;




}