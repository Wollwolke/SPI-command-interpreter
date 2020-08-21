#! /usr/bin/python

import Interpreter

# Fix ANSI Escape Sequences on old Windows Terminal
from platform import system

if "win" in system().lower():
    from ctypes import windll

    k = windll.kernel32
    k.SetConsoleMode(k.GetStdHandle(-11), 7)


def main():

    try:
        interpreter = Interpreter.Interpreter("../data/Registers.json")
        interpreter.interpretFile("../data/presenter1/B01_power_on.txt")
        interpreter.interpretFile("../data/presenter1/A02_button_up.txt")
    except Exception as identifier:
        raise


if __name__ == "__main__":
    main()
