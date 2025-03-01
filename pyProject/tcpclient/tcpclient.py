import socket
import os

class TcpClient:
    def __init__(self, host, port):
        self.client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.client_socket.connect((host, port))
        self.__Menu()

    def __Menu(self):
        while True:
            print("\nМеню:")
            print("1. Отправить файл для MD5")
            print("2. Отправить файл для SHA-256")
            print("3. Отправить файл для SHA-512")
            print("4. Выйти")
            choice = input("Выберите опцию: ")

            if choice == '1':
                self.__DoWrite('md5')
            elif choice == '2':
                self.__DoWrite('sha256')
            elif choice == '3':
                self.__DoWrite('sha512')
            elif choice == '4':
                self.client_socket.send('exit'.encode())
                print("Завершение сеанса.")
                self.client_socket.close()
                break
            else:
                print("Неверный выбор. Пожалуйста, выберите 1, 2, 3 или 4.")

    def __DoWrite(self, algorithm):
        file_path = input("Введите путь к файлу: ")
        if not os.path.isfile(file_path):
            print("Файл не найден. Попробуйте снова.")
            return

        with open(file_path, "rb") as f:
            file_data = f.read()

        self.client_socket.send(algorithm.encode())
        self.client_socket.send(str(len(file_data)).encode())
        ack = self.client_socket.recv(1024).decode()
        print(ack) 
        self.client_socket.send(file_data)

        result = self.client_socket.recv(1024).decode()
        print(f"Результат {algorithm}: {result}")