//imt2022087
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;


int AddBook(int *newsock,char *buffer,int ret)
{
    char book_name[100];
    char author[100];
    int stock;
    printf("Inside AddBook waiting for read\n");
    memset(buffer,0,sizeof(buffer));
    ret=read(*newsock,buffer,sizeof(buffer)-1);
    if(ret<0)
    {
        perror("Error reading from socket\n");
        exit(1);
    }
    buffer[ret]='\0';
    strcpy(book_name,buffer);
    memset(buffer,0,sizeof(buffer));
    ret=read(*newsock,buffer,sizeof(buffer)-1);
    if(ret<0)
    {
        perror("Error reading from socket\n");
        exit(1);
    }
    buffer[ret]='\0';
    stock=atoi(buffer);
    FILE *books = fopen("books.txt","a");
    if(books==NULL)
    {
        perror("Error opening file!\n");
        exit(EXIT_FAILURE);
    }
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();
    int fd = fileno(books);
    fcntl(fd, F_SETLKW, &lock);
    fprintf(books,"%s %d\n",book_name,stock);
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    write(*newsock,"Book added\n",11);
    printf("Book added\n");
    fclose(books);
    return 1;
}

int SearchBook(int *newsock,char *buffer,int ret)
{
    char book_name[100];
    memset(buffer,0,sizeof(buffer));
    ret=read(*newsock,buffer,sizeof(buffer)-1);
    if(ret<0)
    {
        perror("Error reading from socket\n");
        exit(1);
    }
    buffer[ret]='\0';
    strcpy(book_name,buffer);
    FILE *books = fopen("books.txt","r");
    if(books==NULL)
    {
        perror("Error opening file!\n");
        exit(EXIT_FAILURE);
    }
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();
    int fd = fileno(books);
    fcntl(fd, F_SETLKW, &lock);
    char temp[100];
    int found=0;
    while(fscanf(books,"%s",temp)!=EOF)
    {
        if(strcmp(temp,book_name)==0)
        {
            found=1;
            break;
        }
    }
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    fclose(books);
    if(found==1)
    {
        write(*newsock,"Book found\n",11);
    }
    else
    {
        write(*newsock,"Book not found\n",15);
    }
    return 1;
}

int DeleteBook(int *newsock,char *buffer,int ret)
{
    char book_name[100];
    memset(buffer,0,sizeof(buffer));
    ret=read(*newsock,buffer,sizeof(buffer)-1);
    if(ret<0)
    {
        perror("Error reading from socket\n");
        exit(1);
    }
    buffer[ret]='\0';
    strcpy(book_name,buffer);
    FILE *books = fopen("books.txt","r");
    if(books==NULL)
    {
        perror("Error opening file!\n");
        exit(EXIT_FAILURE);
    }
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();
    int fd = fileno(books);
    fcntl(fd, F_SETLKW, &lock);

    FILE *temp = fopen("temp.txt","w");
    if(temp==NULL)
    {
        perror("Error opening file!\n");
        exit(EXIT_FAILURE);
    }
    struct flock lock1;
    lock1.l_type = F_WRLCK;
    lock1.l_whence = SEEK_SET;
    lock1.l_start = 0;
    lock1.l_len = 0;
    lock1.l_pid = getpid();
    int fd1 = fileno(temp);
    fcntl(fd1, F_SETLKW, &lock1);

    char temp1[100];
    char temp2[100];
    int temp3;
    int found=0;
    while(fscanf(books,"%s %s",temp1,temp2)!=EOF)
    {
        if(strcmp(temp1,book_name)==0)
        {
            found=1;
            continue;
        }
        fprintf(temp,"%s %s\n",temp1,temp2);
    }
    lock1.l_type = F_UNLCK;
    fcntl(fd1, F_SETLK, &lock1);
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    fclose(books);
    fclose(temp);
    remove("books.txt");
    rename("temp.txt","books.txt");
    if(found==1)
    {
        write(*newsock,"Book deleted\n",13);
    }
    else
    {
        write(*newsock,"Book not found\n",15);
    }
    return 1;
}

