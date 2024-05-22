//imt2022087
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/types.h>

#define MAX_USER_TYPE_LENGTH 10
#define MAX_USERNAME_LENGTH 20
#define MAX_PASSWORD_LENGTH 20
#define MAX_MEMBER_USERNAME_LENGTH 20
#define MAX_BOOK_TITLE_LENGTH 50
#define MAX_NUMBER_OF_CHOICES 10

int Addmember(int sock)
{
    char member_username[MAX_MEMBER_USERNAME_LENGTH];
    char password [MAX_MEMBER_USERNAME_LENGTH];
    int memberID[MAX_MEMBER_USERNAME_LENGTH];
    char buf[100];
    int ret;
    printf("Enter member username: ");
    scanf("%19s", member_username);
    write(sock, member_username, strlen(member_username));
    printf("Enter password: ");
    scanf("%19s", password);
    write(sock, password, strlen(password));
    ret = read(sock, buf, 100);
    if(strcmp(buf, "Member already exists") == 0)
    {
        printf("%s\n", buf);
    }
    else if(strcmp(buf, "Member added") == 0)
    {
        printf("%s\n", buf);
    }
    if (ret <= 0) 
    {
        if (ret == 0)
        {
            printf("Server closed connection\n");
        }
        else
        {
            perror("read error");
        }
        exit(EXIT_FAILURE);
    }
    printf("%s", buf);
    return 1;
}

void searchbook(int sock)
{
    char search_title[MAX_BOOK_TITLE_LENGTH];
    char buf[100];
    int ret;
    //write(sock, "A2", strlen("A2"));
    printf("Enter book title: ");
    scanf("%49s", search_title);
    write(sock, search_title, strlen(search_title));
    ret = read(sock, buf, sizeof(buf));
    if (ret == -1) {
        perror("read error");
        exit(EXIT_FAILURE);
    }
    write(1, buf, ret);
}

void membersearchbook(int sock)
{
    char search_title[MAX_BOOK_TITLE_LENGTH];
    char buf[100];
    int ret;
    //write(sock, "M1", strlen("M1"));
    printf("Enter book title: ");
    scanf("%49s", search_title);
    write(sock, search_title, strlen(search_title));
    ret = read(sock, buf, sizeof(buf));
    if (ret == -1) {
        perror("read error");
        exit(EXIT_FAILURE);
    }
    write(1, buf, ret);
}

void deletebook(int sock)
{
    char book_title[MAX_BOOK_TITLE_LENGTH];
    char buf[100];
    int ret;
    //write(sock, "A3", strlen("A3"));
    printf("Enter book title: ");
    scanf("%49s", book_title);
    write(sock, book_title, strlen(book_title));
    ret = read(sock, buf, sizeof(buf));
    if (ret == -1) {
        perror("read error");
        exit(EXIT_FAILURE);
    }
    write(1, buf, ret);
}

void updatestock(int sock)
{
    char book_title[MAX_BOOK_TITLE_LENGTH];
    char buf[100];
    int ret;
    char stock[MAX_BOOK_TITLE_LENGTH];
    //write(sock, "A4", strlen("A4"));
    printf("Enter book title: ");
    scanf("%49s", book_title);
    write(sock, book_title, strlen(book_title));
    //ret = read(sock, buf, sizeof(buf));
    printf("Enter new stock: ");
    scanf("%49s", stock);
    write(sock, stock, strlen(stock));
    ret = read(sock, buf, sizeof(buf));
    printf("%s\n", buf);

    if (ret == -1) 
    {
        perror("read error");
        exit(EXIT_FAILURE);
    }
    //write(1, buf, ret);
}

int viewallbooks(int sock)
{
    char buf[100];
    int ret;
    while((ret = read(sock, buf, sizeof(buf)-1)) > 0)
    {
        buf[ret] = '\0';  // Null-terminate the string
        if(strcmp(buf, "End of file") == 0 || strcmp(buf, "EOF") == 0)
        {
            break;
        }
        if(ret == -1)
        {
            perror("read error");
            exit(EXIT_FAILURE);
            return 0;
        }
        write(1, buf, ret);
    }
    if (ret < 0) 
    {
        perror("read error");
        exit(EXIT_FAILURE);
    }
    printf("EOF, all books viewed\n");
    return 1;
}

void memberviewallbooks(int sock)
{
    char buf[100];
    char lock[100];
    printf("before checking for lock\n");
    read(sock, lock, sizeof(lock));
    //make lock end with null terminator
    lock[sizeof(lock)-1] = '\0';
    printf("%s\n", lock);
    if(strcmp(lock, "Failed to acquire lock") == 0)
    {
        return;
    }
    printf("after checking for lock\n");
    int ret;
    //write(sock, "M2", strlen("M2"));
    ret = read(sock, buf, sizeof(buf));
    if (ret == -1) {
        perror("read error");
        exit(EXIT_FAILURE);
    }
    write(1, buf, ret);
}

