# Чек-лист


Автор: Ликонцев Николай


## Задачи
1) Установить вспомогательные ресурсы, требуемые для корректной работы утилиты
2) Установить утилиты
3) Добавить Thread Group 
4) Добавить элементы JMeter
    - Добавить HTTP Request Defaults
    - Добавить HTTP Request
    - Добавить Graph Result
5) Реализовать нагрузочное тестирование

## Задачи 1-2
Перед началом работы требуется установить требуемые ресурсы.

Был скачан бинарный файл «JMeter»с с официального сайта JMeter.
![JMeter page](img/jmeter_page.png)
 
После этого был установлен Java с официального ресурса.
![Installing Java](img/installing_java.png)

После разархивирования архива с JMeter и установки Java, мы запускаем JMeter.
![JMeter interface](img/jmeter_interface.png)

После установки менеджера плагонов, мы устанавливаем нужные плагины.
![Installing plugins](img/installing_plugins.png)

После применении настроек и перезагрузки утилита готова к работе.

## Задача 3 
Для начала добавим новую группу потоков. Для этого 
1)	Кликните правой кнопкой мыши на Test Plan в дереве проекта JMeter.
2)	В контекстном меню выберите: Add → Threads (Users) → Thread Group
После этого мы увидим на экране состояние, показанное на следующем изображении.
![Thread group](img/thread_group.png)
Далее, мы заполняем параметры: 
1.	Number of Threads (users) → 100
2.	Loop Count → 10
3.	Ramp-Up Period (seconds) → 100
![Thread properties](img/thread_properties.png)
## Задача 4
### Добавление HTTP Request Defaults
Теперь нам нужно добавить элемент HTTP Request Defaults. Для этого нужно выполнить следующие действия:
1)	Правой кнопкой мыши кликните на Thread Group в дереве теста.
2)	В контекстном меню выберите: Add → Config Element → HTTP Request Defaults
Теперь добавим адрес сайта с калькуляторами.
![HTPP Request Defaults](img/http_request_defaults.png)

### Добавление HTTP Request
Для добавления семплера
1)	Кликаем правой кнопкой на Thread Group →Add → Sampler → HTTP Request.
2)	Это создаст новый HTTP-запрос в рамках группы потоков.
После этого заполняем параметры для запроса:

Name: Калькулятор НДС

Server Name or IP: IP-сервера

Path: kalkulyator-nds

### Добавление Graph Result
Для отображения результатов нагрузочного теста в виде графика следуем следующее:
1.	Правой кнопкой мыши кликнуть на Test Plan.
2.	В контекстном меню выбрать: Add → Listener → Graph Results

 
## Задача 5
Ниже будут представлены графики, которые были получены в результате тестирования сайта с калькулятором НДС.

Graph Results

Назначение: Общая визуализация производительности системы.
![Graph Results graph](img/graph_results_graph.png)


Active Threads Over Time 

Назначение: Контроль нагрузки, создаваемой виртуальными пользователями.
![Active Threads Over Time](img/active_threads_over_time_graph.png)


Bytes Throughput Over Time

Назначение: Оценка нагрузки на сеть.
![Bytes Throughput Over Time](img/bytes_throughput_over_time_graph.png)


Connect Times Over Time

Назначение: Анализ сетевой задержки.
![Connect Times Over Time](image.png)
 

Hits per Second

Назначение: Интенсивность работы системы.
![Hits per Second](img/hits_per_secons_graph.png)


Response Codes per Second 

Назначение: Выявление ошибок.
![Response Codes per Second ](img/response_codes_per_second_graph.png)
 

Response Latencies Over Time

Назначение: Время до первого байта.
![Response Latencies Over Time](img/response_latencies_over_time_graph.png)


Response Times Over Time

Назначение: Основной показатель производительности.
![Response Times Over Time](img/response_times_over_time_graph.png)


Transactions per Second

Назначение: Эффективность обработки бизнес-логики.
![Transactions per Second](img/transactions_per_second_graph.png)


