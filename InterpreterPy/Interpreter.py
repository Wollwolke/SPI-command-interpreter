#! /bin/usr/python

import json
import Registers

file = "../data/Registers.json"
jsonFile = {}

with open(file, "r") as regFile:
    jsonFile = json.load(regFile)

reg = jsonFile["registers"]

RegData = Registers.Registers(reg)

print(reg)
