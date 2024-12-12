from pyftpdlib.authorizers import DummyAuthorizer
from pyftpdlib.handlers import FTPHandler
from pyftpdlib.servers import FTPServer
from ftplib import FTP
from threading import Thread
import os
import logging

# Настройка логирования
logging.basicConfig(level=logging.DEBUG, format="%(asctime)s - %(levelname)s - %(message)s")
logger = logging.getLogger(__name__)

# Настройка FTP-сервера с конечным автоматом
class CustomFTPHandler(FTPHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.permit_foreign_addresses = True
        self.enable_size = True

    def ftp_USER(self, username):
        logger.debug(f"User {username} logged in")
        super().ftp_USER(username)

    def ftp_PASS(self, password):
        logger.debug(f"Password entered: {password}")
        if not self.authenticated and not self.username:
            self.respond("503 Login with USER first")
        elif self.authenticated:
            self.respond("503 Already logged")
        elif self.username == "error_user":
            super().ftp_PASS(password)
        elif self.username == "correct_user":
            if password == "correct_pass":
                super().ftp_PASS(password)
            else:
                self.respond("530 Error password")
                self.username = "correct_user"
    def on_disconnect(self):
        logger.debug("Client disconnected")
        super().on_disconnect()
    def on_connect(self):
        logger.debug("Client connected")
        super().on_connect()
    def ftp_SIZE(self, filename):
        if os.path.exists(filename):
            file_size = os.path.getsize(filename)
            self.respond(f"213 {file_size}")  # Код ответа 213: размер файла
        else:
            self.respond("550 No file")  # Ответ на ошибку, если файл не существует
    def ftp_REIN(self, line):
        super().ftp_REIN(line)
    def ftp_QUIT(self,line):
        super().ftp_QUIT(line)
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
    server.timeout = 3000  # Увеличиваем тайм-аут для стабильности соединения

    logger.info("FTP-сервер запущен на порту 2121...")
    server.serve_forever()

# Программа-тестировщик для проверки FTP-сервера
def test_ftp_server(test_cases):
    ftp = FTP()

    results = []
    test_results = []

    try:
        ftp.connect("localhost", 2121, timeout=300)  # Подключаемся к порту 2121
    except Exception as e:
        logger.error(f"Connection failed: {e}")
        return
    for i in range(len(test_cases)):
        count = 0
        logger.info(f"Running test case {i+1}")
        for j in range(len(test_cases[i])):
            test_case = test_cases[i][j]
            for command, expected_response in test_case.items():
                try:
                    if command.startswith("USER"):
                        # Обработка команды USER
                        username = command.split(" ")[1]
                        response = ftp.sendcmd(f"USER {username}")
                        logger.debug("USER command executed")
                    elif command.startswith("PASS"):
                        # Обработка команды PASS
                        password = command.split(" ")[1]
                        try:
                            response = ftp.sendcmd(f"PASS {password}")
                        except Exception as temp_e:
                            response = str(temp_e)  # Сохраняем ошибку как ответ
                    elif command.startswith("RETR"):
                        # Обработка команды RETR
                        filename = command.split(" ")[1]
                        try:
                            response = ftp.retrbinary(f"RETR {filename}", None)  # Пример команды RETR
                        except Exception as temp_e:
                            response = str(temp_e)
                    elif command.startswith("REIN"):
                        # Обработка команды REIN
                        try:
                            response = ftp.sendcmd(f"REIN")
                        except Exception as temp_e:
                            response = str(temp_e)
                    elif command.startswith("SIZE"):
                        # Обработка команды SIZE
                        try:
                            ftp.sendcmd("TYPE I")
                            filename = command.split(" ")[1]
                            logger.debug(f"Requesting SIZE for file: {filename}")
                            response = ftp.sendcmd(f"SIZE {filename}")  # Отправляем команду SIZE
                        except Exception as temp_e:
                            response = str(temp_e)
                    if expected_response in str(response):
                        count += 1
                        results.append((command, "Passed"))
                    else:
                        results.append((command, f"Failed - Expected: {expected_response}, Got: {response}"))
                except Exception as e:
                    results.append((command, f"Error - {e}"))

        if count == len(test_cases[i]):
            test_results.append(f"Test {i} passed\n")
        else:
            test_results.append(f"Test {i} failed\n")
        count = 0
        ftp.quit()  # Завершаем сессию
        ftp.connect("localhost", 2121)  # Переподключаемся
    ftp.close()
    return results, test_results

def read_test_file(file_path):
    """
    Читает файл теста и извлекает команды и их ответы.
    
    :param file_path: Путь к файлу с тестовыми данными.
    :return: Итоговый список с парами (команда:ответ).
    """
    result = []  # Итоговый список

    try:
        with open(file_path, 'r') as file:
            for line in file:
                temp = []
                line = line.strip()

                # Пропускаем пустые строки
                if not line:
                    continue

                # Разделяем строку по двоеточию на команду и ответы
                parts = line.split(":")
                if len(parts) != 2:
                    logger.warning(f"Некорректная строка (не найдено двоеточие): {line}. Пропускаем её.")
                    continue
                
                # Разделяем команду и ответы
                commands = parts[0].split(",")  # Команды
                responses = parts[1].split(",")  # Ответы

                # Проверяем, что количество команд и ответов совпадает
                if len(commands) != len(responses):
                    logger.warning(f"Ошибка в строке (некорректное количество команд или ответов): {line}. Пропускаем её.")
                    continue
                # Заполняем итоговый список
                for i in range(len(commands)):
                    temp.append({commands[i].strip(): responses[i].strip()})
                result.append(temp)

    except FileNotFoundError:
        logger.error(f"Файл {file_path} не найден.")
    except Exception as e:
        logger.error(f"Ошибка при обработке файла: {e}")

    return result

# Функция для записи в файлы
def write_to_files(results):
    with open("results.txt", "w", encoding="utf-8") as result_file:
        for item in results:
            if isinstance(item, tuple) and len(item) == 2:
                command, result = item
                result_file.write(f"Command: {command}, Result: {result}\n")
            else:
                result_file.write("\n")  # Обработка "плохих" элементов

# Основная функция для запуска FTP-сервера
if __name__ == "__main__":
    # Запускаем сервер в отдельном потоке
    server_thread = Thread(target=run_ftp_server)
    server_thread.daemon = True
    server_thread.start()
    file_path = "w-method.txt"  # Укажите путь к вашему файлу
    test_cases = read_test_file(file_path)

    # Тестируем сервер и получаем список команд и результатов
    results, test_results = test_ftp_server(test_cases)

    # Записываем команды и результаты в файлы
    write_to_files(results)
    with open("test_results.txt", "w", encoding="utf-8") as test_result_file:
        for result in test_results:
            test_result_file.write(result)

    logger.info("Commands and results have been written to files.")
