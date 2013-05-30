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

GREEN_MIN = np.array([20, 100, 100],np.uint8)	#HSV values : Needs to be configured to real-ball : Presently works for yellow balls.
GREEN_MAX = np.array([35, 255, 255],np.uint8)

HIGH = 80 # adjust this experimentally
LOW = 70

Rad = 40    # True Radius of Ball at "catching" distance from bot
Rad_eps=15  # The maximum deviation we choose to allow, to be chosen experiment based on size of Bot's picking mechanism

X = 320     # My webcam takes 640 x 480 image, so my center is 320,240
Y = 240
X_eps = 40     # Error that we allow while adjusting bot everytime

# No sense of moving bot/camera in Y direction : We got to fix it at Rad distance from ground
 

camera_port = 0 # my default Camera port
camera = cv2.VideoCapture(camera_port) 


def get_image():        # take a shot
 retval, im = camera.read()
 return im


def search():      # searches a ball, returns radius and coordinate of its center in picture frame. If (!found), returns everything as 0

    Radius=x=y=0

    img=get_image()
    
    hsv_img = cv2.cvtColor(img,cv2.COLOR_BGR2HSV)

    frame_threshed= cv2.inRange(hsv_img, GREEN_MIN, GREEN_MAX)

    cv2.imwrite('output2.jpg', frame_threshed)
    
    temp_img = cv.LoadImage('output2.jpg')
    # <Saurav> tried to use last 2 line as one, without making and output image and taking it back, but didn't seem to work!

    gray_scale = cv.CreateImage(cv.GetSize(temp_img), 8, 1)
    processed = cv.CreateImage(cv.GetSize(temp_img), 8, 1)

    cv.Smooth(temp_img, temp_img, cv.CV_GAUSSIAN, 3, 3) # please see what the parameters mean
    cv.CvtColor(temp_img, gray_scale, cv.CV_RGB2GRAY)
    cv.Erode(gray_scale, processed, None, 10)
    cv.Dilate(processed, processed, None, 10)
    cv.Canny(processed, processed, 5, 70, 3)
    cv.Smooth(processed, processed, cv.CV_GAUSSIAN, 15, 15)

    ''' 
        These parameters ( or more image transforming functions ) should be adjusted carefully to reduce maximum noise and be tested under different light conditions 
        and different background conditions
    '''


    storage = cv.CreateMat(temp_img.width, 1, cv.CV_32FC3)
    
    try: 
        
        cv.HoughCircles(processed, storage, cv.CV_HOUGH_GRADIENT, 2, 32.0, HIGH, LOW)

       '''   If bot confuses in **MANY  BALL  SITUATION** : Do something here (below):
            Presently it scans all circles ( I gave only 1 in it) and keeps the last one

            Suggest something for many ball situation.
            Need not worry about different colored balls, already taken care of in line 66-71

        '''
        for i in range(0, len(np.asarray(storage))):
        #	print "circle #%d" %i
            Radius = int(np.asarray(storage)[i][0][2])
        #	print "Radius= %d" %Radius
            x = int(np.asarray(storage)[i][0][0])
            y = int(np.asarray(storage)[i][0][1])
        #	center = (x, y)

        #    print "Center= %d,%d" %(x,y)
        	# green dot on center and red circle around
        #    cv.Circle(temp_img, center, 1, cv.CV_RGB(0, 255, 0), -1, 8, 0)
        #	cv.Circle(temp_img, center, Radius, cv.CV_RGB(255, 0, 0), 3, 8, 0)
        #	cv.Circle(processed, center, 1, cv.CV_RGB(0, 255, 0), -1, 8, 0)
        #	cv.Circle(processed, center, Radius, cv.CV_RGB(255, 0, 0), 3, 8, 0)
    except:
       	#print "nothing found"
    	pass
    return Radius,x,y

    # show images
    #cv.ShowImage("image - press 'q' to quit", temp_img)
    #cv.ShowImage("post-process", processed)
    #cv2.imshow('Temp Image', temp_img)
    #cv2.imshow('Processed', processed)
    #cv2.waitKey(0)
    #cv2.destroyAllWindows()

        
def in_range(Radius,x,y):   # Checks if presently obtained data are in specified ranges or not
    if( Radius>Rad+Rad_eps or Radius < Rad-Rad_eps):
        return 0
    if(x>X+X_eps or x < X-X_eps):
        return 0
    return 1

def adjust(Radius,x,y):   # Dictates the motion of bot. Presently configured to test static camera (laptop) and moving ball (in hand)
    if( Radius>Rad+Rad_eps):
        print "Go away!"
    elif (Radius < Rad-Rad_eps):
        print "Come near!"
    elif (x>X+X_eps):
        print "Move Right!" # Camera inverts images laterally
    elif (x < X-X_eps):
        print "Move Left!"
    return

    '''
    We need some strategy on how much to move on Go away or come near . We have values of present x, we need to know bot's constants and pass appropriate (within error limits) current for "t" time.
    
    In this situation, we won't need the bot to rotate to make the ball's image come to center. Just two-side motion will suffice. It will be slow!
    Suggest ideas for this (e.g. we can pass current for "t1" time to rotate by some angle... )
    In case, the image goes out of canvas, we rotate by 30 degrees ( which side? :P ... this can be embarrasing in present scheme, if you know what I mean)
   
    '''





Radius,x,y =search()    #STARTING!
turns=0
#print "Radius=%d" %Radius
#print "Center=(%d,%d)" %(x,y)

while(Radius==0 and  turns!=12):  
    turns+=1
    print "No circlular object detected in this frame. Rotating bot by 30 degrees..."
    # <Saurav> Never got this printed! Code exits with some error. Most probably because of imperfect noise-filtering, and wall on my back is yellow :P
    # Rotate_bot(30) : <Saurav> : I have no idea how to code it. Need to understand GPIO of Raspberry Pi. 
    Radius,x,y =search()

else:
    while(in_range(Radius,x,y)==0):
        adjust(Radius,x,y)
        time.sleep(0.2) # time delay for bot (presently you are the bot) to move
        Radius,x,y =search()

    else:
        print "Congrats! You have reached destination. Pick it up :D"

del(camera)
