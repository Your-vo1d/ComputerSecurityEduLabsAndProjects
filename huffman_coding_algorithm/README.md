# Huffman Coding

This Python script provides functions for compressing and decompressing text files using Huffman coding.

## Usage

1. Ensure you have Python installed on your system.
2. Clone this repository to your local machine.
3. Open a terminal or command prompt and navigate to the directory containing the script.
4. Run the script by executing the command: `python main.py`.
5. Follow the on-screen instructions to compress or decompress text files.

## Description

### `huffman_coding.py`

This script contains the following functions:

- `build_huffman_tree_from_codes`: Builds a Huffman tree from a dictionary of Huffman codes.
- `build_huffman_tree`: Builds a Huffman tree from a given text.
- `build_huffman_codes`: Builds Huffman codes for each character in the tree.
- `compress`: Compresses a given text file using Huffman coding.
- `decompress`: Decompresses a compressed file back to its original form.

## Example

Suppose you have a text file named `example.txt` that you want to compress. You can use this script as follows:

1. Run the script and provide the name of the file (`example.txt`) when prompted.
2. The script will compress the file and save the compressed version as `compressed.txt` in the `result` directory.
3. To decompress the file, run the script again and select the compressed file (`compressed.txt`) when prompted.
4. The script will decompress the file and save the decompressed version as `decompressed.txt` in the `result` directory.
