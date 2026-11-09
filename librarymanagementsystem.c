
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_BOOKS 500
#define MAX_STUDENTS 200
#define MAX_ISSUES 1000

#define FINE_PER_DAY 5.0
#define LOAN_DAYS 14

// ================= STRUCTURES =================

// Book Structure
struct Book {
    int id;
    char isbn[30];
    char title[100];
    char author[60];
    char publisher[60];
    char category[40];
    char edition[20];
    int year;
    char rackNo[20];
    int totalCopies;
    int availableCopies;
};

// Student Structure
struct Student {
    int id;
    char name[60];
    char course[40];
    char department[50];
    char email[80];
    char phone[20];
    char password[40];
    float pendingFine;
};

// Issue Structure
struct Issue {
    int issueId;
    int bookId;
    int studentId;
    time_t issueDate;
    time_t dueDate;
    time_t returnDate;
    int returned;
    float fine;
};

// Global Arrays
struct Book books[MAX_BOOKS];
struct Student students[MAX_STUDENTS];
struct Issue issues[MAX_ISSUES];

int bookCount = 0;
int studentCount = 0;
int issueCount = 0;

// ================= INPUT FUNCTIONS =================

void readLine(char *str, int size) {
    if (fgets(str, size, stdin) != NULL) {
        str[strcspn(str, "\n")] = '\0';
    }
}

int readInt(const char *message) {
    char buffer[100];
    int value;
    char extra;

    while (1) {
        printf("%s", message);
        readLine(buffer, sizeof(buffer));

        if (sscanf(buffer, "%d %c", &value, &extra) == 1)
            return value;

        printf("Invalid input! Enter a number.\n");
    }
}

float readFloat(const char *message) {
    char buffer[100];
    float value;
    char extra;

    while (1) {
        printf("%s", message);
        readLine(buffer, sizeof(buffer));

        if (sscanf(buffer, "%f %c", &value, &extra) == 1 &&
            value >= 0)
            return value;

        printf("Invalid amount! Try again.\n");
    }
}

void pauseScreen() {
    printf("\nPress Enter to continue...");
    getchar();
}

void getCurrentDate(char *buffer, int size) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    strftime(buffer, size, "%d-%m-%Y", t);
}

void formatDate(time_t date, char *buffer, int size) {
    struct tm *t = localtime(&date);

    if (t != NULL)
        strftime(buffer, size, "%d-%m-%Y", t);
    else
        strcpy(buffer, "N/A");
}

time_t addDays(time_t date, int days) {
    struct tm *temp = localtime(&date);

    if (temp == NULL)
        return date;

    struct tm result = *temp;
    result.tm_mday += days;
    result.tm_hour = 12;
    result.tm_min = 0;
    result.tm_sec = 0;

    return mktime(&result);
}

// ================= FILE HANDLING =================

void saveBooks() {
    FILE *fp = fopen("books.dat", "wb");

    if (fp == NULL) {
        printf("Error saving books!\n");
        return;
    }

    fwrite(&bookCount, sizeof(int), 1, fp);
    fwrite(books, sizeof(struct Book), bookCount, fp);

    fclose(fp);
}

void loadBooks() {
    FILE *fp = fopen("books.dat", "rb");

    if (fp == NULL)
        return;

    if (fread(&bookCount, sizeof(int), 1, fp) != 1 ||
        bookCount < 0 || bookCount > MAX_BOOKS) {
        bookCount = 0;
        fclose(fp);
        return;
    }

    if (fread(books, sizeof(struct Book), bookCount, fp)
        != (size_t)bookCount) {
        bookCount = 0;
    }

    fclose(fp);
}

void saveStudents() {
    FILE *fp = fopen("students.dat", "wb");

    if (fp == NULL) {
        printf("Error saving students!\n");
        return;
    }

    fwrite(&studentCount, sizeof(int), 1, fp);
    fwrite(students, sizeof(struct Student), studentCount, fp);

    fclose(fp);
}

