from vision_utils import DataSource, Interpreter, Networking, DetectionModelClassParser
from flask import Response
from flask import Flask
from flask import render_template
import threading
import argparse
import datetime
import imutils
import time
import cv2 as cv


THRESHOLD = 0.50

outputFrame = None
lock = threading.Lock()

app = Flask(__name__)

dataSource = DataSource(1, 'cvsource')
interpreter = Interpreter('../models/object_int8_edgetpu.tflite')
modelClasses = DetectionModelClassParser.parse('../models/classes.csv')
# networking = Networking('5690', 'default')

inputDetails = interpreter.input_details
inputShape = (inputDetails[0]['shape'][1], inputDetails[0]['shape'][2])
destW, destH = inputShape
colors = [modelClass.color for modelClass in modelClasses]

@app.route('/')
def index():
    return render_template('index.html')

def detectObjects():
    global outputFrame, lock, dataSource, interpreter, destW, destH, colors

    while True:
        frame = dataSource.getImage()
        frame = DataSource.preprocessImage(destW, destH, frame)
        outputs = interpreter.invoke(frame)
        filteredOutputs = []
        for output in outputs:
            if output.score >= THRESHOLD:
                filteredOutputs.append(output)
                print('Found: ', output)
        frame = dataSource.drawBoundingBoxes(filteredOutputs, frame, colors)
        print()
        with lock:
            outputFrame = frame.copy()
        # networking.write(filteredOutputs)
        
def generate():
    global outputFrame, lock
    
    while True:
        with lock:
            if outputFrame is None:
                continue
            
            flag, encodedImage = cv.imencode('.jpg', outputFrame)
            
            if not flag:
                continue
            
        yield(b'--frame\r\n' b'Content-Type: image/jpeg\r\n\r\n' + 
			bytearray(encodedImage) + b'\r\n')
        
@app.route('/stream')
def stream():
    return Response(generate(),
                    mimetype = 'multipart/x-mixed-replace; boundary=frame')
    
t = threading.Thread(target=detectObjects)
t.daemon = True
t.start()

print(f"OpenCV output mjpg server listening at http://0.0.0.0:{5001}")
app.run(host='0.0.0.0', port=5001, debug=True, threaded=True, use_reloader=False)