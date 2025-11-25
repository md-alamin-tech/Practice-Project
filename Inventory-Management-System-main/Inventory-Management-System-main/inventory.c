#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FILE_NAME "inventory.dat"
#define HISTORY_FILE "history.dat"

// <<<< ====== Structure of inventory product ====== >>>>
struct Product
{
    int id;
    char name[100];
    int quantity;
    float price;
};

struct Product products[100];
int count = 0;

// <<<< ====== Structure for Purchase History ====== >>>>
struct PurchaseHistory
{
    int product_id;
    char customer_name[50];
    int quantity;
    char date[20];
    float total_price;
};

struct PurchaseHistory history[500];
int history_count = 0;

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
int input_positive_int(const char *prompt);
float input_positive_float(const char *prompt);

// <<<< ====== Load data from the inventory.dat ====== >>>>
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

void get_current_date(char *date)
{
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(date, 20, "%d-%m-%Y %H:%M", tm_info);
}

int input_positive_int(const char *prompt)
{
    int num;

    printf("%s", prompt);
    scanf("%d", &num);

    while (num < 0)
    {
        printf(" Cannot be negative. Try again.\n");
        printf("%s", prompt);
        scanf("%d", &num);
    }

    return num;
}


float input_positive_float(const char *prompt)
{
    float num;

    printf("%s", prompt);
    scanf("%f", &num);

    while (num < 0)
    {
        printf(" Cannot be negative. Try again.\n");
        printf("%s", prompt);
        scanf("%f", &num);
    }

    return num;
}

void add_product()
{
    struct Product p;

    p.id = (count == 0) ? 1 : products[count - 1].id + 1;

    getchar();
    printf("\nEnter Product Name: ");
    fgets(p.name, sizeof(p.name), stdin);
    p.name[strcspn(p.name, "\n")] = '\0';

    p.quantity = input_positive_int("Enter Quantity: ");
    p.price = input_positive_float("Enter Price: ");

    products[count++] = p;
    save_to_file();
    printf("\n Product added successfully! (ID: %d)\n", p.id);

    // Stock alert
    if (p.quantity == 1)
        printf("  ALERT: Only 1 unit left!\n");
    else if (p.quantity == 0)
        printf(" ALERT: Product is OUT OF STOCK!\n");
}

void update_product()
{
    int id, found = 0;
    id = input_positive_int("\nEnter Product ID to update: ");

    for (int i = 0; i < count; i++)
    {
        if (products[i].id == id)
        {
            products[i].quantity = input_positive_int("Enter new quantity: ");

            if (products[i].quantity > 0)
            {
                products[i].price = input_positive_float("Enter new price: ");
            }
            else
            {
                printf(" Quantity is 0, price not updated.\n");
            }

            save_to_file();
            printf("\n Product updated successfully!\n");

            // Stock alert
            if (products[i].quantity == 1)
                printf(" ALERT: Only 1 unit left!\n");
            else if (products[i].quantity == 0)
                printf("  ALERT: Product is OUT OF STOCK!\n");

            found = 1;
            break;
        }
    }
    if (!found)
        printf("Product not found.\n");
}