int UpdateStock(int *newsock,char *buffer,int ret)
{
    char book_name[100];
    int stock;
    memset(buffer,0,sizeof(buffer));
    ret=read(*newsock,buffer,sizeof(buffer)-1);
    if(ret<0)
    {
        perror("Error reading from socket\n");
        exit(1);
    }
    buffer[ret]='\0';
    strcpy(book_name,buffer);
    memset(buffer,0,sizeof(buffer));
    ret=read(*newsock,buffer,sizeof(buffer)-1);
    if(ret<0)
    {
        perror("Error reading from socket\n");
        exit(1);
    }
    buffer[ret]='\0';
    stock=atoi(buffer);
    FILE *books = fopen("books.txt","r");
    if(books==NULL)
    {
        perror("Error opening file!\n");
        exit(EXIT_FAILURE);
    }
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();
    int fd = fileno(books);
    fcntl(fd, F_SETLKW, &lock);
    FILE *temp = fopen("temp.txt","w");
    struct flock lock1;
    lock1.l_type = F_WRLCK;
    lock1.l_whence = SEEK_SET;
    lock1.l_start = 0;
    lock1.l_len = 0;
    lock1.l_pid = getpid();
    int fd1 = fileno(temp);
    fcntl(fd1, F_SETLKW, &lock1);
    if(temp==NULL)
    {
        perror("Error opening file!\n");
        exit(EXIT_FAILURE);
    }
    char temp1[100];
    int temp3;
    int found=0;
    while(fscanf(books,"%s %d",temp1,&temp3)!=EOF)
    {
        if(strcmp(temp1,book_name)==0)
        {
            found=1;
            fprintf(temp,"%s %d\n",temp1,stock);
        }
        else
        {
            fprintf(temp,"%s %d\n",temp1,temp3);
        }
    }
    lock1.l_type = F_UNLCK;
    fcntl(fd1, F_SETLK, &lock1);
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    fclose(books);
    fclose(temp);
    remove("books.txt");
    rename("temp.txt","books.txt");
    if(found==1)
    {
        write(*newsock,"Stock updated\n",14);
    }
    else
    {
        write(*newsock,"Book not found\n",15);
    }
    return 1;
}

int AddMember(int *newsock,char *buffer,int ret)
{

    char username[100];
    char password[100];
    
    memset(buffer,0,sizeof(buffer));
    ret=read(*newsock,buffer,sizeof(buffer)-1);
    if(ret<0)
    {
        perror("Error reading from socket\n");
        exit(1);
    }
    buffer[ret]='\0';
    strcpy(username,buffer);
    memset(buffer,0,sizeof(buffer));
    ret=read(*newsock,buffer,sizeof(buffer)-1);
    if(ret<0)
    {
        perror("Error reading from socket\n");
        exit(1);
    }
    buffer[ret]='\0';
    strcpy(password,buffer);
    FILE *members = fopen("members.txt","a");
    if(members==NULL)
    {
        perror("Error opening file!\n");
        exit(EXIT_FAILURE);
    }
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();
    int fd = fileno(members);
    fcntl(fd, F_SETLKW, &lock);
    //check if the member already exists
    char temp[100];
    int found=0;
    while(fscanf(members,"%s",temp)!=EOF)
    {
        if(strcmp(temp,username)==0)
        {
            found=1;
            break;
        }
    }
    if(found==1)
    {
        write(*newsock,"Member already exists",21);
        return 1;
    }
    fprintf(members,"%s\n",username);
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    fclose(members);
    FILE *users = fopen("users.txt","a");
    if(users==NULL)
    {
        perror("Error opening file!\n");
        exit(EXIT_FAILURE);
    }
    struct flock lock1;
    lock1.l_type = F_WRLCK;
    lock1.l_whence = SEEK_SET;
    lock1.l_start = 0;
    lock1.l_len = 0;
    lock1.l_pid = getpid();
    int fd1 = fileno(users);
    fcntl(fd1, F_SETLKW, &lock1);
    fprintf(users,"%s %s\n",username,password);
    lock1.l_type = F_UNLCK;
    fcntl(fd1, F_SETLK, &lock1);
    fclose(users);
    printf("Member added\n");
    write(*newsock,"Member added\n",13);
    return 1;
}

