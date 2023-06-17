#ifndef CARD_H
# define  CARD_H

# define _CRT_SECURE_NO_WARNINGS
# define MAX_CARD_NAME 24
# define MAX_CARD_EXPIRY 5
# define MIN_CARD_NAME 20
# define MAX_CARD_PAN 19
# define MIN_CARD_PAN 16
# define NUM_TEST_CASES_CARD 6
# define TEMP_storage 200
# define SPACE_ASCII_CODE 32
# define CARD_EXPIRY_size 5

#include <stdio.h>
#include <string.h>
#include < ctype.h >
typedef unsigned char uint8_t ;

typedef struct ST_cardData_t
{
    uint8_t cardHolderName[25];
    uint8_t primaryAccountNumber[20];
    uint8_t cardExpirationDate[6];
}ST_cardData_t;

typedef enum EN_cardError_t
{
    CARD_OK, WRONG_NAME, WRONG_EXP_DATE, WRONG_PAN
}EN_cardError_t;

typedef enum EN_alphabetic_check_t
{
    ALPHABET, NON_ALPHABET
}EN_alphabetic_check_t;

typedef enum EN_format_check_expiry_t
{
    CORRECT_FORMAT_EXPIRY, WRONG_FORMAT_EXPIRY
}EN_format_check_expiry_t;

typedef enum EN_numeric_check_t
{
    NUMERIC, NON_NUMERIC
}EN_numeric_check_t;

EN_cardError_t getCardHolderName(ST_cardData_t* cardData);
EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData);
EN_cardError_t getCardPAN(ST_cardData_t* cardData);
void getCardHolderNameTest(void);
void getCardExpiryDateTest(void);
void getCardPANTest(void);

#endif 