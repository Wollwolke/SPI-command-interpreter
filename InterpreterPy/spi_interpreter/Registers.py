#! /usr/bin/python

from spi_interpreter import utils


class Registers:
    class Register:
        def __init__(self, r, w, values):
            self.rNames = r
            self.wNames = w
            self.values = values

        def readBit(self, bitname):
            if bitname in self.rNames:
                return self.values[self.rNames.index(bitname)]
            if bitname in self.wNames:
                return self.values[self.wNames.index(bitname)]
            raise utils.ERR_REGISTER(f"Error trying to read bit {bitname}")

        def writeRegister(self, byteString):
            counter = 0
            for bit in byteString:
                self.values[counter] = int(bit, 2)
                counter += 1

    def __init__(self, registers):
        self.regMap = {}
        for _, value in registers.items():
            try:
                name = value["name"]
                reg = self.Register(value["r"], value["w"], value["values"])
                self.regMap.update({name: reg})
            except KeyError:
                raise utils.ERR_JSONPARSER_REGISTER(f"{value}")

    def readBit(self, registername, bitname):
        if registername not in self.regMap:
            raise utils.ERR_REGISTER(
                f"Tried to read non-existing bit at {registername}"
            )
        try:
            return self.regMap[registername].readBit(bitname)
        except utils.ERR_REGISTER as e:
            raise e

    def writeByte(self, registername, hexString):
        if registername not in self.regMap:
            raise utils.ERR_EXECUTE_CMD(
                f"Tried to write non-existing byte at {registername}"
            )
        try:
            byte = bytes.fromhex(hexString[2:])
            bits = bin(ord(byte))[2:].rjust(8, "0")
        except (ValueError, TypeError):
            raise utils.ERR_INTERPRET(
                f"Error reading hex string from input file: {hexString}"
            )

        self.regMap[registername].writeRegister(bits)