int DeleteMember(int *newsock,char *buffer,int ret)
{
    char username[100];
    memset(buffer,0,sizeof(buffer));
    ret=read(*newsock,buffer,sizeof(buffer)-1);
    if(ret<0)
    {
        perror("Error reading from socket\n");
        exit(1);
    }
    buffer[ret]='\0';
    strcpy(username,buffer);
    FILE *members = fopen("members.txt","r");
    if(members==NULL)
    {
        perror("Error opening file!\n");
        exit(EXIT_FAILURE);
    }
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();
    int fd = fileno(members);
    fcntl(fd, F_SETLKW, &lock);
    FILE *temp = fopen("temp.txt","w");
    if(temp==NULL)
    {
        perror("Error opening file!\n");
        exit(EXIT_FAILURE);
    }
    struct flock lock1;
    lock1.l_type = F_WRLCK;
    lock1.l_whence = SEEK_SET;
    lock1.l_start = 0;
    lock1.l_len = 0;
    lock1.l_pid = getpid();
    int fd1 = fileno(temp);
    fcntl(fd1, F_SETLKW, &lock1);
    char temp1[100];
    int found=0;
    while(fscanf(members,"%s",temp1)!=EOF)
    {
        if(strcmp(temp1,username)==0)
        {
            found=1;
            continue;
        }
        fprintf(temp,"%s\n",temp1);
    }
    lock1.l_type = F_UNLCK;
    fcntl(fd1, F_SETLK, &lock1);
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    fclose(members);
    fclose(temp);
    remove("members.txt");
    rename("temp.txt","members.txt");
    if(found==1)
    {
        write(*newsock,"Member deleted\n",15);
    }
    else
    {
        write(*newsock,"Member not found\n",17);
    }
    //delete from users.txt
    FILE *users = fopen("users.txt","r");
    if(users==NULL)
    {
        perror("Error opening file!\n");
        exit(EXIT_FAILURE);
    }
    struct flock lock2;
    lock2.l_type = F_WRLCK;
    lock2.l_whence = SEEK_SET;
    lock2.l_start = 0;
    lock2.l_len = 0;
    lock2.l_pid = getpid();
    int fd2 = fileno(users);
    fcntl(fd2, F_SETLKW, &lock2);
    FILE *tempfile = fopen("tempfile.txt","w");

    if(tempfile==NULL)
    {
        perror("Error opening file!\n");
        exit(EXIT_FAILURE);
    }
    struct flock lock3;
    lock3.l_type = F_WRLCK;
    lock3.l_whence = SEEK_SET;
    lock3.l_start = 0;
    lock3.l_len = 0;
    lock3.l_pid = getpid();
    int fd3 = fileno(tempfile);
    fcntl(fd3, F_SETLKW, &lock3);
    char temp2[100];
    char temp3[100];
    int found1=0;

    while(fscanf(users,"%s %s",temp2,temp3)!=EOF)
    {
        if(strcmp(temp2,username)==0)
        {
            found1=1;
            continue;
        }
        fprintf(tempfile,"%s %s\n",temp2,temp3);
    }
    lock3.l_type = F_UNLCK;
    fcntl(fd3, F_SETLK, &lock3);
    lock2.l_type = F_UNLCK;
    fcntl(fd2, F_SETLK, &lock2);
    fclose(users);
    fclose(tempfile);
    remove("users.txt");
    rename("tempfile.txt","users.txt");
    if(found1==1)
    {
        write(*newsock,"User deleted\n",13);
    }
    else
    {
        write(*newsock,"User not found\n",15);
    }
    return 1;
}

int UpdateMember(int *newsock,char *buffer,int ret)
{
    char email[100];
    char phone[100];
    char address[100];
    memset(buffer,0,sizeof(buffer));
    ret=read(*newsock,buffer,sizeof(buffer)-1);
    if(ret<0)
    {
        perror("Error reading from socket\n");
        exit(1);
    }
    buffer[ret]='\0';
    strcpy(email,buffer);
    memset(buffer,0,sizeof(buffer));
    ret=read(*newsock,buffer,sizeof(buffer)-1);
    if(ret<0)
    {
        perror("Error reading from socket\n");
        exit(1);
    }
    buffer[ret]='\0';
    strcpy(phone,buffer);
    memset(buffer,0,sizeof(buffer));
    ret=read(*newsock,buffer,sizeof(buffer)-1);
    if(ret<0)
    {
        perror("Error reading from socket\n");
        exit(1);
    }
    buffer[ret]='\0';
    strcpy(address,buffer);
    FILE *members = fopen("members.txt","r");
    if(members==NULL)
    {
        perror("Error opening file!\n");
        exit(EXIT_FAILURE);
    }
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();
    int fd = fileno(members);
    fcntl(fd, F_SETLKW, &lock);
    FILE *temp = fopen("temp.txt","w");
    if(temp==NULL)
    {
        perror("Error opening file!\n");
        exit(EXIT_FAILURE);
    }
    struct flock lock1;
    lock1.l_type = F_WRLCK;
    lock1.l_whence = SEEK_SET;
    lock1.l_start = 0;
    lock1.l_len = 0;
    lock1.l_pid = getpid();
    int fd1 = fileno(temp);
    fcntl(fd1, F_SETLKW, &lock1);
    char temp1[100];
    char temp2[100];
    char temp3[100];
    char temp4[100];
    int found=0;
    while(fscanf(members,"%s %s %s %s",temp1,temp2,temp3,temp4)!=EOF)
    {
        if(strcmp(temp2,email)==0)
        {
            found=1;
            fprintf(temp,"%s %s %s %s\n",temp1,temp2,phone,address);
        }
        else
        {
            fprintf(temp,"%s %s %s %s\n",temp1,temp2,temp3,temp4);
        }
    }
    lock1.l_type = F_UNLCK;
    fcntl(fd1, F_SETLK, &lock1);
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    fclose(members);
    fclose(temp);
    remove("members.txt");
    rename("temp.txt","members.txt");
    if(found==1)
    {
        write(*newsock,"Member updated\n",15);
    }
    else
    {
        write(*newsock,"Member not found\n",17);
    }
    return 1;
}

