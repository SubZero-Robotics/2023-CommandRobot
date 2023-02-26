import cv2 as cv
import numpy as np
from .output import Output
from typing import List, Tuple


class DataSource:
    def __init__(self, src: int, sourceName: str, port: int = 8082):
        self.src = cv.VideoCapture(src)
        frame = self.getImage()

    def getImage(self):
        _, frame = self.src.read()
        return frame

    def preprocessImage(self, destW: int, destH: int, frame):
        frame = cv.resize(frame, (destW, destH))
        # frame = cv.cvtColor(frame, cv.COLOR_RGB2GRAY)
        return frame

    def drawBoundingBoxes(self, outputs: List[Output], frame, colors: List[Tuple[int, int, int]]):
        for output in outputs:
            bbox = output.bbox
            x = bbox[1]
            y = bbox[0]
            right = bbox[3]
            bottom = bbox[2]
            cv.rectangle(frame, (int(x), int(y)), (int(right), int(
                bottom)), colors[output.classId], thickness=2)
        return frame
