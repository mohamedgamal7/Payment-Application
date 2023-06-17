# include "app.h"

int main(void) 
{

	while (1) {
	
		appStart();
	}
}

void appStart(void)
{

	ST_cardData_t cardData;
	EN_cardError_t card_data_state;

	ST_terminalData_t termData;
	EN_terminalError_t term_data_state;

	ST_transaction_t transData;
	EN_transState_t trans_data_state;


	//                                       * card module *                                     //
	
	// card name 
	do { card_data_state = getCardHolderName(&cardData); 
	} while (card_data_state != CARD_OK);

	// card expiry date 
	do { card_data_state = getCardExpiryDate(&cardData);
	} while (card_data_state != CARD_OK);

	// get card PAN
	do { card_data_state = getCardPAN(&cardData);
	} while (card_data_state != CARD_OK);

	//                                        * Terminal module *                              //

	// transaction date
	do
	{
		term_data_state = getTransactionDate(&termData);
	} while (term_data_state != TERMINAL_OK);

	// is card expired 

	term_data_state = isCardExpired(&cardData, &termData);
	if (term_data_state == EXPIRED_CARD) {

		printf(" Declined Expired Card \n");

	}

	else 
	{
		do
		{
		term_data_state = getTransactionAmount(&termData);
		} while (term_data_state != TERMINAL_OK);

		float maxTransAmount=0;
		setMaxAmount(&termData, maxTransAmount);
		term_data_state = isBelowMaxAmount(&termData);

		if (term_data_state == EXCEED_MAX_AMOUNT)
		{
			printf("Declined amount exceeding limit \n");
		}

		else
		{
			term_data_state = isValidCardPAN(&cardData);
			if (term_data_state == INVALID_CARD)
			{
				printf("Declined Fraud card (failed in Luhun check) \n");
			}
			else
			{
				//                                        * Server module *                              //
				transData.cardHolderData = cardData;
				transData.terminalData = termData;
				trans_data_state = recieveTransactionData(&transData);

				if (trans_data_state == FRAUD_CARD)
				{
					printf("Declined invalid account\n");

				}

				else if (trans_data_state == DECLINED_INSUFFECIENT_FUND)
				{
					printf("Declined insuffecient funds\n");

				}

				else if (trans_data_state == DECLINED_STOLEN_CARD)
				{
					printf("Declined stolen card \n");

				}
				else if (trans_data_state == INTERNAL_SERVER_ERROR)
				{
					printf("Internal server error \n");

				}

				else
				{
					printf("Transaction approved \n");
					printf("\n");
				}
				
				

			}
		}

	}



}