int ViewBooks(int *newsock,char *buffer,int ret)
{
    // FILE *books = fopen("books.txt","r");
    // if(books==NULL)
    // {
    //     perror("Error opening file!\n");
    //     exit(EXIT_FAILURE);
    // }
    // struct flock lock;
    // lock.l_type = F_WRLCK;
    // lock.l_whence = SEEK_SET;
    // lock.l_start = 0;
    // lock.l_len = 0;
    // lock.l_pid = getpid();
    // int fd = fileno(books);
    // fcntl(fd, F_SETLKW, &lock);

    // checking for lock

    // if (fcntl(fd, F_SETLK, &lock) == -1) 
    // {
    //     if (errno == EACCES || errno == EAGAIN) 
    //     {

    //         printf("File is locked by another process\n");
    //         //exit(1);
    //     }

    //     if (fcntl(fd, F_SETLKW, &lock) == -1) 
    //     {
    //         //perror("fcntl");
    //         printf("Error locking file because another process is locking and i can't allow another function to acess it\n");
    //         write(*newsock,"File is locked",14);
    //         return -1;
    //     }
    
    // }
//--------------------------------------------------------------------
    // FILE *books = fopen("books.txt","r+");
    // if(books==NULL)
    // {
    //     perror("Error opening file!\n");
    //     exit(EXIT_FAILURE);
    // }
    // struct flock lock;
    // lock.l_type = F_WRLCK; // set lock type to write lock
    // lock.l_whence = SEEK_SET;
    // lock.l_start = 0;
    // lock.l_len = 0;
    // lock.l_pid = getpid();
    // int fd = fileno(books);
    // if (fcntl(fd, F_SETLKW, &lock) == -1) // checking for lock
    // {
    //     if (errno == EACCES || errno == EAGAIN) 
    //     {
    //         printf("File is locked by another process\n");
    //         write(*newsock,"File is locked",14);
    //         return -1;
    //     }
    // }
    // printf("File is not locked\n");
    // write(*newsock,"File not locked\n",17);
    // printf("sleeping\n");
    // sleep(999); //to check for lock
    // printf("awake\n");
    // char temp[100],temp1[100],temp2[100];
    // int temp3;
    // while(fscanf(books,"%s %d",temp1,&temp3)!=EOF)
    // {
    //     write(*newsock,temp1,strlen(temp1));
    //     write(*newsock,"\n",1);
    // }
    // ssize_t write_ret;
    // if(feof(books))
    // {
        
    //     if (write_ret < 0) 
    //     {
    //         perror("Error writing to socket");
    //         exit(1);
    //     }
    //     fsync(*newsock);
    // }
    // write_ret = write(*newsock,"End of file",11);
    // lock.l_type = F_UNLCK;
    // fcntl(fd, F_SETLK, &lock);
    // fclose(books);
    // printf("Books viewed\n");
    // return 1;
    printf("Inside ViewBooks\n");
    int lock_status = pthread_mutex_trylock(&lock); // attempt to acquire the lock
    printf("lock status: %d\n", lock_status);
    if (lock_status == 0) 
    {
        printf("Lock acquired successfully\n");
        write(*newsock, "Lock acquired successfully", 26);
    }
    else 
    {
        printf("Failed to acquire lock\n");
        write(*newsock, "Failed to acquire lock", 23);
        return -1;
        // handle error or return from function
    }
    
    printf("lock checked\n");
    FILE *books = fopen("books.txt","r+");
    if(books==NULL)
    {
        perror("Error opening file!\n");
        exit(EXIT_FAILURE);
    }
    //uncomment for checking lock
    // printf("sleeping\n");
    // sleep(999); //to check for lock
    // printf("awake\n");

    char temp[100],temp1[100],temp2[100];
    int temp3;
    while(fscanf(books,"%s %d",temp1,&temp3)!=EOF)
    {
        write(*newsock,temp1,strlen(temp1));
        write(*newsock,"\n",1);
    }
    ssize_t write_ret;
    if(feof(books))
    {
        
        if (write_ret < 0) 
        {
            perror("Error writing to socket");
            exit(1);
        }
        fsync(*newsock);
    }
    write_ret = write(*newsock,"End of file",11);



    fclose(books);
    printf("Books viewed\n");

    pthread_mutex_unlock(&lock); // release the lock

    return 1;
}

int AdminViewBooks(int *newsock,char *buffer,int ret)
{
    FILE *books = fopen("books.txt","r");
    if(books==NULL)
    {
        perror("Error opening file!\n");
        exit(EXIT_FAILURE);
    }
    struct flock lock;
    lock.l_type = F_WRLCK; // Use a write lock to prevent other processes from reading or writing
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();
    int fd = fileno(books);
    
    // checking for lock

    // if (fcntl(fd, F_SETLK, &lock) == -1) {
    // if (errno == EACCES || errno == EAGAIN) {
    //     printf("File is locked by another process\n");
    //     exit(1);
    // }

    // if (fcntl(fd, F_SETLKW, &lock) == -1) {
    //     perror("fcntl");
    //     printf("Error locking file because another process is locking and i can't allow another function to acess it\n");
    //     return -1;
    // }
    // }

    
    //sleep(999); //to check for lock
    

    char temp[100];
    char temp1[100];
    int temp2;
    while(fscanf(books,"%s %d",temp1,&temp2)!=EOF)
    {
        write(*newsock,temp1,strlen(temp1));
        write(*newsock," - ",3);
        char temp3[100];
        sprintf(temp3,"%d",temp2);
        write(*newsock,temp3,strlen(temp3));
        write(*newsock,"\n",1);
    }
    ssize_t write_ret;
    if(feof(books))
    {
        
        if (write_ret < 0) 
        {
            perror("Error writing to socket");
            exit(1);
        }
        fsync(*newsock);
    }
    write_ret = write(*newsock,"End of file",11);
     

     lock.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }

    fclose(books);
    printf("Books viewed\n");
    return 1;
}

