#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TITLE_LEN 50
#define MAX_AUTHOR_LEN 50
#define MAX_ISBN_LEN 30

typedef struct {
    char title[MAX_TITLE_LEN];
    char author[MAX_AUTHOR_LEN];
    char isbn[MAX_ISBN_LEN];
} Book;

static void trimNewline(char *text)
{
    size_t len = strlen(text);

    while (len > 0 && (text[len - 1] == '\n' || text[len - 1] == '\r'))
    {
        text[len - 1] = '\0';
        len--;
    }
}

static int readInt(const char *prompt)
{
    char buffer[64];
    char *end = NULL;
    long value;

    do {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
        {
            return -1;
        }

        trimNewline(buffer);
        value = strtol(buffer, &end, 10);
    } while (end == buffer || *end != '\0');

    return (int)value;
}

static void readRequiredText(const char *prompt, char *buffer, size_t size)
{
    while (1)
    {
        printf("%s", prompt);
        if (fgets(buffer, (int)size, stdin) == NULL)
        {
            buffer[0] = '\0';
            return;
        }

        trimNewline(buffer);
        if (buffer[0] != '\0')
        {
            return;
        }
    }
}

static int equalsIgnoreCase(const char *left, const char *right)
{
    unsigned char a;
    unsigned char b;

    while (*left != '\0' && *right != '\0')
    {
        a = (unsigned char)*left;
        b = (unsigned char)*right;

        if (toupper(a) != toupper(b))
        {
            return 0;
        }

        left++;
        right++;
    }

    return *left == '\0' && *right == '\0';
}

static void printBook(const Book *book)
{
    printf("Title: %s\n", book->title);
    printf("Author: %s\n", book->author);
    printf("ISBN: %s\n", book->isbn);
}

static void addBook(Book books[], int count)
{
    for (int i = 0; i < count; i++)
    {
        printf("Enter details for Book %d\n", i + 1);
        readRequiredText("Enter title: ", books[i].title, sizeof(books[i].title));
        readRequiredText("Enter author: ", books[i].author, sizeof(books[i].author));
        readRequiredText("Enter ISBN: ", books[i].isbn, sizeof(books[i].isbn));
    }
}

static void listBooks(const Book books[], int count)
{
    if (count == 0)
    {
        printf("No books are currently stored.\n");
        return;
    }

    for (int i = 0; i < count; i++)
    {
        printf("\nBook %d\n", i + 1);
        printBook(&books[i]);
    }
}

static void searchBook(const Book books[], int count, const char *searchText)
{
    int found = 0;

    for (int i = 0; i < count; i++)
    {
        if (strcmp(books[i].isbn, searchText) == 0 ||
            equalsIgnoreCase(books[i].title, searchText) ||
            equalsIgnoreCase(books[i].author, searchText))
        {
            printf("Book found!\n");
            printBook(&books[i]);
            found = 1;
            break;
        }
    }

    if (!found)
    {
        printf("Book not found.\n");
    }
}

static void deleteBook(Book books[], int *count, const char *isbn)
{
    for (int i = 0; i < *count; i++)
    {
        if (strcmp(books[i].isbn, isbn) == 0)
        {
            for (int j = i; j < *count - 1; j++)
            {
                books[j] = books[j + 1];
            }

            (*count)--;
            printf("Book removed.\n");
            return;
        }
    }

    printf("Book not found.\n");
}

int main(void)
{
    int totalBooks = 0;
    int choice = 0;
    char input[64];
    Book *books = NULL;

    totalBooks = readInt("Enter number of books: ");
    if (totalBooks <= 0)
    {
        printf("Number of books must be greater than zero.\n");
        return 1;
    }

    books = calloc((size_t)totalBooks, sizeof(*books));
    if (books == NULL)
    {
        printf("Memory allocation failed.\n");
        return 1;
    }

    addBook(books, totalBooks);

    do
    {
        printf("\nLibrary Menu\n");
        printf("1. Add books\n");
        printf("2. Search book\n");
        printf("3. View all books\n");
        printf("4. Delete book\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            break;
        }

        trimNewline(input);
        choice = (int)strtol(input, NULL, 10);

        if (choice == 1)
        {
            addBook(books, totalBooks);
        }
        else if (choice == 2)
        {
            printf("Enter search term (ISBN, title, or author): ");
            if (fgets(input, sizeof(input), stdin) == NULL)
            {
                break;
            }
            trimNewline(input);
            searchBook(books, totalBooks, input);
        }
        else if (choice == 3)
        {
            listBooks(books, totalBooks);
        }
        else if (choice == 4)
        {
            printf("Enter ISBN to delete: ");
            if (fgets(input, sizeof(input), stdin) == NULL)
            {
                break;
            }
            trimNewline(input);
            deleteBook(books, &totalBooks, input);
        }
        else if (choice == 5)
        {
            printf("Thank you!\n");
        }
        else
        {
            printf("Wrong input. Try again.\n");
        }

    } while (choice != 5);

    free(books);
    return 0;
}