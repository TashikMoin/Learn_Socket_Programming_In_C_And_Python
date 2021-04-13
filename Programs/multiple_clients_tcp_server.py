import socket
import sys
import os
# defining the socket for server 
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# socket function takes two arguments
# 1. socket family    (AF_INTET for IPV4, AF_INET6 for IPV6 etc)
# 2. socket type      (SOCK_STREAM for TCP socket, SOCK_DGRAM for UDP socket etc) 

# binding socket
server_socket.bind(('127.0.0.1', int(sys.argv[1]) ))
# socket bind function takes a tuple of two values,
# 1. Any Specific IP Address / Hostname       Hostname is also an IPV4 of current machine
# 2. Port number on which server will listen the requests.
# Example,
#   socket.bind('127.0.0.1', 80)
# gethostname() returns the IP address of current local machine and hosts the server on that IP address of current machine.


server_socket.listen(10)
# listen function starts listening on defined socket and it takes an integer value that represents how much
# clients can be placed inside a queue to connect with the server. It does not mean that the clients will be 
# connected simultaneously, but they will be connected one after another.

print("Server started listening on port # " + str(sys.argv[1]) )
while True:
    client_socket, address = server_socket.accept()
    # accept() blocks and waits for an incoming connection. When a client connects, it returns a new socket object 
    # representing the connection and a tuple holding the address of the client. The tuple will contain (host, port) 
    # for IPv4 connections or (host, port, flowinfo, scopeid) for IPv6.
    # The accept function returns two things,
    #   1. The socket object of the client that was connected that contains,
    #       example,
    #           <socket.socket fd=4, family=AddressFamily.AF_INET, type=SocketKind.SOCK_STREAM, proto=0, laddr=('127.0.0.1', 8888), raddr=('127.0.0.1', 46802)>
    #   2. A tuple having ip address of client and port(process id) of client process
    #        example --> ('127.0.0.1', 46802)
    if os.fork() == 0 :
        server_socket.close() # so that server socket can get ready to accept new clients simultaneously
        print("Connected with client # ", address)
        print("Enter 'q' to terminate connection")
        Send_Message = " "
        while Send_Message != 'q':
            Send_Message = input("Enter a Message to send it to the client : ")
            client_socket.send(bytes(Send_Message, "utf-8"))
            Recv_Message = str(client_socket.recv(1024))
            print(Recv_Message)
        client_socket.close()
    # send function can takes bytes as an argument to send them to the client and we also specify the format of byte 
    # that is utf-8. If we send a string object directly in send function then we will get a 'b' before string in our
    # output that represent bytes. example --> b'Hello World"
    # close() function closes the connection.