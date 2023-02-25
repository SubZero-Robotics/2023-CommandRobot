from networktables import NetworkTables
from .output import Output
from typing import List
import socket


class Networking:

    def __init__(self, teamNum: str, table: str, port: int):
        NetworkTables.initialize(server=f'10.{teamNum}.2')
        self.teamNum = teamNum
        self.table = NetworkTables.getTable(table)
        hostname = socket.gethostname()
        ipAddress = socket.gethostbyname(hostname)
        url = f'mjpeg:http://{ipAddress}:{port}'
        ntPath = '/CameraPublisher/coral/streams'
        print(f'Placing NT stream URL {url} under {ntPath}')
        self.table.putStringArray(ntPath, [url])

    def write(self, outputs: List[Output]) -> List[float]:
        outputNumArray: List[float] = [len(outputs)]
        flattenedOutputList = [output.flatten() for output in outputs]
        for flattenedOutput in flattenedOutputList:
            for value in flattenedOutput:
                outputNumArray.append(value)
        self.table.putNumberArray('detections', outputNumArray)
        return outputNumArray
