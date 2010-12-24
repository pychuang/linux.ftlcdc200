# this test needs fbset tool
FBSET=`which fbset`
test -z $FBSET && echo please enable fbset of busybox && exit 1
test ! -f $FBSET && echo please enable fbset of busybox && exit 1
test ! -x $FBSET && echo please enable fbset of busybox && exit 1

# in case we forgot to create device files
mknod /dev/fb0 c 29 0
mknod /dev/fb1 c 29 1
mknod /dev/fb2 c 29 2

echo "320,240" > /sys/class/graphics/fb0/virtual_size

# paste background
cat 565_320x240_patterns/01_565_320x240.bmp.bin > /dev/fb0

# setup image 1 size
fbset -fb /dev/fb1 -g 80 80 80 160 16

# prepare image 1
cat /dev/zero > /dev/fb1
cat 565_80x80_patterns/10_565_80x80.bmp.bin > /dev/fb1

# setup image 2 size
fbset -fb /dev/fb2 -g 80 80 80 160 16

# prepare image 2
cat /dev/zero > /dev/fb2
cat 565_80x80_patterns/11_565_80x80.bmp.bin > /dev/fb2

# enable single pip
echo pip = `cat /sys/devices/platform/ftlcdc200.0/pip`
echo 1 > /sys/devices/platform/ftlcdc200.0/pip
echo "enable single pip (`cat /sys/devices/platform/ftlcdc200.0/pip`)"

# pip position
echo 0,0 > /sys/class/graphics/fb1/position
echo position = `cat /sys/class/graphics/fb1/position`
sleep 2
echo 100,40 > /sys/class/graphics/fb1/position
echo move position to `cat /sys/class/graphics/fb1/position`

# enable double pip
echo 2 > /sys/devices/platform/ftlcdc200.0/pip
echo "enable double pip (`cat /sys/devices/platform/ftlcdc200.0/pip`)"

# pip position
echo 0,0 > /sys/class/graphics/fb2/position
echo position = `cat /sys/class/graphics/fb2/position`
sleep 2
echo 140,60 > /sys/class/graphics/fb2/position
echo move position to `cat /sys/class/graphics/fb2/position`

# set blend1 value
echo blend1 = `cat /sys/devices/platform/ftlcdc200.0/blend1`
for i in 0 2 4 6 8 10 12 14 16
do
	sleep 1
	echo $i > /sys/devices/platform/ftlcdc200.0/blend1
	echo set blend1 to `cat /sys/devices/platform/ftlcdc200.0/blend1`
done

# set blend2 value
echo blend2 = `cat /sys/devices/platform/ftlcdc200.0/blend2`
for i in 0 2 4 6 8 10 12 14 16
do
	sleep 1
	echo $i > /sys/devices/platform/ftlcdc200.0/blend2
	echo set blend2 to `cat /sys/devices/platform/ftlcdc200.0/blend2`
done

# pan sub images
for yoffset in 0 20 40 60 80 0
do
	echo "0,$yoffset" > /sys/class/graphics/fb1/pan
	sleep 1
	echo "0,$yoffset" > /sys/class/graphics/fb2/pan
	sleep 1
done

# disable double pip
echo 0 > /sys/devices/platform/ftlcdc200.0/pip
echo "disable double pip (`cat /sys/devices/platform/ftlcdc200.0/pip`)"

