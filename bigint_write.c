#include "bigint.h"
#include <stdlib.h>
#include <ctype.h>

/* Converts a digit value to the character representing it. This is a
   helper function that can be used in following functions. */
static inline char digit_to_char(baseint_t num) {
  
  if (num <= 9)
    return num + '0';
  else if (num <= 35)
    return num + 'a' - 10;
  else
    return '?';
}



/* Prints a big integer to a file or terminal in its original base.
   If the big integer is negative, a '-' is printed. After that, the
   number is printed using one character per digit (as determined by
   digit_to_char), with no leading zeros or spaces.
   
   Parameters:
   - bigint: the big integer to be printed.
   - file: the file or terminal to print data to. This may be 'stdout'
     to print to the standard output.
 */
void print_bigint(const bigint_t *bigint, FILE *file) {

  // YOUR CODE HERE
  // Print the sign
  // Print the characters using digit to char
  // Before that remove the leading zeroes


  if (bigint->sign == SIGN_NEGATIVE) {
    fprintf(file, "-");
  }
  else if (bigint->sign == SIGN_POSITIVE) {
  }
  else {
    fprintf(file, "0");
    return;
  }

  digit_t *p = bigint->first;
  char val;

  // 1010

  while (p != NULL) {
    val = digit_to_char(p->value);
    // conversion
    fprintf(file, "%c", val);
    p = p->next;
  }

  // Good practice
  p = NULL;

  // fprintf(file, "%c", '\0');
}







/* Converts a big integer into its string representation in its
   original base. If the big integer is negative, the string starts
   with a '-'. After that, the string is filled with one character per
   digit (as determined by digit_to_char), with no leading zeros or
   spaces. The string is terminated with a null byte as expected.

   Parameters:
   - bigint: the big integer to be printed.
   - buf: the string buffer to be filled with the number's
     digits. Must be at least bigint_num_digits(bigint)+2 bytes long
     (to account for the sign and termination byte).
 */
void bigint_to_str(const bigint_t *bigint, char *buf) {


  //int count = 0;
  //digit_t *pointer = bigint->first;
  //while (pointer != NULL) {
    //count++;
    //pointer = pointer->next;
  //}

  //pointer = NULL;

  // YOUR CODE HERE
  //int numdigits = count;


  // Because of null byte terminator and sign symbol
  //buf = (char *) malloc(numdigits + 2);

  int i = 0;

  if (bigint->sign == SIGN_NEGATIVE) {
    buf[0] = '-';
    i++;
  }
  else if (bigint->sign == SIGN_ZERO) {
    buf[0] = '0';
    buf[1] = '\0';
    return;
  }
  //else {
    //buf[0] = '+';
    //i++;
  //}

  digit_t *p = bigint->first;
  
  while (p != NULL) {
    buf[i] = digit_to_char(p->value);
    p = p->next;
    i++;
  }

  // Best practice
  p = NULL;

  buf[i] = '\0';

}






/* Converts the absolute value of a big integer (i.e., ignoring the
   sign) into its unsigned integer representation. This function is
   only expected to provide a reliable value if the number represented
   by the big integer fits the uintmax_t type. If the big integer is
   larger than what uintmax_t can support, the result is
   undetermined. The sign of the big integer is ignored.

   Parameters:
   - bigint: the big integer to be converted.
 */
uintmax_t bigint_to_uint(const bigint_t *bigint) {

  // YOUR CODE HERE
  // 12344

  if (bigint->sign == SIGN_ZERO) {
    return (uintmax_t) 0;
  }

  digit_t *p = bigint->last;
  uintmax_t i = 1;
  uintmax_t num = 0;

  while (p != NULL) {
    num += (p->value) * i;
    p = p->prev;
    i *= bigint->base;
  }

  p = NULL;

  return num;

}





/* Converts a big integer into its signed integer representation. This
   function is only expected to provide a reliable value if the number
   represented by the big integer fits the intmax_t type. If the big
   integer is larger (or smaller) than what intmax_t can support, the
   result is undetermined.
   
   Parameters:
   - bigint: the big integer to be converted.
 */
intmax_t bigint_to_int(const bigint_t *bigint) {

  
  if (bigint->sign == SIGN_ZERO) {
    return (intmax_t) 0;
  }


  digit_t *p = bigint->last;
  intmax_t i = 1;
  intmax_t num = 0;



  while (p != NULL) {
    num += (p->value) * i;
    p = p->prev;
    i *= bigint->base;
  }


  if (bigint->sign == SIGN_NEGATIVE) {
    num *= -1;
  }


  p = NULL;


  return num;
}