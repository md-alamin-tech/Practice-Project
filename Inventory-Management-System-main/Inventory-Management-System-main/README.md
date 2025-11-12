# Inventory Management System in C

A simple **console-based Inventory Management System** built in **C** that allows you to **add, view, update, and delete products** with real-time stock alerts. Data is stored persistently in a binary file.

---

## **Table of Contents**

- [Description](#description)  
- [Features](#features)  
- [Installation & How to Run](#installation--how-to-run)  
- [Data Structure](#data-structure)  
- [Future Enhancements](#future-enhancements)  
- [License](#license)  

---

## **Description**

The Inventory Management System (IMS) is designed to efficiently manage products in a small business, warehouse, or personal inventory. It demonstrates **file handling, structures, and input validation** in C programming.  

- Automatically assigns unique IDs to products.  
- Alerts users when stock is low or out of stock.  
- Saves inventory data to a binary file (`inventory.dat`) for persistence.  

---

## **Features**

- ✅ Add new products with name, quantity, and price.  
- ✅ View all products in a tabular format.  
- ✅ Update product quantity and price.  
- ✅ Delete products from inventory.  
- ✅ Stock alerts for low or out-of-stock items.  
- ✅ Persistent data storage using binary files.  

---

## **Installation & How to Run**

Follow these steps to set up and run the Inventory Management System:

### **Step 1: Install a C Compiler**
You need a C compiler like **GCC**.  

- **Windows:**  
  1. Install [MinGW](http://www.mingw.org/).  
  2. Add `C:\MinGW\bin` to your system PATH.  

- **macOS:**  
```bash
  brew install gcc
```

### **Step 2 : Download the Project**
Clone the GitHub repository and enter the project folder:.  

```bash
  git clone https://github.com/rahat9016/Inventory-Management-System.git
  cd Inventory-Management-System
```

### **Step 3: Compile the Program**
Use GCC to compile the C file:
```bash
  gcc inventory.c -o inventory
```

