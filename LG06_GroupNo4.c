#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/* ================= STRUCT ================= */

typedef struct {
    char category[20];	//Family, Individual; Adult, Children, Toddler, Baby
    int vipSeat;		//1 = Yes, 0 = No
    int foodCoupon;		//1 = Yes, 0 = No
    float price;		//Ticket price
} Ticket;

typedef struct {
    int receiptNo;
    int numTickets;
    Ticket tickets[5];	//Maximum 5 tickets per transaction
    float totalAmount;
} Transaction;

/* ================= FUNCTION PROTOTYPE ================= */

void displayMenu();																	//to display menu and system control
void inputTransaction(Transaction *org, char category, int id);						//to collect purchase details (family)
void inputTicket(Ticket *ticket);													//to collect purchase details (individual)

float calculateTicketPrice(Ticket ticket);											//to calculate the ticket price
float calculateTotal(Transaction t);												//to calculate total amount of transaction


void saveToFile(Transaction transactions[], int start_index, int end_index);		//file handling
void displayReceipt(Transaction t[], int session, int count);						//to display receipt of the pruchase
void displayAllTransaction();														//to display transaction history
int getLastReceiptNo();																//to get receipt number

/* ================= GLOBAL ================= */

Transaction transactions[100];														//store up to 100 transactions
int count = 0;																		//counter index for total transactions

/* ================= MAIN ================= */

