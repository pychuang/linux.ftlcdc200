# in case we forgot to create device files
mknod /dev/fb0 c 29 0

for file in 565_320x240_patterns/*
do
	if [ -f $file ]
	then
		cat $file > /dev/fb0
		sleep 1
	fi
done

