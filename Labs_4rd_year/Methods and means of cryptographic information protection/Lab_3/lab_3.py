#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
Дифференциальный криптоанализ SPN (9 бит, 3 раунда)
Вариант: S-блок 0->6, 1->3, 2->4, 3->2, 4->5, 5->0, 6->7, 7->1
Входные разности: 000100000, 000000100, 000000001

Программа выводит:
- Подробный анализ S-блока (дифференциальная таблица)
- Для каждой заданной входной разности ΔA:
   * Таблицы пар открытых текстов (X, X') с проверкой условия X' = X ⊕ ΔA
   * Результаты шифрования (Y, Y')
   * Пошаговый криптоанализ: вычисление ΔC, разбиение на S-блоки, перебор подключей
- Итоговый секретный ключ и его проверку
"""

import random
import itertools
import time
from collections import defaultdict

# ------------------------------------------------------------
# 1. Параметры варианта (заданные в лабораторной работе)
# ------------------------------------------------------------
SBOX = [6, 3, 4, 2, 5, 0, 7, 1]   # 0->6, 1->3, 2->4, 3->2, 4->5, 5->0, 6->7, 7->1
DELTA_A_STRINGS = [
    "000100000",   # разность 1
    "000000100",   # разность 2
    "000000001"    # разность 3
]
NUM_PAIRS = 5                     # количество пар для каждой входной разности

# Инициализация генератора случайных чисел системным временем
random.seed(time.time())

# ------------------------------------------------------------
# 2. Базовые функции SPN (9 бит, 3 раунда)
# ------------------------------------------------------------
def int_to_bits_9(x):
    """9-битное число -> список битов (старший бит в индексе 0)"""
    return [(x >> i) & 1 for i in range(8, -1, -1)]

def bits_9_to_int(bits):
    """Список битов (0 - старший) -> число"""
    return sum((bits[i] << (8 - i)) for i in range(9))

def split_to_sboxes(bits_9):
    """Разбивает 9 битов на 3 части по 3 бита, возвращает кортеж (s1, s2, s3) как числа 0..7"""
    s1 = (bits_9[0] << 2) | (bits_9[1] << 1) | bits_9[2]
    s2 = (bits_9[3] << 2) | (bits_9[4] << 1) | bits_9[5]
    s3 = (bits_9[6] << 2) | (bits_9[7] << 1) | bits_9[8]
    return s1, s2, s3

def combine_from_sboxes(s1, s2, s3):
    """Три числа 0..7 -> список из 9 битов"""
    bits = []
    for v in (s1, s2, s3):
        bits.append((v >> 2) & 1)
        bits.append((v >> 1) & 1)
        bits.append(v & 1)
    return bits

def permute(bits_9):
    """Перестановка битов (транспонирование) для SPN"""
    # b0 b3 b6 | b1 b4 b7 | b2 b5 b8
    return [bits_9[0], bits_9[3], bits_9[6],
            bits_9[1], bits_9[4], bits_9[7],
            bits_9[2], bits_9[5], bits_9[8]]

def sbox_apply(val):
    """Применение S-блока к 3-битному значению"""
    return SBOX[val]

def encrypt_block(plain_int, key_int):
    """
    Шифрование одного 9-битного блока на ключе key_int (9 бит).
    Алгоритм: 3 раунда, после каждого раунда XOR с ключом,
    затем S-блоки, затем перестановка (кроме последнего раунда).
    """
    state = plain_int
    # Раунд 1
    state ^= key_int
    bits = int_to_bits_9(state)
    s1, s2, s3 = split_to_sboxes(bits)
    s1, s2, s3 = sbox_apply(s1), sbox_apply(s2), sbox_apply(s3)
    bits = combine_from_sboxes(s1, s2, s3)
    bits = permute(bits)
    state = bits_9_to_int(bits)

    # Раунд 2
    state ^= key_int
    bits = int_to_bits_9(state)
    s1, s2, s3 = split_to_sboxes(bits)
    s1, s2, s3 = sbox_apply(s1), sbox_apply(s2), sbox_apply(s3)
    bits = combine_from_sboxes(s1, s2, s3)
    bits = permute(bits)
    state = bits_9_to_int(bits)

    # Раунд 3 (после S-блоков XOR с ключом, нет перестановки)
    state ^= key_int
    bits = int_to_bits_9(state)
    s1, s2, s3 = split_to_sboxes(bits)
    s1, s2, s3 = sbox_apply(s1), sbox_apply(s2), sbox_apply(s3)
    bits = combine_from_sboxes(s1, s2, s3)
    state = bits_9_to_int(bits)
    state ^= key_int
    return state

# ------------------------------------------------------------
# 3. Дифференциальная таблица S-блока (Δin -> Δout)
# ------------------------------------------------------------
def build_diff_table():
    """
    Строит дифференциальную таблицу S-блока.
    Возвращает:
      possible_out[din] = список возможных Δout
      diff_counts[din][dout] = количество пар (a,b), дающих эту разность
    """
    diff_counts = defaultdict(lambda: defaultdict(int))
    for a in range(8):
        for b in range(8):
            din = a ^ b
            dout = SBOX[a] ^ SBOX[b]
            diff_counts[din][dout] += 1
    possible_out = {din: list(diff_counts[din].keys()) for din in range(8)}
    return possible_out, diff_counts

def print_diff_table(diff_counts):
    """Вывод дифференциальной таблицы в формате, аналогичном таблице 4 лабораторной работы."""
    print("\n" + "="*70)
    print("1. АНАЛИЗ S-БЛОКА ЗАМЕНЫ (построение таблицы зависимости ΔC от ΔA)")
    print("="*70)
    print("Таблица 4. Анализ блока замены")
    print("     ΔC→ ", "  ".join(f"{i:03b}" for i in range(8)))
    for din in range(8):
        row = []
        for dout in range(8):
            cnt = diff_counts[din].get(dout, 0)
            row.append(f"{cnt:2d}" if cnt > 0 else " .")
        print(f"ΔA={din:03b} : ", "  ".join(row))
    print("\n(число = количество пар (a,b), дающих данную Δout; . = 0)\n")

# ------------------------------------------------------------
# 4. Распространение разности через два раунда
# ------------------------------------------------------------
def propagate_delta_through_two_rounds(deltaA_int, possible_out):
    """
    Возвращает множество всех возможных 9-битных разностей
    на входе S-блоков третьего раунда (после двух раундов и перестановки).
    """
    states = {deltaA_int}
    for rnd in range(2):  # два полных раунда
        new_states = set()
        for st in states:
            bits = int_to_bits_9(st)
            s1, s2, s3 = split_to_sboxes(bits)
            # Возможные выходные разности каждого S-блока
            out1 = [0] if s1 == 0 else possible_out.get(s1, [])
            out2 = [0] if s2 == 0 else possible_out.get(s2, [])
            out3 = [0] if s3 == 0 else possible_out.get(s3, [])
            for o1, o2, o3 in itertools.product(out1, out2, out3):
                bits_out = combine_from_sboxes(o1, o2, o3)
                bits_perm = permute(bits_out)
                new_states.add(bits_9_to_int(bits_perm))
        states = new_states
        if not states:
            break
    return states

# ------------------------------------------------------------
# 5. Генерация пар открытых текстов и шифрование (с проверкой условия)
# ------------------------------------------------------------
def generate_and_print_pairs(deltaA_int, secret_key, num_pairs):
    """
    Генерирует num_pairs пар (X, X') с X' = X xor deltaA_int,
    шифрует их на секретном ключе, выводит таблицу и проверку условия.
    Возвращает список кортежей (X, Y, X', Y').
    """
    pairs = []
    print(f"\n--- Анализ для ΔA = {deltaA_int:09b} ---")
    print("Сформированные пары открытых текстов (проверка условия X' = X ⊕ ΔA):")
    print("№ |    X     |    X'    | X ⊕ ΔA | Совпадает?")
    for i in range(num_pairs):
        X = random.randint(0, 511)
        Xp = X ^ deltaA_int
        check = (Xp == (X ^ deltaA_int))
        print(f"{i+1:2} | {X:09b} | {Xp:09b} | {X ^ deltaA_int:09b} | {'Да' if check else 'Нет'}")
        Y = encrypt_block(X, secret_key)
        Yp = encrypt_block(Xp, secret_key)
        pairs.append((X, Y, Xp, Yp))
    print("\nРезультаты шифрования (таблица пар X→Y и X'→Y'):")
    print("№ |    X     |    Y     |    X'    |    Y'")
    for i, (X, Y, Xp, Yp) in enumerate(pairs, 1):
        print(f"{i:2} | {X:09b} | {Y:09b} | {Xp:09b} | {Yp:09b}")
    return pairs

# ------------------------------------------------------------
# 6. Анализ одной пары для заданных возможных входных разностей S-блоков
# ------------------------------------------------------------
def analyze_single_pair(Y, Yp, possible_input_sets, possible_out, verbose=True):
    """
    possible_input_sets = (set31, set32, set33) — множества возможных Δin
    для каждого S-блока третьего раунда (из предсказания).
    Возвращает три множества кандидатов для подключей K1, K2, K3.
    """
    # Выходная разность шифртекстов
    deltaC = Y ^ Yp
    bitsC = int_to_bits_9(deltaC)
    s31_out, s32_out, s33_out = split_to_sboxes(bitsC)

    if verbose:
        print(f"\n  Анализ пары:")
        print(f"    Y  = {Y:09b}")
        print(f"    Y' = {Yp:09b}")
        print(f"    ΔC = Y ⊕ Y' = {deltaC:09b}")
        print(f"    Разбиение ΔC на S-блоки третьего раунда:")
        print(f"      S31: {s31_out:03b}, S32: {s32_out:03b}, S33: {s33_out:03b}")

    # Разбиваем шифртексты на три 3-битовые части
    def get_sbox_part(cipher, idx):
        bits = int_to_bits_9(cipher)
        if idx == 0:
            return (bits[0] << 2) | (bits[1] << 1) | bits[2]
        elif idx == 1:
            return (bits[3] << 2) | (bits[4] << 1) | bits[5]
        else:
            return (bits[6] << 2) | (bits[7] << 1) | bits[8]

    y1, y2, y3 = get_sbox_part(Y, 0), get_sbox_part(Y, 1), get_sbox_part(Y, 2)
    yp1, yp2, yp3 = get_sbox_part(Yp, 0), get_sbox_part(Yp, 1), get_sbox_part(Yp, 2)

    if verbose:
        print(f"    Части шифртекстов:")
        print(f"      Y  = ({y1:03b}, {y2:03b}, {y3:03b})")
        print(f"      Y' = ({yp1:03b}, {yp2:03b}, {yp3:03b})")

    # Функция для одного S-блока
    def candidate_keys(delta_out, possible_din_set, y_val, yp_val, block_name):
        if delta_out == 0:            # нулевая выходная разность -> любой подключ возможен
            if verbose:
                print(f"    {block_name}: выходная разность 0 → любой подключ (0..7) возможен")
            return set(range(8))
        cand = set()
        # Находим все Δin, которые могут дать данную Δout (по таблице)
        possible_din_from_table = [din for din in range(8) if delta_out in possible_out.get(din, [])]
        valid_din = set(possible_din_from_table) & possible_din_set
        if verbose:
            print(f"    {block_name}: выходная разность = {delta_out:03b}")
            print(f"      Возможные Δin по таблице: {[f'{d:03b}' for d in possible_din_from_table]}")
            print(f"      Пересечение с предсказанными {[f'{d:03b}' for d in sorted(possible_din_set)]} → {[f'{d:03b}' for d in sorted(valid_din)]}")
        for din in valid_din:
            for a in range(8):
                ap = a ^ din
                out_a = SBOX[a]
                out_ap = SBOX[ap]
                if (out_a ^ out_ap) != delta_out:
                    continue
                key1 = out_a ^ y_val
                key2 = out_ap ^ yp_val
                if key1 == key2:
                    cand.add(key1)
                    if verbose:
                        print(f"        din={din:03b}, пара (a,ap)=({a:03b},{ap:03b}) → out_a={out_a:03b}, out_ap={out_ap:03b} → ключ = {key1:03b}")
        if verbose:
            print(f"      Кандидаты для {block_name}: {sorted(cand)}")
        return cand

    K1_cand = candidate_keys(s31_out, possible_input_sets[0], y1, yp1, "K1 (S31)")
    K2_cand = candidate_keys(s32_out, possible_input_sets[1], y2, yp2, "K2 (S32)")
    K3_cand = candidate_keys(s33_out, possible_input_sets[2], y3, yp3, "K3 (S33)")
    return K1_cand, K2_cand, K3_cand

# ------------------------------------------------------------
# 7. Основная функция дифференциального анализа
# ------------------------------------------------------------
def differential_attack(verbose=True, fixed_key=None):
    """
    Проводит дифференциальный криптоанализ.
    Если fixed_key задан, использует его (для теста), иначе генерирует случайный.
    Возвращает найденный ключ.
    """
    # ---- Шаг 1. Построение дифференциальной таблицы S-блока ----
    possible_out, diff_counts = build_diff_table()
    if verbose:
        print_diff_table(diff_counts)

    # ---- Шаг 2. Для каждой ΔA определить возможные Δin на S-блоках 3-го раунда ----
    delta_info = {}
    if verbose:
        print("\n" + "="*70)
        print("2. ПРЕДВАРИТЕЛЬНЫЙ АНАЛИЗ: распространение входных разностей через 2 раунда")
        print("="*70)
    for delta_str in DELTA_A_STRINGS:
        delta_int = int(delta_str, 2)
        possible_round3 = propagate_delta_through_two_rounds(delta_int, possible_out)
        # Разбиваем каждую возможную 9-битную разность на три S-блока
        set31, set32, set33 = set(), set(), set()
        for d3 in possible_round3:
            bits = int_to_bits_9(d3)
            s31, s32, s33 = split_to_sboxes(bits)
            set31.add(s31)
            set32.add(s32)
            set33.add(s33)
        delta_info[delta_int] = (set31, set32, set33)
        if verbose:
            print(f"\nΔA = {delta_str} (0x{delta_int:03x})")
            print(f"  Возможные разности на входе 3-го раунда: {[f'{x:09b}' for x in sorted(possible_round3)]}")
            print(f"  Соответствующие входные разности S-блоков:")
            print(f"    S31: {sorted(set31)}")
            print(f"    S32: {sorted(set32)}")
            print(f"    S33: {sorted(set33)}")

    # ---- Шаг 3. Генерация секретного ключа (или использование заданного) ----
    if fixed_key is None:
        secret_key = random.randint(0, 511)
        if verbose:
            print(f"\n*** Секретный ключ (сгенерирован случайно): {secret_key:09b} (дес. {secret_key}) ***")
    else:
        secret_key = fixed_key
        if verbose:
            print(f"\n*** Используем заданный ключ: {secret_key:09b} ***")

    # ---- Шаг 4. Сбор кандидатов на подключи K1, K2, K3 ----
    global_K1 = set(range(8))
    global_K2 = set(range(8))
    global_K3 = set(range(8))

    for delta_int, (set31, set32, set33) in delta_info.items():
        if verbose:
            print(f"\n{'='*70}")
            print(f"3. ДИФФЕРЕНЦИАЛЬНЫЙ АНАЛИЗ ДЛЯ ΔA = {delta_int:09b}")
            print('='*70)
        # Генерация пар и вывод таблицы
        pairs = generate_and_print_pairs(delta_int, secret_key, NUM_PAIRS)

        # Анализ каждой пары
        for idx, (X, Y, Xp, Yp) in enumerate(pairs):
            if verbose:
                print(f"\n--- Анализ пары {idx+1} ---")
            K1_cand, K2_cand, K3_cand = analyze_single_pair(Y, Yp, (set31, set32, set33), possible_out, verbose)
            # Пересекаем с глобальными кандидатами
            old_K1, old_K2, old_K3 = global_K1.copy(), global_K2.copy(), global_K3.copy()
            global_K1 &= K1_cand
            global_K2 &= K2_cand
            global_K3 &= K3_cand
            if verbose:
                print(f"\n  Обновлённые множества кандидатов после пары {idx+1}:")
                print(f"    K1: {sorted(global_K1)} (было {sorted(old_K1)})")
                print(f"    K2: {sorted(global_K2)} (было {sorted(old_K2)})")
                print(f"    K3: {sorted(global_K3)} (было {sorted(old_K3)})")
            # Если все подключи определены однозначно, можно закончить досрочно
            if len(global_K1) == 1 and len(global_K2) == 1 and len(global_K3) == 1:
                if verbose:
                    print("\n*** Все подключи определены однозначно ***")
                break
        # Досрочный выход из цикла по ΔA, если ключ найден
        if len(global_K1) == 1 and len(global_K2) == 1 and len(global_K3) == 1:
            break

    if verbose:
        print("\n" + "="*70)
        print("4. ИТОГОВЫЕ КАНДИДАТЫ ДЛЯ ПОДКЛЮЧЕЙ")
        print("="*70)
        print(f"K1 = {sorted(global_K1)}")
        print(f"K2 = {sorted(global_K2)}")
        print(f"K3 = {sorted(global_K3)}")

    # ---- Шаг 5. Составление полных ключей-кандидатов и проверка ----
    possible_keys = []
    for k1 in global_K1:
        for k2 in global_K2:
            for k3 in global_K3:
                full_key = (k1 << 6) | (k2 << 3) | k3
                possible_keys.append(full_key)

    if verbose:
        print(f"\nВсего возможных ключей: {len(possible_keys)}")
        if len(possible_keys) <= 20:
            for pk in possible_keys:
                print(f"  {pk:09b} (дес. {pk})")
        else:
            for pk in possible_keys[:10]:
                print(f"  {pk:09b} (дес. {pk})")
            print("  ...")

    # Проверка: используем одну случайную пару (X, Y) для верификации
    test_X = random.randint(0, 511)
    true_Y = encrypt_block(test_X, secret_key)
    correct_key = None
    for cand in possible_keys:
        if encrypt_block(test_X, cand) == true_Y:
            correct_key = cand
            if verbose:
                print(f"\n✓ Найден верный ключ: {cand:09b}")
            break
    if correct_key is None and verbose:
        print("\n✗ Не удалось определить ключ среди кандидатов. Попробуйте увеличить NUM_PAIRS.")
    return correct_key

# ------------------------------------------------------------
# 8. Запуск
# ------------------------------------------------------------
if __name__ == "__main__":
    found = differential_attack(verbose=True, fixed_key=None)
    if found:
        print("\n=== ДИФФЕРЕНЦИАЛЬНЫЙ КРИПТОАНАЛИЗ УСПЕШНО ЗАВЕРШЁН ===")
    else:
        print("\n=== АНАЛИЗ НЕ ДАЛ ОДНОЗНАЧНОГО РЕЗУЛЬТАТА ===")