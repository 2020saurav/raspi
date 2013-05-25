import cv2
import numpy as np

img = cv2.imread('tn11.jpg')

ORANGE_MIN = np.array([20, 100, 100],np.uint8)
ORANGE_MAX = np.array([35, 255, 255],np.uint8)

hsv_img = cv2.cvtColor(img,cv2.COLOR_BGR2HSV)

frame_threshed = cv2.inRange(hsv_img, ORANGE_MIN, ORANGE_MAX)
cv2.imwrite('output2.jpg', frame_threshed)