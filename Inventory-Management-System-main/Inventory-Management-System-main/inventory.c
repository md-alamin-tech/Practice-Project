#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "inventory.dat"

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

void load_from_file();
void save_to_file();
void add_product();
void update_product();
void view_products();
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

void delete_product()
{
    int id, found = 0;
    id = input_positive_int("\nEnter Product ID to delete: ");

    for (int i = 0; i < count; i++)
    {
        if (products[i].id == id)
        {
            for (int j = i; j < count - 1; j++)
                products[j] = products[j + 1];
            count--;
            save_to_file();
            printf("\n Product deleted successfully!\n");
            found = 1;
            break;
        }
    }
    if (!found)
        printf(" Product not found.\n");
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

int main()
{
    int choice;
    load_from_file();

    while (1)
    {
        printf("\n========== INVENTORY MANAGEMENT ==========\n");
        printf("1. Add Product\n");
        printf("2. View Products\n");
        printf("3. Update Product\n");
        printf("4. Delete Product\n");
        printf("5. Exit\n");
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
            delete_product();
            break;
        case 5:
            save_to_file();
            printf("\n Data saved. Exiting program...\n");
            exit(0);
        default:
            printf(" Invalid choice. Try again.\n");
        }
    }

    return 0;
}