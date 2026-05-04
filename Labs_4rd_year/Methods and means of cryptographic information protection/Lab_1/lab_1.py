#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Реализация атаки «встреча посередине» (Meet-in-the-Middle) 
на шифр Double S-DES (20 бит) с отображением кандидатов на каждом шаге.
"""

import random
from collections import defaultdict
from typing import List, Tuple, Set, Dict

# 1. КОНСТАНТЫ И ТАБЛИЦЫ S-DES
IP  = [2, 6, 3, 1, 4, 8, 5, 7]          # Начальная перестановка
FP  = [4, 1, 3, 5, 7, 2, 8, 6]          # Финальная перестановка
P10 = [3, 5, 2, 7, 4, 10, 1, 9, 8, 6]   # Перестановка 10->10 бит
P8  = [6, 3, 7, 4, 8, 5, 10, 9]         # Перестановка 10->8 бит (подключ)
EP  = [4, 1, 2, 3, 2, 3, 4, 1]          # Расширение 4->8 бит
P4  = [2, 4, 3, 1]                      # Перестановка после S-боксов

S0 = [
    [1, 0, 3, 2], [3, 2, 1, 0],
    [0, 2, 1, 3], [3, 1, 3, 2]
]
S1 = [
    [0, 1, 2, 3], [2, 0, 1, 3],
    [3, 0, 1, 0], [2, 1, 0, 3]
]

# 2. ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ
def int_to_bits(n: int, length: int) -> List[int]:
    return [int(b) for b in f"{n:0{length}b}"]

def bits_to_int(bits: List[int]) -> int:
    return int("".join(map(str, bits)), 2)

def permute(bits: List[int], table: List[int]) -> List[int]:
    return [bits[i - 1] for i in table]

def left_shift(bits: List[int], n: int) -> List[int]:
    return bits[n:] + bits[:n]

def xor_bits(a: List[int], b: List[int]) -> List[int]:
    return [x ^ y for x, y in zip(a, b)]

def apply_sbox(input_bits: List[int], sbox: List[List[int]]) -> List[int]:
    row = (input_bits[0] << 1) | input_bits[3]
    col = (input_bits[1] << 1) | input_bits[2]
    val = sbox[row][col]
    return [(val >> 1) & 1, val & 1]

# 3. ГЕНЕРАЦИЯ ПОДКЛЮЧЕЙ
def generate_subkeys(master_key: int) -> Tuple[int, int]:
    k = permute(int_to_bits(master_key, 10), P10)
    left, right = k[:5], k[5:]
    
    ls1_l, ls1_r = left_shift(left, 1), left_shift(right, 1)
    k1 = permute(ls1_l + ls1_r, P8)
    
    ls2_l, ls2_r = left_shift(ls1_l, 2), left_shift(ls1_r, 2)
    k2 = permute(ls2_l + ls2_r, P8)
    
    return bits_to_int(k1), bits_to_int(k2)

# 4. ЯДРО S-DES
def feistel_round(left: List[int], right: List[int], subkey: int) -> Tuple[List[int], List[int]]:
    expanded = permute(right, EP)
    xored = xor_bits(expanded, int_to_bits(subkey, 8))
    s0_out = apply_sbox(xored[:4], S0)
    s1_out = apply_sbox(xored[4:], S1)
    p4_out = permute(s0_out + s1_out, P4)
    return right, xor_bits(left, p4_out)

def sdes_encrypt(plaintext: int, master_key: int) -> int:
    data = permute(int_to_bits(plaintext, 8), IP)
    k1, k2 = generate_subkeys(master_key)
    L, R = data[:4], data[4:]
    L, R = feistel_round(L, R, k1)
    L, R = feistel_round(L, R, k2)
    return bits_to_int(permute(R + L, FP))  # SWAP перед FP

def sdes_decrypt(ciphertext: int, master_key: int) -> int:
    data = permute(int_to_bits(ciphertext, 8), IP)
    k1, k2 = generate_subkeys(master_key)
    L, R = data[:4], data[4:]
    L, R = feistel_round(L, R, k2)  # Обратный порядок
    L, R = feistel_round(L, R, k1)
    return bits_to_int(permute(R + L, FP))

# 5. DOUBLE S-DES
def double_sdes_encrypt(plaintext: int, k1: int, k2: int) -> int:
    return sdes_encrypt(sdes_encrypt(plaintext, k1), k2)

def double_sdes_decrypt(ciphertext: int, k1: int, k2: int) -> int:
    return sdes_decrypt(sdes_decrypt(ciphertext, k2), k1)

# 6. АТАКА «ВСТРЕЧА ПОСЕРЕДИНЕ»
class MITMAttack:
    def __init__(self):
        self.key_space = 1024  # 2^10

    def find_candidates(self, M: int, C: int) -> Set[Tuple[int, int]]:
        enc_table: Dict[int, List[int]] = defaultdict(list)
        for k1 in range(self.key_space):
            intermediate = sdes_encrypt(M, k1)
            enc_table[intermediate].append(k1)
        
        candidates = set()
        for k2 in range(self.key_space):
            intermediate = sdes_decrypt(C, k2)
            if intermediate in enc_table:
                for k1 in enc_table[intermediate]:
                    candidates.add((k1, k2))
        return candidates

    def refine_candidates(self, candidates: Set[Tuple[int, int]], 
                         M: int, C: int) -> Set[Tuple[int, int]]:
        return {(k1, k2) for k1, k2 in candidates 
                if double_sdes_encrypt(M, k1, k2) == C}

    def run(self, true_k1: int, true_k2: int, max_pairs: int = 10):
        log = []
        pairs_used = []
        
        # Итерация 1
        M = random.randint(0, 255)
        C = double_sdes_encrypt(M, true_k1, true_k2)
        pairs_used.append((M, C))
        candidates = self.find_candidates(M, C)
        log.append({"iter": 1, "M": M, "C": C, "candidates": set(candidates)})
        
        # Итерации 2+
        iteration = 2
        while len(candidates) > 1 and iteration <= max_pairs:
            M_new = random.randint(0, 255)
            C_new = double_sdes_encrypt(M_new, true_k1, true_k2)
            pairs_used.append((M_new, C_new))
            
            candidates = self.refine_candidates(candidates, M_new, C_new)
            log.append({"iter": iteration, "M": M_new, "C": C_new, "candidates": set(candidates)})
            iteration += 1
            
        return candidates, pairs_used, log

# 7. ГЛАВНАЯ ФУНКЦИЯ И ВЫВОД
def main():
    random.seed(5435)  # Воспроизводимость

    # Проверка корректности
    print("Проверка корректности S-DES...")
    assert sdes_decrypt(sdes_encrypt(114, 642), 642) == 114
    print("Эталонный тест пройден\n")

    # Запуск атаки
    true_k1 = random.randint(0, 1023)
    true_k2 = random.randint(0, 1023)
    print(f"ЗАПУСК АТАКИ (секрет: K1={true_k1}, K2={true_k2})\n")
    
    attacker = MITMAttack()
    candidates, pairs_used, log = attacker.run(true_k1, true_k2, max_pairs=10)

    # ДИНАМИКА СОКРАЩЕНИЯ + ВЫВОД КАНДИДАТОВ
    print("ДИНАМИКА СОКРАЩЕНИЯ МНОЖЕСТВА ВОЗМОЖНЫХ ПАР КЛЮЧЕЙ (С ПРИМЕРАМИ КАНДИДАТОВ)")
    print(f"{'Итер.':<6} | {'M':<4} | {'C':<4} | {'Кол-во':>6} | {'Кандидаты (K1, K2)'}")
    print("-" * 95)
    
    for row in log:
        cands = list(row["candidates"])
        count = len(cands)
        # Показываем до 4 кандидатов для читаемости
        samples = cands[:4]
        samples_str = ", ".join(f"({k1:3d},{k2:3d})" for k1, k2 in samples)
        if count > 4:
            samples_str += f", ... и ещё {count-4}"
        elif count == 0:
            samples_str = "нет (ошибка данных)"
            
        print(f"{row['iter']:<6} | {row['M']:<4} | {row['C']:<4} | {count:>6} | {samples_str}")

    # ИСПОЛЬЗОВАННЫЕ ПАРЫ И НАЙДЕННЫЙ КЛЮЧ
    print("ИСПОЛЬЗОВАННЫЕ ПАРЫ (ОТКРЫТЫЙ ТЕКСТ, ШИФРОТЕКСТ)")
    print("НАЙДЕННЫЙ КЛЮЧ")
    
    print(f"\n МНОЖЕСТВО ПАР (M, C), ИСПОЛЬЗОВАННЫХ В АТАКЕ:")
    print(f"   {'№':<3} | {'M (dec)':<8} | {'M (bin)':<10} | {'C (dec)':<8} | {'C (bin)':<10}")
    print(f"   {'─'*3} | {'─'*8} | {'─'*10} | {'─'*8} | {'─'*10}")
    for i, (m, c) in enumerate(pairs_used, 1):
        print(f"   {i:<3} | {m:<8} | {m:08b}     | {c:<8} | {c:08b}")
    
    print(f"\n НАЙДЕННЫЙ КЛЮЧ(И):")
    if len(candidates) == 1:
        k1_f, k2_f = list(candidates)[0]
        print(f" Найдена единственная пара:")
        print(f"      K1 = {k1_f:4d} (0b{k1_f:010b}, 0x{k1_f:03X})")
        print(f"      K2 = {k2_f:4d} (0b{k2_f:010b}, 0x{k2_f:03X})")
        if (k1_f, k2_f) == (true_k1, true_k2):
            print(f"      Статус: ПОЛНОЕ СОВПАДЕНИЕ с истинными ключами ✓")
        else:
            print(f"      Статус: Эквивалентный ключ (коллизия шифра)")
    elif len(candidates) > 1:
        print(f"   Осталось кандидатов: {len(candidates)}")
        print("    Для однозначного восстановления требуется больше пар (M, C)")
    else:
        print(f"   Кандидаты не найдены.")
    
    if len(candidates) == 1:
        k1_f, k2_f = list(candidates)[0]
        print(f"\n ВЕРИФИКАЦИЯ:")
        ok = all(double_sdes_encrypt(m, k1_f, k2_f) == c for m, c in pairs_used)
        print(f"   {'OK' if ok else 'Error'} Все пары успешно расшифровываются найденным ключом.")
    
    print("ПРОГРАММА ЗАВЕРШИЛА РАБОТУ УСПЕШНО")

if __name__ == "__main__":
    main()