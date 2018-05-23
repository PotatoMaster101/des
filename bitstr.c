/**
 * FILE:   bitstr.c
 * AUTHOR: PotatoMaster101
 * DATE:   21/03/2018
 * 
 * Utility functions for handling bit strings. 
 *
 * C99
 */

#include "bitstr.h"
#define CHAR_LEN 8

static char *to_bstr(unsigned char c);
static unsigned char to_char(char *str);
static char *str_clone(char *s, size_t len);

/**
 * Returns the length of the specified bit string. If the specified string 
 * is not a bit string, then 0 will be returned. Use this function instead 
 * of strlen() if a check for error is required. 
 *
 * PARAMETERS: 
 * str - the bit string to find length
 *
 * RETURNS: 
 * The length of the bit string, or 0 if any error occurred. 
 */
size_t bstr_len(char *str) {
    if (str == NULL || *str == '\0')
        return 0;

    size_t i = 0;
    for (i = 0; str[i] != '\0'; i++)
        if (str[i] != '0' && str[i] != '1')
            return 0;
    return i;
}

/**
 * Converts a whole string into a bit string. The result bit string will 
 * be allocated dynamically. If any memory error occurred, then NULL will 
 * be returned. The size of the bit string will be 8x the size of the 
 * input string. 
 *
 * PARAMETERS: 
 * str - the string to convert
 *
 * RETURNS: 
 * The bit string of the given string, or NULL if any error occurred. 
 */
char *bstr_new(char *str) {
    if (str == NULL || *str == '\0')
        return NULL;        //input invalid

    size_t len = CHAR_LEN * strlen(str);
    char *bits = malloc((len + 1) * (sizeof *bits));
    if (bits != NULL) {
        bits[len] = '\0';
        size_t i = 0, j = 0;
        for (i = 0; i < len; i += CHAR_LEN)
            memcpy(bits + i, to_bstr(str[j++]), CHAR_LEN * (sizeof *str));
    }
    return bits;
}

/**
 * Constructs the original string from the specified bit string. The 
 * constructed string will be dynamically allocated. The input bit string 
 * length must be a multiple of 8. If any error occurred, then NULL will be 
 * returned. 
 *
 * PARAMETERS: 
 * str - the bit string to reconstruct
 *
 * RETURNS: 
 * The original string. 
 */
char *bstr_original(char *str) {
    size_t len = bstr_len(str);
    if (len == 0 || len % CHAR_LEN != 0)
        return NULL;        //input invalid

    size_t bstrlen = len / CHAR_LEN;
    char *new = malloc((bstrlen + 1) * (sizeof *new));
    if (new != NULL) {
        new[bstrlen] = '\0';
        for (size_t i = 0; i < bstrlen; i++)
            new[i] = to_char(str + (i * 8));
    }
    return new;
}

/**
 * Pads the specified bit string until its length is the same as n. If the 
 * specified string is not a bit string or the n is 0, then this 
 * function will do nothing and NULL returned, otherwise this function will 
 * return a new bit string that is dynamically allocated, which stores the 
 * result of padding. The padding will always be 0. 
 *
 * PARAMETERS: 
 * str - the bit string to pad
 * n   - the new length
 *
 * RETURNS: 
 * NULL if any error occurred, otherwise a pointer to the new padded bit 
 * string. 
 */
char *bstr_pad(char *str, size_t n) {
    size_t len = bstr_len(str);
    if (len == 0 || n == 0)
        return NULL;                //param error
    if (n <= len)
        return str_clone(str, n);   //no need for padding

    char *new = malloc((n + 1) * (sizeof *new));
    if (new != NULL) {
        new[n] = '\0';
        memcpy(new, str, len * (sizeof *new));
        memset(new + len, '0', (n - len) * (sizeof *new));
    }
    return new;
}

/**
 * Left rotates the specified bit string. The return type of this function 
 * indicates whether the operation is successful or not. If true is returned, 
 * then this function is successful, if false returned, then this function 
 * failed to operate on the given parameters. 
 *
 * PARAMETERS: 
 * str - the bit string to rotate
 * n   - the number of shifts
 *
 * RETURNS: 
 * 1 (true) if rotation is successful, 0 (false) otherwise. 
 */
_Bool bstr_lrot(char *str, uint32_t n) {
    size_t len = bstr_len(str);
    if (len == 0)
        return false;
    if (n % len == 0)
        return true;     //no rotates needed

    n %= len;               //reduce to smallest n
    size_t rotlen = len - n;
    char *temp = malloc(n * (sizeof *temp));
    if (temp != NULL) {
        memcpy(temp, str, n * (sizeof *str));          //backup rotated bits
        memmove(str, str + n, rotlen * (sizeof *str));
        memcpy(str + rotlen, temp, n * (sizeof *str)); //restore rotated bits
        free(temp);
        return true;
    }
    return false;    //memory issue
}

/**
 * Right rotates the specified bit string. The return type of this function 
 * indicates whether the operation is successful or not. If true is returned, 
 * then this function is successful, if false returned, then this function 
 * failed to operate on the given parameters. 
 *
 * PARAMETERS: 
 * str - the bit string to rotate
 * n   - the number of shifts
 *
 * RETURNS: 
 * 1 (true) if rotation is successful, 0 (false) otherwise. 
 */
