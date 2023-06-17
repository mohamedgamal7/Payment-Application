#include "terminal.h"
#include <stdio.h>
#include <string.h>
#include < ctype.h >
#include <time.h>


EN_TRANS_FORMAT_CHECK_t checkTansFormat(const char* transactionDate)
{
	char DD[2];
	char MM[2];
	char YYYY[4];
	int DD_int,MM_int,YYYY_int;

	//  check that each of days, months and years inputs are digits only

	for (int i = 0; i < 2; i++) //days
	{
		if (isdigit(transactionDate[i]))
		{
			DD[i] = transactionDate[i];
		}
		else
		{
			return WRONG_FORMAT_TRM;
		}
    }  

	for (int i = 0; i < 2; i++)  //months
	{
		if (isdigit(transactionDate[i+3]))
		{
			MM[i] = transactionDate[i+3];
		}
		else
		{
			return WRONG_FORMAT_TRM;
		}
	}

	for (int i = 0; i < 4; i++) //years
	{

		if (isdigit(transactionDate[i + 6])) {
			YYYY[i] = transactionDate[i + 6]; // start from YYYY index
		}

		else
		{
			return WRONG_FORMAT_TRM;
		}
	}

	DD_int = atoi(DD);  // change the DD array of chars to an int to check that value is not greater than 31
	MM_int = atoi(MM);  // change the MM array of chars to an int to check that value isnot greater than 12

	if (transactionDate[2] != '/' || transactionDate[5] != '/'|| DD_int > 31 || DD_int <= 0 ||  MM_int > 12 || MM_int <= 0)
	{
		 return WRONG_FORMAT_TRM;
	}
	else
		return CORRECT_FORMAT_TRM;


}

