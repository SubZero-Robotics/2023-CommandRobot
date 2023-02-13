from vision_utils import DataSource, Interpreter, Networking, Output, DetectionModelClass, DetectionModelClassParser


dataSource = DataSource('/dev/video1')
interpreter = Interpreter('../models/object_int8.tflite')
modelClasses = DetectionModelClassParser.parse('../models/classes.csv')

while (True):
    frame = dataSource.preprocessImage(, dataSource.getImage())
    outputList = interpreter.invoke()
