import os
import heapq

# Building a tree from codes
def build_huffman_tree_from_codes(huffman_codes):
    # Initialize the root of the tree
    root = {}

    # Traverse through all characters and their codes in Huffman codes
    for char, code in huffman_codes.items():
        # Start from the root for each character
        node = root
        # Traverse through the bits of the character's code
        for bit in code:
            # Create a new node if the bit does not exist yet
            if bit not in node:
                node[bit] = {}
            # Move to the next node
            node = node[bit]

        # Assign the character to the current node
        node["char"] = char

    # Return the constructed tree
    return root


# Building a tree
def build_huffman_tree(text):
    # Create a dictionary of character frequencies in the text
    frequency_map = {}
    for char in text:
        frequency_map[char] = frequency_map.get(char, 0) + 1

    # Initialize a heap with Huffman nodes
    heap = [(freq, char, None, None) for char, freq in frequency_map.items()]
    heapq.heapify(heap)

    # Merge nodes until a full tree is constructed
    while len(heap) > 1:
        # Extract two nodes with the minimum frequencies
        left = heapq.heappop(heap)
        right = heapq.heappop(heap)

        # Merge nodes into a new node with the sum of frequencies
        merged = (left[0] + right[0], "", left, right)

        # Insert the new node into the heap
        heapq.heappush(heap, merged)

    # Return the root of the constructed Huffman tree
    return heap[0] if heap else (0, "", None, None)


# Building Huffman codes
def build_huffman_codes(node, code="", mapping=None):
    # Recursive construction of Huffman codes for each character
    if mapping is None:
        mapping = {}

    if node is not None:
        # If the node represents a character, add its code to the dictionary
        if node[1] != "":
            mapping[node[1]] = code

        # Recursive call for the left and right subtrees
        build_huffman_codes(node[2], code + "0", mapping)
        build_huffman_codes(node[3], code + "1", mapping)

    return mapping


def compress(text, output_filename):
    # Build the Huffman tree based on the input text
    root = build_huffman_tree(text)

    # Build Huffman codes for each character
    codes = build_huffman_codes(root)

    # Create encoded text based on the obtained codes
    encoded_text = "".join(codes[char] for char in text)

    # Calculate the number of bits for padding to a byte
    padding = 8 - len(encoded_text) % 8

    # Add extra zeros for byte alignment if necessary
    if padding != 8:
        encoded_text += "0" * padding

    # Write padding information to the beginning of the compressed file
    with open(output_filename, "wb") as compressed_file:
        compressed_file.write(bytes([padding]))

        # Write Huffman codes
        for char, code in codes.items():
            char_bytes = char.encode("utf-8")
            compressed_file.write(len(char_bytes).to_bytes(1, byteorder="big"))
            compressed_file.write(char_bytes)
            compressed_file.write(len(code).to_bytes(1, byteorder="big"))
            compressed_file.write(
                int(code, 2).to_bytes((len(code) + 7) // 8, byteorder="big")
            )

        # Write a delimiter between codes and encoded text
        compressed_file.write(bytes([0xFF]))

        # Write the encoded text to the file
        for i in range(0, len(encoded_text), 8):
            byte = encoded_text[i : i + 8]
            byte = int(byte, 2)
            compressed_file.write(bytes([byte]))

    # Print a message upon successful compression
    print(f"Compression completed. Result is written to the file {output_filename}")


def decompress(input_filename, output_filename):
    # Open the compressed file for reading in binary mode
    with open(input_filename, "rb") as compressed_file:

        # Read padding information from the beginning of the file
        padding = int.from_bytes(compressed_file.read(1), byteorder="big")

        # Read Huffman codes
        codes = {}
        while True:
            char_len = int.from_bytes(compressed_file.read(1), byteorder="big")
            if char_len == 0xFF:
                break  # Reached the delimiter, stop reading
            char = compressed_file.read(char_len).decode("utf-8")
            code_len = int.from_bytes(compressed_file.read(1), byteorder="big")
            code = bin(
                int.from_bytes(
                    compressed_file.read((code_len + 7) // 8), byteorder="big"
                )
            )[2:].rjust(code_len, "0")
            codes[char] = code

        # Read the encoded text from the file
        encoded_text = ""
        while True:
            byte = compressed_file.read(1)
            if not byte:
                break
            byte = bin(int.from_bytes(byte, byteorder="big"))[2:].rjust(8, "0")
            encoded_text += byte

    # Remove extra zeros added during compression
    encoded_text = encoded_text[:-padding]

    # Decode the text using Huffman codes
    decoded_text = ""
    current_code = ""
    for bit in encoded_text:
        current_code += bit
        if current_code in codes.values():
            decoded_text += [
                char for char, code in codes.items() if code == current_code
            ][0]
            current_code = ""

    # Write the decoded text to the file
    with open(output_filename, "w", encoding="utf-8") as decompressed_file:
        decompressed_file.write(decoded_text)

    # Print a message upon successful decompression
    print(f"Decompression completed. Result is written to the file {output_filename}")


if __name__ == "__main__":
    # Input file name
    input_file_name = input("Enter the name of the file to compress: ")

    # Check if the input file exists in the directory
    if not os.path.exists(input_file_name):
        print(f"The file '{input_file_name}' does not exist in the current directory.")
        exit()

    # Create a directory named "result" if it doesn't exist
    if not os.path.exists("result"):
        os.makedirs("result")

    # File name for the compressed text
    compressed_file = os.path.join("result", "compressed.txt")

    # File name for the decompressed text
    decompressed_file = os.path.join("result", "decompressed.txt")

    # Read text from the input file
    with open(input_file_name, "r", encoding="utf-8") as file:
        text = file.read()

    # Build the Huffman tree based on the text
    root = build_huffman_tree(text)

    # Compress the text and write the result to the file compressed.txt
    compress(text, compressed_file)

    # Decompress the compressed text and write the result to the file decompressed.txt
    decompress(compressed_file, decompressed_file)
