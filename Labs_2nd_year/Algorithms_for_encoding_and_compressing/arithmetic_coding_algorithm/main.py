from collections import Counter
import os

# Writing the specified bit and previously deferred bits
def bits_plus_follow(bits_to_write, bits_F, bit):
    bits_to_write.append(bit)
    while bits_F > 0:
        bits_to_write.append(1 - bit)
        bits_F -= 1

# Encoding the file
def encode(input_file, encode_file):

    # Reading text from the file
    with open(input_file, "r", encoding="utf-8") as file:
        data = file.read()

    # Counting character frequencies
    freq = dict(Counter(data))
    # Sorting the character frequency dictionary
    sorted_freq = dict(sorted(freq.items(), key=lambda x: (x[1], x[0]), reverse=True))
    # Creating a table of character ranges
    low = 0  # Lower bound
    table_ranges = {}  # Dictionary to store character and its range
    for key, value in sorted_freq.items():
        high = low + value  # Calculating upper bound
        table_ranges[key] = (low, high)  # Adding to dictionary
        low = high  # Updating lower bound
    # Variables for low and high ranges
    low = 0
    high = 65535

    # Variables for storing one quarter, half, and three quarters
    qtr_1 = int((high + 1) / 4)  # First quarter
    half = qtr_1 * 2  # Half
    qtr_3 = qtr_1 * 3  # Third quarter

    # Additional variables
    result = []  # List with result bits
    delitel = list(table_ranges.values())[-1][1]  # Divider
    bits_F = 0  # Number of bits to write

    # Iterating through the text
    for symbol in data:
        range = high - low + 1  # Temporary variable
        # Updating the upper bound
        high = int(low + table_ranges[symbol][1] * range / delitel - 1)
        # Updating the lower bound
        low = int(low + table_ranges[symbol][0] * range / delitel)
        # Normalization
        while True:
            if high < half:
                bits_plus_follow(result, bits_F, 0)
            elif low >= half:
                bits_plus_follow(result, bits_F, 1)
                low = low - half
                high = high - half
            elif low >= qtr_3 and high < qtr_1:
                high = high - qtr_1
                low = low - qtr_1
                bits_F += 1
            else:
                break
            low += low
            high += high + 1

    bits_F += 1
    if low < qtr_1:
        bits_plus_follow(result, bits_F, 0)
    else:
        bits_plus_follow(result, bits_F, 1)

    count_chars = len(data)  # Counting the number of elements in the text
    count_chars = count_chars.to_bytes(2, byteorder="big")
    # Writing to file
    st = "".join(map(str, result))
    st = int(st, 2)
    st = st.to_bytes((st.bit_length() + 7) // 8, "big")
    with open(encode_file, "wb") as file:

        # Writing the number of characters in the text
        file.write(count_chars)
        # Writing the frequency dictionary to the table
        for char, code in sorted_freq.items():
            char_bytes = char.encode("utf-8")
            file.write(len(char_bytes).to_bytes(1, byteorder="big"))
            file.write(char_bytes)
            file.write(
                code.to_bytes(4, byteorder="big")
            )  # Writing the character frequency in 4 bytes

        # Writing the delimiter
        file.write(bytes([0xFF]))
        file.write(len(result).to_bytes(2, byteorder="big"))
        # Writing the result to file
        file.write(st)

# Decoding the file
def decode(encode_file, decode_file):
    # Reading the file and parsing it
    with open(encode_file, "rb") as file:
        # Reading the number of characters in the text
        count_chars = int.from_bytes(file.read(2), byteorder="big")
        # Reading the frequency dictionary from the file
        sorted_freq = {}
        while True:
            char_len = int.from_bytes(file.read(1), byteorder="big")
            if char_len == 0xFF:  # Checking for the delimiter
                break
            char = file.read(char_len).decode("utf-8")
            code = int.from_bytes(file.read(4), byteorder="big")
            sorted_freq[char] = code

        # Reading the result from the file
        len_number_list = file.read(2)
        len_number_list = int.from_bytes(len_number_list, byteorder="big")
        number_list = file.read()
        number_list = int.from_bytes(number_list, byteorder="big")
        number_list = [int(bit) for bit in bin(number_list)[2:]]
        while len(number_list) < len_number_list:
            number_list.insert(0, 0)

    # Creating a table of character ranges
    low = 0  # Lower bound
    table_ranges = {}  # Dictionary to store character and its range
    for key, value in sorted_freq.items():
        high = low + value  # Calculating upper bound
        table_ranges[key] = (low, high)  # Adding to dictionary
        low = high  # Updating lower bound

    # Variables for low and high ranges
    low = 0
    high = 65535

    delitel = list(table_ranges.values())[-1][1]

    # Variables for storing one quarter, half, and three quarters
    qtr_1 = int((high + 1) / 4)
    half = qtr_1 * 2
    qtr_3 = qtr_1 * 3

    keys = list(table_ranges.keys())
    keys.insert(0, "-")
    res = list()
    items = [value[0] for value in table_ranges.values()]
    items.append(delitel)
    value_list = number_list[:16]
    number_list = number_list[16:]
    value_string = "".join(map(str, value_list))  # Converting to string
    result = int(value_string, 2)  # Converting to decimal
    k = 0

    for i in range(count_chars):
        freq = int(((result - low + 1) * delitel - 1) / (high - low + 1))
        j = 1
        while items[j] <= freq:
            j += 1
        res.append(keys[j])

        range_temp = high - low + 1  # Temporary variable
        high = int(low + (items[j] * range_temp) / delitel - 1)

        low = int(low + (items[j - 1] * range_temp) / delitel)

        while True:
            if high < half:
                pass
            elif low >= half:
                result = result - half
                low = low - half
                high = high - half
            elif low >= qtr_3 and high < qtr_1:
                high = high - qtr_1
                low = low - qtr_1
                result = result - qtr_1
            else:
                break
            high += high + 1
            low += low
            result += result
            if k < len(number_list):
                result += number_list[k]
            k += 1
    res = "".join(res)
    with open(decode_file, "w", encoding="utf-8") as file:
        file.write(res)

if __name__ == "__main__":
    input_file = input("Enter the name of the file to encode: ")
    if not os.path.exists(input_file):
        print(f"The file '{input_file}' does not exist in the current directory.")
        exit()
        # Create a directory named "result" if it doesn't exist
    if not os.path.exists("result"):
        os.makedirs("result")
    encode_file = os.path.join("result", "encode.txt")
    decode_file = os.path.join("result", "decode.txt")
    encode(input_file, encode_file)
    decode(encode_file, decode_file)
    print("Encoding and decoding completed successfully.")
