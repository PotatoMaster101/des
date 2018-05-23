/**
 * FILE:   des.c
 * AUTHOR: PotatoMaster101
 * DATE:   23/03/2018
 * 
 * An implementation of the DES cipher. 
 *
 * C99
 */

#include "des.h"

/**
 * Key rotation function pointer. 
 */
typedef void (*KeyRot)(char *k56, int r);

static char *des(char *msg, char *k56, KeyRot k_func);
static char *f_func(char *r, char *k48);
static char *sbox_value(char *b, int sbox);
static char *key_init(char *k);
static void key_rot_enc(char *k56, int r);
static void key_rot_dec(char *k56, int r);

/**
 * The initial permutation (64). 
 */
static const int IP[] = {
    58, 50, 42, 34, 26, 18, 10, 2, 
    60, 52, 44, 36, 28, 20, 12, 4, 
    62, 54, 46, 38, 30, 22, 14, 6, 
    64, 56, 48, 40, 32, 24, 16, 8, 
    57, 49, 41, 33, 25, 17, 9, 1, 
    59, 51, 43, 35, 27, 19, 11, 3, 
    61, 53, 45, 37, 29, 21, 13, 5, 
    63, 55, 47, 39, 31, 23, 15, 7
};

/**
 * The inverse of the initial permutation (64). 
 */
static const int IP_INV[] = {
    40, 8, 48, 16, 56, 24, 64, 32, 
    39, 7, 47, 15, 55, 23, 63, 31, 
    38, 6, 46, 14, 54, 22, 62, 30, 
    37, 5, 45, 13, 53, 21, 61, 29, 
    36, 4, 44, 12, 52, 20, 60, 28, 
    35, 3, 43, 11, 51, 19, 59, 27, 
    34, 2, 42, 10, 50, 18, 58, 26, 
    33, 1, 41, 9, 49, 17, 57, 25
};

/**
 * The key permutation PC-1 (56). 
 */
static const int PC1[] = {
    57, 49, 41, 33, 25, 17, 9, 
    1, 58, 50, 42, 34, 26, 18, 
    10, 2, 59, 51, 43, 35, 27, 
    19, 11, 3, 60, 52, 44, 36, 
    63, 55, 47, 39, 31, 23, 15, 
    7, 62, 54, 46, 38, 30, 22, 
    14, 6, 61, 53, 45, 37, 29, 
    21, 13, 5, 28, 20, 12, 4
};

/**
 * The key permutation PC-2 (48). 
 */
static const int PC2[] = {
    14, 17, 11, 24, 1, 5, 
    3, 28, 15, 6, 21, 10, 
    23, 19, 12, 4, 26, 8, 
    16, 7, 27, 20, 13, 2, 
    41, 52, 31, 37, 47, 55, 
    30, 40, 51, 45, 33, 48, 
    44, 49, 39, 56, 34, 53, 
    46, 42, 50, 36, 29, 32
};

/**
 * The expansion permutation for f-function (48). 
 */
static const int EXP[] = {
    32, 1, 2, 3, 4, 5, 
    4, 5, 6, 7, 8, 9, 
    8, 9, 10, 11, 12, 13, 
    12, 13, 14, 15, 16, 17, 
    16, 17, 18, 19, 20, 21, 
    20, 21, 22, 23, 24, 25, 
    24, 25, 26, 27, 28, 29, 
    28, 29, 30, 31, 32, 1
};

/**
 * The permutation that shuffles a 32-bit block (32). 
 */
static const int P[] = {
    16, 7, 20, 21, 29, 12, 28, 17, 
    1, 15, 23, 26, 5, 18, 31, 10, 
    2, 8, 24, 14, 32, 27, 3, 9, 
    19, 13, 30, 6, 22, 11, 4, 25
};

/**
 * All of the 8 s-boxes, 64 numbers each. 
 * S-box dimension = 4 * 16. 
 */
