#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "library.dat"
#define MAX_TITLE 100
#define MAX_AUTHOR 100

typedef struct {
    char title[MAX_TITLE];
    char author[MAX_AUTHOR];
    int bookID;
    int isIssued; // 0 = Available, 1 = Issued
} Book;

void addBook();
void updateBook();
void searchBook();
void issueBook();
void returnBook();
void displayBooks();
int bookExists(int id);

int main() {
    int choice;
    while (1) {
        printf("\n===== Library Management System =====\n");
        printf("1. Add Book\n");
        printf("2. Update Book\n");
        printf("3. Search Book\n");
        printf("4. Issue Book\n");
        printf("5. Return Book\n");
        printf("6. Display All Books\n");
        printf("7. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addBook(); break;
            case 2: updateBook(); break;
            case 3: searchBook(); break;
            case 4: issueBook(); break;
            case 5: returnBook(); break;
            case 6: displayBooks(); break;
            case 7: exit(0);
            default: printf("Invalid choice! Try again.\n");
        }
    }
    return 0;
}

void addBook() {
    FILE *fp = fopen(FILE_NAME, "ab");
    if (!fp) {
        printf("Error opening file!\n");
        return;
    }
    Book b;
    printf("Enter Book ID: ");
    scanf("%d", &b.bookID);
    if (bookExists(b.bookID)) {
        printf("Book with this ID already exists!\n");
        fclose(fp);
        return;
    }
    getchar(); // clear newline
    printf("Enter Title: ");
    fgets(b.title, MAX_TITLE, stdin);
    b.title[strcspn(b.title, "\n")] = 0;
    printf("Enter Author: ");
    fgets(b.author, MAX_AUTHOR, stdin);
    b.author[strcspn(b.author, "\n")] = 0;
    b.isIssued = 0;
    fwrite(&b, sizeof(Book), 1, fp);
    fclose(fp);
    printf("Book added successfully!\n");
}

void updateBook() {
    FILE *fp = fopen(FILE_NAME, "rb+");
    if (!fp) {
        printf("No books found!\n");
        return;
    }
    int id;
    printf("Enter Book ID to update: ");
    scanf("%d", &id);
    Book b;
    int found = 0;
    while (fread(&b, sizeof(Book), 1, fp)) {
        if (b.bookID == id) {
            getchar();
            printf("Enter new Title: ");
            fgets(b.title, MAX_TITLE, stdin);
            b.title[strcspn(b.title, "\n")] = 0;
            printf("Enter new Author: ");
            fgets(b.author, MAX_AUTHOR, stdin);
            b.author[strcspn(b.author, "\n")] = 0;
            fseek(fp, -sizeof(Book), SEEK_CUR);
            fwrite(&b, sizeof(Book), 1, fp);
            printf("Book updated successfully!\n");
            found = 1;
            break;
        }
    }
    if (!found) printf("Book not found!\n");
    fclose(fp);
}

void searchBook() {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) {
        printf("No books found!\n");
        return;
    }
    int id;
    printf("Enter Book ID to search: ");
    scanf("%d", &id);
    Book b;
    int found = 0;
    while (fread(&b, sizeof(Book), 1, fp)) {
        if (b.bookID == id) {
            printf("Book Found:\nID: %d\nTitle: %s\nAuthor: %s\nStatus: %s\n",
                   b.bookID, b.title, b.author, b.isIssued ? "Issued" : "Available");
            found = 1;
            break;
        }
    }
    if (!found) printf("Book not found!\n");
    fclose(fp);
}

void issueBook() {
    FILE *fp = fopen(FILE_NAME, "rb+");
    if (!fp) {
        printf("No books found!\n");
        return;
    }
    int id;
    printf("Enter Book ID to issue: ");
    scanf("%d", &id);
    Book b;
    int found = 0;
    while (fread(&b, sizeof(Book), 1, fp)) {
        if (b.bookID == id) {
            if (b.isIssued) {
                printf("Book already issued!\n");
            } else {
                b.isIssued = 1;
                fseek(fp, -sizeof(Book), SEEK_CUR);
                fwrite(&b, sizeof(Book), 1, fp);
                printf("Book issued successfully!\n");
            }
            found = 1;
            break;
        }
    }
    if (!found) printf("Book not found!\n");
    fclose(fp);
}

void returnBook() {
    FILE *fp = fopen(FILE_NAME, "rb+");
    if (!fp) {
        printf("No books found!\n");
        return;
    }
    int id;
    printf("Enter Book ID to return: ");
    scanf("%d", &id);
    Book b;
    int found = 0;
    while (fread(&b, sizeof(Book), 1, fp)) {
        if (b.bookID == id) {
            if (!b.isIssued) {
                printf("Book was not issued!\n");
            } else {
                b.isIssued = 0;
                fseek(fp, -sizeof(Book), SEEK_CUR);
                fwrite(&b, sizeof(Book), 1, fp);
                printf("Book returned successfully!\n");
            }
            found = 1;
            break;
        }
    }
    if (!found) printf("Book not found!\n");
    fclose(fp);
}

void displayBooks() {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) {
        printf("No books found!\n");
        return;
    }
    Book b;
    printf("\n%-10s %-40s %-40s %-10s\n", "ID", "Title", "Author", "Status");
    for(int i=0;i<102;i++) printf("-");
    printf("\n");
    while (fread(&b, sizeof(Book), 1, fp)) {
        printf("%-10d %-40s %-40s %-10s\n", b.bookID, b.title, b.author, b.isIssued ? "Issued" : "Available");
    }
    fclose(fp);
}

int bookExists(int id) {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) return 0;
    Book b;
    while (fread(&b, sizeof(Book), 1, fp)) {
        if (b.bookID == id) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}