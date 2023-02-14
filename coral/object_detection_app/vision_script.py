from vision_utils import DataSource, Interpreter, Networking, Output, DetectionModelClass, DetectionModelClassParser
import cv2 as cv


dataSource = DataSource(1, 'cvsource')
interpreter = Interpreter('../models/object_int8.tflite')
modelClasses = DetectionModelClassParser.parse('../models/classes.csv')
# TODO: Set table to correct value
networking = Networking('5690', 'default')

inputDetails = interpreter.input_details
inputShape = (inputDetails[0]['shape'][1], inputDetails[0]['shape'][2])
destW, destH = inputShape
colors = [modelClass.color for modelClass in modelClasses]

while (True):
    frame = dataSource.getImage()
    frame = dataSource.preprocessImage(destW, destH, frame)
    outputs = interpreter.invoke(frame)
    print('Outputs: ', outputs)
    frame = dataSource.drawBoundingBoxes(outputs, frame, colors)
    # networking.write(outputs)
    # send resulting frame to image server
    dataSource.putImage(frame)
    cv.imShow('Bounding Box Output', frame)
