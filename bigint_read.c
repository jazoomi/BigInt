#include "bigint.h"
#include <stdlib.h>
#include <ctype.h>

/* Converts an unsigned integer to a big integer.
   
   Parameters:
   - value: the unsigned integer to be converted to a big integer.
   - base: the base at which the number is represented. Must be
     between MIN_BASE and MAX_BASE.

   Returns: a newly allocated big integer representing the same
   numeric value as 'value'.
 */

bigint_t *uint_to_bigint(uintmax_t value, baseint_t base) {


    sign_t t;

    // If condition to assign the sign
    if (value > 0) {
      t = SIGN_POSITIVE;
    }

    // If the value was zero
    if (value == 0) {
      return new_basic_bigint(SIGN_ZERO, base, value);
    }

    bigint_t *new_bigint = malloc(sizeof(bigint_t));
    new_bigint->sign = t;
    new_bigint->base = base;
    uintmax_t remainder;
    digit_t *temp = NULL;
    
    
    while (value != 0) {
      remainder = value % base;
      digit_t *digit = malloc(sizeof(digit_t));
      digit->value = remainder;      

      // Created to fix the problem of one digit, multiple digits
      if (temp) {
        digit->next = temp;
        temp->prev = digit;
        new_bigint->first = digit;
        digit->prev = NULL;
      }
      else {
        digit->next = NULL;
        new_bigint->last = digit;
        new_bigint->first = digit;
        digit->prev = NULL;
      }

      temp = digit;
      value /= base;


    }

  return new_bigint;

}








/* Converts a signed integer to a big integer.
   
   Parameters:
   - value: the signed integer to be converted to a big integer.
   - base: the base at which the number is represented. Must be
     between MIN_BASE and MAX_BASE.

   Returns: a newly allocated big integer representing the same
   numeric value as 'value'.
 */
bigint_t *int_to_bigint(intmax_t value, baseint_t base) {

    sign_t t;

    // If condition to assign the sign
    if (value > 0) {
      t = SIGN_POSITIVE;
    }
    else if (value < 0) {
      t = SIGN_NEGATIVE;
    }

    // If the value was zero
    if (value == 0) {
      return new_basic_bigint(SIGN_ZERO, base, value);
    }

    bigint_t *new_bigint = malloc(sizeof(bigint_t));
    new_bigint->sign = t;
    new_bigint->base = base;
    uintmax_t remainder;
    digit_t *temp = NULL;
    
    
    while (value != 0) {
      remainder = abs(value % base);
      digit_t *digit = malloc(sizeof(digit_t));
      digit->value = remainder;      

      // Created to fix the problem of one digit, multiple digits
      if (temp) {
        digit->next = temp;
        temp->prev = digit;
        new_bigint->first = digit;
        digit->prev = NULL;
      }
      else {
        digit->next = NULL;
        new_bigint->last = digit;
        new_bigint->first = digit;
        digit->prev = NULL;
      }

      temp = digit;
      value /= base;


    }

  return new_bigint;

}



// cleanup leading zeros maybe
// Why are negative numbers not working?











/* Converts a character to the digit value it represents. This is a
   helper function that can be used in following functions. */
static inline baseint_t char_to_digit(int c) {

  if ('0' <= c && c <= '9')
    return c - '0';
  else if ('A' <= c && c <= 'Z')
    return c - 'A' + 10;
  else if ('a' <= c && c <= 'z')
    return c - 'a' + 10;
  else
    return INVALID_DIGIT;
}









/* Converts the string in str to a big integer according to the given
   base. The behaviour of this function is similar to that of strtol:
   an arbitrary amount of white space (as determined by isspace) at
   the start, if found, is ignored. The string starts with a single
   optional '+' or '-' sign; if no sign is found, the number is
   assumed to be positive. Following the optional sign, each character
   is interpreted as a digit in the provided base, as determined by
   char_to_digit. Conversion stops at the first character which is not
   a valid digit in the given base (as determined by the char_to_digit
   function). Spaces after the sign or digits cause the conversion to
   stop.
   
   Parameters:
   - str: the string to be converted to a big integer.
   - endptr: if endptr is not NULL, this function stores the address
     of the first character that is not a valid digit in *endptr. If
     there were no digits at all, it stores the position where the
     first digit was expected to be found (i.e., after spaces and/or
     the sign) in *endptr, and returns the big integer representing
     zero. If the entire string was parsed, **endptr points to the
     '\0' character at the end of the string.
  - base: the base at which the number is represented. Must be between
    MIN_BASE and MAX_BASE. Corresponds both to the base of the
    characters and the base of the resulting bigint object.

   Returns: a newly allocated big integer representing the same
   numeric value as 'str'.
 */