static const int SBOX[8][64] = {
    //sbox 1
    {
        14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7, 
        0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8, 
        4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0, 
        15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13
    }, 
    //sbox 2
    {
        15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
        3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5, 
        0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15, 
        13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9
    }, 
    //sbox 3
    {
        10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
        13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1, 
        13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7, 
        1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12
    }, 
    //sbox 4
    {
        7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15, 
        13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9, 
        10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4, 
        3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14
    }, 
    //sbox 5
    {
        2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9, 
        14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6, 
        4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14, 
        11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3
    }, 
    //sbox 6
    {
        12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11, 
        10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8, 
        9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6, 
        4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13
    }, 
    //sbox 7
    {
        4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1, 
        13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6, 
        1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2, 
        6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12
    }, 
    //sbox 8
    {
        13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7, 
        1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2, 
        7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8, 
        2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11
    }
};

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
char *des_enc(char *msg, char *k64) {
    size_t msglen = bstr_len(msg);
    if (msglen != 64)
        return NULL;    //only 64 bit msg allowed

    char *k56 = key_init(k64);
    char *cipher = NULL;
    if (k56 != NULL) {
        cipher = des(msg, k56, &key_rot_enc);
        free(k56);
    }
    return cipher;
}

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
char *des_dec(char *msg, char *k64) {
    size_t msglen = bstr_len(msg);
    if (msglen != 64)
        return NULL;    //only 64 bit msg allowed

    char *k56 = key_init(k64);
    char *plain = NULL;
    if (k56 != NULL) {
        plain = des(msg, k56, &key_rot_dec);
        free(k56);
    }
    return plain;
}

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
char *des_permute(char *str, const int p[], size_t len) {
    size_t oldlen = bstr_len(str);
    if (oldlen == 0 || len == 0 || p == NULL)
        return NULL;

    char *new = malloc((len + 1) * (sizeof *new));
    if (new != NULL) {
        new[len] = '\0';
        for (size_t i = 0; i < len; i++) {
            if ((p[i] - 1) > oldlen) {
                free(new);      //an element is out of bound
                return NULL;
            }
            new[i] = str[p[i] - 1];
        }
    }
    return new;
}

/**
 * Encrypts or decrypts the message based on the passed in key rotation 
 * function pointer. Give a 64-bit message and a 56-bit key, this function 
 * will encrypt or decrypt the specified message. The result will be stored 
 * on the heap. This function will not perform any error check to increase 
 * efficiency. If any memory error occurred, NULL will be returned. 
 *
 * PARAMETERS: 
 * msg    - the 64-bit message to encrypt/decrypt
 * k56    - the 56-bit key used
 * k_func - the key rotation function, depending on encryption or 
 *          decryption
 *
 * RETURNS: 
 * The encrypted/decrypted text, or NULL if any error occurred. 
 */
static char *des(char *msg, char *k56, KeyRot k_func) {
    char *ct = des_permute(msg, IP, 64);
    char *result = NULL;
    if (ct != NULL) {
        for (int i = 1; i <= 16; i++) {
            k_func(k56, i);     //56-bit key rotation
            char *k48 = des_permute(k56, PC2, 48);   //current 48bit subkey
            if (k48 != NULL) {
                char *ffunc = f_func(ct + 32, k48);
                if (ffunc != NULL) {
                    char temp = ct[32];
                    ct[32] = '\0';
                    bstr_xor(ct, ffunc);    //xor left with f-func
                    ct[32] = temp;
                    bstr_swap(ct);          //swap left and right
                    free(ffunc);
                }
                free(k48);
            }
        }
        bstr_swap(ct);
        result = des_permute(ct, IP_INV, 64);
        free(ct);
    }
    return result;
}

/**
 * The f-function for DES. Takes in a 32-bit message (the right half) 
 * and a key of 48 bits, outputs a 32-bit block used to XOR the left 
 * half. For efficiency reasons, this function will not perform any 
 * error checking. Returns the 32-bit result, or NULL if any memory 
 * error occurred. 
 * 
 * PARAMETERS: 
 * r - the 32-bit block
 * k - the 48-bit key
 *
 * RETURNS: 
 * The 32-bit result, or NULL if any error occurred. 
 */
