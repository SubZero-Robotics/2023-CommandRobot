# from networktables import NetworkTables
import ntcore
from output import Output


class Networking:

    def __init__(self, teamNum: str, table: str):
        self.teamNum = teamNum
        self.inst = ntcore.NetworkTableInstance.getDefault()
        self.table = self.inst.getTable(table)
        self.inst.startClient4('coral client')
        self.inst.setServerTeam(teamNum)
        self.pub = self.table.getFloatArrayTopic('detections').publish()

    def write(self, outputs: list[Output]) -> list[float]:
        outputNumArray: list[float] = [len(outputs)]
        flattenedOutputList = [output.flatten() for output in outputs]
        for flattenedOutput in flattenedOutputList:
            for value in flattenedOutput:
                outputNumArray.append(value)
        self.pub.set(outputNumArray)
        return outputNumArray
