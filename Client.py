# Client.py
import socket

server_ip = '192.168.146.248' 
server_port = 25565

socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
socket.connect((server_ip, server_port))


while True:
    msg = input('msg:') 
    socket.sendall(msg.encode(encoding='utf-8'))
    data = socket.recv(1024)
    msg = data.decode() 
    print('echo msg:', msg)
    
    if msg == '/end':
        break

socket.close()