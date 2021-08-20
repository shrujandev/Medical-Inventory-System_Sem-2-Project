#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "medical_h.h"


// create the following binary files beforehand
char path1[] = "data.bin"; // Files 
char path2[] = "paymentInfo.bin";



int recCount = 0;


// Function to Check the date of manufacture and Date of Expiry
void datecheck(ITEM_T *n, int *datechecker)
{
    if(n->medExp.yy< n->medMan.yy || (n->medExp.yy == n->medMan.yy && n->medExp.mm < n->medMan.mm) || (n->medExp.yy == n->medMan.yy && (n->medExp.mm == n->medMan.mm) && (n->medExp.dd < n->medMan.dd)))
        *datechecker = -1;
    else 
        *datechecker = 1;

}


// Function to add a line 
void line()
{
    printf("\n________________________________________________________________________________________\n");
}

// Function to display all the warnings with the stored inventory
void reviewWarnings()
{
    FILE* fp;
    ITEM_T myrecord;
    DATE_T today;
    int count = 1;

    fp = fopen(path1, "rb");
    if (!fp)
    {
        printf("Unable to open file!");
        exit(0);
    }

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    today.yy = tm.tm_year + 1900;
    today.mm = tm.tm_mon + 1;
    today.dd = tm.tm_mday;

    line();
    while (fread(&myrecord, sizeof(ITEM_T), 1, fp) != NULL)
    {
        if (myrecord.qty <= myrecord.safetyStock)
        {
            printf("\n");
            printf("Serial number : %d\n", count++);
            printf("Name : %s\n", myrecord.medName);
            printf("Date of manufacture : %d / %d / %d\n", myrecord.medMan.dd, myrecord.medMan.mm, myrecord.medMan.yy);
            printf("Date of expiry : %d / %d / %d\n", myrecord.medExp.dd, myrecord.medExp.mm, myrecord.medExp.yy);
            printf("Cost per unit : %f\n", myrecord.unitCost);
            printf("Quantity in stock : %d\n", myrecord.qty);
            printf("Safety stock : %d\n", myrecord.safetyStock);
            printf("Reference ID : %s\n", myrecord.refID);
            printf("Type : %s\n", myrecord.type);
            printf("Warning : Inadequate safety stock. Restock immediately\n", myrecord.type);
            printf("\n");
        }


        if (dateCompare(today, myrecord.medExp) > 0)
        {
            printf("\n");
            printf("Serial number : %d\n", count++);
            printf("Name : %s\n", myrecord.medName);
            printf("Date of manufacture : %d / %d / %d\n", myrecord.medMan.dd, myrecord.medMan.mm, myrecord.medMan.yy);
            printf("Date of expiry : %d / %d / %d\n", myrecord.medExp.dd, myrecord.medExp.mm, myrecord.medExp.yy);
            printf("Cost per unit : %f\n", myrecord.unitCost);
            printf("Quantity in stock : %d\n", myrecord.qty);
            printf("Safety stock : %d\n", myrecord.safetyStock);
            printf("Reference ID : %s\n", myrecord.refID);
            printf("Type : %s\n", myrecord.type);
            printf("Warning : Stock expired. Discard immediately\n", myrecord.type);
            printf("\n");
        }
    }

    fclose(fp);
}

// Function to delete items from Inventory
void deleteItem()
{
    FILE* fp;
    FILE* fp_tmp;
    int found = 0;
    ITEM_T myrecord;
    char searchname[16];

    line();
    printf("Input the name of the record to be deleted : ");
    scanf(" %[^\n]%*c", searchname);
    line();

    fp = fopen(path1, "rb");
    if (!fp)
    {
        printf("Unable to open file");
        exit(0);
    }
    fp_tmp = fopen("tmp.bin", "wb");
    if (!fp_tmp)
    {
        printf("Unable to open temp file");
        exit(0);
    }

    while (fread(&myrecord, sizeof(ITEM_T), 1, fp) != NULL)
    {
        if (strcmp(searchname, myrecord.medName) == 0)
        {
            printf("A record with requested name found and deleted.\n\n");
            found = 1;
        }
        else
        {
            fwrite(&myrecord, sizeof(ITEM_T), 1, fp_tmp);
        }
    }

    if (!found)
    {
        printf("No record(s) found with the requested name: %s\n\n", searchname);
    }

    fclose(fp);
    fclose(fp_tmp);
    remove(path1);
    rename("tmp.bin", path1);
}


