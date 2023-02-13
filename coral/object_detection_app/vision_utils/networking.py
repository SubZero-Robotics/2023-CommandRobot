from networktables import NetworkTables
from output import Output


class Networking:

    def __init__(self, teamnum, server):
        self.teamnum = teamnum
        self.server = server
        NetworkTables.initailize(server=self.server)
        self.table = NetworkTables.getTables('SmartDashboard')

    def write(self, outputs: list[Output]) -> list[float]:
        outputNumArray: list[float] = [len(outputs)]
        flattenedOutputList = [output.flatten() for output in outputs]
        for flattenedOutput in flattenedOutputList:
            for value in flattenedOutput:
                outputNumArray.append(value)
        self.table.putNumberArray('detections', outputNumArray)
        return outputNumArray
