#! /bin/usr/python

import json
import Registers
import Commands
import utils


class Interpreter:
    def __init__(self, confFile):
        try:
            with open(confFile, "r") as regFile:
                self.jsonFile = json.load(regFile)
        except IOError:
            raise utils.ERR_FATAL(f"Error while opening JSON file {confFile}")
        except ValueError:
            raise utils.ERR_FATAL("Error while decoding JSON file")

        try:
            self.registers = Registers.Registers(self.jsonFile["registers"])
            self.commands = Commands.Commands(self.registers, self.jsonFile["commands"])
        except utils.ERR_JSONPARSER_REGISTER:
            raise utils.ERR_FATAL("Error while creating Registers from JSON file")
        except utils.ERR_JSONPARSER_CMD:
            raise utils.ERR_FATAL("Error while creating Commands from JSON file")
        except utils.Error:
            raise utils.ERR_INTERPRET("")

    def interpretFile(self, fname):
        try:
            with open(fname, "r") as file:
                lines = file.readlines()
        except IOError:
            raise utils.ERR_INTERPRET(f"Error while opening file {fname}")

        try:
            for line in lines:
                readBytes = []
                line = line.rstrip("\n").rstrip("\r\n")
                print("\x1B[90m", line, "\x1B[0m", sep="")
                if not line == "" and not line[0] == "#":
                    splits = line.split(" ")
                    for split in splits:
                        if len(split) == 3:
                            split = split + "0"
                        readBytes.append(split)

                    if len(readBytes) > 1:
                        for x in range(1, len(readBytes)):
                            self.commands.executeCommand(readBytes[0], readBytes[x])
                            print(
                                self.commands.interpret(
                                    self.jsonFile["interpret"], readBytes[0]
                                ),
                                end="",
                            )
                    else:
                        print(
                            self.commands.interpret(
                                self.jsonFile["interpret"], readBytes[0]
                            ),
                            end="",
                        )
                    print()
        except utils.ERR_FATAL as e:
            raise utils.ERR_INTERPRET(
                "Unexpected Error occurred while Parsing input text file"
            )