// Function to store payment info
void viewPaymentInfo()
{
    FILE* fp;
    PAY_T myrecord;
    int count = 1;

    fp = fopen(path2, "rb");
    if (!fp)
    {
        printf("Unable to open file!");
        exit(0);
    }

    line();
    while (fread(&myrecord, sizeof(PAY_T), 1, fp) != NULL)
    {
        printf("\n");
        printf("Serial number : %d\n", count++);
        printf("Card number : %s\n", myrecord.cardNum);
        printf("Date of expiry : %d / %d / %d\n", myrecord.expDate.dd, myrecord.expDate.mm, myrecord.expDate.yy);
        printf("CVV : %d\n", myrecord.cvv);
        printf("Amount deducted : %f\n", myrecord.deduct);
        printf("\n");
    }

    fclose(fp);
}

// Function to add items to inventory
void addItem()
{
    FILE* fp;
    ITEM_T newrecord;
    int i = 0;

    // Open for binary writing
    fp = fopen(path1, "ab");

    if (!fp)
    {
        printf("Unable to open file!");
        exit(0);
    }

    /*
    // test cases (old version)
    strcpy(newrecord.medName, "paracetamol");
    newrecord.qty = 12;
    strcpy(newrecord.refID, "A1B2C3");
    strcpy(newrecord.type, "Tablet");
    fwrite(&newrecord, sizeof(ITEM_T), 1, fp);

    strcpy(newrecord.medName, "benadryl");
    newrecord.qty = 13;
    strcpy(newrecord.refID, "A1B2C4");
    strcpy(newrecord.type, "Syrup");
    fwrite(&newrecord, sizeof(ITEM_T), 1, fp);

    strcpy(newrecord.medName, "asthalin");
    newrecord.qty = 14;
    strcpy(newrecord.refID, "A1B2C5");
    strcpy(newrecord.type, "Inhalable");
    fwrite(&newrecord, sizeof(ITEM_T), 1, fp);

    strcpy(newrecord.medName, "covaxin");
    newrecord.qty = 12;
    strcpy(newrecord.refID, "A1B2C6");
    strcpy(newrecord.type, "Injection");
    fwrite(&newrecord, sizeof(ITEM_T), 1, fp);
    */
    do
    {
        recCount++;
        line();
        int datechecker = -1;
        printf("Input name (w/ brand) : ");
        scanf(" %[^\n]%*c", newrecord.medName);
        while(datechecker == -1)
        {
            printf("Input date of manufacture in dd <enter> mm <enter> yy format\n");
            scanf(" %d", &newrecord.medMan.dd);
            scanf(" %d", &newrecord.medMan.mm);
            scanf(" %d", &newrecord.medMan.yy);
            printf("Input date of expiry in dd <enter> mm <enter> yy format\n");
            scanf(" %d", &newrecord.medExp.dd);
            scanf(" %d", &newrecord.medExp.mm);
            scanf(" %d", &newrecord.medExp.yy);

            datecheck(&newrecord,&datechecker); //Dosent allow doe before
            if(datechecker == -1)
                printf("\nSorry Date of Expiry cannot be before Date of manufacture ... \nPlease enter Date again\n");
        }
        
        printf("Input cost per unit : ");
        scanf(" %f", &newrecord.unitCost);
        printf("Input stock quantity : ");
        scanf(" %d", &newrecord.qty);
        printf("Input safety stock quantity : ");
        scanf(" %d", &newrecord.safetyStock);
        printf("Input reference ID : ");
        scanf(" %[^\n]%*c", newrecord.refID);
        printf("Input type (manner of consumption) : ");
        scanf(" %[^\n]%*c", newrecord.type);
        line();

        fwrite(&newrecord, sizeof(ITEM_T), 1, fp);

        char writeAnother;
        int writeAnotherCounter = 0;
        do
        {
            printf("\nWould you like to write another record? (y/n) ");
            scanf("%s", &writeAnother);
            switch (writeAnother)
            {
            case 'y':
                i = 0;
                writeAnotherCounter = 1;
                break;

            case 'n':
                i = 1;
                writeAnotherCounter = 1;
                break;

            default:
                printf("Please enter a valid condition! \n");
                break;
            }
        } while (writeAnotherCounter != 1);

    } while (i != 1);
        
    fclose(fp);
}