int ViewMembers(int *newsock,char *buffer,int ret)
{
    //make this function similar to view books
    FILE *members = fopen("members.txt","r");
    if(members==NULL)
    {
        perror("Error opening file!\n");
        exit(EXIT_FAILURE);
    }
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();
    int fd = fileno(members);
    fcntl(fd, F_SETLKW, &lock);
    char temp[100];
    char temp1[100];
    while(fscanf(members,"%s",temp1)!=EOF)
    {
        write(*newsock,temp1,strlen(temp1));
        write(*newsock,"\n",1);
    }
    ssize_t write_ret;
    if(feof(members))
    {
        
        if (write_ret < 0) 
        {
            perror("Error writing to socket");
            exit(1);
        }
        fsync(*newsock);
    }
    write_ret = write(*newsock,"End of file",11);
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    fclose(members);
    printf("Members viewed\n");
    return 1;
}

int BorrowBook(int *newsock,char *buffer,int ret)
{
    char book_name[100];
    char username[100];
    memset(buffer,0,sizeof(buffer));
    ret=read(*newsock,buffer,sizeof(buffer)-1);
    if(ret<0)
    {
        perror("Error reading from socket\n");
        exit(1);
    }
    buffer[ret]='\0';
    strcpy(username,buffer);
    memset(buffer,0,sizeof(buffer));
    ret=read(*newsock,buffer,sizeof(buffer)-1);
    if(ret<0)
    {
        perror("Error reading from socket\n");
        exit(1);
    }
    buffer[ret]='\0';
    strcpy(book_name,buffer);
    FILE *books = fopen("books.txt","r");
    FILE *temp = fopen("temp.txt", "w");
    if(books==NULL || temp==NULL)
    {
        perror("Error opening file!\n");
        exit(EXIT_FAILURE);
    }
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();
    int fd = fileno(books);
    fcntl(fd, F_SETLKW, &lock);


    char temp1[100];
    int temp2;
    int found=0;
    while(fscanf(books,"%s %d",temp1,&temp2)!=EOF)
    {
        if(strcmp(temp1,book_name)==0)
        {
            found=1;
            if(temp2<=0)
            {
                printf("Book not available\n");
                write(*newsock,"Book not available\n",19);
                return 1;
            }
            temp2--;
        }
        fprintf(temp,"%s %d\n",temp1,temp2);
    }
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    fclose(books);
    fclose(temp);

    if(found==1)
    {
        remove("books.txt");
        rename("temp.txt", "books.txt");
    
        FILE *trans = fopen("trans.txt","a");
        if(trans==NULL)
        {
            perror("Error opening file!\n");
            exit(EXIT_FAILURE);
        }
        struct flock lock1;
        lock1.l_type = F_WRLCK;
        lock1.l_whence = SEEK_SET;
        lock1.l_start = 0;
        lock1.l_len = 0;
        lock1.l_pid = getpid();
        int fd1 = fileno(trans);
        fcntl(fd1, F_SETLKW, &lock1);
        fprintf(trans,"%s %s\n",username,book_name);
        lock1.l_type = F_UNLCK;
        fcntl(fd1, F_SETLK, &lock1);
        fclose(trans);
        write(*newsock,"Book borrowed\n",14);
    }
    else
    {
        write(*newsock,"Book not found\n",15);
    }
    return 1;    
}

