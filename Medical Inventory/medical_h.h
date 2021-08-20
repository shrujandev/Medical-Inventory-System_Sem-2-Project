#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//Structure to store data 
typedef struct date
{
    int dd;
    int mm;
    int yy;

} DATE_T;

// Structures that store Payment Details 
typedef struct paymentInfo
{
    char cardNum[16];
    DATE_T expDate;
    int cvv;
    float deduct;
    char medName2[128];
    int purqty;
    DATE_T saleDate;
} PAY_T;


// Structure to store Item Information
typedef struct itemInfo
{
    char medName[128];
    DATE_T medMan;
    DATE_T medExp;
    float unitCost;
    int qty;
    int safetyStock;
    char refID[32]; // 8 character ID
    char type[32]; // eg. : tablet, syrup, injection, inhalable, etc.
} ITEM_T;


void reviewWarnings();
void deleteItem();
void viewPaymentInfo();
void addItem();
void viewItems();
void newPurchase();
void clrscr();
void delay(int number_of_seconds);
int dateCompare(DATE_T d1, DATE_T d2);
void mainMenu(int ch);
