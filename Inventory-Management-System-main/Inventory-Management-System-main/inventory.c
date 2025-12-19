#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FILE_NAME "inventory.dat"
#define HISTORY_FILE "history.dat"

// Product Structure
struct Product
{
    int id;
    char name[100];
    int quantity;
    float price;
    float total_value;  // quantity * price
};

// Purchase History Structure
struct PurchaseHistory
{
    int product_id;
    char customer_name[50];
    int quantity;
    char date[20];
    float total_price;
};

struct Product products[100];
int count = 0;

struct PurchaseHistory history[500];
int history_count = 0;

// Customer Account Structure
struct Customer
{
    char username[30];
    char password[30];
};

struct Customer customers[100];
int customer_count = 0;

// Functions
void load_from_file();
void save_to_file();
void load_history_from_file();
void save_history_to_file();
void add_product();
void update_product();
void view_products();
void sell_product();
void view_purchase_history();
void get_current_date(char *date);
void customer_portal();
void admin_portal();
int admin_login();
int customer_register();
int customer_login();
int input_positive_int(const char *prompt);
float input_positive_float(const char *prompt);

// Load products
void load_from_file()
{
    FILE *fp = fopen(FILE_NAME, "rb");
    if (fp != NULL)
    {
        fread(&count, sizeof(int), 1, fp);
        fread(products, sizeof(struct Product), count, fp);
        fclose(fp);
    }
}

// Save products
void save_to_file()
{
    FILE *fp = fopen(FILE_NAME, "wb");
    if (fp != NULL)
    {
        fwrite(&count, sizeof(int), 1, fp);
        fwrite(products, sizeof(struct Product), count, fp);
        fclose(fp);
    }
}

// Load history
void load_history_from_file()
{
    FILE *fp = fopen(HISTORY_FILE, "rb");
    if (fp != NULL)
    {
        fread(&history_count, sizeof(int), 1, fp);
        fread(history, sizeof(struct PurchaseHistory), history_count, fp);
        fclose(fp);
    }
}

// Save history
void save_history_to_file()
{
    FILE *fp = fopen(HISTORY_FILE, "wb");
    if (fp != NULL)
    {
        fwrite(&history_count, sizeof(int), 1, fp);
        fwrite(history, sizeof(struct PurchaseHistory), history_count, fp);
        fclose(fp);
    }
}

// Get current date
void get_current_date(char *date)
{
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(date, 20, "%d-%m-%Y %H:%M", tm_info);
}

// Input positive int
int input_positive_int(const char *prompt)
{
    int num;
    printf("%s", prompt);
    scanf("%d", &num);
    while (num < 0)
    {
        printf("Cannot be negative. Try again.\n");
        printf("%s", prompt);
        scanf("%d", &num);
    }
    return num;
}

// Input positive float
float input_positive_float(const char *prompt)
{
    float num;
    printf("%s", prompt);
    scanf("%f", &num);
    while (num < 0)
    {
        printf("Cannot be negative. Try again.\n");
        printf("%s", prompt);
        scanf("%f", &num);
    }
    return num;
}

// Add product
void add_product()
{
    struct Product p;

    printf("\n--- Add Product ---\n");

    p.id = (count == 0) ? 1 : products[count - 1].id + 1;

    getchar();
    printf("Product Name: ");
    fgets(p.name, sizeof(p.name), stdin);
    p.name[strcspn(p.name, "\n")] = '\0';

    p.quantity = input_positive_int("Quantity: ");
    p.price = input_positive_float("Price: ");

    // Calculate total value
    p.total_value = p.quantity * p.price;

    products[count++] = p;
    save_to_file();
    printf("Product added! (ID: %d)\n\n", p.id);
}

