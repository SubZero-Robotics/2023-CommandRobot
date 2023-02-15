from flask import Response
from flask import Flask
from flask import render_template
import threading
import argparse
import sys
import cv2 as cv


PLATFORM = sys.platform
CAM_COUNT = 2
CAM_BASE = '{}' if PLATFORM == 'win32' else '/dev/video{}'
cams = [i for i in range(CAM_COUNT)] if PLATFORM == 'win32' else [
    CAM_BASE.format(i) for i in range(CAM_COUNT)]

parser = argparse.ArgumentParser()
parser.add_argument('-p', '--port', metavar='PORT',
                    help='Port for webserver', default=5001, type=int)
parser.add_argument('-s', '--size', metavar='px', nargs=2,
                    help='Resolution of images - defaults to (320, 240)', default=(320, 240), type=int)
parser.add_argument('-i', '--inputs', metavar='INPUT', nargs=CAM_COUNT,
                    help='Video sources - defaults to ({})'.format(', '.join(map(str, cams))), default=cams, type=type(int) if PLATFORM == 'win32' else type(str))
cliArgs = vars(parser.parse_args())

outputFrames = [None] * CAM_COUNT
lock = threading.Lock()
caps = [cv.VideoCapture(cliArgs['inputs'][i]) for i in range(CAM_COUNT)]
for index, cap in enumerate(caps):
    _, frame = cap.read()
    if type(frame) == type(None):
        print(f'ERROR: Unable to read image from camera {cams[index]}')
        sys.exit(-1)


app = Flask(__name__)


@app.route('/')
def index():
    return render_template('index.html')


def getFrames():
    global outputFrames, lock, cliArgs, caps

    while True:
        for index, cap in enumerate(caps):
            _, frame = cap.read()
            frame = cv.resize(frame, cliArgs['size'])
            with lock:
                outputFrames[index] = frame.copy()


def generateFrame(idx: int):
    global outputFrames, lock

    while True:
        with lock:
            if outputFrames[idx] is None:
                continue

            frame = outputFrames[idx]

            flag, encodedImage = cv.imencode('.jpg', frame)

            if not flag:
                continue

        yield (b'--frame\r\n' b'Content-Type: image/jpeg\r\n\r\n' +
               bytearray(encodedImage) + b'\r\n')


@app.route('/stream/<index>')
def stream(index):
    index = int(index)
    return Response(generateFrame(index),
                    mimetype='multipart/x-mixed-replace; boundary=frame')


t = threading.Thread(target=getFrames)
t.daemon = True
t.start()

app.run(host='0.0.0.0', port=cliArgs['port'],
        debug=True, threaded=True, use_reloader=False)
