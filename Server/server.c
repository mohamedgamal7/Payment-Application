# include "server.h"

// valid accounts database generated online using : https://www.dcode.fr/luhn-algorithm
ST_accountsDB_t accountsDB[MAX_ACCOUNTSDB_SIZE] =
{
	{200000.0, RUNNING, "2487057295381329"},
	{5000.5, BLOCKED, "9248971794773338"},
	{6000.0, RUNNING, "3615452981718657"},
	{8000.0, BLOCKED, "2314690050528318259"},
	{4822.0, RUNNING, "599716181384683227"},
	{3589.0, BLOCKED, "26430874932093027"},
	{12000.0, RUNNING, "1619877621838941101"},
	{20000.0, BLOCKED, "1558394422815080748"},
	{98664.0, RUNNING, "1601887806901360"},
	{2000.0, BLOCKED, "9442187206807051"},

};

ST_transaction_t transactionsDB[MAX_TRANSACTIONSSDB_SIZE] = {0};
ST_transaction_t* transactionDB_ptr =&transactionsDB[0]; // global pointer that points initally to the first element in the DB
ST_accountsDB_t* accountRefrence_global = NULL; // accountReference_global is a pointer to the account into the DB
int trans_seq_number = 0; 

EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
	
	ST_cardData_t* cardHolderData_ptr = &transData->cardHolderData; // cardHolderData_ptr is a pointer to the cardHolderData
	ST_terminalData_t* terminalData_ptr = &transData->terminalData; // terminalData_ptr is a pointer to the terminalData
	EN_serverError_t is_valid;
	EN_serverError_t is_amountAvaiable;
	EN_serverError_t is_blocked;
	EN_serverError_t is_saved;

	is_valid = isValidAccount(cardHolderData_ptr, accountRefrence_global);
	if (is_valid == SERVER_OK)
	{
		is_amountAvaiable = isAmountAvailable(terminalData_ptr, accountRefrence_global);
		if (is_amountAvaiable == SERVER_OK)
		{
			is_blocked = isBlockedAccount(accountRefrence_global);
			if (is_blocked == SERVER_OK)
			{
				is_saved = saveTransaction(transData);
				if (is_saved == SERVER_OK) 
				{
					accountRefrence_global->balance = accountRefrence_global->balance - transData->terminalData.transAmount;
					return APPROVED;
				}
				else
					return INTERNAL_SERVER_ERROR;
					
			}
			else
				return DECLINED_STOLEN_CARD;
		}
		else
			return DECLINED_INSUFFECIENT_FUND;

	}
	else
		return FRAUD_CARD;
}
void recieveTransactionDataTest(void) 
{

		printf("Tester Name : Mohamed Gamal Roushdy\n");
		printf("Function Name : recieveTransactionData\n");

		ST_transaction_t trans_test_object;
		ST_transaction_t* transData_test_ptr = &trans_test_object;


		int output;

		for (unsigned int i = 0; i < NUM_TEST_CASES_SERVER - 1; i++)
		{
			switch (i) {

			case 0:  // test if account is not valid 
				printf("Test Case 1: \n");
				printf("Input Data: A PAN which is not found in data base , PAN-> 2398524928798327398\n");
				printf("Expected Result: 3 \n");
				strcpy(trans_test_object.cardHolderData.primaryAccountNumber, "2398524928798327398");
				output = recieveTransactionData(transData_test_ptr);
				printf("Actual Result: %d \n", output);
				break;
			case 1:  // test if account valid but amount is not available
				printf("Test Case 2: \n");
				printf("Input Data: amount not avaiable , PAN->9248971794773338 , trans_amount->6000 ,balance -> 5000.5 \n");
				printf("Expected Result: 1 \n");
				strcpy(trans_test_object.cardHolderData.primaryAccountNumber, "9248971794773338");
				trans_test_object.terminalData.transAmount = 6000;
				output = recieveTransactionData(transData_test_ptr);
				printf("Actual Result: %d \n", output);
				break;
			case 2:  // test if account valid and amount is available but account is blocked  
				printf("Test Case 3: \n");
				printf("Input Data: A blocked account PAN->9248971794773338 , trans_amount->1000 ,balance -> 5000.5 \n");
				printf("Expected Result: 2 \n");
				strcpy(trans_test_object.cardHolderData.primaryAccountNumber, "9248971794773338");
				trans_test_object.terminalData.transAmount = 1000;
				output = recieveTransactionData(transData_test_ptr);
				printf("Actual Result: %d \n", output);
				break;
			case 3:  //  test if account valid , amount is available , account running but unable to save transaction  as transdb full 
				printf("Test Case 4: \n");
				printf("Input Data: transactionDB full PAN->2487057295381329 , trans_amount->1000 ,balance -> 200000 \n");
				printf("Expected Result: 4 \n");
				strcpy(trans_test_object.cardHolderData.primaryAccountNumber, "2487057295381329");
				trans_test_object.terminalData.transAmount = 1000;
				trans_seq_number = 255;
				output = recieveTransactionData(transData_test_ptr);
				printf("Actual Result: %d \n", output);
				break;
			case 4:  // test if account valid , amount is available , account running and transaction saved  
				printf("Test Case 5: \n");
				printf("Input Data: valid and saved transaction PAN->2487057295381329 , trans_amount->1000 ,balance -> 200000 \n");
				printf("Expected Result: 0 \n");
				strcpy(trans_test_object.cardHolderData.primaryAccountNumber, "2487057295381329");
				strcpy(trans_test_object.cardHolderData.cardHolderName, "Mohamed gamal Rousdhy");
				strcpy(trans_test_object.cardHolderData.cardExpirationDate, "12/25");
				strcpy(trans_test_object.terminalData.transactionDate, "24/12/2022");
				trans_test_object.terminalData.transAmount = 1000;
				trans_test_object.terminalData.maxTransAmount = 10000;
				trans_test_object.transactionSequenceNumber = 1;
				trans_test_object.transState = APPROVED;
				trans_seq_number = 0;
				output = recieveTransactionData(transData_test_ptr);
				printf("Actual Result: %d \n", output);
				break;

			default:
				break;

			}
			printf("===================================================\n");
		}

}

EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountsDB_t* accountRefrence) 
{
	
	for (int i = 0; i< ACCOUNTSDB_SIZE; i++)
	{
		if (!strcmp(cardData->primaryAccountNumber, accountsDB[i].primaryAccountNumber))
		{
			accountRefrence= &accountsDB[i];
			accountRefrence_global = accountRefrence;
			return SERVER_OK;
		}
	}

	accountRefrence = NULL;
	return ACCOUNT_NOT_FOUND;

}
void isValidAccountTest(void) 
{
	
	printf("Tester Name : Mohamed Gamal Roushdy\n");
	printf("Function Name : isValidAccount\n");

	
	ST_accountsDB_t* accountRefrence = NULL;
	int output;

	for (unsigned int i = 0; i < NUM_TEST_CASES_SERVER - 3; i++)
	{
		switch (i) {

		case 0:  // account found in database  
			printf("Test Case 1: \n");
			printf("Input Data: Mohamed Gamal Roushdy ,1601887806901360, 02/25 \n");
			printf("Expected Result: 0 \n");
			ST_cardData_t test_card = {"Mohamed Gamal Roushdy ","1601887806901360","02/25"};
			output = isValidAccount(&test_card, accountRefrence);
			printf("Actual Result: %d \n", output);
			break;
		case 1:  // account found in database  
			printf("Test Case 2: \n");
			printf("Input Data: ahmed reda abdallah ,1558394422815080748, 12/24 \n");
			printf("Expected Result: 0 \n");
			ST_cardData_t test_card_1 = { "ahmed reda abdallah ","1558394422815080748","12/24" };
			output = isValidAccount(&test_card_1, accountRefrence);
			printf("Actual Result: %d \n", output);
			break;
		case 2:  // account not found in database  
			printf("Test Case 3: \n");
			printf("Input Data: ahmed samir kamouna ,6544456535346344, 12/26 \n");
			printf("Expected Result: 3 \n");
			ST_cardData_t test_card_2 = { "ahmed samir kamouna","6544456535346344","12/26" };
			output = isValidAccount(&test_card_2, accountRefrence);
			printf("Actual Result: %d \n", output);
			break;

		default:
			break;

		}
		printf("===================================================\n");
	}
}

EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountRefrence) 
{
	for (int i = 0; i < ACCOUNTSDB_SIZE; i++)
	{
		if (accountRefrence->state==RUNNING)
		{
			
			return SERVER_OK;
		}
	}

	return BLOCKED_ACCOUNT;
}
void isBlockedAccountTest(void) 
{

	printf("Tester Name : Mohamed Gamal Roushdy\n");
	printf("Function Name : isBlockedAccount\n");


	ST_accountsDB_t* accountRefrence = NULL;
	int output;

	for (unsigned int i = 0; i < NUM_TEST_CASES_SERVER - 3; i++)
	{
		switch (i) {

		case 0:  // account blocked
			printf("Test Case 1: \n");
			printf("Input Data: A blocked account with index [1] \n");
			printf("Expected Result: 5 \n");
			accountRefrence = &accountsDB[1];
			output = isBlockedAccount(accountRefrence);
			printf("Actual Result: %d \n", output);
			break;
		case 1:  // account blocked 
			printf("Test Case 2: \n");
			printf("Input Data:  A blocked account with index [3] \n");
			printf("Expected Result: 5 \n");
			accountRefrence = &accountsDB[3];
			output = isBlockedAccount(accountRefrence);
			printf("Actual Result: %d \n", output);
			break;
		case 2:  // account running  
			printf("Test Case 3: \n");
			printf("Input Data: A running account with index [0] \n");
			printf("Expected Result: 0 \n");
			accountRefrence = &accountsDB[0];
			output = isBlockedAccount(accountRefrence);
			printf("Actual Result: %d \n", output);
			break;

		default:
			break;

		}
		printf("===================================================\n");
	}
}
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData, ST_accountsDB_t* accountRefrence) 
{
	if (termData->transAmount <= accountRefrence->balance)
	{
		return SERVER_OK;
	}
	else
		return LOW_BALANCE;
}
void isAmountAvailableTest(void) 
{
	printf("Tester Name : Mohamed Gamal Roushdy\n");
	printf("Function Name : isAmountAvailable\n");


	ST_accountsDB_t* accountRefrence = NULL;
	ST_terminalData_t test_terminal_subject;
	ST_terminalData_t* test_terminal_ptr = & test_terminal_subject;
	int output;

	for (unsigned int i = 0; i < NUM_TEST_CASES_SERVER - 3; i++)
	{
		switch (i) {

		case 0:  // balance not available
			printf("Test Case 1: \n");
			printf("Input Data: trans amount -> 90000 account amount ->6000 \n");
			printf("Expected Result: 4 \n");
			accountRefrence = &accountsDB[2];
			test_terminal_subject.transAmount = 90000;
			output = isAmountAvailable(test_terminal_ptr,accountRefrence);
			printf("Actual Result: %d \n", output);
			break;
		case 1:  // balance not available
			printf("Test Case 2: \n");
			printf("Input Data: trans amount -> 10000 account amount ->5000.5 \n");
			printf("Expected Result: 4 \n");
			accountRefrence = &accountsDB[1];
			test_terminal_subject.transAmount = 10000;
			output = isAmountAvailable(test_terminal_ptr, accountRefrence);
			printf("Actual Result: %d \n", output);
			break;
		case 2:  // balance available
			printf("Test Case 3: \n");
			printf("Input Data: trans amount -> 500 account amount ->200000.0 \n");
			printf("Expected Result: 0 \n");
			accountRefrence = &accountsDB[0];
			test_terminal_subject.transAmount = 500;
			output = isAmountAvailable(test_terminal_ptr, accountRefrence);
			printf("Actual Result: %d \n", output);
			break;

		default:
			break;

		}
		printf("===================================================\n");
	}
}
EN_serverError_t saveTransaction(ST_transaction_t* transData) 
{
	if (trans_seq_number <= MAX_TRANSACTIONSSDB_SIZE-1) //transcationsDb not full
	{
		transData->transState = APPROVED; // only approved transactions are saved

		transactionDB_ptr->cardHolderData = transData->cardHolderData;
		transactionDB_ptr->terminalData = transData->terminalData;
		transactionDB_ptr->transState = transData->transState; 
		// saves approved transaction only based on https://nfpdiscussions.udacity.com/t/server-module-save-transaction-function/444237/3 
		transactionDB_ptr->transactionSequenceNumber = trans_seq_number+1;  // to start sequence numbering from 1 not zero

		trans_seq_number++;
		transactionDB_ptr++;

		listSavedTransactions();
		return SERVER_OK;
	}

	else
	{
		//listSavedTransactions();
		return INTERNAL_SERVER_ERROR;
	}
	



}

