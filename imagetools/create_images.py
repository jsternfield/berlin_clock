#!/usr/bin/env python

# Create the images for the Berlin Clock pebble app.

import Image
import ImageDraw
import sys

BIG_SIZE = ( 30 , 20 , "big" )
LITTLE_SIZE = ( 8 , 20 , "small" )

PATTERNS = ( "light" , "medium" , "dark" )

if __name__ == "__main__":
    # create the images
    for size in (BIG_SIZE, LITTLE_SIZE):
        for pattern in PATTERNS:
	    im = Image.new("RGB", size[:2], "black")
	    draw = ImageDraw.Draw(im)
	    for x in xrange(size[0]):
	        for y in xrange(size[1]):
		    if pattern == "light":
		        if True or (x % 2) == 0 or ((y + x/2) % 2) == 0 :
			    draw.point((x,y), fill="white")
		    elif pattern == "medium":
		        if ((x + y) % 2) == 1 :
			    draw.point((x,y), fill="white")
		    elif pattern == "dark":
		        if (x % 2) == 1 and ((y + x/2) % 2) == 1:
			    pass
			    # draw.point((x,y), fill="white")
		    else:
			sys.stderr.write("Unknown pattern %s" % (pattern,))
			sys.exit(1)
	    im.save("resources/images/%s_%s.png" % (size[2], pattern), "PNG")

   
    