void borrowbook(int sock)
{
    char book_title[MAX_BOOK_TITLE_LENGTH];
    char username[MAX_USERNAME_LENGTH];
    char buf[100];
    int ret;
    //write(sock, "M3", strlen("M3"));
    printf("Enter your username: ");
    scanf("%19s", username);
    write(sock, username, strlen(username));
    printf("Enter book title: ");
    scanf("%49s", book_title);
    write(sock, book_title, strlen(book_title));
    ret = read(sock, buf, sizeof(buf));
    if (ret == -1) {
        perror("read error");
        exit(EXIT_FAILURE);
    }
    //write(1, buf, ret);
    printf("%s\n", buf);
}

int deletemember(int sock)
{
    char member_username[MAX_MEMBER_USERNAME_LENGTH];
    char buf[100];
    int ret;
    //write(sock, "A7", strlen("A7"));
    printf("Enter member username: ");
    scanf("%19s", member_username);
    write(sock, member_username, strlen(member_username));
    ret = read(sock, buf, sizeof(buf));
    if (ret == -1) 
    {
        perror("read error");
        exit(EXIT_FAILURE);
    }
    write(1, buf, ret);
}

int viewallmembers(int sock)
{
    char buf[100];
    int ret;
    while((ret = read(sock, buf, sizeof(buf)-1)) > 0)
    {
        buf[ret] = '\0';
        if(strcmp(buf, "End of file") == 0 || strcmp(buf, "EOF") == 0)
        {
            break;
        }
        if(ret == -1)
        {
            perror("read error");
            exit(EXIT_FAILURE);
            return 0;
        }
        write(1, buf, ret);
    }
    if (ret <= 0) 
    {
        if (ret == 0)
        {
            printf("Server closed connection\n");
        }
        else
        {
            perror("read error");
        }
        exit(EXIT_FAILURE);
    }
    printf("members viewed\n");
    return 1;
}

void returnbook(int sock)
{
    char book_title[MAX_BOOK_TITLE_LENGTH];
    char username[MAX_USERNAME_LENGTH];
    char buf[100];
    int ret;
        //write(sock, "M4", strlen("M4"));
    printf("Enter your username: ");
    scanf("%19s", username);
    write(sock, username, strlen(username));
    printf("Enter book title: ");
    scanf("%49s", book_title);
    write(sock, book_title, strlen(book_title));
    ret = read(sock, buf, sizeof(buf));
    if (ret == -1) 
    {
        perror("read error");
        exit(EXIT_FAILURE);
    }
    printf("%s\n", buf);
    //write(1, buf, ret);
}

void forgotpassword(int sock)
{
    char entered_username[MAX_USERNAME_LENGTH];
    char buf[100];
    int ret;
    //write(sock, "M5", strlen("M5"));
    printf("Enter username: ");
    scanf("%19s", entered_username);
    write(sock, entered_username, strlen(entered_username));
   ret = read(sock, buf, sizeof(buf) - 1); // leave space for null terminator
    if (ret == -1) {
        perror("read error");
        exit(EXIT_FAILURE);
    }
    buf[ret] = '\0'; // null-terminate the string
    printf("%s\n", buf);
    if(strcmp(buf, "Username not found") == 0)
    {
        return;
    }
    printf("Enter new password: ");
    scanf("%19s", entered_username);
    write(sock, entered_username, strlen(entered_username));
    ret = read(sock, buf, sizeof(buf));
    if (ret == -1) {
        perror("read error");
        exit(EXIT_FAILURE);
    }
    printf("%s\n", buf);
    return;

    //write(1, buf, ret);
}

int Addbook(int sock)
{
    char book_title[MAX_BOOK_TITLE_LENGTH];
    char buf[100];
    int ret;
    printf("Enter book title: ");
    scanf("%49s", book_title);
    
    
    // Send "A1" to the server
    //write(sock, "A1", strlen("A1"));
    
    // Then send the book title
    write(sock, book_title, strlen(book_title));
    printf("Enter book stock: ");
    scanf("%49s", book_title);
    write(sock, book_title, strlen(book_title));
    
    ret = read(sock, buf, sizeof(buf));

    if (ret == -1) {
        perror("read error");
        exit(EXIT_FAILURE);
    }
    printf("%s\n", buf);
    //write(1, buf, ret);
}

