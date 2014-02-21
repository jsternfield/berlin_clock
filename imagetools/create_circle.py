#!/usr/bin/env python

# Create the images for the Berlin Clock pebble app.

import Image
import ImageDraw
import math
import sys

size = ( 30 , 30 )
radius = 15
center = ([x/2 for x in size])

def dist(x, y):
    return math.sqrt((x-center[0])**2 + (y-center[1])**2)

PATTERNS = ( "light" , "dark" )

if __name__ == "__main__":
    # create the images
    for pattern in PATTERNS:
        im = Image.new("RGB", size, "black")
        draw = ImageDraw.Draw(im)
        for x in xrange(size[0]):
            for y in xrange(size[1]):
	        if pattern == "light":
	            if dist(x, y) < radius and ((x % 2) == 0 or ((y + x/2) % 2) == 0) :
		        draw.point((x,y), fill="white")
	        elif pattern == "dark":
	            if dist(x, y) < radius and (x % 2) == 1 and ((y + x/2) % 2) == 1:
		        draw.point((x,y), fill="white")
	        else:
		    sys.stderr.write("Unknown pattern %s" % (pattern,))
		    sys.exit(1)
        im.save("resources/images/%s_%s.png" % ("circle", pattern), "PNG")

   
    

