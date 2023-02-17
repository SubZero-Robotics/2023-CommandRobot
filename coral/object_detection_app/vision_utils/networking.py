from networktables import NetworkTables
from .output import Output
from typing import List


class Networking:

    def __init__(self, teamNum: str, table: str):
        NetworkTables.initialize(server='roborio-XXX-frc.local')
        self.teamNum = teamNum
        self.table = NetworkTables.getTable(table)

    def write(self, outputs: List[Output]) -> List[float]:
        outputNumArray: List[float] = [len(outputs)]
        flattenedOutputList = [output.flatten() for output in outputs]
        for flattenedOutput in flattenedOutputList:
            for value in flattenedOutput:
                outputNumArray.append(value)
        self.table.putNumberArray('detections', outputNumArray)
        return outputNumArray
