#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct BankAccount {
    int accNumber;
    char name[50];
    float balance;
};

// function declarations
void createAccount();
void viewAccount();
void depositMoney();
void withdrawMoney();

int main() {
    int choice;

    while (1) {
        printf("\n------ Bank Management System ------\n");
        printf("1. Create New Account\n");
        printf("2. View Account Details\n");
        printf("3. Deposit Money\n");
        printf("4. Withdraw Money\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                createAccount();
                break;
            case 2:
                viewAccount();
                break;
            case 3:
                depositMoney();
                break;
            case 4:
                withdrawMoney();
                break;
            case 5:
                printf("Thank you for using our system!\n");
                exit(0);
            default:
                printf("Invalid choice! Try again.\n");
        }
    }

    return 0;
}

void createAccount() {
    struct BankAccount acc;
    FILE *fp;

    printf("\nEnter Account Number: ");
    scanf("%d", &acc.accNumber);
    printf("Enter Name: ");
    getchar(); // consume newline
    fgets(acc.name, sizeof(acc.name), stdin);
    acc.name[strcspn(acc.name, "\n")] = 0; // remove newline
    printf("Enter Initial Balance: ");
    scanf("%f", &acc.balance);

    fp = fopen("accounts.dat", "ab");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    fwrite(&acc, sizeof(acc), 1, fp);
    fclose(fp);
    printf("Account created successfully!\n");
}

void viewAccount() {
    struct BankAccount acc;
    FILE *fp;
    int accNum, found = 0;

    printf("\nEnter Account Number to view: ");
    scanf("%d", &accNum);

    fp = fopen("accounts.dat", "rb");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.accNumber == accNum) {
            printf("Account Number: %d\n", acc.accNumber);
            printf("Name: %s\n", acc.name);
            printf("Balance: %.2f\n", acc.balance);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Account not found.\n");
    }

    fclose(fp);
}

void depositMoney() {
    struct BankAccount acc;
    FILE *fp;
    int accNum;
    float amount;
    int found = 0;

    printf("\nEnter Account Number to deposit into: ");
    scanf("%d", &accNum);

    fp = fopen("accounts.dat", "rb+");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.accNumber == accNum) {
            printf("Enter amount to deposit: ");
            scanf("%f", &amount);
            acc.balance += amount;

            fseek(fp, -sizeof(acc), SEEK_CUR);
            fwrite(&acc, sizeof(acc), 1, fp);
            printf("Amount deposited successfully.\n");
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Account not found.\n");
    }

    fclose(fp);
}

void withdrawMoney() {
    struct BankAccount acc;
    FILE *fp;
    int accNum;
    float amount;
    int found = 0;

    printf("\nEnter Account Number to withdraw from: ");
    scanf("%d", &accNum);

    fp = fopen("accounts.dat", "rb+");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.accNumber == accNum) {
            printf("Enter amount to withdraw: ");
            scanf("%f", &amount);

            if (amount > acc.balance) {
                printf("Insufficient balance.\n");
            } else {
                acc.balance -= amount;
                fseek(fp, -sizeof(acc), SEEK_CUR);
                fwrite(&acc, sizeof(acc), 1, fp);
                printf("Amount withdrawn successfully.\n");
            }
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Account not found.\n");
    }

    fclose(fp);
}

