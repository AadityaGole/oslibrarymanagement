Server.c
This is a server-side application written in C for a book borrowing system. It allows clients to connect, borrow, and return books. It also provides functionality to view all books and members.

It has file locking,concurency(hanlding multiple clients), password authentication, modyfing users(forgotpassword), and all library functionalities

***THE LOCK ONLY WORKS FOR VIEWBOOKS AND MEMBERVIEWALLBOOKS***
if you want it to be locked then implement like these functions else where

Functions
BorrowBook(int *newsock,char *buffer,int ret): This function allows a client to borrow a book. It reads the username and book name from the client, checks if the book is available, and if so, decreases the book's quantity by one and logs the transaction.

ReturnBook(int *newsock,char *buffer,int ret): This function allows a client to return a book. It reads the username and book name from the client, increases the book's quantity by one, and removes the transaction from the log.

ViewBooks(int *newsock,char *buffer,int ret): This function allows a client to view all books. It reads the book list from a file and sends it to the client.

ViewMembers(int *newsock,char *buffer,int ret): This function allows a client to view all members. It reads the member list from a file and sends it to the client.

Usage
Compile the server.c file using a C compiler:

gcc server.c -o server

Run the server:

./server

The server will listen for incoming connections. Clients can connect to the server, borrow and return books, and view all books and members.

Note
This server uses file locking to prevent race conditions when multiple clients are trying to borrow or return the same book at the same time. It also uses socket programming to communicate with clients.



Client.c
This is a client-side application written in C for a book borrowing system. It allows users to connect to a server, borrow, and return books. It also provides functionality to view all books and members.

Functions
BorrowBook(int *sock,char *buffer): This function allows a user to borrow a book. It sends the username and book name to the server, and receives a response indicating whether the operation was successful.

ReturnBook(int *sock,char *buffer): This function allows a user to return a book. It sends the username and book name to the server, and receives a response indicating whether the operation was successful.

ViewBooks(int *sock,char *buffer): This function allows a user to view all books. It sends a request to the server and receives a list of all books.

ViewMembers(int *sock,char *buffer): This function allows a user to view all members. It sends a request to the server and receives a list of all members.

Usage
Compile the client.c file using a C compiler:

gcc client.c -o client

Run the client:

./client

The client will connect to the server. Users can borrow and return books, and view all books and members.

Note
This client uses socket programming to communicate with the server. It sends requests to the server and receives responses. The server handles all the operations related to borrowing and returning books, and viewing books and members.

