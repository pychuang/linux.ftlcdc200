# in case we forgot to create device files
mknod /dev/fb0 c 29 0

echo "320,480" > /sys/class/graphics/fb0/virtual_size

for file in 565_320x240_patterns/*
do
	if [ -f $file ]
	then
		cp $file /dev/fb0
		sleep 1
	fi
done