int adminviewallbooks(int sock)
{
    char buf[100];
    int ret;
    //write(sock, "A5", strlen("A5"));
    while((ret = read(sock, buf, sizeof(buf)-1)) > 0)
    {
        buf[ret] = '\0';  // Null-terminate the string
        if(strcmp(buf, "End of file") == 0 || strcmp(buf, "EOF") == 0)
        {
            break;
        }
        if(ret == -1)
        {
            perror("read error");
            exit(EXIT_FAILURE);
            return 0;
        }
        //buf[ret] = '\0';

        write(1, buf, ret);
    }
    if (ret < 0) 
    {
        perror("read error");
        exit(EXIT_FAILURE);
    }
    printf("EOF, all books viewed\n");
    return 1;
}

int viewalltrans(int sock)
{
    char buf[100];
    int ret;
    //write(sock, "A9", strlen("A9"));
    while((ret = read(sock, buf, sizeof(buf)-1)) > 0)
    {
        buf[ret] = '\0';  // Null-terminate the string
        if(strcmp(buf, "End of file") == 0 || strcmp(buf, "EOF") == 0)
        {
            break;
        }
        if(ret == -1)
        {
            perror("read error");
            exit(EXIT_FAILURE);
            return 0;
        }
        write(1, buf, ret);
    }
    if (ret < 0) 
    {
        perror("read error");
        exit(EXIT_FAILURE);
    }
    printf("EOF, all transactions viewed\n");
    return 1;
}


