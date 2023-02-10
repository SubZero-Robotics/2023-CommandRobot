import cv2 as cv
import numpy as np


class DataSource:
    def __init__(self, src: str):
        self.src = cv.VideoCapture(src)
        frame = self.getImage()
        self.imgSize = (frame.shape[0], frame.shape[1])

    def getImage(self):
        _, frame = self.src.read()
        return frame

    def preprocessImage(w: int, h: int, frame):
        frame = cv.resize(frame, (w, h))
        frame = cv.cvtColor(frame, cv.COLOR_BGR2RGB)
        return frame

    def drawBoundingBoxes(self, outputs: list, frame, colors):
        rows, cols = self.imgSize
        for output in outputs:
            bbox = output.bbox
            x = bbox[1] * cols
            y = bbox[0] * rows
            right = bbox[3] * cols
            bottom = bbox[2] * rows
            cv.rectangle(frame, (int(x), int(y)), (int(right), int(
                bottom)), colors[str(output.classId)], thickness=2)
    