void loadStudents() {
    FILE *fp = fopen("students.dat", "rb");

    if (fp == NULL)
        return;

    if (fread(&studentCount, sizeof(int), 1, fp) != 1 ||
        studentCount < 0 || studentCount > MAX_STUDENTS) {
        studentCount = 0;
        fclose(fp);
        return;
    }

    if (fread(students, sizeof(struct Student), studentCount, fp)
        != (size_t)studentCount) {
        studentCount = 0;
    }

    fclose(fp);
}

void saveIssues() {
    FILE *fp = fopen("issues.dat", "wb");

    if (fp == NULL) {
        printf("Error saving issue records!\n");
        return;
    }

    fwrite(&issueCount, sizeof(int), 1, fp);
    fwrite(issues, sizeof(struct Issue), issueCount, fp);

    fclose(fp);
}

void loadIssues() {
    FILE *fp = fopen("issues.dat", "rb");

    if (fp == NULL)
        return;

    if (fread(&issueCount, sizeof(int), 1, fp) != 1 ||
        issueCount < 0 || issueCount > MAX_ISSUES) {
        issueCount = 0;
        fclose(fp);
        return;
    }

    if (fread(issues, sizeof(struct Issue), issueCount, fp)
        != (size_t)issueCount) {
        issueCount = 0;
    }

    fclose(fp);
}

// ================= SEARCH FUNCTIONS =================

int findBook(int id) {
    for (int i = 0; i < bookCount; i++) {
        if (books[i].id == id)
            return i;
    }
    return -1;
}

int findStudent(int id) {
    for (int i = 0; i < studentCount; i++) {
        if (students[i].id == id)
            return i;
    }
    return -1;
}

int findIssue(int id) {
    for (int i = 0; i < issueCount; i++) {
        if (issues[i].issueId == id)
            return i;
    }
    return -1;
}

// ================= BOOK MANAGEMENT =================

// Add Book
void addBook() {
    if (bookCount >= MAX_BOOKS) {
        printf("Book storage is full!\n");
        return;
    }

    struct Book *b = &books[bookCount];

    printf("\n========== ADD NEW BOOK ==========\n");

    b->id = bookCount == 0 ? 1 : books[bookCount - 1].id + 1;

    printf("Book ID: %d\n", b->id);

    printf("Enter ISBN: ");
    readLine(b->isbn, sizeof(b->isbn));

    printf("Enter book title: ");
    readLine(b->title, sizeof(b->title));

    printf("Enter author name: ");
    readLine(b->author, sizeof(b->author));

    printf("Enter publisher: ");
    readLine(b->publisher, sizeof(b->publisher));

    printf("Enter category: ");
    readLine(b->category, sizeof(b->category));

    printf("Enter edition: ");
    readLine(b->edition, sizeof(b->edition));

    b->year = readInt("Enter publication year: ");

    printf("Enter shelf/rack number: ");
    readLine(b->rackNo, sizeof(b->rackNo));

    b->totalCopies = readInt("Enter total copies: ");

    if (b->totalCopies <= 0) {
        printf("Invalid number of copies!\n");
        return;
    }

    b->availableCopies = b->totalCopies;

    bookCount++;

    saveBooks();

    printf("\nBook added successfully!\n");
    printf("Book ID: %d\n", b->id);
}

// Display Book Details
void displayBook(struct Book b) {
    printf("\nBook ID        : %d", b.id);
    printf("\nISBN           : %s", b.isbn);
    printf("\nTitle          : %s", b.title);
    printf("\nAuthor         : %s", b.author);
    printf("\nPublisher      : %s", b.publisher);
    printf("\nCategory       : %s", b.category);
    printf("\nEdition        : %s", b.edition);
    printf("\nPublication Yr : %d", b.year);
    printf("\nRack Number    : %s", b.rackNo);
    printf("\nTotal Copies   : %d", b.totalCopies);
    printf("\nAvailable      : %d\n", b.availableCopies);
}

// View All Books
void viewBooks() {
    printf("\n========== ALL LIBRARY BOOKS ==========\n");

    if (bookCount == 0) {
        printf("No books available.\n");
        return;
    }

    for (int i = 0; i < bookCount; i++) {
        displayBook(books[i]);
        printf("--------------------------------------\n");
    }
}

