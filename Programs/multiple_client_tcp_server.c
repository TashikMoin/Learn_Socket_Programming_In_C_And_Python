#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <ctype.h>


int main(int argc, char const *argv[])
{
    int port = atoi(argv[1]);
    /* Describing socket */
    int Server_Socket_File_Descripter = socket(AF_INET , SOCK_STREAM, 0);
    if(!Server_Socket_File_Descripter)
    {
        printf("\nSorry! can't open server socket in server process, Please try again.");
        exit(1);
    }
    /* Creating complete socket of InterNet domain*/
    struct sockaddr_in Server_Address;
    Server_Address.sin_family = AF_INET;
    Server_Address.sin_port = htons(port);
    Server_Address.sin_addr.s_addr = htonl(INADDR_ANY);
    /* Binding */
    if( bind(Server_Socket_File_Descripter, (struct sockaddr*)&Server_Address, sizeof(Server_Address))  <  0)
    {
        printf("\nSocket binding fails, please try again.");
        exit(2); 
    }
    while(1)
    {
        /* Making server ready to listen */
        if( listen(Server_Socket_File_Descripter,1000) ) // returns '0' when there's no problem.
        // Backlog of total 1000 clients. 1000 clients can be connected.
        {
            printf("\nAn error occured during listening, Please try again.");
            exit(3);
        }
        printf("\n... Connection established, the server started listening on port: %d ...", port);
        /* Setting socket options to avoid errors*/
        int Option_Value = 1;
        setsockopt(Server_Socket_File_Descripter, SOL_SOCKET, SO_REUSEADDR, (const void *)&Option_Value, sizeof(int));
        /* Creating client socket address*/
        struct sockaddr_in Client_Address;
        char Buffer[1024]; /* 1 MB limit */
        int Client_Address_Length = 0;
        int Client_File_Descripter = 0;
        int Read_Length = 0;
        /* Accepting client connections */
        Client_Address_Length = sizeof(Client_Address);
        Client_File_Descripter = accept(Server_Socket_File_Descripter, (struct sockaddr*)&Client_Address, &Client_Address_Length);
        if( Client_File_Descripter < 0 )
        {
            printf("\nError in accepting client connection request");
            continue; // To retry
        }
        printf("\nConnected with %s:%d", inet_ntoa(Client_Address.sin_addr), ntohs(Client_Address.sin_port));
        if((fork()) == 0)
        {
            close(Server_Socket_File_Descripter); 
            /* Necessary to close file descripter of server at this point so that other conenctiosn can be connected easily */
            while(1)
            {
                Read_Length = recv(Client_File_Descripter, &Buffer, sizeof(Buffer), 0); 
                if( Read_Length < 0 )
                {
                    printf("\nAn error occured while recieving data from client %d.",getpid());
                    exit(4);
                }
                if( Buffer[0]=='q' && strlen(Buffer)==2 )
                {
                    break;
                }
                printf("\nServer recieved message from client %d --> %s",getpid(),Buffer);
                printf("\nWrite a message for client # %d :", getpid());
                fgets(Buffer,1024,stdin);
                int Sending_Status = send(Client_File_Descripter, &Buffer, sizeof(Buffer), 0);
                if( Sending_Status < 0 )
                {
                    printf("\nAn Error occured while sending data from server to client %d", getpid());
                }
            }
        }
    }
    return 0;
}
