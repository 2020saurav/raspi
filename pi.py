'''
Idea : 

1. Take a picture.
2. Convert to its HSV and apply functions(erode, dilate, canny, smooth) to make it less noisy.
3. Create its CvMat and operate HoughCircle and then find radius and centre
4. ### MOVE THE BOT THROUGH CALCULATED ANGLE SUCH THAT BALL'S CENTRE COMES IN IMAGES' CENTRE WITH SOME ALLOWED ERROR ###
5. ### MOVE THE BOT AHEAD CALCULATED DISTANCE (subject to time of loop) ###
6. Repeat 1 though 5 (setting some experimentally determined time limit for each loop) till size of ball crosses an "experimental" threshold.

7. Orient the bot again (Step 4) and move (Step 5). This step would be more precise than above. Stop when distance of bot is such that 
  it can pick the ball.

'''


import cv
import cv2
import time
import numpy as np

GREEN_MIN = np.array([20, 100, 100],np.uint8)	#HSV values
GREEN_MAX = np.array([35, 255, 255],np.uint8)
HIGH = 80
LOW = 70
WAITKEY_DELAY_MS = 10
STOP_KEY = 'q'

 

camera_port = 0
camera = cv2.VideoCapture(camera_port)



 
def get_image():
 retval, im = camera.read()
 return im

'''for x in xrange(1,10):
	camera_capture = get_image()
	file = "/home/saurav/pi/img/img%d.jpg" %x
	cv2.imwrite(file, camera_capture)
	time.sleep(1)
	'''

img=get_image()
hsv_img = cv2.cvtColor(img,cv2.COLOR_BGR2HSV)
frame_threshed= cv2.inRange(hsv_img, GREEN_MIN, GREEN_MAX)
cv2.imwrite('output2.jpg', frame_threshed)
temp_img = cv.LoadImage('output2.jpg')

gray_scale = cv.CreateImage(cv.GetSize(temp_img), 8, 1)
processed = cv.CreateImage(cv.GetSize(temp_img), 8, 1)

cv.Smooth(temp_img, temp_img, cv.CV_GAUSSIAN, 3, 3) 
cv.CvtColor(temp_img, gray_scale, cv.CV_RGB2GRAY)

# do some processing on the gray scale image
cv.Erode(gray_scale, processed, None, 10)
cv.Dilate(processed, processed, None, 10)
cv.Canny(processed, processed, 5, 70, 3)
cv.Smooth(processed, processed, cv.CV_GAUSSIAN, 15, 15)

storage = cv.CreateMat(temp_img.width, 1, cv.CV_32FC3)
# these parameters need to be adjusted for every single image
try: 
    # extract circles
    cv.HoughCircles(processed, storage, cv.CV_HOUGH_GRADIENT, 2, 32.0, HIGH, LOW)
    for i in range(0, len(np.asarray(storage))):
    	print "circle #%d" %i
    	Radius = int(np.asarray(storage)[i][0][2])
    	print "Radius= %d" %Radius
    	x = int(np.asarray(storage)[i][0][0])
    	y = int(np.asarray(storage)[i][0][1])
    	center = (x, y)
    	# green dot on center and red circle around
    	cv.Circle(temp_img, center, 1, cv.CV_RGB(0, 255, 0), -1, 8, 0)
    	cv.Circle(temp_img, center, Radius, cv.CV_RGB(255, 0, 0), 3, 8, 0)
    	cv.Circle(processed, center, 1, cv.CV_RGB(0, 255, 0), -1, 8, 0)
    	cv.Circle(processed, center, Radius, cv.CV_RGB(255, 0, 0), 3, 8, 0)
except:
   	print "nothing found"
	pass

# show images
cv.ShowImage("image - press 'q' to quit", temp_img)
cv.ShowImage("post-process", processed)
#cv2.imwrite('tmpimg1.jpg', temp_img)
#cv2.imwrite('tmpimg2.jpg', processed)

    

'''
img = cv2.imread('tn11.jpg')
hsv_img = cv2.cvtColor(img,cv2.COLOR_BGR2HSV)

frame_threshed = cv2.inRange(hsv_img, GREEN_MIN, GREEN_MAX)
cv2.imwrite('output2.jpg', frame_threshed)

'''
del(camera)
