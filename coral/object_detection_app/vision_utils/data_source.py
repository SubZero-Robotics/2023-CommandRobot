import cv2 as cv
import numpy as np
import socket
from cscore import CvSource, VideoMode, MjpegServer
from output import Output


class DataSource:
    def __init__(self, src: int, sourceName: str, port: int = 8082):
        self.src = cv.VideoCapture(src)
        frame = self.getImage()
        self.imgSize = (frame.shape[0], frame.shape[1])
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        s.connect(("8.8.8.8", 80))
        ip = s.getsockname()[0]
        s.close()
        self.cvSource = CvSource(
            sourceName, VideoMode.PixelFormat.kMJPEG, self.imgSize[0], self.imgSize[1], 30)
        self.cvMjpegServer = MjpegServer("cvhttpserver", port)
        self.cvMjpegServer.setSource(self.cvSource)

        print(f"OpenCV output mjpg server listening at http://{ip}:{port}")

    def getImage(self):
        _, frame = self.src.read()
        return frame

    def putImage(self, frame):
        self.cvSource.putFrame(frame)

    def preprocessImage(destW: int, destH: int, frame):
        frame = cv.resize(frame, (destW, destH))
        frame = cv.cvtColor(frame, cv.COLOR_BGR2RGB)
        return frame

    def drawBoundingBoxes(self, outputs: list[Output], frame, colors: list[tuple[int, int, int]]):
        rows, cols = self.imgSize
        for output in outputs:
            bbox = output.bbox
            x = bbox[1] * cols
            y = bbox[0] * rows
            right = bbox[3] * cols
            bottom = bbox[2] * rows
            cv.rectangle(frame, (int(x), int(y)), (int(right), int(
                bottom)), colors[output.classId], thickness=2)
        return frame
