# in case we forgot to create device files
mknod /dev/fb0 c 29 0

echo "320,480" > /sys/class/graphics/fb0/virtual_size

cat 565_320x240_patterns/01_565_320x240.bmp.bin > /dev/fb0

for yoffset in 0 20 40 60 80 100 120 140 160 180 200 220 240 0
do
	echo "0,$yoffset" > /sys/class/graphics/fb0/pan
	sleep 1
done

