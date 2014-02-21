#!/usr/bin/env python

import getopt
import Image
import sys
import time

imgdir = "resources/images/"
big_light     = Image.open(imgdir + "big_light.png")
big_medium    = Image.open(imgdir + "big_medium.png")
big_dark      = Image.open(imgdir + "big_dark.png")
small_light   = Image.open(imgdir + "small_light.png")
small_medium  = Image.open(imgdir + "small_medium.png")
small_dark    = Image.open(imgdir + "small_dark.png")
circle_light  = Image.open(imgdir + "circle_light.png")
circle_dark   = Image.open(imgdir + "circle_dark.png")

screenshot = Image.new("RGB", (144, 168), "black");

tick_time = time.localtime(time.time())

(hours, minutes, seconds) = (tick_time.tm_hour, tick_time.tm_min, tick_time.tm_sec)

(opts, args) = getopt.getopt(sys.argv[1:], "t:")

def usage():
    sys.stderr.write("create_snapshot [-t hh:mm:ss] filename\n")

for (opt, val) in opts:
    if opt == '-t':
	(hours, minutes, seconds) = [int(x) for x in val.split(":")]
    else:
	sys.stderr.write("Unknown option %d\n" % (opt,))
	usage()
	sys.exit(1)

if len(args) != 1:
    usage()
    sys.exit(1)

if seconds % 2 == 0:
    screenshot.paste( circle_light, ((144/2)-15, (168/6)-15) )
else:
    screenshot.paste( circle_dark,  ((144/2)-15, (168/6)-15) )


for row in range(2,6):
    if row == 4:
	num_columns = 11
        column_width = 8
        column_spacing = 5
    else:
	num_columns = 4
	column_width = 30
	column_spacing = 6

    if row == 2:
        mark = big_medium
	nomark = big_dark
	num_marks = hours/5
    elif row == 3:
        mark = big_medium
	nomark = big_dark
	num_marks = hours%5
    elif row == 4:
        mark = small_light
	mark_prime = small_medium
	nomark = small_dark
        num_marks = minutes/5
    elif row == 5:
        mark = big_light
	nomark = big_dark
	num_marks = minutes%5

    for column in range(num_columns):
	location = ( 144/2 - (num_columns*column_width + (num_columns-1)*column_spacing)/2 + column*(column_width+column_spacing),
	             (168/6) * row )

	if column < num_marks:
	    if row == 4:
		if (column % 3) == 2:
		    m = mark_prime
		else:
		    m = mark
	    else:
		m = mark
	else:
	    m = nomark

	screenshot.paste(m, location)

screenshot.save(args[0])


