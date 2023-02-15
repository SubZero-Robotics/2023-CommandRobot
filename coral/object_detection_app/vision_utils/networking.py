# from networktables import NetworkTables
from .output import Output
from typing import List


class Networking:

    def __init__(self, teamNum: str, table: str):
        self.teamNum = teamNum
        self.inst = ntcore.NetworkTableInstance.getDefault()
        self.table = self.inst.getTable(table)
        self.inst.startClient4('coral client')
        self.inst.setServerTeam(teamNum)
        self.pub = self.table.getFloatArrayTopic('detections').publish()

    def write(self, outputs: List[Output]) -> List[float]:
        outputNumArray: List[float] = [len(outputs)]
        flattenedOutputList = [output.flatten() for output in outputs]
        for flattenedOutput in flattenedOutputList:
            for value in flattenedOutput:
                outputNumArray.append(value)
        self.pub.set(outputNumArray)
        return outputNumArray
