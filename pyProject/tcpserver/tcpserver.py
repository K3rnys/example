import socket
import threading
import hashlib


def CalculateMd5(data):
    md5_hash = hashlib.md5()
    for chunk in data:
        md5_hash.update(chunk)
    return md5_hash.hexdigest()

def CalculateSha256(data):
    sha256_hash = hashlib.sha256()
    for chunk in data:
        sha256_hash.update(chunk)
    return sha256_hash.hexdigest()

def CalculateSha512(data):
    sha512_hash = hashlib.sha512()
    for chunk in data:
        sha512_hash.update(chunk)
    return sha512_hash.hexdigest()

class TcpServer:
    def __init__(self, host, port):
        server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        server_socket.bind((host, port))
        print(f"Сервер запущен на порте {port}")
        server_socket.listen()
        self.server_socket = server_socket
        self.__StartServer()

    def __StartServer(self):
        while True:
            conn, address = self.server_socket.accept()
            print(f"Соединение от {address}")
            client_thread = threading.Thread(target=self.__HandleClient, args=(conn,))
            client_thread.start()

    def __HandleClient(self, conn):
        while True:
            operation = conn.recv(1024).decode()
            if operation.lower() == 'exit':
                print("Завершение сеанса клиента.")
                conn.close()
                break
            if operation:
                print(f"Выбрана операция: {operation}")
                file_size = int(conn.recv(1024).decode())
                print(f"Размер файла: {file_size} байт")
                conn.send(f"Размер файла: {file_size} байт".encode())
                
                def file_data_generator(conn, file_size):
                    received_size = 0
                    while received_size < file_size:
                        chunk = conn.recv(4096)
                        if not chunk:
                            break
                        received_size += len(chunk)
                        yield chunk

                if operation == 'md5':
                    result = CalculateMd5(file_data_generator(conn, file_size))
                elif operation == 'sha256':
                    result = CalculateSha256(file_data_generator(conn, file_size))
                elif operation == 'sha512':
                    result = CalculateSha512(file_data_generator(conn, file_size))
                
                conn.send(result.encode())