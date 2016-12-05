# LicensePlate+
Better to use before PointCheck. Load image from .set list file. Show lower part of it in the picturebox. Draw 5 keypoints on an Image. Save images and log files. Save key points locations in image file names.

# Usage
A tool to add 5 key points on an image, save a copy of the image with the x-y location as filename.

# What you need before testing the software
1.A folder that only include Images;  
2.A bat in the foler that write the 2 following lines:  
del dir.set   
dir .\*.jpg /s/b > dir.set   
which will delete the old dir.set and generate a new dir.set with all image directories in it.  

#What will the software do
1.Read the images from the dir.set and load the details into listview;  
2.Left click to draw key points, right click to cancel;  
3.Save the log files and output images into "D:\LicensePlate+_Log"  
