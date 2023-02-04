import cv2 as cv
import tflite_runtime.interpreter as tflite
import numpy as np
import interpreter
from interpreter import Interpreter

class Datasource:
    def __init__(self, src: str):
        self.src = cv.VideoCapture(1)

    def preprocessing(w: int, h: int, frame):
        frame = cv.resize(frame, (w, h))
        frame = cv.cvtColor(frame, cv.COLOR_BGR2RGB)

    def drawboundingboxes(output: list):