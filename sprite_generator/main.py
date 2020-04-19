import os
import zipfile
from typing import Tuple
from PIL import Image
import io
from math import ceil


c_header = """#include "sprites.h"

#include <avr/eeprom.h>


// AUTO-GENERATED

"""

h_header = """#ifndef SPRITES_H
#define SPRITES_H

#include <avr/io.h>


// AUTO-GENERATED

"""

h_footer = """
#endif
"""


def generate_content(sprites_directory: str) -> Tuple[str, str]:
    output_h = h_header
    output_c = c_header

    for entry in os.scandir(sprites_directory):
        delta_output_h, delta_output_c = process_spritesheet(entry)
        output_h += delta_output_h
        output_c += delta_output_c

    output_h += h_footer
    return output_h, output_c


def process_spritesheet(entry) -> Tuple[str, str]:
    sprites_name, _ = entry.name.split('.')
    sprites_zip = zipfile.ZipFile(entry.path, 'r')
    width, height = read_sprite(sprites_zip, sprites_zip.filelist[0]).size

    declaration = f'const uint16_t {sprites_name}[{len(sprites_zip.filelist)}][{width}][{ceil(height / 8)}]'
    output_h = declaration + ';\n'
    output_c = declaration + ' EEMEM = {' + process_sprites(sprites_zip) + '};\n'

    return output_h, output_c


def process_sprites(sprites_zip) -> str:
    output = ''
    width, height = read_sprite(sprites_zip, sprites_zip.filelist[0]).size
    for sprite in sprites_zip.filelist:
        output += '{'
        pixels = list(read_sprite(sprites_zip, sprite).getdata())
        for x in range(width):
            output += '{'
            for y in range(0, height, 8):
                fill_bits = ''.join(
                    reversed([
                        ('1' if is_filled(pixels[(y + i) * width + x]) else '0') if (y + i) * width + x < len(pixels) else '0'
                        for i in range(8)
                    ])
                )

                inverted_mask_bits = ''.join(
                    reversed([
                        ('0' if is_translucent(pixels[(y + i) * width + x]) else '1') if (y + i) * width + x < len(pixels) else '0'
                        for i in range(8)
                    ])
                )
                output += f'{binary_to_hex_with_prefix(inverted_mask_bits + fill_bits)},'
            output = output[:-1] + '},'
        output = output[:-1] + '},'

    return output[:-1]


def read_sprite(sprites_zip, entry):
    return Image.open(io.BytesIO(sprites_zip.read(entry.filename)))


def is_translucent(pixel: Tuple[int, int, int, int]) -> bool:
    r, g, b, a = pixel
    return a == 0


def is_filled(pixel: Tuple[int, int, int, int]) -> bool:
    r, g, b, a = pixel
    return r != 0 or g != 0 or b != 0


def binary_to_hex_with_prefix(binary: str) -> str:
    return hex(int(binary, 2))


if __name__ == '__main__':
    dot_h, dot_c = generate_content('spritesheets')
    with open('output/sprites.h', 'w+') as f:
        f.write(dot_h)
    with open('output/sprites.c', 'w+') as f:
        f.write(dot_c)
