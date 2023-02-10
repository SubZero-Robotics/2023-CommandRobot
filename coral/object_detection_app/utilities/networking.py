from networktables import NetworkTables
import logging
import sys
from output import Output

class Networking:

    def __init__(self, teamnum, server):
        self.teamnum = teamnum
        self.server = server
        NetworkTables.initailize(server=server)
        self.table = NetworkTables.getTables('SmartDashboard')

    def write(self, output: list[Output]):
        outputNumArray: list[float]
        self.table.putNumberArray('detections', outputNumArray)