bigint_t *str_to_bigint(char *str, char **endptr, baseint_t base) {


  sign_t t;


  int i;
  // Ignores the whitespaces so that i is assigned the index after the whitespaces of the string
  for (i = 0; str[i] != '\0'; i++) {
      if (!isspace(str[i])) {
        break;
      }
  }

  // "0"

  // Assume it is positive, the else part, no sign means assume it is positive
  t = SIGN_POSITIVE;


  // Finding the sign
  if (str[i] == '-'){
    t = SIGN_NEGATIVE;
    i++;
  }
  else if (str[i] == '+') {
    i++;
  }


  // get the first digit (from left to right) // +,
  if ( (char_to_digit(str[i]) != INVALID_DIGIT) && (char_to_digit(str[i]) < base)) {     // char_to_digit(str[i]) >= 0 is redundant because it returns an unsigned value either way

  }
  // if no digit exists, return
  else {
      printf("Hello");
      *endptr = &str[i];         // Only dereference once, you want the pointer of endptr to point to the invalid digit.
      return new_basic_bigint(SIGN_ZERO, base, 0);
  }



  // if valid, create the first digit in bigint
  bigint_t  *new_bigint = new_basic_bigint(t, base, char_to_digit(str[i])); // for some reason putting these two lines of code in the if statment does not compile
  i++;
  
  digit_t *p = new_bigint->first;

  for (; str[i] != '\0'; i++) {

    //add every next digit into big_int
    if ( (char_to_digit(str[i]) != INVALID_DIGIT) &&  (char_to_digit(str[i]) < base)) {
        digit_t *digit = bigint_add_digit(new_bigint, char_to_digit(str[i]), NULL, p);
        new_bigint->last = digit;
        p->next = digit;
        p = digit;
    // 123ABC
    }
    else {
      *endptr = &str[i];
      break;
    }
  }

  //check if the string reached null terminator, and whole string was parsed
  if (str[i] == '\0'){
    *endptr = &str[i];
  }


  // Removes leading zeroes
  new_bigint = cleanup_bigint(new_bigint);

  if (bigint_num_digits(new_bigint) == 1) {
    if (new_bigint->first->value == 0) {
      t = SIGN_ZERO;
    }
  }



  new_bigint->base = base;
  new_bigint->sign = t;

  return new_bigint;
}











/* Reads a big integer from a file or terminal in the given base.
   
   Characters (initial spaces, an optional sign and individual digits)
   are read using the same rules as str_to_bigint. Reading stops at
   the first character which is not a valid digit in the given base or
   if the stream reaches end-of-file. This invalid digit (if it
   exists) is returned (using ungetc) to the stream.
   
   Parameters:
   - file: the file or terminal to read data from. This may be 'stdin'
     to read from the standard input.
   - base: the base at which the number is represented. Must be
     between MIN_BASE and MAX_BASE.

   Returns: a newly allocated big integer representing the value read
   from the file pointer.
 */
bigint_t *read_bigint(FILE *file, baseint_t base) {

  int i;  
  sign_t t;
  char c;

  // Ignores the whitespaces
  c = fgetc(file);
  for (i = 0; c != '\0'; i++) {
      if(!isspace(c)){
        break;
      }
      c = fgetc(file);
  }


  // Assume it is positive
  t = SIGN_POSITIVE;

  c = fgetc(file);

  if (c == '-') {
    //negative sign
    t = SIGN_NEGATIVE;
    c = fgetc(file);
  }

  else if (c == '+') {
    c = fgetc(file);
  }




  // get the first digit ( from left to right)
  if ( (char_to_digit(c) != INVALID_DIGIT) && (char_to_digit(c) < base)){

  } 
  else { // if no digit exist, return
      ungetc(c, file);
      return new_basic_bigint(SIGN_ZERO, base, 0);
  }


  bigint_t  *new_bigint = new_basic_bigint(t, base, char_to_digit(c)); // for some reason putting these two lines of code in the if statment does not compile
  c = fgetc(file);

  digit_t *p = new_bigint->first;
  for (; c != '\0'; c = fgetc(file))  {
    //add every next digit into big_int
    if ( (char_to_digit(c) != INVALID_DIGIT) && (char_to_digit(c) < base)) {
      digit_t *digit = bigint_add_digit(new_bigint, char_to_digit(c), NULL, p);
      new_bigint->last = digit;
      p->next = digit;
      p = digit;
    }
    else
    {
      ungetc(c, file);
      break;
    }
  }


  // Removes leading zeroes
  new_bigint = cleanup_bigint(new_bigint);

  if (bigint_num_digits(new_bigint) == 1) {
    if (new_bigint->first->value == 0) {
      t = SIGN_ZERO;
    }
  }

  new_bigint->base = base;
  new_bigint->sign = t;

  return new_bigint;

  //   char str[100]; // maximum size of input string
  //   int c;
  //   int i = 0;
  //   char *endptr;

  // // read characters until an invalid digit is encountered or EOF is reached

  // c = fgetc(file);
  // while (c != EOF && ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))){
  //   str[i++] = (char) c;
  //   c = fgetc(file);
  // }
  // str[i] = '\0';

  // // return the invalid digit to the stream
  // if (c != EOF) {
  //     ungetc(c, file);
  // }

  // // parse the input string and create a new bigint_t
  // return 

}