// Function to View Added items 
void viewItems()
{
    FILE* fp;
    ITEM_T myrecord;
    int count = 1;

    fp = fopen(path1, "rb");
    if (!fp)
    {
        printf("Unable to open file!");
        exit(0);
    }

    line();
    while (fread(&myrecord, sizeof(ITEM_T), 1, fp) != NULL)
    {
        printf("\n");
        printf("Serial number : %d\n", count++);
        printf("Name : %s\n", myrecord.medName);
        printf("Date of manufacture : %d / %d / %d\n", myrecord.medMan.dd, myrecord.medMan.mm, myrecord.medMan.yy);
        printf("Date of expiry : %d / %d / %d\n", myrecord.medExp.dd, myrecord.medExp.mm, myrecord.medExp.yy);
        printf("Cost per unit : %f\n", myrecord.unitCost);
        printf("Quantity in stock : %d\n", myrecord.qty);
        printf("Safety stock : %d\n", myrecord.safetyStock);
        printf("Reference ID : %s\n", myrecord.refID);
        printf("Type : %s\n", myrecord.type);
        printf("\n");
    }

    fclose(fp);
}


/*void newPurchase()
{
    FILE* fp;
    FILE* fp_tmp;
    FILE* fp_pay;
    int found = 0;
    int purqty;
    float totalCost;
    ITEM_T myrecord;
    PAY_T mypayment;
    char searchname[16];
    int i = 0;

    fp = fopen(path1, "r");
    if (!fp)
    {
        printf("Unable to open file");
        exit(0);
    }
    fp_tmp = fopen("tmp.bin", "wb");
    if (!fp_tmp)
    {
        printf("Unable to open temp file");
        exit(0);
    }
    fp_pay = fopen(path2, "wb");
    if (!fp_pay)
    {
        printf("Unable to open payment information file");
        exit(0);
    }

    do
    {
        recCount++;
        found = 0;
        line();
        printf("Input the name of the medicine to be purchased : ");
        scanf(" %[^\n]%*c", searchname);
        printf("Input the quantity to be purchased : ");
        scanf(" %d", &purqty);

        printf("Input card number : ");
        scanf(" %[^\n]%*c", mypayment.cardNum);
        printf("Input date of expiry in dd <enter> mm <enter> yy format\n");
        scanf(" %d", &mypayment.expDate.dd);
        scanf(" %d", &mypayment.expDate.mm);
        scanf(" %d", &mypayment.expDate.yy);
        printf("Input CVV : ");
        scanf(" %d", &mypayment.cvv);

        line();

        while (fread(&myrecord, sizeof(ITEM_T), 1, fp) != NULL)
        {
            if (strcmp(searchname, myrecord.medName) == 0)
            {
                mypayment.deduct = purqty * myrecord.unitCost;
                myrecord.qty -= purqty;
                fwrite(&myrecord, sizeof(ITEM_T), 1, fp_tmp);
                fwrite(&mypayment, sizeof(PAY_T), 1, fp_pay);
                found = 1;
            }
            else
            {
                fwrite(&myrecord, sizeof(ITEM_T), 1, fp_tmp);
            }
        }

        if (!found) printf("No record(s) found with the requested name: %s\n\n", searchname);

        char writeAnother;
        int writeAnotherCounter = 0;
        do
        {
            printf("\nWould you like to make another purchase? (y/n) ");
            scanf("%s", &writeAnother);
            switch (writeAnother)
            {
            case 'y':
                i = 0;
                writeAnotherCounter = 1;
                break;

            case 'n':
                i = 1;
                writeAnotherCounter = 1;
                break;

            default:
                printf("Please enter a valid condition! \n");
                break;
            }
        } while (writeAnotherCounter != 1);

    } while (i != 1);

    fclose(fp);
    fclose(fp_tmp);
    fclose(fp_pay);
    remove(path1);
    rename("tmp.bin", path1);
}*/

