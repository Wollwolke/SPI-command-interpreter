#! /usr/bin/python


class Registers:
    def __init__(self, registers):
        self.regMap = {}
        for key, value in registers.items():
            name = value["name"]
            reg = Register(value["r"], value["w"], value["values"])
            self.regMap.update({name: reg})


class Register:
    def __init__(self, r, w, values):
        self.r = r
        self.w = w
        self.values = values
