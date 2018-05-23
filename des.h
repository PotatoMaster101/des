/**
 * FILE:   des.h
 * AUTHOR: PotatoMaster101
 * DATE:   23/03/2018
 * 
 * An implementation of the DES cipher. 
 *
 * C99
 */

#ifndef __des_h__
#define __des_h__
#include <stdlib.h>
#include <string.h>
#include "bitstr.h"

/**
 * Encrypts the specified message with the specified key. The key must be 
 * 64 bits, if it is not then it will be padded or truncated. The result 
 * cipher text will be dynamically allocated. If any error occurred, then 
 * NULL will be returned. The message length must be 64 bits. 
 *
 * PARAMETERS: 
 * msg - the message to encrypt
 * k64 - the 64-bit key
 *
 * RETURNS: 
 * The cipher text, or NULL if any error occurred. 
 */
char *des_enc(char *msg, char *k64);

/**
 * Decrypts the specified message with the given key. The key must be 
 * 64 bits, if it is not then it will be padded or truncated. The result 
 * plain text will be dynamically allocated. If any error occurred, then 
 * NULL will be returned. The message length must be 64 bits. 
 *
 * PARAMETERS: 
 * msg - the message to decrypt
 * k64 - the 64-bit key
 *
 * RETURNS: 
 * The plain text, or NULL if any error occurred. 
 */
char *des_dec(char *msg, char *k64);

/**
 * Permutes the specified bit string using the given permutation and length. 
 * Returns the result bit string after the permutation (dynamically 
 * allocated). If any error occurred during permutation process, NULL will 
 * be returned. 
 *
 * PARAMETERS: 
 * str - the bit string to permute
 * p   - the permutation mapping
 * len - the length of the permutation mapping
 *
 * RETURNS: 
 * The permutated bit string, or NULL if any error occurred. 
 */
char *des_permute(char *str, const int p[], size_t s);

#endif