// View products
void view_products()
{
    if (count == 0)
    {
        printf("\nNo products.\n\n");
        return;
    }

    printf("\n");
    printf("=================================================================================\n");
    printf("                            PRODUCT LIST\n");
    printf("=================================================================================\n");
    printf("%-5s | %-25s | %-5s | %-10s | %-12s | %-10s\n", 
           "ID", "Name", "Qty", "Price", "Total Value", "Status");
    printf("----- | ----------------------- | ----- | ---------- | ------------ | ----------\n");

    for (int i = 0; i < count; i++)
    {
        char status[15];

        if (products[i].quantity == 0)
            strcpy(status, "Out");
        else if (products[i].quantity <= 5)
            strcpy(status, "Low");
        else
            strcpy(status, "OK");

        printf("%-5d | %-25s | %-5d | %-10.2f | %-12.2f | %-10s\n",
               products[i].id,
               products[i].name,
               products[i].quantity,
               products[i].price,
               products[i].total_value,
               status);
    }
    printf("=================================================================================\n\n");
}

// Update product
void update_product()
{
    int id, found = 0;

    printf("\n--- Update Product ---\n");
    id = input_positive_int("Product ID: ");

    for (int i = 0; i < count; i++)
    {
        if (products[i].id == id)
        {
            found = 1;

            printf("Current: %s, Qty: %d, Price: %.2f\n\n", 
                   products[i].name, products[i].quantity, products[i].price);

            products[i].quantity = input_positive_int("New Quantity: ");
            products[i].price = input_positive_float("New Price: ");

            // Recalculate total value
            products[i].total_value = products[i].quantity * products[i].price;

            save_to_file();
            printf("Updated!\n\n");
            break;
        }
    }

    if (!found)
        printf("Not found!\n\n");
}

// Sell product
void sell_product()
{
    int id, found = 0, qty;
    char customer_name[50];
    char confirm;

    printf("\n--- Buy Product ---\n");
    id = input_positive_int("Product ID: ");

    for (int i = 0; i < count; i++)
    {
        if (products[i].id == id)
        {
            found = 1;

            printf("Product: %s\n", products[i].name);
            printf("Available: %d\n", products[i].quantity);
            printf("Price: %.2f\n\n", products[i].price);

            if (products[i].quantity == 0)
            {
                printf("Out of stock!\n\n");
                return;
            }

            qty = input_positive_int("Quantity to buy: ");

            if (qty > products[i].quantity)
            {
                printf("Not enough stock!\n\n");
                return;
            }

            getchar();
            printf("Your name: ");
            fgets(customer_name, sizeof(customer_name), stdin);
            customer_name[strcspn(customer_name, "\n")] = '\0';

            printf("Confirm (Y/N): ");
            scanf(" %c", &confirm);

            if (confirm == 'Y' || confirm == 'y')
            {
                // Save to history
                history[history_count].product_id = products[i].id;
                strcpy(history[history_count].customer_name, customer_name);
                history[history_count].quantity = qty;
                history[history_count].total_price = products[i].price * qty;
                get_current_date(history[history_count].date);
                history_count++;

                // Update quantity
                products[i].quantity -= qty;
                products[i].total_value = products[i].quantity * products[i].price;

                save_to_file();
                save_history_to_file();

                printf("Purchase successful!\n\n");
            }
            else
            {
                printf("Cancelled.\n\n");
            }

            break;
        }
    }

    if (!found)
        printf("Product not found!\n\n");
}

