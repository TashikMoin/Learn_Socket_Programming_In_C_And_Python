import socket
import sys
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect((sys.argv[1], int(sys.argv[2])) )
# connect function take a tuple that contains two values,
#   1. IP address of the server
#   2. Port on which server is running
Send_Message = " "
while Send_Message != 'q':
    Recv_Message = str(client_socket.recv(1024))
    print(Recv_Message)
    Send_Message = input("Enter a Message to send it to the server : ")
    client_socket.send(bytes(Send_Message, "utf-8"))
client_socket.close()