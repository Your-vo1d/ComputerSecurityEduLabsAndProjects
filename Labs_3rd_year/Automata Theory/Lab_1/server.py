from pyftpdlib.authorizers import DummyAuthorizer
from pyftpdlib.handlers import FTPHandler
from pyftpdlib.servers import FTPServer
import os

def start_server():
    # Настройка авторизации
    authorizer = DummyAuthorizer()
    # Настройка пользователя
    authorizer.add_user("user", "password", os.path.join(os.getcwd(), "ftp_files"), perm="elradfmw")
    authorizer.add_anonymous(os.getcwd(), "ftp_files")

    # Настройка обработчика FTP
    handler = FTPHandler
    handler.authorizer - authorizer

    # Создание и запуск сервера
    server = FTPServer(("0.0.0.0", 21), handler)
    print("FTP server started on port 21")
    server.serve_forever()


if __name__ == "__main__":
    # Создание директории для хранения файлов, если она не существует
    if not os.path.exists("ftp_files"):
        os.makedirs("ftp_files")
    start_server()