# this test needs fbset tool
FBSET=`which fbset`
test -z $FBSET && echo please enable fbset of busybox && exit 1
test ! -f $FBSET && echo please enable fbset of busybox && exit 1
test ! -x $FBSET && echo please enable fbset of busybox && exit 1

# in case we forgot to create device files
mknod /dev/fb0 c 29 0
mknod /dev/fb4 c 29 4

echo zoom 160,120 images to full screen

# setup image size
fbset -fb /dev/fb4 -g 160 120 160 240 16

echo 1 > /sys/devices/platform/ftlcdc200.0/zoom
echo "enable zoom (`cat /sys/devices/platform/ftlcdc200.0/zoom`)"

for file in 565_160x120_patterns/*
do
	if [ -f $file ]
	then
		cat $file > /dev/fb4

		# test pan
		for yoffset in 0 40 80 120
		do
			echo "0,$yoffset" > /sys/class/graphics/fb4/pan
			sleep 1
		done
	fi
done

echo 0 > /sys/devices/platform/ftlcdc200.0/zoom
echo "disable zoom (`cat /sys/devices/platform/ftlcdc200.0/zoom`)"