// Update Book
void updateBook() {
    int id = readInt("\nEnter Book ID to update: ");
    int index = findBook(id);

    if (index == -1) {
        printf("Book not found!\n");
        return;
    }

    struct Book *b = &books[index];

    printf("\n1. Update ISBN\n");
    printf("2. Update Title\n");
    printf("3. Update Author\n");
    printf("4. Update Publisher\n");
    printf("5. Update Category\n");
    printf("6. Update Edition\n");
    printf("7. Update Publication Year\n");
    printf("8. Update Rack Number\n");
    printf("9. Update Total Copies\n");

    int choice = readInt("Enter choice: ");

    switch (choice) {
        case 1:
            printf("Enter new ISBN: ");
            readLine(b->isbn, sizeof(b->isbn));
            break;

        case 2:
            printf("Enter new title: ");
            readLine(b->title, sizeof(b->title));
            break;

        case 3:
            printf("Enter new author: ");
            readLine(b->author, sizeof(b->author));
            break;

        case 4:
            printf("Enter new publisher: ");
            readLine(b->publisher, sizeof(b->publisher));
            break;

        case 5:
            printf("Enter new category: ");
            readLine(b->category, sizeof(b->category));
            break;

        case 6:
            printf("Enter new edition: ");
            readLine(b->edition, sizeof(b->edition));
            break;

        case 7:
            b->year = readInt("Enter new publication year: ");
            break;

        case 8:
            printf("Enter new rack number: ");
            readLine(b->rackNo, sizeof(b->rackNo));
            break;

        case 9: {
            int newTotal = readInt("Enter new total copies: ");

            int issued = b->totalCopies - b->availableCopies;

            if (newTotal < issued || newTotal <= 0) {
                printf("Cannot reduce copies below issued copies!\n");
                return;
            }

            b->totalCopies = newTotal;
            b->availableCopies = newTotal - issued;
            break;
        }

        default:
            printf("Invalid choice!\n");
            return;
    }

    saveBooks();

    printf("Book updated successfully!\n");
}

// Delete Book
void deleteBook() {
    int id = readInt("\nEnter Book ID to delete: ");
    int index = findBook(id);

    if (index == -1) {
        printf("Book not found!\n");
        return;
    }

    for (int i = 0; i < issueCount; i++) {
        if (issues[i].bookId == id && !issues[i].returned) {
            printf("Cannot delete! This book is currently issued.\n");
            return;
        }
    }

    for (int i = index; i < bookCount - 1; i++) {
        books[i] = books[i + 1];
    }

    bookCount--;

    saveBooks();

    printf("Book deleted successfully!\n");
}

// Search Book
void searchBook() {
    char keyword[100];
    int found = 0;

    printf("\nEnter title, author or category: ");
    readLine(keyword, sizeof(keyword));

    for (int i = 0; i < bookCount; i++) {

        if (strstr(books[i].title, keyword) != NULL ||
            strstr(books[i].author, keyword) != NULL ||
            strstr(books[i].category, keyword) != NULL ||
            strstr(books[i].isbn, keyword) != NULL) {

            displayBook(books[i]);
            found = 1;
        }
    }

    if (!found)
        printf("No matching books found!\n");
}

// ================= STUDENT MANAGEMENT =================

// Register Student
void registerStudent() {
    if (studentCount >= MAX_STUDENTS) {
        printf("Student storage is full!\n");
        return;
    }

    struct Student *s = &students[studentCount];

    s->id = studentCount == 0 ? 1001 :
            students[studentCount - 1].id + 1;

    printf("\n========== STUDENT REGISTRATION ==========\n");

    printf("Student ID: %d\n", s->id);

    printf("Enter student name: ");
    readLine(s->name, sizeof(s->name));

    printf("Enter course: ");
    readLine(s->course, sizeof(s->course));

    printf("Enter department: ");
    readLine(s->department, sizeof(s->department));

    printf("Enter email: ");
    readLine(s->email, sizeof(s->email));

    printf("Enter phone number: ");
    readLine(s->phone, sizeof(s->phone));

    printf("Set student password: ");
    readLine(s->password, sizeof(s->password));

    if (strlen(s->password) == 0) {
        printf("Password cannot be empty!\n");
        return;
    }

    s->pendingFine = 0;

    studentCount++;

    saveStudents();

    printf("\nStudent registered successfully!\n");
    printf("Your Student ID is: %d\n", s->id);
}