int main()
{
    int session;
    char category, choice, select;													

    do {
        printf("\n========== MOVIE TICKETING SYSTEM ==========\n");
        printf("1. New Transaction\n");
        printf("2. View Transaction History\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        scanf(" %c", &choice);

        switch (choice) {

        case '1':
            displayMenu();		//function call to display menu details
            session = count; 
            
            //customer must have ONE ID for the entire session
            int id = getLastReceiptNo() + 1;
            
            do {	
            	//input validation for customer enters the category
                do {
                    printf("\nSelect category (F - Family | I - Individual): ");
                    scanf(" %c", &category);
                    category = toupper(category);		//convert to uppercase to avoid case sensitive
    
                    if (category != 'F' && category != 'I')
                        printf("Invalid category! Try again.\n");
    
                } while (category != 'F' && category != 'I');
    			
    			//call function to perform transaction details
                inputTransaction(&transactions[count], category, id);
                transactions[count].totalAmount = calculateTotal(transactions[count]); 	//call function to perform total amount calculation
    
                count++;	//update the number of transaction after purchase
                
                //input validation for customer to make another purchase or not
                do {
			        printf("\nDo you want to make another purchase? (Y - Yes | N - No): ");
			        scanf(" %c", &select);
			        select = toupper(select);	//convert to uppercase
			
			        if (select != 'Y' && select != 'N') {
			            printf("Invalid input! Please enter Y or N.\n");
			        }
			        
			    } while (select != 'Y' && select != 'N');
                
            } while (select == 'Y');
            
           //call function to display receipt
            displayReceipt(transactions, session, count);
            
            break;

        case '2':
            displayAllTransaction();
            break;

        case '0':
            printf("Thank you. Program terminated.\n");
            break;

        default:
            printf("Invalid choice!\n");
            
        }

    } while (choice != '0');

    return 0;
}

/* ================= DISPLAY MENU ================= */

void displayMenu()
{
     printf("\n===========================================================================================\n");
     printf("\t\t\t\tMovie Ticketing Details\t\t\t\t\n");
     printf("===========================================================================================\n");
     
     printf("\tCategory""\t|""\tTicket(RM)""\t|" "\tVIP seat(RM)""\t|" "Food and drink(RM)\n");
     printf("-------------------------------------------------------------------------------------------\n");
     printf("Family\t  ""|""2 Adults""\t|""30.00""\t\t\t|""40.00""\t\t\t|""20.00\n");
     printf("---------------------""\t|""per family"                  "\t\t|""per family""\t\t|""per family\n");
     printf("      \t  ""|""2 Children""\t|"      "\t\t\t|""""\t\t\t|""\n");
     printf("-------------------------------------------------------------------------------------------\n");
     printf("Individual""|""Adults""\t|""10.00""\t\t\t|""12.00""\t\t\t|""8 per pax\n");
     printf("          |""\t\t|""per person"                  "\t\t|""per person""\t\t|\n");
     printf("-------------------------------------------------------------------------------------------\n");
     printf("        ""  |""Children""\t|""6.00""\t\t\t|""8.00""\t\t\t|""6 per pax\n");
     printf("          |""\t\t|""per person"                  "\t\t|""per person""\t\t|\n");
     printf("-------------------------------------------------------------------------------------------\n");
}

/* ================= RECEIPT NUMBER ================= */

int getLastReceiptNo()
{
	// Open the file using read mode
    FILE *fptr = fopen("transactions.txt", "r");
    
    //to check whether the file is NULL
    if (fptr == NULL)
        return 0;

    int last = 0, temp;
    char line[300];

	//to read the file line by line until the end of file
    while (fgets(line, sizeof(line), fptr)) {
        if (sscanf(line, "%d", &temp) == 1)
            last = temp;	//'last' will hold the ID from the last row
    }

    fclose(fptr);	// Always close the file to save data properly
    return last;
}

/* ================= INPUT TRANSACTION ================= */

void inputTransaction(Transaction *org, char category, int id)
{
    int i;
    org->receiptNo = id;
    
    char vip, coupon;
    
     // --- FAMILY CATEGORY ---
    if( category == 'F'){
        org->numTickets = 1;        //1 family = 1 ticket
        
        //input validation for customers to purchase VIP seat
        do{ 
            printf("\nPurchase for Family VIP seat? (1- YES | 0- NO): ");
            scanf(" %c", &vip);
            
            if(vip != '0' && vip != '1'){
                printf("\nInvalid Input. Please enter again! ");
            }   
        }while(vip != '0' && vip != '1');
        
        //convert char to int for VIPSeat
        org->tickets[0].vipSeat = (vip == '1') ? 1 : 0;
        
        //input validation for customers to add on Food Coupon
        do{
            printf("Add Food Coupon for the family? (1 - YES | 0 - NO): ");
            scanf(" %c", &coupon);
            
            if(coupon != '0' && coupon != '1'){
                    printf("\nInvalid Input. Please enter again!\n ");
            }   
        }while(coupon != '0' && coupon != '1');
        
        //convert char to int for foodCoupon
        org->tickets[0].foodCoupon = (coupon == '1') ? 1 : 0;
        
        //to copy 'FAMILY' name category in struct
        strcpy(org->tickets[0].category, "Family");
    }
     // --- INDIVIDUAL CATEGORY ---
    else {
       
        printf("Number of tickets (max 5): ");
        scanf("%d", &org->numTickets);
		
		//input validation to ensure customers dont exceed ticket limit of 5
        while (org->numTickets < 1 || org->numTickets > 5) {
            
            printf("Invalid. Enter again (1-5): ");
            scanf("%d", &org->numTickets);
        }

        for (i = 0; i < org->numTickets; i++) {
            
            printf("\nTicket #%d\n", i + 1);
            inputTicket(&org->tickets[i]);		//call function to calculate ticket price (pass address)
        }
    }
}

/* ================= INPUT TICKET ================= */

void inputTicket(Ticket *ticket)
{
    int age;
    char vip, coupon;

    printf("Enter age: ");
    scanf("%d", &age);

	//to categorize the ticket based on age
    if (age > 12)
        strcpy(ticket->category, "Adult");
    else if (age >= 5)
        strcpy(ticket->category, "Children");
    else if (age >= 2)
        strcpy(ticket->category, "Toddler");
    else
        strcpy(ticket->category, "Baby");
        
    //input validation for VIP seat add on
    do {
        printf("VIP Seat? (1-Yes 0-No): ");
        scanf(" %c", &vip);
    } while (vip != '0' && vip != '1');
    
    ticket->vipSeat = (vip == '1') ? 1 : 0;

    //input validation for Food Coupon add on
    do {
        printf("Food Coupon? (1-Yes 0-No): ");
        scanf(" %c", &coupon);
    } while (coupon != '0' && coupon != '1');
    
    ticket->foodCoupon = (coupon == '1') ? 1 : 0;
}

/* ================= PRICE CALCULATION ================= */

float calculateTicketPrice(Ticket ticket)
{
    float price = 0;
	
	//compare the category name to get each price
    if (strcmp(ticket.category, "Adult") == 0) {
        price = (ticket.vipSeat) ? 12 : 10;
        if (ticket.foodCoupon) price += 8;
    }
    else if (strcmp(ticket.category, "Children") == 0) {
        price = (ticket.vipSeat) ? 8 : 6;
        if (ticket.foodCoupon) price += 6;
    }
    else if (strcmp(ticket.category, "Toddler") == 0) {
        price = 6;
        if (ticket.vipSeat) price += 2;
    }
    else {
        price = 0;		//baby will get free ticket
    }

    return price;
}

float calculateTotal(Transaction t)
{
    float total = 0;
    int i;

	//check the condition if the purchase is a Family Package
    if (strcmp(t.tickets[0].category, "Family") == 0) {
        total = 30;
        if (t.tickets[0].vipSeat) total += 40;
        if (t.tickets[0].foodCoupon) total += 20;
    }
    else {
        for (i = 0; i < t.numTickets; i++)
            total += calculateTicketPrice(t.tickets[i]);	//call function to get ticket price
    }

    return total;
}

/* ================= SAVE TO FILE  ================= */

void saveToFile(Transaction transactions[], int start_index, int end_index) {

	// Open the file using append mode
    FILE *fptr = fopen("transactions.txt", "a"); 

	// Safety mode -> to display an error if the file cannot be oppened
    if (fptr == NULL) {
        printf("Error: Could not open file for saving.\n");
        return;
    }


    fseek(fptr, 0, SEEK_END);			// Move cursor to end of file.
    long fileSize = ftell(fptr);		// Get current position (size in bytes).
    
    // Check the file size to determine whether to put the header or not.If there a data inside it will skip the header
    if (fileSize == 0) {
        fprintf(fptr, "%-12s | %-25s | %-10s | %-8s | %-12s | %-10s\n", 
            "Receipt No.", "Category", "Qty", "VIP Seat", "Food/Drink", "Amount(RM)");
        fprintf(fptr, "---------------------------------------------------------------------------------------------\n");
    }


    float sessionGrandTotal = 0;
    char idStr[20];
    
  	// Format the Receipt ID to be 4 digits with leading zeros (e.g., 1 becomes "0001")
    sprintf(idStr, "%04d", transactions[start_index].receiptNo);
    
    int isFirstLine = 1;	// Flag to control printing the id only on the first row of group

    
    int i;
    for (i = start_index; i < end_index; i++) {
        
        Transaction t = transactions[i];
        sessionGrandTotal += t.totalAmount; // Add to combined total

        // OPTION A: Family Category Handling
        if (strcmp(t.tickets[0].category, "Family") == 0) {
            char vipStr[5], foodStr[5];
            char currentIdStr[20];
			
			// Short Form of if else (change 1 to "1" and change else to "-")
            strcpy(vipStr, (t.tickets[0].vipSeat == 1) ? "1" : "-");
            strcpy(foodStr, (t.tickets[0].foodCoupon == 1) ? "1" : "-");

             // Using if-else to print Receipt ID only on the first row, then empty strings after it
            if(isFirstLine) {
                strcpy(currentIdStr, idStr);
                isFirstLine = 0; 				//set flag to false so the next lines are blank
            } else {
                strcpy(currentIdStr, "");
            }

			// Write the Family row to file
            fprintf(fptr, "%-12s | %-25s | %-10d | %-8s | %-12s | %10.2f\n",
                    currentIdStr, "Family Package", 1, vipStr, foodStr, t.totalAmount);
        }
        //OPTION B: Individual Category Handling
        else {
            int j;
            for (j = 0; j < t.numTickets; j++) {
                char fullCategory[40];
                char vipStr[5], foodStr[5];
                char currentIdStr[20];
                float singlePrice = calculateTicketPrice(t.tickets[j]);

                sprintf(fullCategory, "Individual - %s", t.tickets[j].category);
                
                // Short Form of if else (change 1 to "1" and change else to "-")
                strcpy(vipStr, (t.tickets[j].vipSeat == 1) ? "1" : "-");
                strcpy(foodStr, (t.tickets[j].foodCoupon == 1) ? "1" : "-");

                // Using if else to print Receipt ID only on the first row, then empty strings after it
                if (isFirstLine) {
                    strcpy(currentIdStr, idStr);
                    isFirstLine = 0; 				//set flag to false so the next lines are blank 
                } else {
                    strcpy(currentIdStr, ""); 
                }
				
				// Write the Individual row to file
                fprintf(fptr, "%-12s | %-25s | %-10d | %-8s | %-12s | %10.2f\n",
                        currentIdStr, fullCategory, 1, vipStr, foodStr, singlePrice);
            }
        }
    }
    
  	// Print the total amount for this batch of transactions
    fprintf(fptr, "%-12s | %-25s | %-10s | %-8s | %-12s | %10.2f\n", "", "", "", "", "Total :", sessionGrandTotal);
    
    // Draw the closing line
    fprintf(fptr, "---------------------------------------------------------------------------------------------\n");
    
    // Always close the file to save data properly
    fclose(fptr);
}

/* ================= READ FILE ================= */
void displayAllTransaction()
{
	// Open the file using read mode
    FILE *fptr = fopen("transactions.txt", "r");
    char line[300];

	//to check whether the file is exist. the file will be NULL, if it doesnt exist
    if (fptr == NULL) {
        printf("\nNo transaction history found.\n");
        return;
    }

    printf("\n=========== TRANSACTION HISTORY ===========\n");
    
    	//to read the file line by line until the end of file
    	while (fgets(line, sizeof(line), fptr)) {
        	printf("%s", line);		//to print the current line in the text file
    	}
    printf("===========================================\n");

    fclose(fptr);		// Always close the file to save data properly
}

/* ================== DISPLAY RECEIPT ====================== */
void displayReceipt(Transaction t[], int session, int count){
	
	float grandTotal = 0;
    char label[30];
    int j;

    printf("\n===============================================");
    printf("\n                  RECEIPT                 ");
    printf("\n                ID NO: %04d                    ", transactions[session].receiptNo);
    printf("\n===============================================");
    printf("\n %-12s | %-18s | %-10s", "Category", "Quantity", "Amount");
    printf("\n-----------------------------------------------");

    //loop through all items purchased
    for (j = session; j < count; j++) {
       
        //to print each purchase detail
       printf("\n %-20s  %-13d  RM %.2f", 
	   	transactions[j].tickets[0].category, transactions[j].numTickets, transactions[j].totalAmount);

        // to perform total price calculation
        grandTotal += transactions[j].totalAmount;
        
    }

    printf("\n-----------------------------------------------");
    printf("\nTOTAL PRICE  \t\t\t      RM %.2f", grandTotal);
    printf("\n===============================================");
    printf("\n      Thank you for your purchase!             ");
    printf("\n      All data saved to transactions.txt       ");
    printf("\n===============================================\n");
    
     //call function to save transaction to file
    saveToFile(transactions, session, count);
}

