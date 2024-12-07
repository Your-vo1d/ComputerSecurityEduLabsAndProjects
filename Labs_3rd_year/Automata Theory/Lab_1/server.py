from pyftpdlib.authorizers import DummyAuthorizer
from pyftpdlib.handlers import FTPHandler
from pyftpdlib.servers import FTPServer
from ftplib import FTP
from threading import Thread
import os

# Модель конечного автомата для управления состояниями FTP-сервера


# Настройка FTP-сервера с конечным автоматом
class CustomFTPHandler(FTPHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

    def on_connect(self):
        print("Client connected")

    def on_login(self, username):
        print(f"User {username} logged in")
        if username == "error_user":
            self.respond("331 Invalid user")  # Возвращаем ошибку для неверного пользователя


    def on_password(self, password):
        print(f"Password entered: {password}")

    def on_disconnect(self):
        print("Client disconnected")

    def on_retr(self, filename):
        """Обработка команды RETR (загрузка файла) в бинарном режиме"""
        if filename == "correct_file.txt":
            try:
                with open(filename, "rb") as file:
                    self.respond("150 Opening data connection.")
                    self.push_file(file)  # push_file уже передает данные через FTP
                    self.respond("226 Transfer complete.")  # Успешное завершение передачи
            except FileNotFoundError:
                self.respond("550 File not found")  # Ошибка, если файл не найден
        else:
            self.respond("550 File not found")  # Ответ на ошибку, если файл не существует


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

    print("FTP-сервер запущен на порту 2121...")
    server.serve_forever()


# Программа-тестировщик для проверки FTP-сервера
def test_ftp_server(test_cases):
    ftp = FTP()

    results = []
    test_results = []

    try:
        ftp.connect("localhost", 2121, timeout=300)  # Подключаемся к порту 2121
    except Exception as e:
        return
    for i in range(len(test_cases)):
        count = 0
        print(test_cases[i])
        for j in range(len(test_cases[i])):
            test_case = test_cases[i][j]
            for command, expected_response in test_case.items():
                try:
                    if command.startswith("USER"):
                        username = command.split(" ")[1]
                        response = ftp.sendcmd(f"USER {username}")
                        print("fdsfsdfsdfsfsdfsfdsf")
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
                    elif command.startswith("REIN"):
                        try:
                            response = ftp.sendcmd(f"REIN")
                        except Exception as temp_e:
                            response = str(temp_e)
                        ftp.quit()  # Завершаем сессию
                        ftp.connect("localhost", 2121)  # Переподключаемся
                    if expected_response in str(response):
                        count += 1
                        results.append((command, "Passed"))
                    else:
                        results.append((command, f"Failed - Expected: {expected_response}, Got: {response}"))
                except Exception as e:
                    results.append((command, f"Error - {e}"))

        if count == len(test_cases[i]):
            test_results.append((f"Тест {i} is passed \n"))
        else:
            test_results.append((f"Тест {i} was not passed \n"))
        count = 0
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
                    print(f"Некорректная строка (не найдено двоеточие): {line}. Пропускаем её.")
                    continue
                
                # Разделяем команду и ответы
                commands = parts[0].split(",")  # Команды
                responses = parts[1].split(",")  # Ответы

                # Проверяем, что количество команд и ответов совпадает
                if len(commands) != len(responses):
                    print(f"Ошибка в строке (некорректное количество команд или ответов): {line}. Пропускаем её.")
                    continue
                # Заполняем итоговый список
                for i in range(len(commands)):
                    temp.append({commands[i].strip(): responses[i].strip()})
                result.append(temp)

    except FileNotFoundError:
        print(f"Файл {file_path} не найден.")
    except Exception as e:
        print(f"Ошибка при обработке файла: {e}")

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
    file_path = "test.txt"  # Укажите путь к вашему файлу
    test_cases = read_test_file(file_path)

    ## Тестируем сервер и получаем список команд и результатов
    results, test_results = test_ftp_server(test_cases)
#
    ## Записываем команды и результаты в файлы
    write_to_files(results)
    with open("test_results.txt", "w", encoding="utf-8") as test_result_file:
        for result in test_results:
            test_result_file.write(result)
#
    print("Commands and results have been written to files.")