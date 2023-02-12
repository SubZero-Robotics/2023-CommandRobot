from networktables import NetworkTables
from output import Output


class Networking:

    def __init__(self, teamnum, server):
        self.teamnum = teamnum
        self.server = server
        NetworkTables.initailize(server=server)
        self.table = NetworkTables.getTables('SmartDashboard')

    def write(self, outputs: list[Output]) -> list[float]:
        outputNumArray: list[float] = []
        flattenedOutputList = [output.flatten() for output in outputs]
        # TODO: Flatten the 2D flattenedOutputList into a 1D array
        self.table.putNumberArray('detections', outputNumArray)
        return outputNumArray