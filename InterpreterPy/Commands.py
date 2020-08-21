#! /usr/bin/python

import utils
import cexprtk


class Commands:
    class Command:
        def __init__(self, isStrobe, registers):
            self.isStrobe = isStrobe
            self.registers = registers

        def interpret(self, config):
            return ""

    class RegCommand(Command):
        def __init__(self, isRead, registername, registers):
            super().__init__(False, registers)
            self.isRead = isRead
            self.registername = registername

        def interpret(self, config):
            config = config[self.registername]
            out = ""
            try:
                if "ibits" in config:
                    out += self.interpretRegisters(config["ibits"])
                else:
                    if "ibitsr" in config and self.isRead:
                        out += self.interpretRegisters(config["ibitsr"])
                    if "ibitsw" in config and not self.isRead:
                        out += self.interpretRegisters(config["ibitsw"])
                return out
            except KeyError:
                raise utils.ERR_INTERPRET(
                    f"Failed to load interpretation bits from JSON file - missing registername{self.registername}"
                )
            except utils.Error:
                raise

        def interpretRegisters(self, ibits):
            result = ""
            try:
                for key, bit in ibits.items():
                    if "highlight" in bit:
                        color = bit["highlight"]
                        result += f"\x1B[{color}m\x1B[1m"

                    if bit["isfunc"] == True:
                        result += self.interpretFunction(bit) + "\n"
                    else:
                        result += self.interpretBits(bit) + "\n"

                    result += "\x1B[0m"
                return result
            except KeyError:
                raise utils.ERR_INTERPRET(
                    f"Error parsing isfunc flag in JSON file {self.registername}"
                )
            except utils.Error:
                raise

        def interpretBits(self, ibit):

            currentBitSeq = ""
            try:
                iwith = ibit["iwith"]

                for bit in iwith:
                    pos = bit.find(":")
                    if pos is not -1:
                        currentBitSeq += str(
                            self.registers.readBit(bit[:pos], bit[pos + 1 :])
                        )
                    else:
                        currentBitSeq += str(
                            self.registers.readBit(self.registername, bit)
                        )

                if "NA" in ibit["ipret"]:
                    number = int(currentBitSeq, 2)
                    na = ibit["ipret"]["NA"]
                    na += " " + currentBitSeq + " (" + str(number) + ")"
                    return na
                return ibit["ipret"][currentBitSeq]

            except KeyError:
                raise utils.ERR_INTERPRET(
                    f"Error parsing bit interpretation in JSON File at {self.registername}"
                )
            except utils.Error:
                raise

        def interpretFunction(self, ibit):
            symbols = {}
            for key, value in ibit["iwith"].items():
                symbols.update({key: self.intFromRegisters(value)})

            symbolTable = cexprtk.Symbol_Table(symbols, add_constants=True)

            expression = cexprtk.Expression(ibit["func"], symbolTable)

            return ibit["ipret"] + " " + str(expression())

        def intFromRegisters(self, ibits):
            currentBitSeq = ""

            for bit in ibits:
                pos = bit.find(":")
                if pos is not -1:
                    currentBitSeq += str(
                        self.registers.readBit(bit[:pos], bit[pos + 1 :])
                    )
                else:
                    currentBitSeq += str(self.registers.readBit(self.registername, bit))

            return int(currentBitSeq, 2)

    class StrobeCommand(Command):
        def __init__(self, ilogic, registers):
            super().__init__(True, registers)
            self.ilogic = ilogic

        def interpret(self, config):
            return self.ilogic + "\n"

    def __init__(self, registers, commands):
        self.cmds = {}
        try:
            for key in commands:
                command = commands[key]
                if command["isstrobe"]:
                    cmd = self.StrobeCommand(command["logic"], registers)
                else:
                    cmd = self.RegCommand(
                        command["isread"], command["registername"], registers
                    )
                self.cmds[command["hex"]] = cmd
        except KeyError:
            raise utils.ERR_JSONPARSER_CMD(
                "Error while creating Command Objects from JSON File"
            )

    def interpret(self, interpret, hexcmd):
        if hexcmd not in self.cmds:
            raise utils.ERR_INTERPRET(f"Tried to interpret unknown command {hexcmd}")
        try:
            return self.cmds[hexcmd].interpret(interpret)
        except utils.Error:
            raise

    def executeCommand(self, cmdString, data):
        if cmdString not in self.cmds:
            raise utils.ERR_INTERPRET(f"Tried to interpret unknown command {cmdString}")
        localCmd = self.cmds[cmdString]
        if not localCmd.isStrobe:
            if not localCmd.isRead:
                try:
                    localCmd.registers.writeByte(localCmd.registername, data)
                except utils.Error:
                    raise
