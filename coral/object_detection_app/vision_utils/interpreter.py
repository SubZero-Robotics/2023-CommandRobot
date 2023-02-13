import cv2 as cv
import tflite_runtime.interpreter as tflite
import numpy as np
from output import Output


class Interpreter:

    def __init__(self, modelpath: str):
        self.model = tflite.Interpreter(model_path=self.modelpath,
                                        experimental_delegates=[tflite.load_delegate('libedgetpu.so.1')])
        self.model.allocate_tensors()
        self.input_details = self.model.get_input_details()
        self.output_details = self.model.get_output_details()

    def invoke(self, frame) -> list:
        inp = np.expand_dims(frame, axis=0).astype(np.int8)
        self.model.set_tensor(self.input_details[0]['index'], inp)
        self.model.invoke()
        scores = self.model.get_tensor(self.output_details[0]['index'])[0]
        boxes = self.model.get_tensor(self.output_details[1]['index'])[0]
        num = self.model.get_tensor(self.output_details[2]['index'])[0]
        classes = self.model.get_tensor(self.output_details[3]['index'])[0]
        outputlist = []
        for i in range(int(num)):
            classId = int(classes[i])
            score = float(scores[i])
            bbox = [float(v) for v in boxes[i]]
            outputlist.append(Output(bbox, classId, score))
        return outputlist
