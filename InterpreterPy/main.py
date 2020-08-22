#! /usr/bin/python

import Interpreter
import os
import argparse

# Fix ANSI Escape Sequences on old Windows Terminal
from platform import system

if "win" in system().lower():
    from ctypes import windll

    k = windll.kernel32
    k.SetConsoleMode(k.GetStdHandle(-11), 7)


def main():

    parser = argparse.ArgumentParser()
    parser.add_argument(
        "-c",
        "--config",
        type=str,
        help="JSON File for Register Layout (Defaults to ./Register.json)",
    )
    parser.add_argument(
        "dataFile",
        nargs="+",
        help="<Required> Text-Files to analyse (Pay attention to the correct order)",
    )
    args = parser.parse_args()

    if not args.config:
        args.config = os.path.dirname(os.path.abspath(__file__)) + "/Registers.json"

    try:
        interpreter = Interpreter.Interpreter(args.config)
        for file in args.dataFile:
            interpreter.interpretFile(file)
    except Exception as e:
        print(e)


if __name__ == "__main__":
    main()
