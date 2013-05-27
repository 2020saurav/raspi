raspi
=====

(Edit) : The file pi.py contains this.

We will be using OpenCV to detect green ball (tennis ball) and the code and camera will guide the bot to reach the ball.

1. We shall convert image to HSV plane using cvtColor() function
2. Extract green color from it using inRange() function
3. Apply erosion and dilation to avoid noise using erode() and dilate() functions
4. Find Contours using findContours() function
5. Draw the contours using drawContours() function.
6. Find the apparent radius of ball and calculate the distance required for the bot to move.

Then comes the job to instruct the bot.


