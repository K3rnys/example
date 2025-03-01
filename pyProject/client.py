from tcpclient import TcpClient

if __name__ == "__main__":
    host = "127.0.0.1"
    port = 65432
    server = TcpClient(host, port)