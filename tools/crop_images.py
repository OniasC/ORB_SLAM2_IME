import cv2
import numpy as np
import sys
import argparse
import os, os.path


def main() :
    parser = argparse.ArgumentParser()
    parser.add_argument("originPath", help="path of origin folder")
    parser.add_argument("destPath", help="path of destination folder")
    parser.parse_args()
    args = parser.parse_args()

    print(args.destPath)
    print(args.originPath)
    if (os.path.exists(args.destPath) == False):
        os.mkdir(args.destPath, 0o777)


    for file in os.listdir(args.originPath):
        img = cv2.imread(args.originPath + "/" +file)
        (x, y, _) = img.shape # Print image shape
        #print(x,y)
        newX = 0.9*x
        newY = 0.9*y
        centerImg = (x/2, y/2)
        #print(0.05*x, 0.95*x, round(0.05*y), round(0.95*y))
        #x_lowerbound = 0.05*x
        # Cropping an image
        
        cropped_image = img[int(0.05*x):int(0.95*x), int(round(0.05*y)):int(round(0.95*y))]

        # Save the cropped image
        destinationFile = args.destPath + "/" + file
        #print(destinationFile)
        cv2.imwrite(destinationFile, cropped_image)
        #break

'''
    img = cv2.imread('test.jpg')
    print(img.shape) # Print image shape
    cv2.imshow("original", img)
    
    # Cropping an image
    cropped_image = img[80:280, 150:330]
    
    # Display cropped image
    cv2.imshow("cropped", cropped_image)
    
    # Save the cropped image
    cv2.imwrite("Cropped Image.jpg", cropped_image)
    
    cv2.waitKey(0)
    cv2.destroyAllWindows()
'''

if __name__ == "__main__":
    main()