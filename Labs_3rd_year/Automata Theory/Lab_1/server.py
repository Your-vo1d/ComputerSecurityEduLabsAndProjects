from pyftpdlib.authorizers import DummyAuthorizer
from pyftpdlib.handlers import FTPHandler
from pyftpdlib.servers import FTPServer
from ftplib import FTP
from threading import Thread

# Модель конечного автомата для управления состояниями FTP-сервера
class FTPStateMachine:
    def __init__(self):
        self.state = "S0"  # Начальное состояние: Ожидание подключения

    def transition(self, command):
        if self.state == "S0" and command == "USER":
            self.state = "S1"  # Переход в состояние аутентификации
        elif self.state == "S1" and command == "PASS":
            self.state = "S2"  # Переход в состояние ожидания команд
        elif self.state == "S2" and command in ["LIST", "RETR", "STOR"]:
            self.state = "S3"  # Переход в состояние обработки команды
        elif self.state == "S3" and command == "QUIT":
            self.state = "S4"  # Переход в состояние закрытия соединения
        else:
            pass  # Оставляем текущее состояние без изменений

        print(f"Transitioned to state: {self.state}")

# Настройка FTP-сервера с конечным автоматом
class CustomFTPHandler(FTPHandler):
    def on_connect(self):
        print("Client connected")
    
    def on_login(self, username):
        print(f"User {username} logged in")

    def on_disconnect(self):
        print("Client disconnected")

def run_ftp_server():
    # Авторизатор для управления пользователями
    authorizer = DummyAuthorizer()

    # Добавляем обычного пользователя
    authorizer.add_user("user", "12345", ".", perm="elradfmw")  # Права: полный доступ

    # Настройка обработчика для FTP-сессий
    handler = CustomFTPHandler
    handler.authorizer = authorizer

    # Запуск FTP-сервера
    server = FTPServer(("0.0.0.0", 2121), handler)  # Используем порт 2121 для тестов
    server.timeout = 300  # Увеличиваем тайм-аут для стабильности соединения

    print("FTP-сервер запущен на порту 2121...")
    server.serve_forever()

# Программа-тестировщик для проверки FTP-сервера
def test_ftp_server(test_cases):
    ftp = FTP()
    results = []
    commands = []  # Список для команд и их результатов

    try:
        ftp.connect("localhost", 2121)  # Подключаемся к порту 2121
        print("Connected to FTP server")
    except Exception as e:
        print(f"Failed to connect: {e}")
        return

    for command, expected_response in test_cases:
        try:
            commands.append(f"Executing: {command}")  # Добавляем команду в список
            if command.startswith("USER"):
                username = command.split(" ")[1]
                response = ftp.sendcmd(f"USER {username}")
            elif command.startswith("PASS"):
                password = command.split(" ")[1]
                response = ftp.sendcmd(f"PASS {password}")
            elif command.startswith("LIST"):
                response = ftp.retrlines("LIST")
            elif command.startswith("RETR"):
                filename = command.split(" ")[1]
                with open(filename, 'wb') as file:
                    response = ftp.retrbinary(f"RETR {filename}", file.write)
            elif command == "QUIT":
                response = ftp.quit()

            # Добавляем результат выполнения команды в список
            if expected_response in str(response):
                results.append((command, "Passed"))
            else:
                results.append((command, f"Failed - Expected: {expected_response}, Got: {response}"))
        except Exception as e:
            results.append((command, f"Error - {e}"))

    ftp.close()
    return commands, results

# Функция для записи в файлы
def write_to_files(commands, results):
    with open("commands.txt", "w") as command_file:
        for command in commands:
            command_file.write(command + "\n")

    with open("results.txt", "w") as result_file:
        for command, result in results:
            result_file.write(f"Command: {command}, Result: {result}\n")

# Основная функция для запуска FTP-сервера
if __name__ == "__main__":
    # Запускаем сервер в отдельном потоке
    server_thread = Thread(target=run_ftp_server)
    server_thread.daemon = True
    server_thread.start()

    # Пример тестовых случаев
    test_cases = [
        ("USER user", "331"),
        ("PASS 12345", "230"),
        ("LIST", "226"),
        ("QUIT", "221")
    ]

    # Тестируем сервер и получаем список команд и результатов
    commands, results = test_ftp_server(test_cases)

    # Записываем команды и результаты в файлы
    write_to_files(commands, results)

    print("Commands and results have been written to files.")