// View All Students
void viewStudents() {
    printf("\n========== REGISTERED STUDENTS ==========\n");

    if (studentCount == 0) {
        printf("No students registered.\n");
        return;
    }

    for (int i = 0; i < studentCount; i++) {
        printf("\nStudent ID : %d", students[i].id);
        printf("\nName       : %s", students[i].name);
        printf("\nCourse     : %s", students[i].course);
        printf("\nDepartment : %s", students[i].department);
        printf("\nEmail      : %s", students[i].email);
        printf("\nPhone      : %s\n", students[i].phone);
        printf("----------------------------------\n");
    }
}

// Student Login
int studentLogin() {
    int id;
    char password[40];

    printf("\n========== STUDENT LOGIN ==========\n");

    id = readInt("Enter Student ID: ");

    printf("Enter password: ");
    readLine(password, sizeof(password));

    int index = findStudent(id);

    if (index != -1 &&
        strcmp(students[index].password, password) == 0) {

        printf("\nWelcome, %s!\n", students[index].name);
        return index;
    }

    printf("Invalid Student ID or password!\n");
    return -1;
}

// ================= ISSUE MANAGEMENT =================

// Issue Book
void issueBook() {
    if (issueCount >= MAX_ISSUES) {
        printf("Issue record storage is full!\n");
        return;
    }

    int studentId = readInt("\nEnter Student ID: ");
    int bookId = readInt("Enter Book ID: ");

    int si = findStudent(studentId);
    int bi = findBook(bookId);

    if (si == -1) {
        printf("Student not found!\n");
        return;
    }

    if (bi == -1) {
        printf("Book not found!\n");
        return;
    }

    if (books[bi].availableCopies <= 0) {
        printf("Book is currently unavailable!\n");
        return;
    }

    int activeCount = 0;

    for (int i = 0; i < issueCount; i++) {
        if (issues[i].studentId == studentId &&
            !issues[i].returned) {
            activeCount++;
        }
    }

    if (activeCount >= 3) {
        printf("Student cannot borrow more than 3 books at a time!\n");
        return;
    }

    struct Issue *is = &issues[issueCount];

    is->issueId = issueCount == 0 ? 1 :
                  issues[issueCount - 1].issueId + 1;

    is->studentId = studentId;
    is->bookId = bookId;

    is->issueDate = time(NULL);
    is->dueDate = addDays(is->issueDate, LOAN_DAYS);
    is->returnDate = 0;
    is->returned = 0;
    is->fine = 0;

    books[bi].availableCopies--;

    issueCount++;

    saveBooks();
    saveIssues();

    char issueDate[20], dueDate[20];

    formatDate(is->issueDate, issueDate, sizeof(issueDate));
    formatDate(is->dueDate, dueDate, sizeof(dueDate));

    printf("\nBook issued successfully!\n");
    printf("Issue ID    : %d\n", is->issueId);
    printf("Book Name   : %s\n", books[bi].title);
    printf("Student     : %s\n", students[si].name);
    printf("Issue Date  : %s\n", issueDate);
    printf("Due Date    : %s\n", dueDate);
}

// Calculate Current Fine
float calculateFine(time_t dueDate, time_t returnDate) {
    double seconds = difftime(returnDate, dueDate);

    if (seconds <= 0)
        return 0;

    int days = (int)(seconds / 86400);

    if (seconds > days * 86400)
        days++;

    return days * FINE_PER_DAY;
}

