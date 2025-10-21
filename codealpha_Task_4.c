#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ===== Structure Definition =====
struct Account {
    int acc_no;
    char name[50];
    float balance;
};

// ===== Function Prototypes =====
void createAccount();
void depositMoney();
void withdrawMoney();
void checkBalance();

// ===== Main Function =====
int main() {
    int choice;

    while (1) {
        printf("\n===== Bank Account Management System =====\n");
        printf("1. Create Account\n");
        printf("2. Deposit Money\n");
        printf("3. Withdraw Money\n");
        printf("4. Check Balance\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: createAccount(); break;
            case 2: depositMoney(); break;
            case 3: withdrawMoney(); break;
            case 4: checkBalance(); break;
            case 5: 
                printf("Exiting the program...\n");
                exit(0);
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
    return 0;
}

// ===== Function to Create New Account =====
void createAccount() {
    struct Account a;
    FILE *fp = fopen("accounts.dat", "ab");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter Account Number: ");
    scanf("%d", &a.acc_no);
    getchar(); // clear newline
    printf("Enter Account Holder Name: ");
    fgets(a.name, 50, stdin);
    a.name[strcspn(a.name, "\n")] = '\0'; // remove newline
    printf("Enter Initial Deposit Amount: ");
    scanf("%f", &a.balance);

    fwrite(&a, sizeof(a), 1, fp);
    fclose(fp);

    printf("Account created successfully!\n");
}

// ===== Function to Deposit Money =====
void depositMoney() {
    struct Account a;
    int acc_no, found = 0;
    float amount;

    FILE *fp = fopen("accounts.dat", "rb+");
    if (fp == NULL) {
        printf("No accounts found!\n");
        return;
    }

    printf("Enter Account Number: ");
    scanf("%d", &acc_no);

    while (fread(&a, sizeof(a), 1, fp)) {
        if (a.acc_no == acc_no) {
            printf("Enter amount to deposit: ");
            scanf("%f", &amount);
            a.balance += amount;

            fseek(fp, -sizeof(a), SEEK_CUR);
            fwrite(&a, sizeof(a), 1, fp);
            printf("✅ Deposit successful! New Balance: %.2f\n", a.balance);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Account not found!\n");

    fclose(fp);
}

// ===== Function to Withdraw Money =====
void withdrawMoney() {
    struct Account a;
    int acc_no, found = 0;
    float amount;

    FILE *fp = fopen("accounts.dat", "rb+");
    if (fp == NULL) {
        printf("No accounts found!\n");
        return;
    }

    printf("Enter Account Number: ");
    scanf("%d", &acc_no);

    while (fread(&a, sizeof(a), 1, fp)) {
        if (a.acc_no == acc_no) {
            printf("Enter amount to withdraw: ");
            scanf("%f", &amount);

            if (amount > a.balance) {
                printf("Insufficient balance! Current balance: %.2f\n", a.balance);
            } else {
                a.balance -= amount;
                fseek(fp, -sizeof(a), SEEK_CUR);
                fwrite(&a, sizeof(a), 1, fp);
                printf("Withdrawal successful! Remaining Balance: %.2f\n", a.balance);
            }
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Account not found!\n");

    fclose(fp);
}

// ===== Function to Check Account Balance =====
void checkBalance() {
    struct Account a;
    int acc_no, found = 0;

    FILE *fp = fopen("accounts.dat", "rb");
    if (fp == NULL) {
        printf("No accounts found!\n");
        return;
    }

    printf("Enter Account Number: ");
    scanf("%d", &acc_no);

    while (fread(&a, sizeof(a), 1, fp)) {
        if (a.acc_no == acc_no) {
            printf("\nAccount Number: %d\n", a.acc_no);
            printf("Account Holder: %s\n", a.name);
            printf("Current Balance: %.2f\n", a.balance);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Account not found!\n");

    fclose(fp);
}