int ReturnBook(int *newsock,char *buffer,int ret)
{
    char book_name[100];
    char username[100];
    memset(buffer,0,sizeof(buffer));
    ret=read(*newsock,buffer,sizeof(buffer)-1);
    if(ret<0)
    {
        perror("Error reading from socket\n");
        exit(1);
    }
    buffer[ret]='\0';
    strcpy(username,buffer);
    memset(buffer,0,sizeof(buffer));
    ret=read(*newsock,buffer,sizeof(buffer)-1);
    if(ret<0)
    {
        perror("Error reading from socket\n");
        exit(1);
    }
    buffer[ret]='\0';
    strcpy(book_name,buffer);

    FILE *books = fopen("books.txt","r");
    FILE *temp = fopen("temp.txt", "w");
    if(books==NULL || temp==NULL)
    {
        perror("Error opening file!\n");
        exit(EXIT_FAILURE);
    }
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();
    int fd = fileno(books);
    fcntl(fd, F_SETLKW, &lock);
    char temp1[100];
    int temp2;
    int found=0;
    while(fscanf(books,"%s %d",temp1,&temp2)!=EOF)
    {
        if(strcmp(temp1,book_name)==0)
        {
            found=1;
            temp2++;
        }
        fprintf(temp,"%s %d\n",temp1,temp2);
    }
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    fclose(books);
    fclose(temp);

    if(found==1)
    {
        remove("books.txt");
        rename("temp.txt", "books.txt");
    }

    // Open trans.txt and tempfile.txt
    FILE *trans = fopen("trans.txt","r");
    FILE *tempfile = fopen("tempfile.txt","w");

    if(trans==NULL || tempfile==NULL)
    {
        perror("Error opening file!\n");
        exit(EXIT_FAILURE);
    }

    char trans_user[100], trans_book[100];

    // Read trans.txt line by line
    while(fscanf(trans,"%s %s",trans_user,trans_book)!=EOF)
    {
        // If the current line is not the transaction to be deleted, write it to tempfile.txt
        if(!(strcmp(trans_user,username)==0 && strcmp(trans_book,book_name)==0))
        {
            fprintf(tempfile,"%s %s\n",trans_user,trans_book);
        }
    }

    // Close the files
    fclose(trans);
    fclose(tempfile);

    // Delete trans.txt and rename tempfile.txt to trans.txt
    remove("trans.txt");
    rename("tempfile.txt","trans.txt");

    write(*newsock,"Book returned\n",14);
    printf("Book returned\n");

    return 1;
}

int ViewTransactions(int *newsock,char *buffer,int ret)
{
    FILE *trans = fopen("trans.txt","r");
    // show entire trans.txt file like view books
    if(trans==NULL)
    {
        perror("Error opening file!\n");
        exit(EXIT_FAILURE);
    }
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();
    int fd = fileno(trans);
    fcntl(fd, F_SETLKW, &lock);

    char temp2[100];
    char temp1[100];
    while(fscanf(trans,"%s %s",temp1,temp2)!=EOF)
    {
        write(*newsock,temp1,strlen(temp1));
        write(*newsock," - ",3);
        write(*newsock,temp2,strlen(temp2));
        write(*newsock,"\n",1);
    }
    ssize_t write_ret;
    if(feof(trans))
    {
        
        if (write_ret < 0) 
        {
            perror("Error writing to socket");
            exit(1);
        }
        fsync(*newsock);
    }
    write_ret = write(*newsock,"End of file",11);
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    fclose(trans);
    printf("All transactions viewed\n");
    return 1;
    

}

int Register(int *newsock,char *buffer,int ret)
{
    char username[100];
    char password[100];
    memset(buffer,0,sizeof(buffer));
    ret=read(*newsock,buffer,sizeof(buffer)-1);
    if(ret<0)
    {
        perror("Error reading from socket\n");
        exit(1);
    }
    buffer[ret]='\0';
    strcpy(username,buffer);
    memset(buffer,0,sizeof(buffer));
    ret=read(*newsock,buffer,sizeof(buffer)-1);
    if(ret<0)
    {
        perror("Error reading from socket\n");
        exit(1);
    }
    buffer[ret]='\0';
    strcpy(password,buffer);
    FILE *users = fopen("users.txt","a");
    if(users==NULL)
    {
        perror("Error opening file!\n");
        exit(EXIT_FAILURE);
    }
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();
    int fd = fileno(users);
    fcntl(fd, F_SETLKW, &lock);
    //check if the user already exists in members.txt
    FILE *members = fopen("members.txt","r");
    if(members==NULL)
    {
        perror("Error opening file!\n");
        exit(EXIT_FAILURE);
    }
    char temp[100];
    int found=0;
    while(fscanf(members,"%s",temp)!=EOF)
    {
        if(strcmp(temp,username)==0)
        {
            found=1;
            break;
        }
    }
    if(found==1)
    {
        write(*newsock,"User already exists",20);
        return 1;
    }
    fclose(members);
    fprintf(users,"%s %s\n",username,password);
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    fclose(users);
    //add to members.txt
    FILE *members2 = fopen("members.txt","a");
    if(members2==NULL)
    {
        perror("Error opening file!\n");
        exit(EXIT_FAILURE);
    }
    struct flock lock1;
    lock1.l_type = F_WRLCK;
    lock1.l_whence = SEEK_SET;
    lock1.l_start = 0;
    lock1.l_len = 0;
    lock1.l_pid = getpid();
    int fd1 = fileno(members2);

    fcntl(fd1, F_SETLKW, &lock1);
    fprintf(members2,"%s\n",username);
    lock1.l_type = F_UNLCK;
    fcntl(fd1, F_SETLK, &lock1);
    fclose(members2);

    write(*newsock,"Registered",10);
    printf("Registered\n");
    return 1;
}

