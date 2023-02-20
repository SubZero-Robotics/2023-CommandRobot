import cv2
from mjpeg.client import MJPEGClient
import numpy as np
import sys

url = sys.argv[1]
index = 0

client = MJPEGClient(url)

bufs = client.request_buffers(65536, 50)
for b in bufs:
    client.enqueue_buffer(b)

client.start()

while True:
    buf = client.dequeue_buffer().data
    im = cv2.imdecode(np.fromstring(buf, dtype=np.int8), cv2.CV_LOAD_IMAGE_COLOR)
    cv2.imwrite(f'../saved_stream/{index}.jpg', im)
    cv2.imshow('i', im)
    index += 1
    if cv2.waitKey(1) == 27:
        client.stop()
        sys.exit(0)
    client.enqueue_buffer(buf)