void saveTransactionTest(void) 
{
	ST_transaction_t* transData__test_ptr = NULL;
	ST_transaction_t trans_test_object; 
	int output; 
	printf("Tester Name : Mohamed Gamal Roushdy\n");
	printf("Function Name : saveTransaction\n");

	for (unsigned int i = 0; i < NUM_TEST_CASES_SERVER - 4; i++)
	{
		switch (i) {


		case 0:  // properly saved 
			printf("Test Case 1: \n");
			printf("Input Data:  name ->Mohamed Gamal Roushdy ,PAN->3615452981718657,expiry-> 12/24 ,transdate->12/01/2023 ,amount-> 100 ,max->1000  \n");
			printf("Expected Result: 0 \n");
			strcpy(trans_test_object.cardHolderData.primaryAccountNumber, "3615452981718657");
			strcpy(trans_test_object.cardHolderData.cardExpirationDate, "12/24");
			strcpy(trans_test_object.cardHolderData.cardHolderName, "Mohamed gamal Roushdy");

			strcpy(trans_test_object.terminalData.transactionDate, "12/01/2023");
			trans_test_object.terminalData.transAmount = 100;
			trans_test_object.terminalData.maxTransAmount = 1000;

			trans_test_object.transactionSequenceNumber = 1;
			trans_test_object.transState = APPROVED;
			transData__test_ptr=& trans_test_object;
			output = saveTransaction(transData__test_ptr);
			printf("Actual Result: %d \n", output);
			break;

		case 1:  // transaction sequnce number exceeded maxDB available
			printf("Test Case 2: \n");
			printf("Input Data: a sequnce number exceeding the avaiable DB (lists saved transaction is commented) \n");
			printf("Expected Result: 4 \n");
			trans_seq_number = 255;
			output = saveTransaction(transData__test_ptr);
			printf("Actual Result: %d \n", output);
			break;

		default:
			break;

		}
		printf("===================================================\n");
	}

}

void listSavedTransactions(void) 
{
	printf("#########################\n");

	for (int i = 0; i < trans_seq_number; i++)
	{ 
		
		printf("Transaction Sequence Number:%d\n",transactionsDB[i].transactionSequenceNumber);
		printf("Transaction Date:%s\n", transactionsDB[i].terminalData.transactionDate);
		printf("Transaction Amount:%.2f\n", transactionsDB[i].terminalData.transAmount);
		printf("Transaction State:%d\n", transactionsDB[i].transState);
		printf("Terminal Max Amount:%.2f\n", transactionsDB[i].terminalData.maxTransAmount);
		printf("Cardholder Name:%s\n", transactionsDB[i].cardHolderData.cardHolderName);
		printf("PAN:%s\n", transactionsDB[i].cardHolderData.primaryAccountNumber);
		printf("Card Expiration Date:%s\n", transactionsDB[i].cardHolderData.cardExpirationDate);
		printf("#########################\n");
	}
}

void listSavedTransactionsTest(void) 
{

	printf("Tester Name : Mohamed Gamal Roushdy\n");
	printf("Function Name : listSavedTransactions\n");
	// 2 transactions will be filled manually and printed using listsaved transactions 

	for (int i = 0 ;i <2 ; i ++)
	{
		transactionsDB[i].transactionSequenceNumber = i + 1; 
		strcpy(transactionsDB[i].terminalData.transactionDate, "24/12/2022");
		transactionsDB[i].terminalData.transAmount = 100;
		transactionsDB[i].transState = APPROVED;
		transactionsDB[i].terminalData.maxTransAmount = 1000;
		strcpy(transactionsDB[i].cardHolderData.cardHolderName, "MOhamed Gamal Roushdy");
		strcpy(transactionsDB[i].cardHolderData.primaryAccountNumber, "3615452981718657");
		strcpy(transactionsDB[i].cardHolderData.cardExpirationDate, "11/24");
		trans_seq_number++;


	}

	printf("Test Case 1: \n");
	printf("Input Data: name ->Mohamed Gamal Roushdy ,PAN->3615452981718657,expiry-> 11/24 ,transdate->24/12/2022 ,amount-> 100 ,max->1000 \n");
	printf("Expected Result: printed twice in the format of listsavedtrans function \n");
	printf("Actual Result: % \n");
	trans_seq_number = 2;
	listSavedTransactions();


}

