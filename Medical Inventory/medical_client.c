#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "medical_h.h"

int main()
{
    int ch;
    char dummy;

A:

    clrscr();

    line();
    printf("MENU");
    line();
    printf("1. New purchase\n");
    printf("2. Add items\n");
    printf("3. Delete item\n");
    printf("4. View items\n");
    printf("5. View payment information\n");
    printf("6. Review warnings\n");
    printf("7. Sales Report\n");
    printf("0. Exit");
    line();
    printf("Input your choice : ");
    scanf(" %d", &ch);
    line();

    switch (ch)
    {

    case 1:

        mainMenu(1);
        break;

    case 2:

        mainMenu(2);
        break;

    case 3:

        mainMenu(3);
        break;

    case 4:

        mainMenu(4);
        delay(10);
        break;

    case 5:

        mainMenu(5);
        break;

    case 6:

        mainMenu(6);
        break;
         
    case 7:

        mainMenu(7);
        break;

    case 0:

        mainMenu(0);
        break;

    default:

        printf("\nInvalid entry. Try again ... ");
        goto A;
        break;
    }

    system("pause");
    goto A;

    return 0;
}

