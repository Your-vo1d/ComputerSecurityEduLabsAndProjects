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
