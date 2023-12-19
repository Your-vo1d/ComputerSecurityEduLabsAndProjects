import heapq

#Построение дерева из кодов
def build_huffman_tree_from_codes(huffman_codes):
    # Инициализируем корень дерева
    root = {}

    # Проходим по всем символам и их кодам в Huffman-кодах
    for char, code in huffman_codes.items():
        # Начинаем с корня для каждого символа
        node = root
        # Проходим по битам кода символа
        for bit in code:
            # Если бит еще не существует, создаем новый узел
            if bit not in node:
                node[bit] = {}
            # Переходим к следующему узлу
            node = node[bit]

        # Присваиваем символ текущему узлу
        node['char'] = char

    # Возвращаем построенное дерево
    return root


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

        # Запись кодов Хаффмана
        for char, code in codes.items():
            char_bytes = char.encode('utf-8')
            compressed_file.write(len(char_bytes).to_bytes(1, byteorder='big'))
            compressed_file.write(char_bytes)
            compressed_file.write(len(code).to_bytes(1, byteorder='big'))
            compressed_file.write(int(code, 2).to_bytes((len(code) + 7) // 8, byteorder='big'))

        # Запись разделителя между кодами и закодированным текстом
        compressed_file.write(bytes([0xFF]))

        # Запись закодированного текста в файл
        for i in range(0, len(encoded_text), 8):
            byte = encoded_text[i:i+8]
            byte = int(byte, 2)
            compressed_file.write(bytes([byte]))

    # Вывод сообщения об успешном завершении сжатия
    print(f'Сжатие завершено. Результат записан в файл {output_filename}')


def decompress(input_filename, output_filename):
    # Открытие сжатого файла для чтения в бинарном режиме
    with open(input_filename, 'rb') as compressed_file:
        
        # Чтение информации о дополнении из начала файла
        padding = int.from_bytes(compressed_file.read(1), byteorder='big')

        # Чтение кодов Хаффмана
        codes = {}
        while True:
            char_len = int.from_bytes(compressed_file.read(1), byteorder='big')
            if char_len == 0xFF:
                break  # Достигнут разделитель, прекращаем чтение
            char = compressed_file.read(char_len).decode('utf-8')
            code_len = int.from_bytes(compressed_file.read(1), byteorder='big')
            code = bin(int.from_bytes(compressed_file.read((code_len + 7) // 8), byteorder='big'))[2:].rjust(code_len, '0')
            codes[char] = code

        # Чтение закодированного текста из файла
        encoded_text = ''
        while True:
            byte = compressed_file.read(1)
            if not byte:
                break
            byte = bin(int.from_bytes(byte, byteorder='big'))[2:].rjust(8, '0')
            encoded_text += byte

    # Удаление дополнительных нулей, добавленных при сжатии
    encoded_text = encoded_text[:-padding]

    # Декодирование текста с использованием кодов Хаффмана
    decoded_text = ''
    current_code = ''
    for bit in encoded_text:
        current_code += bit
        if current_code in codes.values():
            decoded_text += [char for char, code in codes.items() if code == current_code][0]
            current_code = ''

    # Запись декодированного текста в файл
    with open(output_filename, 'w', encoding='utf-8') as decompressed_file:
        decompressed_file.write(decoded_text)

    # Вывод сообщения об успешном завершении декомпрессии
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
decompress(compressed_file, decompressed_file)