// Function for New purchase
void newPurchase()
{
    FILE* fp;
    FILE* fp_tmp;
    FILE* fp_pay;
    int found = 0;
    
    float totalCost;
    ITEM_T myrecord;
    PAY_T mypayment;
    char searchname[16];
    int i = 0;



    fp = fopen(path1, "r");
    if (!fp)
    {
        printf("Unable to open file");
        exit(0);
    }
    fp_tmp = fopen("tmp.bin", "wb");
    if (!fp_tmp)
    {
        printf("Unable to open temp file");
        exit(0);
    }
    fp_pay = fopen(path2, "wb");
    if (!fp_pay)
    {
        printf("Unable to open payment information file");
        exit(0);
    }



    do
    {
        recCount++;
        found = 0;
        line();
        printf("Input the name of the medicine to be purchased : ");
        scanf(" %[^\n]%*c", searchname);
        printf("Input the quantity to be purchased : ");
        scanf(" %d", &mypayment.purqty);



        printf("Input card number : ");
        scanf(" %[^\n]%*c", mypayment.cardNum);
        printf("Input date of expiry in dd <enter> mm <enter> yy format\n");
        scanf(" %d", &mypayment.expDate.dd);
        scanf(" %d", &mypayment.expDate.mm);
        scanf(" %d", &mypayment.expDate.yy);
        printf("Input CVV : ");
        scanf(" %d", &mypayment.cvv);



        strcpy(mypayment.medName2, searchname);



        time_t t  = time(NULL);
        struct tm tm = *localtime(&t);
        mypayment.saleDate.yy = tm.tm_year + 1900;
        mypayment.saleDate.mm = tm.tm_mon + 1;
        mypayment.saleDate.dd = tm.tm_mday;



        line();



        while (fread(&myrecord, sizeof(ITEM_T), 1, fp) != NULL)
        {
            if (strcmp(searchname, myrecord.medName) == 0)
            {
                mypayment.deduct = mypayment.purqty * myrecord.unitCost;
                myrecord.qty -= mypayment.purqty;
                fwrite(&myrecord, sizeof(ITEM_T), 1, fp_tmp);
                fwrite(&mypayment, sizeof(PAY_T), 1, fp_pay);
                found = 1;
            }
            else
            {
                fwrite(&myrecord, sizeof(ITEM_T), 1, fp_tmp);
            }
        }



        if (!found) printf("No record(s) found with the requested name: %s\n\n", searchname);



        char writeAnother;
        int writeAnotherCounter = 0;
        do
        {
            printf("\nWould you like to make another purchase? (y/n) ");
            scanf("%s", &writeAnother);
            switch (writeAnother)
            {
                case 'y':
                        i = 0;
                        writeAnotherCounter = 1;
                        break;

                case 'n':
                        i = 1;
                        writeAnotherCounter = 1;
                        break;



                default:
                        printf("Please enter a valid condition! \n");
                        break;
                }
        
        } while (writeAnotherCounter != 1);



    } while (i != 1);



    fclose(fp);
    fclose(fp_tmp);
    fclose(fp_pay);
    remove(path1);
    rename("tmp.bin", path1);
}

//Clears the Screen
void clrscr()
{
    system("@cls||clear");
}

//Delays Execution
void delay(int number_of_seconds)
{
    int milli_seconds = 1000 * number_of_seconds;
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds);
}


// function to accept date variables and compare - returns 1(d1 > d2), 0(d1 = d2), -1(d1 < d2)
int dateCompare(DATE_T d1, DATE_T d2) 
{
    if (d1.yy > d2.yy) return 1;
    else if (d1.yy < d2.yy) return -1;
    else
    {
        if (d1.mm > d2.mm) return 1;
        else if (d1.mm < d2.mm) return -1;
        else
        {
            if (d1.dd > d2.dd) return 1;
            else if (d1.dd < d2.dd) return -1;
            else return 0;
        }
    }
}

//Switch Case for Menu
void mainMenu(int ch)
{

A:
    switch (ch)
    {
    case 1:
        newPurchase();
        break;

    case 2:
        addItem();
        break;

    case 3:
        deleteItem();
        break;

    case 4:
        viewItems();
        break;

    case 5:
        viewPaymentInfo();
        break;

    case 6:
        reviewWarnings();
        break;

    case 7:
        salesReport();
        break;

    case 0:
        exit(0);
        break;

    default:
        printf("\nInvalid entry. Try again ... ");
        goto A;
        break;
    }
}


// Sales Statistics and Analysis 
void salesReport()
{ 
    FILE* fp;
    PAY_T myrecord;
    int count = 1; fp = fopen(path2, "rb");
    if (!fp)
    {
        printf("Unable to open file!");
        exit(0);
    } 
    line();
    while (fread(&myrecord, sizeof(PAY_T), 1, fp) != NULL)
    {
        printf("\n");
        printf("Serial number : %d\n", count++);
        printf("Medicine purchased : %s\n", myrecord.medName2);
        printf("Date of purchase : %d / %d / %d\n", myrecord.saleDate.dd, myrecord.saleDate.mm, myrecord.saleDate.yy);
        printf("Quantity purchased : %d\n", myrecord.purqty);
        printf("Amount deducted : %f\n", myrecord.deduct);
        printf("\n");
    } 
    fclose(fp);
}


