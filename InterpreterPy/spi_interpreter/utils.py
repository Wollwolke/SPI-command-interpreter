#! /bin/usr/python


class Error(Exception):
    """Base class for exceptions in this module."""

    pass


class ERR_INTERPRET(Error):
    def __init__(self, message):
        self.message = message


class ERR_REGISTER(Error):
    def __init__(self, message):
        self.message = message


class ERR_EXECUTE_CMD(Error):
    def __init__(self, message):
        self.message = message


class ERR_JSONPARSER_CMD(Error):
    def __init__(self, message):
        self.message = message


class ERR_FATAL(Error):
    def __init__(self, message):
        self.message = message


class ERR_JSONPARSER_REGISTER(Error):
    def __init__(self, message):
        self.message = message
