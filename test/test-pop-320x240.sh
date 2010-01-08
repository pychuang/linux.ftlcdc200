# in case we forgot to create device files
mknod /dev/fb0 c 29 0
mknod /dev/fb1 c 29 1
mknod /dev/fb2 c 29 2
mknod /dev/fb3 c 29 3

# enable pop
echo pop = `cat /sys/devices/platform/ftlcdc200.0/pop`
echo 1 > /sys/devices/platform/ftlcdc200.0/pop
echo "enable pop (`cat /sys/devices/platform/ftlcdc200.0/pop`)"

state=0
for file in 565_160x120_patterns/*
do
	case $state in
	0)
		state=1
		cp $file /dev/fb0
		;;
	1)
		state=2
		cp $file /dev/fb1
		;;
	2)
		state=3
		cp $file /dev/fb3
		;;
	3)
		state=0
		cp $file /dev/fb2
		;;
	esac
	sleep 1
done

# disable pop
echo 0 > /sys/devices/platform/ftlcdc200.0/pop
echo "disable pop (`cat /sys/devices/platform/ftlcdc200.0/pop`)"

# clear background after test finished since fb0 are messed up by us
cp /dev/zero /dev/fb0
