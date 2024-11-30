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
        self.log = []  # Лог переходов состояний

    def transition(self, command, response):
        """Обработка команд и переход между состояниями на основе команды и ответа"""
        self.log.append(f"State: {self.state}, Command: {command}, Response: {response}")
        print(f"Current state: {self.state}, Command: {command}, Response: {response}")

        if self.state == "S0":
            if command == "USER correct_user":
                self.state = "S1"  # Пользователь найден, запрос пароля
            elif command == "USER error_user":
                self.state = "S2"        
            elif command == "PASS correct_pass" or command == "PASS error_pass":
                self.state = "S0"  # Сначала требуется ввести логин

        elif self.state == "S1":
            if command == "PASS correct_pass":
                self.state = "S2"  # Успешная аутентификация
            elif command == "PASS error_pass":
                self.state = "S0"  # Неверный пароль, сброс состояния
            elif command == "USER correct":
                self.state = "S1"  # Если команда USER введена снова в состоянии S1, сброс состояния
            elif command == "USER error_user":
                self.state = "s2"
        elif self.state == "S2":
            if command in ["LIST", "RETR", "STOR", "SIZE"] and "226" in response:
                self.state = "S3"  # Успешное выполнение команды
        elif self.state == "S3":
            if command == "QUIT" and "221" in response:
                self.state = "S4"  # Завершение сеанса
        else:
            self.log.append(f"Invalid transition for Command: {command}")

        self.log.append(f"Transitioned to state: {self.state}")
        print(f"Transitioned to state: {self.state}")

    def save_log(self, filename="state_log.txt"):
        """Сохраняем лог переходов состояний в файл"""
        with open(filename, "w") as file:
            for entry in self.log:
                file.write(entry + "\n")


# Настройка FTP-сервера с конечным автоматом
class CustomFTPHandler(FTPHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.state_machine = FTPStateMachine()
        self.file_transfer_in_progress = {}  # Словарь для отслеживания состояний команд

    def on_connect(self):
        print("Client connected")

    def on_login(self, username):
        if not self.authorizer.has_user(username=username):
            print(f"User {username} logged in with a false username")
            self.state_machine.transition("USER", "530")
        print(f"User {username} logged in")
        self.state_machine.transition("USER", "331")

    def on_password(self, password):
        print(f"Password entered: {password}")
        if password == "correct_pass":
            self.state_machine.transition("PASS", "230")  # Успешная аутентификация
        else:
            self.state_machine.transition("PASS", "530")  # Неверный пароль

    def on_disconnect(self):
        print("Client disconnected")
    
    def on_get(self, filename):
        """Обработка команды GET, которая соответствует RETR для загрузки файла"""
        if self.file_transfer_in_progress.get(filename, False):
            self.respond("550 File transfer in progress")  # Предотвращаем повторный запрос
            self.state_machine.transition("GET", "550")
            return

        print(f"Processing GET command for file: {filename}")
        try:
            with open(filename, "rb") as f:
                self.respond("150 Opening data connection.")  # Ответ на начало передачи данных
                self.push_file(f)  # Отправка файла
            self.respond("226 Transfer complete.")  # Успешное завершение передачи
            self.state_machine.transition("GET", "226")
            self.file_transfer_in_progress[filename] = True  # Устанавливаем флаг, что файл передается
        except FileNotFoundError:
            self.respond("550 File not found")  # Ошибка при отсутствии файла
            self.state_machine.transition("GET", "550")

    def on_size(self, filename):
        """Обработка команды SIZE для получения размера файла"""
        print(f"Processing SIZE command for file: {filename}")
        try:
            # Получаем размер файла
            file_size = os.path.getsize(filename)
            self.respond(f"213 {file_size}")  # Ответ с размером файла
            self.state_machine.transition("SIZE", "213")
        except FileNotFoundError:
            self.respond("550 File not found")  # Ошибка, если файл не найден
            self.state_machine.transition("SIZE", "550")

    def on_retr(self, filename):
        """Обработка команды RETR (загрузка файла)"""
        if self.file_transfer_in_progress.get(filename, False):
            self.respond("550 File transfer in progress")  # Предотвращаем повторный запрос
            self.state_machine.transition("RETR", "550")
            return

        print(f"Processing RETR command for file: {filename}")
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
    state_machine = FTPStateMachine()  # Создаем экземпляр конечного автомата
    results = []
    commands = []

    try:
        ftp.connect("localhost", 2121)  # Подключаемся к порту 2121
        print("Connected to FTP server")
    except Exception as e:
        print(f"Failed to connect: {e}")
        return

    for command, expected_response in test_cases:
        try:
            commands.append(f"Executing: {command}")
            if command.startswith("USER"):
                username = command.split(" ")[1]
                response = ftp.sendcmd(f"USER {username}")
            elif command.startswith("PASS"):
                password = command.split(" ")[1]
                response = ftp.sendcmd(f"PASS {password}")
            elif command.startswith("LIST"):
                response = ftp.retrlines("LIST")
            elif command.startswith("QUIT"):
                response = ftp.quit()
            elif command.startswith("RETR"):
                filename = command.split(" ")[1]
                response = ftp.retrbinary(f"RETR {filename}", None)  # Пример команды RETR
            elif command.startswith("SIZE"):
                filename = command.split(" ")[1]
                response = ftp.sendcmd(f"SIZE {filename}")

            if expected_response in str(response):
                results.append((command, "Passed"))
            else:
                results.append((command, f"Failed - Expected: {expected_response}, Got: {response}"))
        except Exception as e:
            results.append((command, f"Error - {e}"))

    ftp.close()
    state_machine.save_log()  # Сохраняем лог конечного автомата
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
        ("PASS 12345", "230"),      # Успешный пароль
        ("PASS 123451", "230"),      # Успешный пароль
        ("RETR nonexistent.txt", "550"), # Файл не найден
        ("RETR test_file.txt", "550"), # Файл не найден
        ("QUIT", "221")             # Завершение сессии
    ]

    # Тестируем сервер и получаем список команд и результатов
    commands, results = test_ftp_server(test_cases)

    # Записываем команды и результаты в файлы
    write_to_files(commands, results)

    print("Commands and results have been written to files.")