int main()
{
    struct sockaddr_in server;
    int sock;
    char message[1000], server_reply[2000];
    char user_type[MAX_USER_TYPE_LENGTH];
    char user_choice[MAX_USER_TYPE_LENGTH];
    char entered_username[MAX_USERNAME_LENGTH];
    char entered_password[MAX_PASSWORD_LENGTH];
    char member_username[MAX_MEMBER_USERNAME_LENGTH];
    char entered_MemberID [MAX_MEMBER_USERNAME_LENGTH];
    int memberID[MAX_MEMBER_USERNAME_LENGTH];
    char book_title[MAX_BOOK_TITLE_LENGTH], search_title[MAX_BOOK_TITLE_LENGTH];
    char buf[100];
    int ret;
    char choice[MAX_NUMBER_OF_CHOICES];
    int auth_status=0;
    int admin = 0;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    printf("Socket created\n");
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(8040);
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
    // Write "connection started" to the server
    //write(sock, "connection started", strlen("connection started"));

    // Read the server's response and print it
    char server_message[2000];
    // int bytes_read = read(sock, server_message, sizeof(server_message) - 1);

    // if (bytes_read < 0) 
    // {
    //     printf("Failed to read from server\n");
    // } 
    // else 
    // {
    //     server_message[bytes_read] = '\0'; // Add null terminator
    //     printf("Server message: %s\n", server_message);
    //     printf("Connected\n");
    // }
    printf("Library management system\n");
    while(auth_status==0)
    {
        printf("Enter login if you already have an account\n");
        printf("Enter register if you don't have an account\n");
        printf("Enter admin if you are an admin\n");
        printf("Enter forgot if you forgot your password\n");
        printf("Enter end to exit\n");
        scanf("%9s", user_choice);
        write(sock, user_choice, strlen(user_choice));
        if(strcmp(user_choice, "register") == 0)
        {
            printf("Enter username: ");
            scanf("%19s", entered_username);
            write(sock, entered_username, strlen(entered_username));
            printf("Enter password (don't forget): ");
            scanf("%19s", entered_password);
            write(sock, entered_password, strlen(entered_password));
            read(sock, server_reply, 2000);
            if(strcmp(server_reply, "User already exists") == 0)
            {
                printf("%s\n", server_reply);
                auth_status = 0;
                continue;
            }
            else if(strcmp(server_reply, "Registered") == 0)
            {
                printf("%s\n", server_reply);
                auth_status = 1;
            }
            //ret = read(sock, buf, sizeof(buf));
            if (ret == -1) 
            {
                perror("read error");
                exit(EXIT_FAILURE);
            }
            //write(1, buf, ret);
            printf("auth_status: %d\n",auth_status);
            if(strcmp(buf,"Registered\n")==0)
            {
                auth_status=1;
            }
            else
            {
                printf("Invalid username or password\n");
                auth_status=0;
                continue;
            }
        }
        else if(strcmp(user_choice, "login") == 0)
        {
            printf("Enter username: ");
            scanf("%19s", entered_username);
            write(sock, entered_username, strlen(entered_username));
            printf("Enter password: ");
            scanf("%19s", entered_password);
            write(sock, entered_password, strlen(entered_password));
            read(sock, server_reply, 2000);
            printf("server reply:%s\n", server_reply);
            if(strcmp(server_reply,"Login successful")==0)
            {
                auth_status=1;
            }
            else
            {
                printf("Invalid username or password\n");
            }
            printf("auth_status: %d\n",auth_status);
         }
        else if (strcmp(user_choice, "admin") == 0)
        {
            //write(sock, "admin", strlen("admin"));
            printf("Enter password: ");
            scanf("%19s", entered_password);
            //sleep(3);
            write(sock, entered_password, strlen(entered_password));
            read(sock, server_reply, 2000);
            printf("%s\n", server_reply);
            if(strcmp(server_reply, "Login successful") == 0)
            {
                auth_status = 1;
                // read(sock, &admin, sizeof(admin));
                admin = 1;
            }
            else if(strcmp(server_reply, "Login failed") == 0)
            {
                printf("Invalid password\n");
                continue;
            }
            // read from the server
            // if(strcmp(entered_password,"admin")==0)
            // {
            //     auth_status=1;
            //     admin=1;
            // }
                //ret = read(sock, buf, sizeof(buf));
            if (ret == -1) 
            {
                perror("read error");
                exit(EXIT_FAILURE);
            }
            // write(1, buf, ret);
            // printf("buf: %s\n",buf);
            // if(strcmp(buf,"Login successful!\n")==0 || strcmp(buf,"Login successful!")==0){
            //     auth_status=1;
                
            // }
            printf("auth_status: %d\n",auth_status);
            printf("Admin\n");
            while(auth_status==1 && admin==1)
            {
                printf("Enter 1 to add a book\n");
                printf("Enter 2 to search for a book\n");
                printf("Enter 3 to delete a book\n");
                printf("Enter 4 to update stock of a book\n");
                printf("Enter 5 to view all books\n");
                printf("Enter 6 to Add a member\n");
                printf("Enter 7 to Delete a member\n");
                printf("Enter 8 to view all members\n");
                printf("Enter 9 to view all borrowed books\n");
                printf("Enter 0 to exit\n");
                scanf("%9s", choice);
                printf("choice: %s\n",choice);
                //write(sock, choice, strlen(choice));

                if(strcmp(choice,"1")==0)
                {
                    write(sock, "A1", strlen("A1"));
                    Addbook(sock);
                }
                else if(strcmp(choice,"2")==0)
                {
                    write(sock, "A2", strlen("A2"));
                    searchbook(sock);
                }
                else if(strcmp(choice,"3")==0)
                {
                    write(sock, "A3", strlen("A3"));
                    deletebook(sock);
                }
                else if(strcmp(choice,"4")==0)
                {
                    write(sock, "A4", strlen("A4"));
                    updatestock(sock);
                }
                else if(strcmp(choice,"5")==0)
                {
                    write(sock, "A5", strlen("A5"));
                    adminviewallbooks(sock);
                    printf("Back to menu\n");
                    continue;
                }
                else if(strcmp(choice,"6")==0)
                {
                    write(sock, "A6", strlen("A6"));
                    Addmember(sock);
                }
                else if(strcmp(choice,"7")==0)
                {
                    write(sock, "A7", strlen("A7"));
                    deletemember(sock);
                }
                else if(strcmp(choice,"8")==0)
                {
                    write(sock, "A8", strlen("A8"));
                    viewallmembers(sock);
                }
                else if(strcmp(choice,"9")==0)
                {
                    write(sock, "A9", strlen("A9"));
                    viewalltrans(sock);
                }
                else if(strcmp(choice,"0")==0)
                {
                    write(sock, "A0", strlen("A0"));
                    close(sock);
                    exit(0);
                
                }
                else
                {
                    printf("Invalid choice\n");
                }

            }
        }
        else if(strcmp(user_choice, "forgot") == 0)
        {
            forgotpassword(sock);
        }
        else if(strcmp(user_choice, "end") == 0)
        {
            printf("Exiting\n");
            write(sock, "end", strlen("end"));
            close(sock);
            exit(0);
        }
        else 
        {
            printf("please enter register or login only\n");
        }
    }

    if(auth_status==1)
    {
        printf("Member\n");
        while(1)
        {
            printf("Enter 1 to search for a book\n");
            printf("Enter 2 to view all books\n");
            printf("Enter 3 to borrow a book\n");
            printf("Enter 4 to return a book\n");
            printf("Enter 0 to exit\n");
            scanf("%9s", choice);
            //write(sock, choice, strlen(choice));
            if(strcmp(choice,"1")==0)
            {
                write(sock, "M1", strlen("M1"));
                membersearchbook(sock);
            }
            else if(strcmp(choice,"2")==0)
            {
                write(sock, "M2", strlen("M2"));
                memberviewallbooks(sock);
            }
            else if(strcmp(choice,"3")==0)
            {
                write(sock, "M3", strlen("M3"));
                borrowbook(sock);
            }
            else if(strcmp(choice,"4")==0)
            {
                write(sock, "M4", strlen("M4"));
                returnbook(sock);
            }
            else if(strcmp(choice,"0")==0)
            {
                write(sock, "M0", strlen("M0"));
                close(sock);
                exit(0);
            }
            else
            {
                printf("Invalid choice\n");
            }
        }
    }


}