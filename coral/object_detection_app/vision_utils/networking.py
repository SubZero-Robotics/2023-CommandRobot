from networktables import NetworkTables
from .output import Output
from typing import List
import socket


class Networking:

    def __init__(self, teamNum: str, table: str, port: int):
        NetworkTables.initialize(server=f'10.{teamNum}.2')
        self.teamNum = teamNum
        self.table = NetworkTables.getTable(table)
        self.streamTable = NetworkTables.getTable('CameraPublisher')
        # TODO: Pass last IP digits as arg from CLI
        url = f'mjpg:http://10.{teamNum}.4:{port}/stream'
        ntPath = '/coral'
        print(f'Placing NT stream URL {url} under {ntPath}')
        self.streamTable.putStringArray(ntPath + '/streams', [url])
        self.streamTable.putString(ntPath + '/description', 'coral')
        self.streamTable.putString(ntPath + '/mode', '320x240 MJPEG 22fps')
        self.streamTable.putStringArray(
            ntPath + '/modes', ['320x240 MJPEG 22fps'])
        self.streamTable.putString(
            ntPath + '/source', f'ip:http://10.{teamNum}.4:{port}/stream')

    def write(self, outputs: List[Output]) -> List[float]:
        outputNumArray: List[float] = [len(outputs)]
        flattenedOutputList = [output.flatten() for output in outputs]
        for flattenedOutput in flattenedOutputList:
            for value in flattenedOutput:
                outputNumArray.append(value)
        self.table.putNumberArray('detections', outputNumArray)
        return outputNumArray
