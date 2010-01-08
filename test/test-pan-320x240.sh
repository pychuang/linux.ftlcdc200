# in case we forgot to create device files
mknod /dev/fb0 c 29 0

echo "320,480" > /sys/class/graphics/fb0/virtual_size

for file in 565_320x240_patterns/*
do
	if [ -f $file ]
	then
		cp $file /dev/fb0

		for yoffset in 0 20 40 60 80 100 120 140 160 180 200 220 240 0
		do
			echo "0,$yoffset" > /sys/class/graphics/fb0/pan
		done
		sleep 1
	fi
done

