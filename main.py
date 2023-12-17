import heapq


def build_huffman_tree(text):
    frequency_map = {}
    for char in text:
        frequency_map[char] = frequency_map.get(char, 0) + 1

    heap = [(freq, char, None, None) for char, freq in frequency_map.items()]
    heapq.heapify(heap)

    while len(heap) > 1:
        left = heapq.heappop(heap)
        right = heapq.heappop(heap)
        merged = (left[0] + right[0], '', left, right)
        heapq.heappush(heap, merged)

    return heap[0] if heap else (0, '', None, None)


def build_huffman_codes(node, code="", mapping=None):
    # Рекурсивное построение кодов Хаффмана для каждого символа
    if mapping is None:
        mapping = {}

    if node is not None:
        if node.char is not None:
            mapping[node.char] = code
        build_huffman_codes(node.left, code + "0", mapping)
        build_huffman_codes(node.right, code + "1", mapping)

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
    with open(input_filename, 'rb') as compressed_file:

        padding = ord(compressed_file.read(1))

        bytes_array = compressed_file.read()

    encoded_text = ''.join(format(byte, '08b') for byte in bytes_array)

    encoded_text = encoded_text[:-padding]

    node = huffman_tree
    current_node = node

    decoded_text = ""
    for bit in encoded_text:
        if bit == '0':
            current_node = current_node[2]
        else:
            current_node = current_node[3]

        if current_node[1] != '':
            decoded_text += current_node[1]
            current_node = huffman_tree

    with open(output_filename, 'w', encoding='utf-8') as decompressed_file:
        decompressed_file.write(decoded_text)

    print(f'Распаковка завершена. Результат записан в файл {output_filename}')


input_file = 'input.txt'
compressed_file = 'compressed.txt'
decompressed_file = 'decompressed.txt'

with open(input_file, 'r', encoding='utf-8') as file:
    text = file.read()

root = build_huffman_tree(text)
compress(text, compressed_file)
decompress(compressed_file, decompressed_file, root)
