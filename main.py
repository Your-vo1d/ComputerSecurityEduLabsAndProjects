import heapq

class HuffmanNode:
    def __init__(self, char, frequency):
        # Инициализация узла Хаффмана с символом и частотой
        self.char = char
        self.frequency = frequency
        self.left = None
        self.right = None

    def __lt__(self, other):
        # Метод сравнения для сортировки узлов в куче
        return self.frequency < other.frequency


def build_huffman_tree(text):
    # Построение дерева Хаффмана на основе частот символов в тексте
    frequency_map = {}
    for char in text:
        frequency_map[char] = frequency_map.get(char, 0) + 1

    # Инициализация кучи с узлами Хаффмана
    heap = [HuffmanNode(char, freq) for char, freq in frequency_map.items()]
    heapq.heapify(heap)

    # Слияние узлов до построения полного дерева
    while len(heap) > 1:
        left = heapq.heappop(heap)
        right = heapq.heappop(heap)
        merged = HuffmanNode(None, left.frequency + right.frequency)
        merged.left, merged.right = left, right
        heapq.heappush(heap, merged)

    return heap[0]