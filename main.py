import heapq

# Построение дерева
def build_huffman_tree(text):
    # Создание словаря частот символов в тексте
    frequency_map = {}
    for char in text:
        frequency_map[char] = frequency_map.get(char, 0) + 1

    # Инициализация кучи с узлами Хаффмана
    heap = [(freq, char, None, None) for char, freq in frequency_map.items()]
    heapq.heapify(heap)

    # Слияние узлов до построения полного дерева
    while len(heap) > 1:
        # Извлечение двух узлов с минимальными частотами
        left = heapq.heappop(heap)
        right = heapq.heappop(heap)
        
        # Объединение узлов в новый узел с суммой частот
        merged = (left[0] + right[0], '', left, right)
        
        # Вставка нового узла в кучу
        heapq.heappush(heap, merged)

    # Возврат корня построенного дерева Хаффмана
    return heap[0] if heap else (0, '', None, None)

# Построение кодов Хаффмана
def build_huffman_codes(node, code="", mapping=None):
    # Рекурсивное построение кодов Хаффмана для каждого символа
    if mapping is None:
        mapping = {}

    if node is not None:
        # Если узел представляет символ, добавляем его код в словарь
        if node[1] != '':
            mapping[node[1]] = code
        
        # Рекурсивный вызов для левого и правого поддеревьев
        build_huffman_codes(node[2], code + "0", mapping)
        build_huffman_codes(node[3], code + "1", mapping)

    return mapping

def compress(text, output_filename):
    # Построение дерева Хаффмана на основе входного текста
    root = build_huffman_tree(text)

    # Построение кодов Хаффмана для каждого символа
    codes = build_huffman_codes(root)

    # Создание закодированного текста на основе полученных кодов
    encoded_text = ''.join(codes[char] for char in text)

    # Вычисление количества бит для дополнения до байта
    padding = 8 - len(encoded_text) % 8

    # Добавление дополнительных нулей для выравнивания по байту, если необходимо
    if padding != 8:
        encoded_text += '0' * padding

    # Запись информации о дополнении в начало сжатого файла
    with open(output_filename, 'wb') as compressed_file:
        compressed_file.write(bytes([padding]))

        # Запись закодированного текста в файл
        for i in range(0, len(encoded_text), 8):
            byte = encoded_text[i:i+8]
            byte = int(byte, 2)
            compressed_file.write(bytes([byte]))

    # Вывод сообщения об успешном завершении сжатия
    print(f'Сжатие завершено. Результат записан в файл {output_filename}')


def decompress(input_filename, output_filename, huffman_tree):
    # Открытие сжатого файла для чтения в бинарном режиме
    with open(input_filename, 'rb') as compressed_file:
        
        # Чтение информации о дополнении из начала файла
        padding = ord(compressed_file.read(1))
        
        # Чтение закодированного текста из файла
        bytes_array = compressed_file.read()

    # Преобразование байт в строку из битов
    encoded_text = ''.join(format(byte, '08b') for byte in bytes_array)

    # Удаление дополнительных бит в конце закодированного текста
    encoded_text = encoded_text[:-padding]

    # Инициализация узла дерева для декодирования
    node = huffman_tree
    current_node = node

    # Раскодирование текста
    decoded_text = ""
    for bit in encoded_text:
        if bit == '0':
            current_node = current_node[2]
        else:
            current_node = current_node[3]

        if current_node[1] != '':
            decoded_text += current_node[1]
            current_node = huffman_tree

    # Запись раскодированного текста в файл
    with open(output_filename, 'w', encoding='utf-8') as decompressed_file:
        decompressed_file.write(decoded_text)

    # Вывод сообщения об успешном завершении распаковки
    print(f'Распаковка завершена. Результат записан в файл {output_filename}')


# Имя входного файла
input_file = 'input.txt'

# Имя файла для сжатого текста
compressed_file = 'compressed.txt'

# Имя файла для распакованного текста
decompressed_file = 'decompressed.txt'

# Чтение текста из входного файла
with open(input_file, 'r', encoding='utf-8') as file:
    text = file.read()

# Построение дерева Хаффмана на основе текста
root = build_huffman_tree(text)

# Сжатие текста и запись результата в файл compressed.txt
compress(text, compressed_file)

# Распаковка сжатого текста и запись результата в файл decompressed.txt
decompress(compressed_file, decompressed_file, root)