_Bool bstr_rrot(char *str, uint32_t n) {
    size_t len = bstr_len(str);
    if (len == 0)
        return false;
    if (n % len == 0)
        return true;     //no rotates needed

    n %= len;               //reduce to smallest n
    size_t rotlen = len - n;
    char *temp = malloc(n * (sizeof *temp));
    if (temp != NULL) {
        memcpy(temp, str + rotlen, n * (sizeof *str));  //backup rotated bits
        memmove(str + n, str, rotlen * (sizeof *str));
        memcpy(str, temp, n * (sizeof *str));           //restore bits
        free(temp);
        return true;
    }
    return false;    //memory issue
}

/**
 * Performs XOR on the given bit strings and store the result on the left 
 * oprand. If this function is succssful, then true will be returned, 
 * otherwise false will be returned. 
 *
 * PARAMETERS: 
 * a - the left oprand
 * b - the right oprand
 *
 * RETURNS: 
 * 1 (true) if this function is successful, 0 (false) otherwise. 
 */
_Bool bstr_xor(char *a, char *b) {
    size_t alen = bstr_len(a);
    size_t blen = bstr_len(b);
    if (alen == 0 || blen == 0 || alen != blen)
        return false;        //a or b is invalid, or length differ

    for (size_t i = 0; i < alen; i++)
        a[i] = ((a[i] - '0') ^ (b[i] - '0')) + '0';
    return true;
}

/**
 * Flips the specified bit string, changing every 1 to 0 and 0 to 1. If any 
 * error occured with the input, then false will be returned. Otherwise 
 * this function will return true. 
 *
 * PARAMETERS: 
 * str - the string to flip
 *
 * RETURNS: 
 * 1 (true) if this function is successful, 0 (false) otherwise. 
 */
_Bool bstr_flip(char *str) {
    if (str == NULL || *str == '\0')
        return false;

    for (size_t i = 0; str[i] != '\0'; i++) {
        if (str[i] != '1' && str[i] != '0')
            return false;    //str is not a bit string
        str[i] = (str[i] == '1' ? '0' : '1');
    }
    return true;
}

/**
 * Swaps the specified bit string, placing the first half of the bit string 
 * into the second half and the second half into the first half. The return 
 * value indicates whether this function is successful. Note that the input 
 * string must have an even length. 
 *
 * PARAMETERS: 
 * str - the string to swap
 * 
 * RETURNS: 
 * 1 (true) if this function is successful, 0 (false) otherwise. 
 */
_Bool bstr_swap(char *str) {
    size_t len = bstr_len(str);
    if (len == 0 || len % 2 != 0)
        return false;    //error string or odd length

    size_t half = len / 2;
    for (size_t i = 0; i < half; i++) {
        char curr = str[i];
        str[i] = str[i + half];
        str[i + half] = curr;
    }
    return true;
}

/**
 * Converts a single character to binary notation (8 bits). The resulting 
 * binary will be stored in a string. 
 *
 * PARAMETERS: 
 * c - the character to convert
 *
 * RETURNS: 
 * The bit string representation, or 0 if any error occurred. 
 */
static char *to_bstr(unsigned char c) {
    static char str[] = "00000000";
    memset(str, '0', 8  * (sizeof *str));

    int bi = CHAR_LEN - 1;     //length index of bit string
    do {
        str[bi--] = (c & 1) + '0';
    } while ((c >>= 1) > 0);
    return str;
}

/**
 * Converts a bit string to a single char. The bit string 
 * must contain more than 8 bits. No checking will be performed 
 * in this function. 
 *
 * PARAMETERS: 
 * str - the bit string to convert
 *
 * RETURNS: 
 * The char represented by the bit string. 
 */
static unsigned char to_char(char *str) {
    unsigned char c = 0;
    unsigned char pow = 1;
    for (int i = 7; i >= 0; i--) {
        if (str[i] == '1')
            c += pow;
        pow <<= 1;
    }
    return c;
}

/**
 * Clones a bit string. If any memory error or parameter error occurred, 
 * then NULL will be returned. 
 *
 * NOTE: this function is mainly used in the situation where if a function 
 * needs to return a dynamically allocated string, but somehow needed to 
 * return the original unchanged string based on the situation. Returning 
 * the original string will lead to memory issues later on because the 
 * caller function will not have any knowledge on whether the returned 
 * string is the original input string or a new dynamically allocated 
 * string. If the function need to return the original input string, then
 * this function will be invoked to return a clone of the original string, 
 * ensuring that the returned string is newly created on the heap to avoid 
 * any memory issues caused by other areas of the program later on. 
 *
 * PARAMETERS: 
 * s   - the bit string to clone
 * len - the length of the bit string, cache this for improved performance
 *
 * RETURNS: 
 * A pointer to the new bit string, or NULL if any error occurred. 
 */
static char *str_clone(char *s, size_t len) {
    if (s == NULL || len == 0)
        return NULL;

    char *new = malloc((len + 1) * (sizeof *new));
    if (new != NULL) {
        new[len] = '\0';
        memcpy(new, s, len * (sizeof *s));
    }
    return new;
}