// Return Book
void returnBook() {
    int issueId = readInt("\nEnter Issue ID: ");

    int index = findIssue(issueId);

    if (index == -1) {
        printf("Issue record not found!\n");
        return;
    }

    struct Issue *is = &issues[index];

    if (is->returned) {
        printf("This book has already been returned!\n");
        return;
    }

    int bi = findBook(is->bookId);
    int si = findStudent(is->studentId);

    if (bi == -1 || si == -1) {
        printf("Associated record not found!\n");
        return;
    }

    is->returnDate = time(NULL);
    is->returned = 1;

    is->fine = calculateFine(is->dueDate, is->returnDate);

    books[bi].availableCopies++;

    students[si].pendingFine += is->fine;

    saveBooks();
    saveIssues();
    saveStudents();

    printf("\nBook returned successfully!\n");
    printf("Book: %s\n", books[bi].title);
    printf("Late fine: Rs. %.2f\n", is->fine);
}

// View All Issued Books
void viewIssuedBooks() {
    printf("\n========== ALL ISSUE RECORDS ==========\n");

    int found = 0;

    for (int i = 0; i < issueCount; i++) {
        int si = findStudent(issues[i].studentId);
        int bi = findBook(issues[i].bookId);

        if (si == -1 || bi == -1)
            continue;

        char issueDate[20], dueDate[20], returnDate[20];

        formatDate(issues[i].issueDate, issueDate, sizeof(issueDate));
        formatDate(issues[i].dueDate, dueDate, sizeof(dueDate));

        if (issues[i].returned)
            formatDate(issues[i].returnDate, returnDate, sizeof(returnDate));
        else
            strcpy(returnDate, "Not returned");

        printf("\nIssue ID    : %d", issues[i].issueId);
        printf("\nStudent     : %s", students[si].name);
        printf("\nStudent ID  : %d", issues[i].studentId);
        printf("\nBook        : %s", books[bi].title);
        printf("\nIssue Date  : %s", issueDate);
        printf("\nDue Date    : %s", dueDate);
        printf("\nReturn Date : %s", returnDate);
        printf("\nStatus      : %s",
               issues[i].returned ? "Returned" : "Issued");
        printf("\nFine        : Rs. %.2f\n", issues[i].fine);

        found = 1;
    }

    if (!found)
        printf("No issue records found!\n");
}

// View Overdue Books
void viewOverdueBooks() {
    time_t now = time(NULL);
    int found = 0;

    printf("\n========== OVERDUE BOOKS ==========\n");

    for (int i = 0; i < issueCount; i++) {

        if (!issues[i].returned &&
            difftime(now, issues[i].dueDate) > 0) {

            int si = findStudent(issues[i].studentId);
            int bi = findBook(issues[i].bookId);

            if (si == -1 || bi == -1)
                continue;

            float fine = calculateFine(issues[i].dueDate, now);

            printf("\nIssue ID : %d", issues[i].issueId);
            printf("\nStudent  : %s", students[si].name);
            printf("\nBook     : %s", books[bi].title);
            printf("\nFine     : Rs. %.2f\n", fine);

            found = 1;
        }
    }

    if (!found)
        printf("No overdue books!\n");
}

// ================= STUDENT PANEL =================

// View Student Profile
void viewProfile(int index) {
    struct Student s = students[index];

    printf("\n========== MY PROFILE ==========\n");

    printf("Student ID : %d\n", s.id);
    printf("Name       : %s\n", s.name);
    printf("Course     : %s\n", s.course);
    printf("Department : %s\n", s.department);
    printf("Email      : %s\n", s.email);
    printf("Phone      : %s\n", s.phone);
    printf("Pending Fine: Rs. %.2f\n", s.pendingFine);
}