int Login(int *newsock,char *buffer,int ret)//int *auth)
{
    char username[100];
    char password[100];
    memset(buffer,0,sizeof(buffer));
    ret=read(*newsock,buffer,sizeof(buffer)-1);
    if(ret<0)
    {
        perror("Error reading from socket\n");
        exit(1);
    }
    buffer[ret]='\0';
    strcpy(username,buffer);
    memset(buffer,0,sizeof(buffer));
    ret=read(*newsock,buffer,sizeof(buffer)-1);
    if(ret<0)
    {
        perror("Error reading from socket\n");
        exit(1);
    }
    buffer[ret]='\0';
    strcpy(password,buffer);
    FILE *users = fopen("users.txt", "r");
    if (users == NULL) {
        perror("Error opening file cant open users.txt\n");
        return -1;
    }
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();
    int fd = fileno(users);
    fcntl(fd, F_SETLKW, &lock);
    //go through the file and check if the username and password match
    char line[128];
    char *token;
    char *name;
    char *pass;
    int found = 0;
    while(fgets(line, sizeof(line), users))
    {
        char *space = strchr(line, ' ');
        if (space == NULL) continue; // Skip lines without a space
        *space = '\0'; // Split the line into two strings
        name = line;
        pass = space + 1;

        // Remove trailing newline from password, if present
        size_t len = strlen(pass);
        if (len > 0 && pass[len - 1] == '\n') pass[len - 1] = '\0';

        if (strcmp(name, username) == 0 && strcmp(pass, password) == 0)
        {
            found = 1;
            break;
        }
    }
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    fclose(users);
    if (found)
    {
        write(*newsock,"Login successful",17);
        //*auth=1;
    }
    else
    {
        write(*newsock,"Login failed",13);
    }
    return 1;
    
}

int ForgotPassword(int *newsock,char *buffer,int ret)
{
    //read the username from the client
    char username[100];
    memset(buffer,0,sizeof(buffer));
    ret=read(*newsock,buffer,sizeof(buffer)-1);
    printf("Username read: %s\n",buffer);
    if(ret<0)
    {
        perror("Error reading from socket\n");
        exit(1);
    }
    buffer[ret]='\0';
    strcpy(username,buffer);
    //search for the username in the users.txt file
    FILE *users = fopen("users.txt","r");
    if(users==NULL)
    {
        perror("Error opening file!\n");
        exit(EXIT_FAILURE);
    }
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();
    int fd = fileno(users);
    fcntl(fd, F_SETLKW, &lock);
    char line[128];
    char *token;
    char *name;
    char *pass;
    int found = 0;
    char temp1[100];
    char temp2[100];
    // Add a flag to check if the username was found
    int username_found = 0;
    FILE *tempfile = fopen("tempfile.txt","w");
    if(tempfile==NULL)
    {
        perror("Error opening file!\n");
        exit(EXIT_FAILURE);
    }
    struct flock lock1;
    lock1.l_type = F_WRLCK;
    lock1.l_whence = SEEK_SET;
    lock1.l_start = 0;
    lock1.l_len = 0;
    lock1.l_pid = getpid();
    int fd1 = fileno(tempfile);
    fcntl(fd1, F_SETLKW, &lock1);
    char temp3[100];
    char temp4[100];


    while(fscanf(users,"%s %s",temp1,temp2)!=EOF)
    {
        if(strcmp(temp1,username)==0)
        {
            // Set the flag to 1 if the username was found
            username_found = 1;
            write(*newsock,"Username found",14);
        }
        else
        {
            fprintf(tempfile,"%s %s\n",temp1,temp2);
        }
    }

    // Check if the username was found after reading the file
    if(username_found == 0)
    {
        write(*newsock,"Username not found",18);
        return -1; // Return an error code
    }
    //read the new password from the client
    char new_password[100];
    memset(buffer,0,sizeof(buffer));
    ret=read(*newsock,buffer,sizeof(buffer)-1);
    if(ret<0)
    {
        perror("Error reading from socket\n");
        exit(1);
    }
    buffer[ret]='\0';
    strcpy(new_password,buffer);
    //make new file tempfile.txt and write the new password to it then copy the contents of users.txt to tempfile.txt line by line, skip the line with the username and write the rest of the lines to tempfile.txt
    

    fprintf(tempfile,"%s %s\n",username,new_password);
    
    lock1.l_type = F_UNLCK;
    fcntl(fd1, F_SETLK, &lock1);
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    fclose(users);
    fclose(tempfile);
    remove("users.txt");
    rename("tempfile.txt","users.txt");
    write(*newsock,"Password updated",16);
    return 1;
    
}

