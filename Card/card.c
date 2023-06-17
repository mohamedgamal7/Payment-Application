#include "card.h"


uint8_t tempCardHolderName[TEMP_storage] = "temp";

EN_alphabetic_check_t isAllAlhpabetic(const char* cardHolderName) {
	unsigned int i;
	for (i = 0;cardHolderName[i];i++)
		if (!isalpha(cardHolderName[i])&& cardHolderName[i]!= SPACE_ASCII_CODE) 
			// accept aplhabets and white spaces only 
			return NON_ALPHABET;
	return ALPHABET;
}

EN_cardError_t getCardHolderName(ST_cardData_t* cardData) {

	
	if (strlen(tempCardHolderName)) {
		printf("PLease enter the Card holder's name :\n");
	}
	// created to prevent stackoverflow of cardholdername when user enters a very long string
	fgets(tempCardHolderName, TEMP_storage, stdin);
	tempCardHolderName[strcspn(tempCardHolderName, "\r\n")] = 0;
	if ((NULL == tempCardHolderName) || strlen(tempCardHolderName) > MAX_CARD_NAME
	    || strlen(tempCardHolderName) < MIN_CARD_NAME || isAllAlhpabetic(tempCardHolderName) )

	{
		
		return WRONG_NAME;
	}
	
	else
	{
		strcpy(cardData->cardHolderName, tempCardHolderName);
		return CARD_OK;
	}
}

void getCardHolderNameTest(void) {

	printf("Tester Name : Mohamed Gamal Roushdy\n");
	printf("Function Name : getCardHolderName\n");

	ST_cardData_t test_subject;
	int output;

	for ( unsigned int i = 0; i < NUM_TEST_CASES_CARD; i++)
	{   
		switch (i) {
		
		case 0 :  // A very long name 
			printf("Test Case 1: \n");
			printf("Input Data: skfajjjjk slfkjglkfjdlksgjafdl dskajgfssdkfkadsk kasdkffgslkd asdkggdsl zsdklgfsd  ghjkds \n");
			printf("Expected Result: 1 \n"); 
			output = getCardHolderName(&test_subject);
			printf("Actual Result: %d \n",output); 
			break;
		case 1:  // A short name 
			printf("Test Case 2: \n");
			printf("Input Data: mg gamal \n");
			printf("Expected Result: 1 \n");
			output = getCardHolderName(&test_subject);
			printf("Actual Result: %d \n", output);
			break;
		case 2:  // A name with non- alphabetic characters
			printf("Test Case 3: \n");
			printf("Input Data: !!Q@#$#adhfadj adgjfasj  \n");
			printf("Expected Result: 1 \n");
			output = getCardHolderName(&test_subject);
			printf("Actual Result: %d \n", output);
			break;

		case 3:  // no name entered 
			printf("Test Case 4: \n");
			printf("Input Data:  \n");
			printf("Expected Result: 1 \n");
			output = getCardHolderName(&test_subject);
			printf("Actual Result: %d \n", output);
			break;

		case 4:  // a name in capital letters
			printf("Test Case 5: \n");
			printf("Input Data: MOHAMED GAMAL ROUSHDY \n");
			printf("Expected Result: 0 \n");
			output = getCardHolderName(&test_subject);
			printf("Actual Result: %d \n", output);
			break;
		case 5:  // a name in small letters
			printf("Test Case 6: \n");
			printf("Input Data: mohamed gamal roushdy \n");
			printf("Expected Result: 0 \n");
			output = getCardHolderName(&test_subject);
			printf("Actual Result: %d \n", output);
			break;
		default :
			break;
		
		}

		printf("===================================================\n");

	}
	
}

EN_format_check_expiry_t isFormatCorrect(const char* cardExpiryDate) {
	// months should be numeric values that don't exceed 12 and 
	// years should be numerics starting from 22(current year) and a slash between  MM/YY
	char MM[2];
	char YY[2];
	int MM_int;

	//  check that each of years and months inputs are digits only

	for (int i = 0; i < 2; i++) {
		if (isdigit(cardExpiryDate[i]))
		{
			MM[i] = cardExpiryDate[i];
		}
		else
		{
			return WRONG_FORMAT_EXPIRY;
		}
	}
	for (int i = 0; i < 2; i++) {

		if (isdigit(cardExpiryDate[i + 3])) {
			YY[i] = cardExpiryDate[i + 3]; // start from YY index
		}

		else
		{  
			return WRONG_FORMAT_EXPIRY;
		}
	}

	MM_int = atoi(MM);  // change the MM array of chars to an int to check that value isnot greater than 12
	if (cardExpiryDate[2] != '/' || MM_int > 12 || MM_int <= 0)
	{	 return WRONG_FORMAT_EXPIRY;
    }
	else
		return CORRECT_FORMAT_EXPIRY;

}


EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData) 
{
	printf("PLease enter the Card expiry date : \n");
	uint8_t tempCardExpiryDate[TEMP_storage] = "";
	// created to prevent stackoverflow of cardholdername when user enters very long strings
	//gets(tempCardExpiryDate);
	fgets(tempCardExpiryDate, TEMP_storage, stdin);
	tempCardExpiryDate[strcspn(tempCardExpiryDate, "\r\n")] = 0;
	if ((NULL == tempCardExpiryDate) || strlen(tempCardExpiryDate) != MAX_CARD_EXPIRY || 
		isFormatCorrect(tempCardExpiryDate))
	{
		return WRONG_EXP_DATE;
	}
	else
		strcpy(cardData->cardExpirationDate, tempCardExpiryDate);
		return CARD_OK;
}


