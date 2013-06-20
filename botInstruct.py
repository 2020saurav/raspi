#import our i2c libraries to be able to speak to the chip
#ASSUMING THEY HAVE BEEN PROPERLY INSTALLED
#this code is just sapmle similar code after proper varification will be put into our program
import i2c
import RPi.GPIO as GPIO

#assuming 12,13 contors left wheel , and 14,15 controls the right wheel

def right:
	#rotating left wheel forward
	GPIO.output(12, True)
	GPIO.output(13, False)
	#rotating right wheel backward
	GPIO.output(14, False)
	GPIO.output(15, True)
	#time of sleep to be determined experimentally
	time.sleep(0.2)
	GPIO.cleanup()
	return 1

def left:
	#rotating left wheel backward
	GPIO.output(12, False)
	GPIO.output(13, True)
	#rotating right wheel forward
	GPIO.output(14, True)
	GPIO.output(15, False)
	#time of sleep to be determined experimentally
	time.sleep(0.2)
	GPIO.cleanup()
	return 1

def forward:
	#rotating both wheel forward
	GPIO.output(12, True)
	GPIO.output(13, False)
	GPIO.output(14, True)
	GPIO.output(15, False)
	time.sleep(0.2)
	GPIO.cleanup()
	return 1

def backwards:
	#rotating both wheel backwards
	GPIO.output(12, False)
	GPIO.output(13, True)
	GPIO.output(14, False)
	GPIO.output(15, True)
	time.sleep(0.2)
	GPIO.cleanup()
	return 1

def reached:
	#some code to power up 3rd motor(CATHER)
	#we can handle only 2 motors with motor driver i have seen
	#we will have to order specific motor driver
	#general motor driver requires 5V but PI gives 3.3V
	#some code to make bot come back to original position
	#NOW TERMINATE THE PROGRAM