static char *f_func(char *r, char *k48) {
    char *expanded = des_permute(r, EXP, 48);    //expansion to 48 bits
    if (expanded == NULL)
        return NULL;

    bstr_xor(expanded, k48);
    int r_index = 0;            //store current index of r for sbox index
    int fill_index = 0;         //store current index of r for filling bits
    for (int i = 0; i < 8; i++) {
        char *sbox_bin = sbox_value(expanded + r_index, i);
        memcpy(expanded + fill_index, sbox_bin, 4 * sizeof(char));
        r_index += 6;           //original separates by 6-bit chunk
        fill_index += 4;        //result separates by 4-bit chunk
    }
    expanded[32] = '\0';

    char *result = des_permute(expanded, P, 32);
    free(expanded);
    return result;
}

/**
 * Retrieves the specified sbox value (4 bits) based on the input binary. 
 * The binary must be 6 bits or more in total. This function will not 
 * validate the input for increased performance. 
 *
 * PARAMETERS: 
 * b    - the binary to extract the sbox
 * sbox - the sbox to use, from 0 to 7
 *
 * RETURNS: 
 * The 4-bit s-box value binary that the specified input represents. 
 */
static char *sbox_value(char *b, int sbox) {
    static char bin[] = "0000";
    bin[0] = bin[1] = bin[2] = bin[3] = '0';    //reset bits

    char row[] = { b[0], b[5], '\0' };
    char col[] = { b[1], b[2], b[3], b[4], '\0' };
    int sbox_index = 16 * strtol(row, NULL, 2) + strtol(col, NULL, 2);
    int sbox_val = SBOX[sbox][sbox_index];
    if (sbox_val > 15 || sbox_val < 0)
        return bin;     //sbox out of range, return 0000

    int i = 3;  //length index of bit string
    do {
        bin[i--] = (sbox_val & 1) + '0';
    } while ((sbox_val >>= 1) > 0);
    return bin;
}

/**
 * Initialises the specified key, permutating it to 56 bits. The returned 
 * key will be dynamically allocated. If any memory error occurred, then 
 * NULL will be returned. If the key is short, then it will be padded with 
 * 0. 
 *
 * PARAMETERS: 
 * k - the key to initialise
 *
 * RETURNS: 
 * The initialised key, or NULL if any error occurred. 
 */
static char *key_init(char *k) {
    size_t len = bstr_len(k);
    if (len == 0)
        return NULL;    //key is NULL or empty, unfixable
    if (len < 64)
        k = bstr_pad(k, 64);

    char *new = des_permute(k, PC1, 56);
    if (len < 64)
        free(k);    //bstr_pad() dynamically allocates
    return new;
}

/**
 * Rotates the specified 56-bit key according to the current round number. 
 * The key will be split in half and be rotated by 1 or 2 on each half based 
 * on the round number. This key rotation function is used for encryption. 
 *
 * PARAMETERS: 
 * k56 - the 56-bit key to rotate
 * r   - the round number
 */
static void key_rot_enc(char *k56, int r) {
    char mid = k56[28];
    k56[28] = '\0';      //split string

    if ((r >= 3 && r <= 8) || (r >= 10 && r <= 15)) {
        bstr_lrot(k56, 2);
        k56[28] = mid;
        bstr_lrot(k56 + 28, 2);
    } else {            //round 1, 2, 9, 16
        bstr_lrot(k56, 1);
        k56[28] = mid;
        bstr_lrot(k56 + 28, 1);
    }
}

/**
 * Rotates the specified 56-bit key according to the current round number. 
 * The key will be split in half and be rotated by 1 or 2 on each half based 
 * on the round number. This key rotation function is used for decryption. 
 *
 * PARAMETERS: 
 * k56 - the 56-bit key to rotate
 * r   - the round number
 */
static void key_rot_dec(char *k56, int r) {
    if (r == 1)
        return;     //no change in round 1 decryption

    char mid = k56[28];
    k56[28] = '\0';      //split string

    if ((r >= 3 && r <= 8) || (r >= 10 && r <= 15)) {
        bstr_rrot(k56, 2);
        k56[28] = mid;
        bstr_rrot(k56 + 28, 2);
    } else {            //round 2, 9, 16
        bstr_rrot(k56, 1);
        k56[28] = mid;
        bstr_rrot(k56 + 28, 1);
    }
}
