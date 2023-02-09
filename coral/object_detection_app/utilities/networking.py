from networktables import NetworkTables
from Datasource import drawboundingboxes
import logging
import sys


class Networking:

    def __init__(self, teamnum, server):
        self.teamnum = teamnum
        self.server = server

    def write(self, output: list):