// View My Books
void viewMyBooks(int studentId) {
    int found = 0;

    printf("\n========== MY BORROWED BOOKS ==========\n");

    for (int i = 0; i < issueCount; i++) {

        if (issues[i].studentId == studentId) {

            int bi = findBook(issues[i].bookId);

            if (bi == -1)
                continue;

            char issueDate[20], dueDate[20], returnDate[20];

            formatDate(issues[i].issueDate, issueDate, sizeof(issueDate));
            formatDate(issues[i].dueDate, dueDate, sizeof(dueDate));

            printf("\nIssue ID   : %d", issues[i].issueId);
            printf("\nBook Name  : %s", books[bi].title);
            printf("\nAuthor     : %s", books[bi].author);
            printf("\nIssue Date : %s", issueDate);
            printf("\nDue Date   : %s", dueDate);

            if (issues[i].returned) {
                formatDate(issues[i].returnDate, returnDate, sizeof(returnDate));
                printf("\nReturn Date: %s", returnDate);
                printf("\nStatus     : Returned");
                printf("\nFine       : Rs. %.2f\n", issues[i].fine);
            } else {
                printf("\nStatus     : Currently Issued");

                float fine = calculateFine(issues[i].dueDate, time(NULL));

                printf("\nCurrent Late Fine: Rs. %.2f\n", fine);
            }

            found = 1;
        }
    }

    if (!found)
        printf("You have no borrowing history.\n");
}

// View Available Books
void viewAvailableBooks() {
    printf("\n========== AVAILABLE BOOKS ==========\n");

    int found = 0;

    for (int i = 0; i < bookCount; i++) {

        if (books[i].availableCopies > 0) {
            displayBook(books[i]);
            found = 1;
        }
    }

    if (!found)
        printf("No books currently available.\n");
}

// Change Student Password
void changePassword(int index) {
    char oldPass[40], newPass[40];

    printf("\nEnter current password: ");
    readLine(oldPass, sizeof(oldPass));

    if (strcmp(oldPass, students[index].password) != 0) {
        printf("Incorrect current password!\n");
        return;
    }

    printf("Enter new password: ");
    readLine(newPass, sizeof(newPass));

    if (strlen(newPass) == 0) {
        printf("Password cannot be empty!\n");
        return;
    }

    strcpy(students[index].password, newPass);

    saveStudents();

    printf("Password changed successfully!\n");
}

// Collect Fine
void collectFine() {
    int id = readInt("\nEnter Student ID: ");

    int index = findStudent(id);

    if (index == -1) {
        printf("Student not found!\n");
        return;
    }

    printf("Pending Fine: Rs. %.2f\n",
           students[index].pendingFine);

    if (students[index].pendingFine <= 0) {
        printf("No pending fine!\n");
        return;
    }

    float payment = readFloat("Enter amount received: ");

    if (payment <= 0 || payment > students[index].pendingFine) {
        printf("Invalid payment amount!\n");
        return;
    }

    students[index].pendingFine -= payment;

    saveStudents();

    printf("Payment recorded successfully!\n");
    printf("Remaining Fine: Rs. %.2f\n",
           students[index].pendingFine);
}

// ================= LIBRARY REPORT =================

void libraryStatistics() {
    int totalCopies = 0;
    int available = 0;
    int activeIssues = 0;
    int overdue = 0;

    time_t now = time(NULL);

    for (int i = 0; i < bookCount; i++) {
        totalCopies += books[i].totalCopies;
        available += books[i].availableCopies;
    }

    for (int i = 0; i < issueCount; i++) {
        if (!issues[i].returned) {
            activeIssues++;

            if (difftime(now, issues[i].dueDate) > 0)
                overdue++;
        }
    }

    printf("\n========== LIBRARY STATISTICS ==========\n");

    printf("Total Book Titles : %d\n", bookCount);
    printf("Total Book Copies : %d\n", totalCopies);
    printf("Available Copies  : %d\n", available);
    printf("Issued Copies     : %d\n", activeIssues);
    printf("Registered Students: %d\n", studentCount);
    printf("Total Issue Records: %d\n", issueCount);
    printf("Overdue Books     : %d\n", overdue);
}

// ================= LIBRARIAN LOGIN =================

