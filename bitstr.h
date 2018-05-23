/**
 * FILE:   bitstr.h
 * AUTHOR: PotatoMaster101
 * DATE:   21/03/2018
 * 
 * Utility functions for handling bit strings. 
 *
 * C99
 */

#ifndef __bstr_h__
#define __bstr_h__
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

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
size_t bstr_len(char *str);

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
char *bstr_new(char *str);

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
char *bstr_original(char *str);

/**
 * Pads the specified bit string until its length is the same as n. If the 
 * specified string is not a bit string or the n is 0, then this 
 * function will do nothing and NULL returned, otherwise this function will 
 * return a new bit string that is dynamically allocated, which stores the 
 * result of padding. 
 *
 * PARAMETERS: 
 * str - the bit string to pad
 * n   - the new length
 *
 * RETURNS: 
 * NULL if any error occurred, otherwise a pointer to the new padded bit 
 * string. 
 */
char *bstr_pad(char *str, size_t n);

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
_Bool bstr_lrot(char *str, uint32_t n);

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
_Bool bstr_rrot(char *str, uint32_t n);

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
_Bool bstr_xor(char *a, char *b);

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
_Bool bstr_flip(char *str);

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
_Bool bstr_swap(char *str);

#endif
