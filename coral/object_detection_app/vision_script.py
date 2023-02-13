from vision_utils import DataSource, Interpreter, Networking, Output, DetectionModelClass, DetectionModelClassParser
import cv2 as cv


dataSource = DataSource('/dev/video1')
interpreter = Interpreter('../models/object_int8.tflite')
modelClasses = DetectionModelClassParser.parse('../models/classes.csv')
networking = Networking('5690', '')

inputDetails = interpreter.input_details
inputShape = (inputDetails[0]['shape'][1], inputDetails[0]['shape'][2])
destW, destH = inputShape
colors = [modelClass.color for modelClass in modelClasses]

while (True):
    frame = dataSource.getImage()
    frame = dataSource.preprocessImage(destH, destH, frame)
    outputs = interpreter.invoke(frame)
    frame = dataSource.drawBoundingBoxes(outputs, frame, colors)
    networking.write(outputs)
    cv.imShow('Bounding Box Output', frame)