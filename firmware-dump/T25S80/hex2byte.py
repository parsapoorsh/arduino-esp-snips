#!/bin/python3
import sys

txt = sys.stdin.read()
txt = txt.replace("\r", "").replace("\n", "")

bytes_in_hex_string = txt.split()

for i in bytes_in_hex_string:
    the_byte = int(i, 16).to_bytes()
    sys.stdout.buffer.write(the_byte)