EN_terminalError_t getTransactionDate(ST_terminalData_t* termData) 
{
	char buffer[11];
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	sprintf(buffer, "%02d/%02d/%02d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
	//printf("Please enter the transaction date \n");
	//gets(buffer);

	if ((NULL == buffer) || strlen(buffer) < MIN_TRANS_DATE || checkTansFormat(buffer))

	{
		return WRONG_DATE;
	}

	else
	{
		strcpy(termData->transactionDate, buffer);
		return TERMINAL_OK;
	}


}


void getTransactionDateTest(void) 
{
	printf("Tester Name : Mohamed Gamal Roushdy\n");
	printf("Function Name : getTransactionDate\n");

	ST_terminalData_t test_subject;
	int output;

	for (unsigned int i = 0; i < NUM_TEST_CASES_TRANS; i++)
	{
		switch (i) {

		case 0:  // not all digits  
			printf("Test Case 1: \n");
			printf("Input Data: sa/12/2022 \n");
			printf("Expected Result: 1 \n");
			output = getTransactionDate(&test_subject);
			printf("Actual Result: %d \n", output);
			break;
		case 1:  // woring seprator 
			printf("Test Case 2: \n");
			printf("Input Data: 20-05-2022 \n");
			printf("Expected Result: 1 \n");
			output = getTransactionDate(&test_subject);
			printf("Actual Result: %d \n", output);
			break;
		case 2:  // wrong month
			printf("Test Case 3: \n");
			printf("Input Data: 12/13/2022  \n");
			printf("Expected Result: 1 \n");
			output = getTransactionDate(&test_subject);
			printf("Actual Result: %d \n", output);
			break;

		case 3:  // wrong day 
			printf("Test Case 4: \n");
			printf("Input Data: 33/10/2025  \n");
			printf("Expected Result: 1 \n");
			output = getTransactionDate(&test_subject);
			printf("Actual Result: %d \n", output);
			break;

		case 4:  // correct date
			printf("Test Case 5: \n");
			printf("Input Data: 20/12/2022 \n");
			printf("Expected Result: 0 \n");
			output = getTransactionDate(&test_subject);
			printf("Actual Result: %d \n", output);
			break;
		case 5:  // correct date
			printf("Test Case 6: \n");
			printf("Input Data: 15/09/2029 \n");
			printf("Expected Result: 0 \n");
			output = getTransactionDate(&test_subject);
			printf("Actual Result: %d \n", output);
			break;
		default:
			break;

		}
		printf("===================================================\n");
	}
		
}


EN_terminalError_t isCardExpired(ST_cardData_t* cardData, ST_terminalData_t* termData)
{
	char MM_Card[2];
	char YY_Card[2];

	char MM_Trans[2];
	char YY_Trans[2];

	int MM_Card_int, YY_Card_int, MM_Trans_int, YY_Trans_int;


	for (int i = 0 ;i<2; i++) //get MM_card
	{
		MM_Card[i] = cardData->cardExpirationDate[i];
	}

	for (int i = 0;i<2 ; i++) //get YY_card
	{
		YY_Card[i] = cardData->cardExpirationDate[i+3];
	}

	for (int i = 0; i<2 ; i++) // get MM_Trans 
	{ 
		MM_Trans[i] = termData->transactionDate[i+3];
	}

	for (int i = 0; i<2 ; i++)  // get YY_Trans
	{
		YY_Trans[i] = termData->transactionDate[i + 8];
	}

	MM_Card_int = atoi(MM_Card);
	YY_Card_int = atoi(YY_Card);
	MM_Trans_int = atoi(MM_Trans);
	YY_Trans_int = atoi(YY_Trans);

	if (YY_Trans_int < YY_Card_int) // card not expired (card expiry year is after transaction year)
	{
		return TERMINAL_OK;
	}
	else if (YY_Trans_int > YY_Card_int) // card expired (card expiry year is before transaction year)
	{
		return EXPIRED_CARD;
	}

	else
	{
		if (MM_Trans_int < MM_Card_int)  // card not expired (card expiry month is after transaction month)
		{
			return TERMINAL_OK;

		}
		else  // card expired (card expiry month is before or equal to transaction month)
		{
			return EXPIRED_CARD; 
		}

	}


}

void isCardExpriedTest(void) 
{
	printf("Tester Name : Mohamed Gamal Roushdy\n");
	printf("Function Name : isCardExpired\n");

	ST_terminalData_t test_terminal_subject;
	ST_cardData_t test_card_subject;
	int output;

	for (unsigned int i = 0; i < NUM_TEST_CASES_TRANS; i++)
	{
		switch (i) {

		case 0:  // expired (year)
			printf("Test Case 1: \n");
			printf("Input Data: card->11/22 trans->11/11/2023 \n");
			strcpy(test_card_subject.cardExpirationDate, "11/22");
			strcpy(test_terminal_subject.transactionDate, "11/11/2023");
			printf("Expected Result: 2 \n");
			output = isCardExpired(&test_card_subject,&test_terminal_subject);
			printf("Actual Result: %d \n", output);
			break;
		case 1:  // expired (month)
			printf("Test Case 2: \n");
			printf("Input Data: card->11/22 trans->11/12/2022 \n");
			strcpy(test_card_subject.cardExpirationDate, "11/22");
			strcpy(test_terminal_subject.transactionDate, "11/12/2022");
			printf("Expected Result: 2 \n");
			output = isCardExpired(&test_card_subject, &test_terminal_subject);
			printf("Actual Result: %d \n", output);
			break;
		case 2:  // expired (same transaction date as card )
			printf("Test Case 3: \n");
			printf("Input Data: card->11/22 trans->12/11/2022 \n");
			strcpy(test_card_subject.cardExpirationDate, "11/22");
			strcpy(test_terminal_subject.transactionDate, "12/11/2022");
			printf("Expected Result: 2 \n");
			output = isCardExpired(&test_card_subject, &test_terminal_subject);
			printf("Actual Result: %d \n", output);
			break;

		case 3:  // valid 
			printf("Test Case 4: \n");
			printf("Input Data: card->11/23 trans->12/10/2023 \n");
			strcpy(test_card_subject.cardExpirationDate, "11/23");
			strcpy(test_terminal_subject.transactionDate, "12/10/2023");
			printf("Expected Result: 0 \n");
			output = isCardExpired(&test_card_subject, &test_terminal_subject);
			printf("Actual Result: %d \n", output);
			break;

		case 4:  // valid
			printf("Test Case 5: \n");
			printf("Input Data: card->11/24 trans->12/11/2023 \n");
			strcpy(test_card_subject.cardExpirationDate, "11/24");
			strcpy(test_terminal_subject.transactionDate, "12/11/2023");
			printf("Expected Result: 0 \n");
			output = isCardExpired(&test_card_subject, &test_terminal_subject);
			printf("Actual Result: %d \n", output);
			break;
		case 5:  // valid
			printf("Test Case 6: \n");
			printf("Input Data: card->05/24 trans->10/01/2023 \n");
			strcpy(test_card_subject.cardExpirationDate, "05/24");
			strcpy(test_terminal_subject.transactionDate, "10/01/2023");
			printf("Expected Result: 0 \n");
			output = isCardExpired(&test_card_subject, &test_terminal_subject);
			printf("Actual Result: %d \n", output);
			break;
		default:
			break;

		}
		printf("===================================================\n");
	}
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData) 
{
	float transAmountTemp=0.0;
	printf("PLease enter the transaction amount : \n");
	scanf("%f", &transAmountTemp);
	if (transAmountTemp <= 0)
	{
		return INVALID_AMOUNT;
	}
	else
	{
		termData->transAmount = transAmountTemp; 
		return TERMINAL_OK;
	}

}

void getTransactionAmountTest(void) 
{
	printf("Tester Name : Mohamed Gamal Roushdy\n");
	printf("Function Name : getTransactionAmount\n");

	ST_terminalData_t test_terminal_subject;
	int output;

	for (unsigned int i = 0; i < NUM_TEST_CASES_TRANS-3; i++)
	{
		switch (i) {

		case 0:  // trans amount less than 0 
			printf("Test Case 1: \n");
			printf("Input Data: -1000  \n");
			printf("Expected Result: 4 \n");
			output = getTransactionAmount(&test_terminal_subject);
			printf("Actual Result: %d \n", output);
			break;
		case 1:  // trans amount equal 0  
			printf("Test Case 2: \n");
			printf("Input Data: 0 \n");
			printf("Expected Result: 4 \n");
			output = getTransactionAmount(&test_terminal_subject);
			printf("Actual Result: %d \n", output);
			break;
		case 2:  // trans amount correct 
			printf("Test Case 3: \n");
			printf("Input Data: 3000.55  \n");
			printf("Expected Result: 0 \n");
			output = getTransactionAmount(&test_terminal_subject);
			printf("Actual Result: %d \n", output);
			break;

		default:
			break;

		}
		printf("===================================================\n");
	}
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{


	if (termData->transAmount > termData->maxTransAmount)
	{
		return EXCEED_MAX_AMOUNT;
	}
	else
	{
		return TERMINAL_OK;
	}
}

void isBelowMaxAmountTest(void) 
{
	printf("Tester Name : Mohamed Gamal Roushdy\n");
	printf("Function Name : isBelowMaxAmount\n");

	ST_terminalData_t test_terminal_subject;
	int output;

	for (unsigned int i = 0; i < NUM_TEST_CASES_TRANS - 3; i++)
	{
		switch (i) {

		case 0:  // trans amount more than max amount  
			printf("Test Case 1: \n");
			printf("Input Data: trans amount->50000.6 max amount->10000  \n");
			printf("Expected Result: 5 \n");
			test_terminal_subject.transAmount = 50000.6;
			test_terminal_subject.maxTransAmount = 10000;
			output = isBelowMaxAmount(&test_terminal_subject);
			printf("Actual Result: %d \n", output);
			break;
		case 1:  // trans amount equal the max amount
			printf("Test Case 2: \n");
			printf("Input Data: trans amount->10000 max amount->10000  \n");
			printf("Expected Result: 0 \n");
			test_terminal_subject.transAmount = 10000;
			test_terminal_subject.maxTransAmount = 10000;
			output = isBelowMaxAmount(&test_terminal_subject);
			printf("Actual Result: %d \n", output);
			break;
		case 2:  // trans amount less than max mount 
			printf("Test Case 3: \n");
			printf("Input Data: trans amount->9000.5 max amount->10000  \n");
			printf("Expected Result: 0 \n");
			test_terminal_subject.transAmount = 9000.5;
			test_terminal_subject.maxTransAmount = 10000;
			output = isBelowMaxAmount(&test_terminal_subject);
			printf("Actual Result: %d \n", output);
			break;

		default:
			break;

		}
		printf("===================================================\n");
	}
}

EN_terminalError_t setMaxAmount(ST_terminalData_t* termData, float maxAmount)
{    
	printf("PLease enter the max transaction amount : \n");
	scanf("%f", &maxAmount);
	if (maxAmount <=0)
	{
		return INVALID_MAX_AMOUNT;
	}
	else
	{
		termData->maxTransAmount = maxAmount;
		return TERMINAL_OK;
	}
}

void setMaxAmountTest(void) 
{
	printf("Tester Name : Mohamed Gamal Roushdy\n");
	printf("Function Name : setMaxAmount\n");

	ST_terminalData_t test_terminal_subject;
	float maxAmountTemp=0.0;
	int output;

	for (unsigned int i = 0; i < NUM_TEST_CASES_TRANS - 3; i++)
	{
		switch (i) {

		case 0:  // max amount less than 0
			printf("Test Case 1: \n");
			printf("Input Data: max amount->-163  \n");
			printf("Expected Result: 6 \n");
			output = setMaxAmount(&test_terminal_subject, maxAmountTemp);
			printf("Actual Result: %d \n", output);
			break;
		case 1:  //  max amount equal 0 
			printf("Test Case 2: \n");
			printf("Input Data: max amount->0  \n");
			printf("Expected Result: 6 \n");
			output = setMaxAmount(&test_terminal_subject, maxAmountTemp);
			printf("Actual Result: %d \n", output);
			break;
		case 2:  //  max mount correct 
			printf("Test Case 3: \n");
			printf("Input Data: max amount->10000  \n");
			printf("Expected Result: 0 \n");
			output = setMaxAmount(&test_terminal_subject, maxAmountTemp);
			printf("Actual Result: %d \n", output);
			break;

		default:
			break;

		}
		printf("===================================================\n");
	}

}

EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData) // Optional
{
	
	int primaryAccountNumberLength = strlen(cardData->primaryAccountNumber);
	// 1st case of luhn's algorithm
	char lastDigit;
	int sum = 0;
	char isSecond = 0;
	for (int i = primaryAccountNumberLength-1; i>=0 ; i--) // start from index zero 
	{
		lastDigit = cardData->primaryAccountNumber[i] -'0';  // remove the ascii value of zero (48) to get correct sum
		if (isSecond) 
		{
			lastDigit = lastDigit * 2;
			
		}

		sum += lastDigit / 10 + lastDigit % 10;
		isSecond = !(isSecond);
	}
	if (sum % 10 == 0)
	{   
		return TERMINAL_OK;
	}
	else
		return INVALID_CARD;
}

void isValidCardPANTest(void)
{
	printf("Tester Name : Mohamed Gamal Roushdy\n");
	printf("Function Name : isValidCardPAN\n");

	ST_cardData_t test_card_subject;
	int output;

	for (unsigned int i = 0; i < NUM_TEST_CASES_TRANS - 2; i++)
	{
		switch (i) {

		case 0:  // INVALID PAN 19 numbers 
			printf("Test Case 1: \n");
			printf("Input Data: 2542548758954562158 \n");
			printf("Expected Result: 3 \n");
			strcpy(test_card_subject.primaryAccountNumber, "2542548758954562158");
			output = isValidCardPAN(&test_card_subject);
			printf("Actual Result: %d \n", output);
			break;
		case 1:  // INVALID PAN 16 numbers
			printf("Test Case 2: \n");
			printf("Input Data: 2545878954856215 \n");
			printf("Expected Result: 3 \n");
			strcpy(test_card_subject.primaryAccountNumber, "2545878954856215");
			output = isValidCardPAN(&test_card_subject);
			printf("Actual Result: %d \n", output);
			break;
		case 2:  //  valid PAN generated online 16 numbers
			printf("Test Case 3: \n");
			printf("Input Data: 8069266611712405 \n");
			printf("Expected Result: 0 \n");
			strcpy(test_card_subject.primaryAccountNumber, "8069266611712405");
			output = isValidCardPAN(&test_card_subject);
			printf("Actual Result: %d \n", output);
			break;
		case 3:  //  valid PAN generated online 19 numbers
			printf("Test Case 3: \n");
			printf("Input Data: 6484653611759738390 \n");
			printf("Expected Result: 0 \n");
			strcpy(test_card_subject.primaryAccountNumber, "6484653611759738390");
			output = isValidCardPAN(&test_card_subject);
			printf("Actual Result: %d \n", output);
			break;

		default:
			break;

		}
		printf("===================================================\n");
	}
}
