import io
import os
import zipfile
from math import ceil
from sys import argv
from typing import Tuple

from PIL import Image

c_header = """#include "texts.h"

#include <avr/eeprom.h>


// AUTO-GENERATED

"""

h_header = """#ifndef TEXTS_H
#define TEXTS_H

#include <avr/io.h>


// AUTO-GENERATED

"""

h_footer = """
#endif
"""

letters = [
    "a",
    "á",
    "b",
    "c",
    "d",
    "e",
    "é",
    "f",
    "g",
    "h",
    "i",
    "í",
    "j",
    "k",
    "l",
    "m",
    "n",
    "o",
    "ó",
    "ö",
    "ő",
    "p",
    "q",
    "r",
    "s",
    "t",
    "u",
    "ú",
    "ü",
    "ű",
    "v",
    "w",
    "x",
    "y",
    "z",
    "0",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    ",",
    ".",
    "!",
    "?",
    " ",
]


def generate(input_text: str) -> Tuple[str, str]:
    indices = []
    for line in input_text.split("\n"):
        for char in line.strip().lower().replace(", ", ","):
            indices.append(letters.index(char))
        indices.append(255)
    indices.append(255)

    if len(indices) > 512:
        print(f"Input is too long ({len(indices)} > 512)")
        exit()

    output_h = h_header + f"const uint8_t texts[{len(indices)}];\n" + h_footer
    output_c = c_header + f"const uint8_t texts[{len(indices)}] EEMEM = {{"

    output_c += ", ".join(str(i) for i in indices)
    output_c += "};\n"

    return output_h, output_c


if __name__ == "__main__":
    if len(argv) == 1:
        print("Specify an input file")
        exit()

    with open(argv[1], "r", encoding="utf-8") as f:
        input_text = f.read()

    dot_h, dot_c = generate(input_text)

    with open("output/texts.h", "w+") as f:
        f.write(dot_h)
    with open("output/texts.c", "w+") as f:
        f.write(dot_c)
