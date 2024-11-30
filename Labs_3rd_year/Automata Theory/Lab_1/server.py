from pyftpdlib.authorizers import DummyAuthorizer
from pyftpdlib.handlers import FTPHandler
from pyftpdlib.servers import FTPServer
from ftplib import FTP
from threading import Thread
import os

# Модель конечного автомата для управления состояниями FTP-сервера
class FTPStateMachine:
    def __init__(self):
        self.state = "S0"  # Начальное состояние: Ожидание подключения

    def transition(self, command, response):
        if self.state == "S0": # Начальное состояние
            if command == "USER correct_user":
                self.state = "S1"  # Пользователь найден, запрос пароля
            elif command == "USER error_user":
                self.state = "S2"        
            elif command in ["PASS correct_pass", "PASS error_pass", "RETR correct_file.txt", "RETR error_file.txt", "REIN"]:
                self.state = "S0"  # Сначала требуется ввести логин        
        elif self.state == "S1": # Введен верный логин
            if command == "PASS correct_pass":
                self.state = "S3"  # Успешная аутентификация
            elif command == "PASS error_pass" or command == "REIN":
                self.state = "S0"  # Неверный пароль, сброс состояния
            elif command in ["USER correct", "RETR correct_file.txt", "RETR error_file.txt"]:
                self.state = "S1"  # Если команда USER введена снова в состоянии S1, сброс состояния
            elif command == "USER error_user":
                self.state = "s2"
        elif self.state == "S2": # Введен неверный логин
            if command == "USER correct_user":
                self.state = "S1"  # Пользователь найден, запрос пароля
            elif command in ["USER error_user", "RETR correct_file.txt", "error_file.txt"]:
                self.state = "S2"        
            elif command == "PASS correct_pass" or command == "PASS error_pass" or command == "REIN":
                self.state = "S0"  # Сначала требуется ввести логин
        elif self.state == "S3":
            if command == "USER correct_user":
                self.state = "S1"  # Пользователь найден, запрос пароля
            elif command == "USER error_user":
                self.state = "S2"        
            elif command in ["PASS correct_pass", "PASS error_pass", "RETR correct_file.txt", "RETR error_file.txt"]:
                self.state = "S3"  
            elif command == "REIN":
                self.state = "S0"


# Настройка FTP-сервера с конечным автоматом
class CustomFTPHandler(FTPHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.state_machine = FTPStateMachine()
        self.file_transfer_in_progress = {}  # Словарь для отслеживания состояний команд

    def on_connect(self):
        print("Client connected")

    def on_login(self, username):
        print(f"User {username} logged in")
        self.state_machine.transition(f"USER {username}", "331")

    def on_password(self, password):
        print(f"Password entered: {password}")
        if password == "correct_pass":
            self.state_machine.transition(f"PASS {password}", "230")  # Успешная аутентификация
        else:
            self.state_machine.transition(f"PASS {password}", "530")  # Неверный пароль

    def on_disconnect(self):
        print("Client disconnected")
    
    def on_retr(self, filename):
        """Обработка команды RETR (загрузка файла)"""
        if self.file_transfer_in_progress.get(filename, False):
            self.respond("550 File transfer in progress")  # Предотвращаем повторный запрос
            self.state_machine.transition("RETR", "550")
            return

        try:
            with open(filename, "rb") as file:
                self.respond("150 Opening data connection.")  # Ответ на начало передачи данных
                self.push_file(file)  # Отправка файла
            self.respond("226 Transfer complete.")  # Успешное завершение передачи
            self.state_machine.transition("RETR", "226")
            self.file_transfer_in_progress[filename] = True  # Устанавливаем флаг, что файл передается
        except FileNotFoundError:
            self.respond("550 File not found")  # Ошибка, если файл не найден
            self.state_machine.transition("RETR", "550")

def run_ftp_server():
    # Авторизатор для управления пользователями
    authorizer = DummyAuthorizer()

    # Добавляем обычного пользователя
    authorizer.add_user("correct_user", "correct_pass", ".", perm="elradfmw")  # Права: полный доступ

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
    state_machine = FTPStateMachine()  # Создаем экземпляр конечного автомата
    results = []
    commands = []

    try:
        ftp.connect("localhost", 2121)  # Подключаемся к порту 2121
    except Exception as e:
        return
    for command, expected_response in test_cases:
        try:
            commands.append(f"Executing: {command}")
            if command.startswith("USER"):
                username = command.split(" ")[1]
                response = ftp.sendcmd(f"USER {username}")
            elif command.startswith("PASS"):
                password = command.split(" ")[1]
                try:
                    response = ftp.sendcmd(f"PASS {password}")
                except Exception as temp_e:
                    response = str(temp_e)  # Сохраняем ошибку как ответ
            elif command.startswith("RETR"):
                filename = command.split(" ")[1]
                try:
                    response = ftp.retrbinary(f"RETR {filename}", None)  # Пример команды RETR
                except Exception as temp_e:
                    response = str(temp_e)

            if expected_response in str(response):
                results.append((command, "Passed"))
            else:
                results.append((command, f"Failed - Expected: {expected_response}, Got: {response}"))
        except Exception as e:
            results.append((command, f"Error - {e}"))

    ftp.close()
    return commands, results

def read_test_cases_from_file(file_path):
    test_cases = []
    try:
        with open(file_path, "r") as file:
            for line in file:
                line = line.strip()
                if line and not line.startswith("#"):  # Пропускаем пустые строки и комментарии
                    parts = line.split(",")
                    if len(parts) == 2:
                        command = parts[0].strip()
                        expected_response = parts[1].strip()
                        test_cases.append((command, expected_response))
                    else:
                        print(f"Skipping invalid line: {line}")
    except FileNotFoundError:
        print(f"Test cases file not found: {file_path}")
    except Exception as e:
        print(f"Error reading test cases file: {e}")
    return test_cases

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
    ("USER correct_user", "331"),   # Запрос пароля
    ("PASS correct_pass", "230"),          # Успешный вход
    ("RETR test_file.txt", "226"),    # Файл отсутствует
    ("REIN", "226"),
    ("USER correct_user", "331"),   # Повторное подключение
    ("PASS 12345", "230")           # Успешный вход после QUIT
    ]
    ts = read_test_cases_from_file("test_case.txt")
    # Тестируем сервер и получаем список команд и результатов
    commands, results = test_ftp_server(test_cases)

    # Записываем команды и результаты в файлы
    write_to_files(commands, results)

    print("Commands and results have been written to files.")
    print("fdsfsd", ts)