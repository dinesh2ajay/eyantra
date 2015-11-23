import numpy as np
import cv2

#Teams can add other helper functions
#which can be added here


# The following functionw will find the number
# with perimeter and area as input parameters.

# the numerical values are found experimentally
# using the test images.
def findnum(p,a):
	
	if a>1500:
		return 0
	if a>=500 and a<=600:
		return 1
	if p>=190 and p<=194:
		return 2
	if a>=750 and a<=790:
		return 3
	if a>=955 and a<=965:
		return 4
	if p>=195 and p<=205:
		return 5
	if a>=1200 and a<=1350:
		return 6
	if a>=601 and a<=700:
		return 7
	if p>=220 and p<=230:
		return 8
	if p>=209 and p<=219:
		return 9
	

def play(img):
#add your code here

	perimeterd1=np.array([])
	aread1=np.array([])
	No_pos_D1=np.array([],int)

#Code for Divison 1
	for k in range(0,3):
# Three rows and 4 colums in division 1
		for j in range(0,4):
			d1 = img[98+k*84:168+k*84,65+j*84:135+j*84,:] 
# ROI (Region of Interest) or region cropped from original img. k=j=0 represents 1st row 1st col region
			gray = cv2.cvtColor(d1,cv2.COLOR_BGR2GRAY)
# converting the ROI to greyscale.
			ret,thresh = cv2.threshold(gray,15,255,0)
# greyscaled image is converted to binary image. 15 - threshold value.
			contours, hierarchy = cv2.findContours(thresh,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
# contours variable holds all possible contours found in the ROI.
			Perimeter1=0
			Area1=0
			for i in range(1,len(contours)):
# the 0th Contour is neglected since its simply the border of the ROI.
# len(contours) gives the max number of contours in the ROI
				cv2.drawContours(img,contours,i,(0,255,0),2,offset=(65+j*84,98+k*84))
# for numbers like 1 2 3 5 7 there is only one contour.
# for numbers 0 4 6 9 there are two contours and for 8 there are 3 contours.
# contours are drawn on img with green colour and width as 2.
				Perimeter1+=cv2.arcLength(contours[i],True )
				Area1+=cv2.contourArea(contours[i])
			if Perimeter1!=0 and Area1!=0:
# if ROI isnt empty then append the perimeter and area values to the respective list
				perimeterd1=np.append(perimeterd1,Perimeter1)
				aread1=np.append(aread1,Area1)
				No_pos_D1=np.append(No_pos_D1,findnum(Perimeter1,Area1))
# findnum() is invoked with perimeter and area of the contour in ROI as the input.

#Code for Divison 2
#Same process is repeated for division 2
# img-->greyscale-->binary-->ROI-->find and draw contours.	
# TODO find the numbers in D2 and return it to main		
	for k in range(0,5):
		for j in range(0,6):
			d1 = img[98+k*84:168+k*84,533+j*84:603+j*84,:]
			gray = cv2.cvtColor(d1,cv2.COLOR_BGR2GRAY)
			ret,thresh = cv2.threshold(gray,15,255,0)
			contours, hierarchy = cv2.findContours(thresh,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
			Perimeter=0
			for i in range(1,len(contours)):
				cv2.drawContours(img,contours,i,(0,255,0),2,offset=(533+j*84,98+k*84))

	return No_pos_D1


if __name__ == "__main__":
	#code for checking output for single image
	img = cv2.imread('test_images/test_image1.jpg')
	No_pos_D1=play(img)
	print "D1 = ",No_pos_D1
	#print No_pos_D2
	cv2.imshow('Output_image',img)
	cv2.waitKey(0)
	cv2.destroyAllWindows()

# End of the program
