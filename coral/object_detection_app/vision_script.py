from vision_utils import DataSource, Interpreter, Networking, DetectionModelClassParser
from flask import Response
from flask import Flask
from flask import render_template
import threading
import argparse
import imutils
import cv2 as cv

parser = argparse.ArgumentParser()
parser.add_argument('-v', '--verbose', help='Print verbose output in terminal',
                    action='store_true')
parser.add_argument('-N', '--no-networking', help='Don\'t output data via networktables',
                    action='store_true')
parser.add_argument('-t', '--threshold', metavar='THRESHOLD',
                    help='Set object detection threshold', default=0.5, type=float)
parser.add_argument('-p', '--port', metavar='PORT',
                    help='Port for webserver', default=8080, type=int)
parser.add_argument('-m', '--model', help='Path to TPU tflite model',
                    default='../models/object_int8_edgetpu.tflite')
parser.add_argument('-c', '--class-list', help='Path to CSV class list',
                    default='../models/classes.csv')
parser.add_argument('--no-boxes', help='Don\'t display bounding boxes on the stream',
                    action='store_true')
parser.add_argument('--camera_passthrough', help='Only display the camera stream; does not invoke the model',
                    action='store_true')
parser.add_argument('-r', '--resize', help='Resize the resulting frame as WIDTH, HEIGHT; example: -r 240, 240',
                    type=int, nargs=2)
args = vars(parser.parse_args())

THRESHOLD = args['threshold']

outputFrame = None
lock = threading.Lock()

app = Flask(__name__)

dataSource = DataSource(1, 'cvsource')
interpreter = Interpreter(args['model'])
modelClasses = DetectionModelClassParser.parse(args['classList'])
if not args['no_networking']:
    networking = Networking('56.90', 'shuffleboard', args['port'])

inputDetails = interpreter.input_details
inputShape = (inputDetails[0]['shape'][1], inputDetails[0]['shape'][2])
destW, destH = inputShape
colors = [modelClass.color for modelClass in modelClasses]


def detectObjects():
    global outputFrame, lock, dataSource, interpreter, destW, destH, colors

    while True:
        frame = dataSource.getImage()
        frame = dataSource.preprocessImage(destW, destH, frame)
        if not args['camera_passthrough']:
            outputs = interpreter.invoke(frame)
            filteredOutputs = []
            for output in outputs:
                if output.score >= THRESHOLD:
                    filteredOutputs.append(output)
                    if args['verbose']:
                        print('Found: ', output)
            if not args['no_boxes']:
                frame = dataSource.drawBoundingBoxes(
                    filteredOutputs, frame, colors)
            if args['verbose'] and len(filteredOutputs) > 0:
                print('-' * 30)
        if 'resize' in args:
            frame = cv.resize(frame, tuple(args['resize']))
        with lock:
            outputFrame = frame.copy()
        if not args['no_networking']:
            networking.write(filteredOutputs)


def generate():
    global outputFrame, lock

    while True:
        with lock:
            if outputFrame is None:
                continue

            flag, encodedImage = cv.imencode('.jpg', outputFrame)

            if not flag:
                continue

        yield (b'--frame\r\n' b'Content-Type: image/jpeg\r\n\r\n' +
               bytearray(encodedImage) + b'\r\n')


@app.route('/')
def stream():
    return Response(generate(),
                    mimetype='multipart/x-mixed-replace; boundary=frame')


t = threading.Thread(target=detectObjects)
t.daemon = True
t.start()

print(f"OpenCV output mjpg server listening at http://0.0.0.0:{args['port']}")
app.run(host='0.0.0.0', port=args['port'], debug=True,
        threaded=True, use_reloader=False)
