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
    if( argc < 3 )
    {
        printf("\nUse this format when running client --> <Execuatable> <IP/Hostname> <Port>");
        exit(1);
    }
    char hostname[100] ;
    strcpy(hostname,argv[1]);
    int port = atoi(argv[2]);
    /* Describing socket */
    int Client_Socket_File_Descripter = socket(AF_INET , SOCK_STREAM, 0);
    if(!Client_Socket_File_Descripter)
    {
        printf("\nSorry! can't open server socket in client process, Please try again.");
        exit(2);
    }
    /* Creating complete socket of InterNet domain*/
    struct hostent *Hostent_Object = gethostbyname(hostname);
    struct sockaddr_in Server_Address;
    bzero((char*)&Server_Address, sizeof(Server_Address));
    Server_Address.sin_family = AF_INET;
    bcopy((char*)Hostent_Object->h_addr, (char*)&Server_Address.sin_addr.s_addr, Hostent_Object->h_length);
    Server_Address.sin_port = htons(port);
    /* Connection */
    if (connect(Client_Socket_File_Descripter, (struct sockaddr *)&Server_Address, sizeof(Server_Address)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    printf("\nEnter 'q' to close the connection");
    /* Creating client socket address*/
    char Buffer[1024]; /* 1 MB limit */
    int Buffer_Length = 0;
    while(1)
    {
        printf("\nEnter a message to send it to the server:");
        fgets(Buffer,1024,stdin);
        if ( Buffer[0]=='q' && strlen(Buffer)==2 )
        {
            send(Client_Socket_File_Descripter, &Buffer, sizeof(Buffer), 0);
            break;
        }
        if( send(Client_Socket_File_Descripter, &Buffer, sizeof(Buffer), 0) < 0 )
        {
            printf("\nSorry! can't send data to server");
            exit(4);
        }
        if( (Buffer_Length = recv(Client_Socket_File_Descripter, &Buffer, sizeof(Buffer), 0)) < 0)
        {
            printf("\nError while recieving data from server.");
            exit(5);
        }
        printf("\nData recieved from server --> %s", Buffer);
    }
    close(Client_Socket_File_Descripter);
    return 0;
}