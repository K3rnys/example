from tcpserver import TcpServer

if __name__ == "__main__":
    host = "127.0.0.1"
    port = 65432
    server = TcpServer(host, port)