void sell_product()
{
    int id, found = 0, qty;
    char customer_name[50];
    char confirm;
    
    id = input_positive_int("\nEnter Product ID to sell: ");

    for (int i = 0; i < count; i++)
    {
        if (products[i].id == id)
        {
            found = 1;
            
            printf("\n+====================================+\n");
            printf("|        SELL PRODUCT                |\n");
            printf("+====================================+\n");
            printf("Product ID: %d\n", products[i].id);
            printf("Product Name: %s\n", products[i].name);
            printf("Current Quantity: %d\n", products[i].quantity);
            printf("Price: %.2f\n\n", products[i].price);
            
            if (products[i].quantity == 0)
            {
                printf("[!] Product out of stock!\n\n");
                return;
            }
            
            qty = input_positive_int("Enter quantity to sell: ");
            
            if (qty > products[i].quantity)
            {
                printf("[!] Not enough stock! Available: %d\n\n", products[i].quantity);
                return;
            }
            
            printf("Enter customer name: ");
            fgets(customer_name, sizeof(customer_name), stdin);
            customer_name[strcspn(customer_name, "\n")] = '\0';
            
            printf("\nConfirm sale of %d unit(s)? (Y/N): ", qty);
            scanf(" %c", &confirm);
            
            if (confirm == 'Y' || confirm == 'y')
            {
                // Record purchase history
                history[history_count].product_id = products[i].id;
                strcpy(history[history_count].customer_name, customer_name);
                history[history_count].quantity = qty;
                history[history_count].total_price = products[i].price * qty;
                get_current_date(history[history_count].date);
                history_count++;
                
                // Update quantity
                products[i].quantity -= qty;
                save_to_file();
                save_history_to_file();
                
                printf("\n+====================================+\n");
                printf("| SALE RECORDED SUCCESSFULLY         |\n");
                printf("| Remaining Quantity: %d             |\n", products[i].quantity);
                printf("+====================================+\n\n");
            }
            else
            {
                printf("\n[!] Sale cancelled.\n\n");
            }
            break;
        }
    }
    
    if (!found)
    {
        printf("\n+====================================+\n");
        printf("|      PRODUCT NOT FOUND             |\n");
        printf("|  No product with ID: %d\n", id);
        printf("+====================================+\n\n");
    }
}
void view_products()
{
    if (count == 0)
    {
        printf("\n No products found.\n");
        return;
    }

    printf("\n================ INVENTORY LIST ================\n");
    printf("| %-4s | %-25s | %-7s | %-10s | %-12s | %-10s |\n",
           "ID", "Name", "Qty", "Price", "Status", "Available");
    printf("-----------------------------------------------------------------------------\n");

    for (int i = 0; i < count; i++)
    {
        char status[20] = "";
        char available[10] = "";

        if (products[i].quantity == 0)
            strcpy(status, "Stock OUT ");
        else if (products[i].quantity == 1)
            strcpy(status, "Only 1 left ");
        else
            strcpy(status, "");

        strcpy(available, (products[i].quantity > 0) ? "Yes " : "No ");

        printf("| %-4d | %-25s | %-7d | %-10.2f | %-12s | %-10s |\n",
               products[i].id,
               products[i].name,
               products[i].quantity,
               products[i].price,
               status,
               available);
    }

    printf("-----------------------------------------------------------------------------\n");
}

void view_purchase_history()
{
    if (history_count == 0)
    {
        printf("\n No purchase history found.\n");
        return;
    }

    printf("\n================ PURCHASE HISTORY ================\n");
    printf("| %-5s | %-15s | %-10s | %-15s | %-15s |\n",
           "PID", "Customer", "Qty", "Total Price", "Date");
    printf("-------------------------------------------------------------\n");

    for (int i = 0; i < history_count; i++)
    {
        printf("| %-5d | %-15s | %-10d | %-15.2f | %-15s |\n",
               history[i].product_id,
               history[i].customer_name,
               history[i].quantity,
               history[i].total_price,
               history[i].date);
    }

    printf("-------------------------------------------------------------\n");
}

int main()
{
    int choice;
    load_from_file();
    load_history_from_file();

    while (1)
    {
        printf("\n========== INVENTORY MANAGEMENT ==========\n");
        printf("1. Add Product\n");
        printf("2. View Products\n");
        printf("3. Update Product\n");
        printf("4. Sell Product\n");
        printf("5. View Purchase History\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
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
            sell_product();
            break;
        case 5:
            view_purchase_history();
            break;
        case 6:
            save_to_file();
            save_history_to_file();
            printf("\n Data saved. Exiting program...\n");
            exit(0);
        default:
            printf(" Invalid choice. Try again.\n");
        }
    }

    return 0;
}