int librarianLogin() {
    char id[50], password[50];
    char fileId[50], filePass[50];

    FILE *fp = fopen("librarian.dat", "r");

    if (fp == NULL) {
        printf("Librarian credentials file not found!\n");
        return 0;
    }

    printf("\n========== LIBRARIAN LOGIN ==========\n");

    printf("Enter Librarian ID: ");
    readLine(id, sizeof(id));

    printf("Enter Password: ");
    readLine(password, sizeof(password));

    while (fscanf(fp, "%49s %49s", fileId, filePass) == 2) {

        if (strcmp(id, fileId) == 0 &&
            strcmp(password, filePass) == 0) {

            fclose(fp);
            printf("\nLogin successful!\n");
            return 1;
        }
    }

    fclose(fp);

    printf("Invalid login credentials!\n");
    return 0;
}

// ================= LIBRARIAN PANEL =================

void librarianPanel() {
    int choice;

    do {
        printf("\n====================================\n");
        printf("         LIBRARIAN DASHBOARD\n");
        printf("====================================\n");

        printf("1. Add New Book\n");
        printf("2. View All Books\n");
        printf("3. Update Book\n");
        printf("4. Delete Book\n");
        printf("5. Search Book\n");
        printf("6. Register Student\n");
        printf("7. View All Students\n");
        printf("8. Issue Book\n");
        printf("9. Return Book\n");
        printf("10. View Issued Books\n");
        printf("11. View Overdue Books\n");
        printf("12. Collect Fine\n");
        printf("13. Library Statistics\n");
        printf("14. Logout\n");

        choice = readInt("\nEnter your choice: ");

        switch (choice) {
            case 1: addBook(); break;
            case 2: viewBooks(); break;
            case 3: updateBook(); break;
            case 4: deleteBook(); break;
            case 5: searchBook(); break;
            case 6: registerStudent(); break;
            case 7: viewStudents(); break;
            case 8: issueBook(); break;
            case 9: returnBook(); break;
            case 10: viewIssuedBooks(); break;
            case 11: viewOverdueBooks(); break;
            case 12: collectFine(); break;
            case 13: libraryStatistics(); break;
            case 14: printf("Librarian logged out.\n"); break;
            default: printf("Invalid choice!\n");
        }

        if (choice != 14)
            pauseScreen();

    } while (choice != 14);
}

// ================= STUDENT PANEL =================

void studentPanel(int index) {
    int choice;

    do {
        printf("\n====================================\n");
        printf("           STUDENT DASHBOARD\n");
        printf("====================================\n");

        printf("Welcome, %s!\n", students[index].name);

        printf("1. View My Profile\n");
        printf("2. Search Books\n");
        printf("3. View Available Books\n");
        printf("4. View My Borrowed Books\n");
        printf("5. View My Fine\n");
        printf("6. Change Password\n");
        printf("7. Logout\n");

        choice = readInt("\nEnter your choice: ");

        switch (choice) {
            case 1:
                viewProfile(index);
                break;

            case 2:
                searchBook();
                break;

            case 3:
                viewAvailableBooks();
                break;

            case 4:
                viewMyBooks(students[index].id);
                break;

            case 5:
                printf("\nPending Fine: Rs. %.2f\n",
                       students[index].pendingFine);
                break;

            case 6:
                changePassword(index);
                break;

            case 7:
                printf("Student logged out.\n");
                break;

            default:
                printf("Invalid choice!\n");
        }

        if (choice != 7)
            pauseScreen();

    } while (choice != 7);
}

// ================= MAIN FUNCTION =================

int main() {
    int choice;

    loadBooks();
    loadStudents();
    loadIssues();

    do {
        printf("\n");
        printf("========================================\n");
        printf("       LIBRARY MANAGEMENT SYSTEM\n");
        printf("========================================\n");

        printf("1. Librarian Login\n");
        printf("2. Student Login\n");
        printf("3. Exit\n");

        choice = readInt("\nEnter your choice: ");

        switch (choice) {

            case 1:
                if (librarianLogin())
                    librarianPanel();
                break;

            case 2: {
                int index = studentLogin();

                if (index != -1)
                    studentPanel(index);

                break;
            }

            case 3:
                printf("\nThank you for using Library Management System!\n");
                break;

            default:
                printf("Invalid choice! Try again.\n");
        }

        if (choice != 3)
            pauseScreen();

    } while (choice != 3);

    return 0;
}
