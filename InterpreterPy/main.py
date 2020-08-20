#! /usr/bin/python

import Interpreter


def main():
    try:
        interpreter = Interpreter.Interpreter("../data/Registers.json")
        interpreter.interpretFile("../data/presenter1/B01_power_on.txt")
        interpreter.interpretFile("../data/presenter1/A02_button_up.txt")
    except Exception as identifier:
        raise


if __name__ == "__main__":
    main()