// View purchase history
void view_purchase_history()
{
    if (history_count == 0)
    {
        printf("\nNo purchase history.\n\n");
        return;
    }

    printf("\n");
    printf("================================================================================\n");
    printf("                          PURCHASE HISTORY\n");
    printf("================================================================================\n");
    printf("%-6s | %-20s | %-5s | %-12s | %-20s\n", 
           "PID", "Customer", "Qty", "Amount", "Date");
    printf("-------+----------------------+-------+--------------+--------------------\n");

    for (int i = 0; i < history_count; i++)
    {
        printf("%-6d | %-20s | %-5d | %-12.2f | %-20s\n",
               history[i].product_id,
               history[i].customer_name,
               history[i].quantity,
               history[i].total_price,
               history[i].date);
    }
    printf("======================================================================
        ====\n\n");
}

// Customer Register
int customer_register()
{
    char username[30];
    char password[30];

    printf("\n--- Register ---\n");
    printf("Username: ");
    scanf("%29s", username);

    // Check if username exists
    for (int i = 0; i < customer_count; i++)
    {
        if (strcmp(customers[i].username, username) == 0)
        {
            printf("Username already exists!\n\n");
            return 0;
        }
    }

    printf("Password: ");
    scanf("%29s", password);

    // Add new customer
    strcpy(customers[customer_count].username, username);
    strcpy(customers[customer_count].password, password);
    customer_count++;

    printf("Registration successful!\n\n");
    return 1;
}

// Customer Login
int customer_login()
{
    char username[30];
    char password[30];

    printf("\n--- Login ---\n");
    printf("Username: ");
    scanf("%29s", username);
    printf("Password: ");
    scanf("%29s", password);

    // Check credentials
    for (int i = 0; i < customer_count; i++)
    {
        if (strcmp(customers[i].username, username) == 0 &&
            strcmp(customers[i].password, password) == 0)
        {
            printf("Login OK!\n\n");
            return 1;
        }
    }

    printf("Wrong username or password!\n\n");
    return 0;
}

// Admin login
int admin_login()
{
    char password[20];
    char correct[] = "admin123";

    printf("\n--- Admin Login ---\n");
    printf("Password: ");
    getchar();  // Clear leftover newline
    scanf("%19s", password);

    if (strcmp(password, correct) == 0)
    {
        printf("Login OK!\n\n");
        return 1;
    }
    else
    {
        printf("Wrong password!\n\n");
        return 0;
    }
}

// Customer portal
void customer_portal()
{
    int choice;

    while (1)
    {
        printf("\n--- Customer ---\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Back\n");
        printf("Choice: ");
        scanf("%d", &choice);

        if (choice == 1)
        {
            customer_register();
        }
        else if (choice == 2)
        {
            if (customer_login())
            {
                // Customer Menu after login
                int subchoice;
                while (1)
                {
                    printf("\n--- Customer Menu ---\n");
                    printf("1. View Products\n");
                    printf("2. Buy Product\n");
                    printf("3. My Purchases\n");
                    printf("4. Logout\n");
                    printf("Choice: ");
                    scanf("%d", &subchoice);

                    switch (subchoice)
                    {
                    case 1:
                        view_products();
                        break;
                    case 2:
                        sell_product();
                        break;
                    case 3:
                        view_purchase_history();
                        break;
                    case 4:
                        printf("Logged out.\n\n");
                        goto back_to_customer;
                    default:
                        printf("Invalid!\n");
                    }
                }
                back_to_customer:;
            }
        }
        else if (choice == 3)
        {
            return;
        }
        else
        {
            printf("Invalid!\n");
        }
    }
}

// Admin portal
void admin_portal()
{
    int choice;

    if (!admin_login())
        return;

    while (1)
    {
        printf("\n--- Admin Menu ---\n");
        printf("1. Add Product\n");
        printf("2. View Products\n");
        printf("3. Update Product\n");
        printf("4. Purchase History\n");
        printf("5. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            add_product();
            break;
        case 2:
            view_products();
            break;
        case 3:
            update_product();
            break;
        case 4:
            view_purchase_history();
            break;
        case 5:
            return;
        default:
            printf("Invalid!\n");
        }
    }
}

// Main
int main()
{
    int choice;

    load_from_file();
    load_history_from_file();

    while (1)
    {
        printf("\n--- Main Menu ---\n");
        printf("1. Customer\n");
        printf("2. Admin\n");
        printf("3. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            customer_portal();
            break;
        case 2:
            admin_portal();
            break;
        case 3:
            save_to_file();
            save_history_to_file();
            printf("Goodbye!\n");
            exit(0);
        default:
            printf("Invalid!\n");
        }
    }

    return 0;
}