void getCardExpiryDateTest(void) {

	printf("Tester Name : Mohamed Gamal Roushdy\n");
	printf("Function Name : getCardExpiryDate\n");

	ST_cardData_t test_subject;
	int output;

	for (unsigned int i = 0; i < NUM_TEST_CASES_CARD; i++)
	{
		switch (i) {

		case 0:  // wrong month
			printf("Test Case 1: \n");
			printf("Input Data: 00/11  \n");
			printf("Expected Result: 2 \n");
			output = getCardExpiryDate(&test_subject);
			printf("Actual Result: %d \n", output);
			break;
		case 1:  // wrong month 
			printf("Test Case 2: \n");
			printf("Input Data: 13/25 \n");
			printf("Expected Result: 2 \n");
			output = getCardExpiryDate(&test_subject);
			printf("Actual Result: %d \n", output);
			break;
		case 2:  // wrong input
			printf("Test Case 3: \n");
			printf("Input Data: df/4f  \n");
			printf("Expected Result: 2 \n");
			output = getCardExpiryDate(&test_subject);
			printf("Actual Result: %d \n", output);
			break;

		case 3:  // wrong seprator 
			printf("Test Case 4: \n");
			printf("Input Data: 12-25 \n");
			printf("Expected Result: 2 \n");
			output = getCardExpiryDate(&test_subject);
			printf("Actual Result: %d \n", output);
			break;

		case 4:  // wrong input
			printf("Test Case 5: \n");
			printf("Input Data: 2/26 \n");
			printf("Expected Result: 2 \n");
			output = getCardExpiryDate(&test_subject);
			printf("Actual Result: %d \n", output);
			break;
		case 5:  // correct input
			printf("Test Case 6: \n");
			printf("Input Data: 05/23 \n");
			printf("Expected Result: 0 \n");
			output = getCardExpiryDate(&test_subject);
			printf("Actual Result: %d \n", output);
			break;
		default:
			break;

		}

		printf("===================================================\n");

	}

}

EN_numeric_check_t isNumeric(const char* PAN)
{
	for (int i = 0 ; PAN[i] ; i++) 
	{
		if (isdigit(PAN[i]))
			continue;
		else 
		{
			return NON_NUMERIC;
		}
	}
	return NUMERIC;
}

EN_cardError_t getCardPAN(ST_cardData_t* cardData) 
{
	printf("PLease enter the card's Primary Account Number : \n");
	uint8_t tempCardPAN[TEMP_storage] = "";
	// created to prevent stackoverflow of cardholdername when user enters very long strings
	//gets(tempCardPAN);
	fgets(tempCardPAN, TEMP_storage, stdin);
	tempCardPAN[strcspn(tempCardPAN, "\r\n")] = 0;
	if ((NULL == tempCardPAN) || strlen(tempCardPAN) > MAX_CARD_PAN 
		|| strlen(tempCardPAN) < MIN_CARD_PAN || isNumeric(tempCardPAN))
	{
		return WRONG_PAN;
	}
	else
		strcpy(cardData->primaryAccountNumber, tempCardPAN);
	return CARD_OK;
}

void getCardPANTest(void) {

	printf("Tester Name : Mohamed Gamal Roushdy\n");
	printf("Function Name : getCardPAN\n");

	ST_cardData_t test_subject;
	int output;

	for (unsigned int i = 0; i < NUM_TEST_CASES_CARD; i++)
	{
		switch (i) {

		case 0:  // more than 19 numbers
			printf("Test Case 1: \n");
			printf("Input Data: 15426542598756458975625  \n");
			printf("Expected Result: 3 \n");
			output = getCardPAN(&test_subject);
			printf("Actual Result: %d \n", output);
			break;
		case 1:  // less than 16 numbers 
			printf("Test Case 2: \n");
			printf("Input Data: 132535245621 \n");
			printf("Expected Result: 3 \n");
			output = getCardPAN(&test_subject);
			printf("Actual Result: %d \n", output);
			break;
		case 2:  // wrong input
			printf("Test Case 3: \n");
			printf("Input Data: fdr12554225425  \n");
			printf("Expected Result: 3 \n");
			output = getCardPAN(&test_subject);
			printf("Actual Result: %d \n", output);
			break;

		case 3:  // correct input 16 numbers
			printf("Test Case 4: \n");
			printf("Input Data: 1225564875482598 \n");
			printf("Expected Result: 0 \n");
			output = getCardPAN(&test_subject);
			printf("Actual Result: %d \n", output);
			break;

		case 4:  // correct input 19 numbers
			printf("Test Case 5: \n");
			printf("Input Data: 1254872154698546589 \n");
			printf("Expected Result: 0 \n");
			output = getCardPAN(&test_subject);
			printf("Actual Result: %d \n", output);
			break;
		case 5:  // correct input 17 numbers
			printf("Test Case 6: \n");
			printf("Input Data: 12542565458565456 \n");
			printf("Expected Result: 0 \n");
			output = getCardPAN(&test_subject);
			printf("Actual Result: %d \n", output);
			break;
		default:
			break;

		}

		printf("===================================================\n");

	}
}