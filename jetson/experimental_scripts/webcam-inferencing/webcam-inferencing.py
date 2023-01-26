import cv2 as cv
import tensorflow as tf
import numpy as np

colors = {
    '1': (0, 255, 255),
    '2': (255, 0, 200)
}

interpreter = tf.lite.Interpreter(model_path='model.lite')

vid = cv.VideoCapture(0)

# Get input and output tensors.
input_details = interpreter.get_input_details()
output_details = interpreter.get_output_details()

for idx, d in enumerate(output_details):
    print(f"name: {d['name']}, shape: {d['shape']}")

# Allocate tensors
interpreter.allocate_tensors()
# Read and preprocess an image.
while(True):
    ret, frame = vid.read()
    rows = frame.shape[0]
    cols = frame.shape[1]
    frame = cv.resize(frame, (320, 320))
#    frame = cv.cvtColor(frame, cv.COLOR_RGB2HSV)
    inp = np.expand_dims(frame, axis=0).astype(np.float32)

    interpreter.set_tensor(input_details[0]['index'], inp)
# Run inference
    interpreter.invoke()

# output_details[0]['index'] = the index which provides the input
    scores = interpreter.get_tensor(output_details[0]['index'])[0]
    boxes = interpreter.get_tensor(output_details[1]['index'])[0]
    num = interpreter.get_tensor(output_details[2]['index'])[0]
    classes = interpreter.get_tensor(output_details[3]['index'])[0]
    # Visualize detected bounding boxes.
    for i in range(int(num)):
        classId = int(classes[i])
        score = float(scores[i])
        bbox = [float(v) for v in boxes[i]]
        if score > 0.33:
            x = bbox[1] * cols
            y = bbox[0] * rows
            right = bbox[3] * cols
            bottom = bbox[2] * rows
            cv.rectangle(frame, (int(x), int(y)), (int(right), int(bottom)), colors[str(classId)], thickness=2)

    cv.imshow('TensorFlow MobileNet-SSD', frame)
    if cv.waitKey(1) & 0xFF == ord('q'):
        break

vid.release()

cv.destroyAllWindows()