int admin(int *newsock,char *buffer,int ret)
{
    printf("Inside admin\n");
    char password[100];
    //int password_length = 5;  // Change this to the actual length of the password
    //int bytes_read = 0;

    int result = read(*newsock, password, sizeof(password) - 1);  
    if (result < 0) 
    {
        perror("Error reading from socket");
        return -1;
    }
    
    
    password[result] = '\0';
    printf("Password: %s\n",password);
    if(strcmp(password,"admin")==0)
    {
        printf("Admin login successful\n");
        write(*newsock,"Login successful",17);
    }
    else
    {
        write(*newsock,"Login failed",13);
    }
    //printf("\n");

    return 1;
}

void *operator(void *arg)
{
    int nsd = *(int*)arg;
    char buffer[1024];
    int ret;
    while(1)
    {
        ret=read(nsd,buffer,sizeof(buffer)-1);
        if(ret<=0) // Change this line
        {
            if (ret < 0) {
                perror("Error reading from socket\n");
            } else {
                printf("Client closed the connection\n");
            }
            close(nsd);
            break;
        }
        buffer[ret]='\0';
        printf("User chose %s\n",buffer);
        if(strcmp(buffer,"admin")==0)
        {
            admin(&nsd,buffer,ret);
        }
        else if(strcmp(buffer,"login")==0)
        {
            Login(&nsd,buffer,ret);
        }
        else if(strcmp(buffer,"register")==0)
        {
            Register(&nsd,buffer,ret);
        }
        else if(strcmp(buffer,"forgot")==0)
        {
            ForgotPassword(&nsd,buffer,ret);
        }
        else if(strcmp(buffer,"A1")==0)
        {
            AddBook(&nsd,buffer,ret);
        }
        else if(strcmp(buffer,"A2")==0)
        {
            SearchBook(&nsd,buffer,ret);
        }
        else if(strcmp(buffer,"A3")==0)
        {
            DeleteBook(&nsd,buffer,ret);
        }
        else if(strcmp(buffer,"A4")==0)
        {
            UpdateStock(&nsd,buffer,ret);
        }
        else if(strcmp(buffer,"A5")==0)
        {
            AdminViewBooks(&nsd,buffer,ret);
        }
        else if(strcmp(buffer,"A6")==0)
        {
            AddMember(&nsd,buffer,ret);
        }
        else if(strcmp(buffer,"A7")==0)
        {
            DeleteMember(&nsd,buffer,ret);
        }
        else if(strcmp(buffer,"A8")==0)
        {
            ViewMembers(&nsd,buffer,ret);
        }
        else if(strcmp(buffer,"A9")==0)
        {
            ViewTransactions(&nsd,buffer,ret);
        }
        else if(strcmp(buffer,"A0")==0)
        {
            close(nsd);
            printf("Client closed the connection\n");
            return NULL;
        }
        else if(strcmp(buffer,"M1")==0)
        {
            SearchBook(&nsd,buffer,ret);
        }
        else if(strcmp(buffer,"M2")==0)
        {
            ViewBooks(&nsd,buffer,ret);
        }
        else if(strcmp(buffer,"M3")==0)
        {
            BorrowBook(&nsd,buffer,ret);
        }
        else if(strcmp(buffer,"M4")==0)
        {
            ReturnBook(&nsd,buffer,ret);
        }
        else if(strcmp(buffer,"M5")==0)
        {
            ForgotPassword(&nsd,buffer,ret);
        }
        else if(strcmp(buffer,"M0")==0)
        {
            close(nsd);
            printf("Client closed the connection\n");
            return NULL;
        }
        else if(strcmp(buffer,"forgot")==0)
        {
            ForgotPassword(&nsd,buffer,ret);
        }
        else if(strcmp(buffer,"M0")==0)
        {
            close(nsd);
        }
        else if(strcmp(buffer,"end")==0)
        {
            printf("Client closed the connection\n");
            close(nsd);
            return NULL;
        }
        else
        {
            printf("Invalid choice\n");
        }
    }
    close(nsd);
    free(arg);
    return NULL;
}

int main()
{
    struct sockaddr_in server, cli,clientconnection; //serv==server, cli==clientconnection
    int sock, client_sock,newsock, c, read_size; //sd==sock and nsd==newsock
    char client_message[2000];
    char buffer[1024];
    int ret;

    printf("Server started\n");

    sock = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&server, sizeof(server));

    if (sock == -1)
    {
        perror("Could not create socket\n");
        exit(1);
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8040);

    if (bind(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("Bind failed\n");
        exit(1);
    }

    if(listen(sock, 50) < 0)
    {
        perror("Listen failed\n");
        exit(1);
    }

    printf("Server listening on port %d\n", ntohs(server.sin_port));
    //---------------------------------------------------------------------------
    while(1)
    {
        int len = sizeof(cli);
        int *nsd = malloc(sizeof(int));
        *nsd = accept(sock, (struct sockaddr*)&cli, &len);//accept connection ;sock is sd
        pthread_t tid;
        pthread_create(&tid, NULL, operator, (void*)nsd);//create a thread for each client
        //pthread_join(tid, NULL);
        //i want to create a thread for each client so that the server can handle multiple clients at the same time
    }
    close(sock);
    return 0;
}

