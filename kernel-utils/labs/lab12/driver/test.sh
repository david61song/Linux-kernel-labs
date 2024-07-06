#!/bin/sh

dmesg -c > /dev/null
n=100
while [ $n -gt 0 ]; do
	./ioctl_test &
	n=$(($n-1))
done

sleep 2

irq_p=$(dmesg | grep IRQ | grep sum | awk 'START {point=0} $NF==-15 {point++} END {print point}')
ioc0_p=$(dmesg | grep IOCTL0 | awk 'START {point=0} $NF==15 {point++} END {print point}')
ioc1_p=$(dmesg | grep IOCTL1 | awk 'START {point=0} $NF==10 {point++} END {print point}')
ioc2_p=$(dmesg | grep IOCTL2 | awk 'START {point=0} $NF==25 {point++} END {print point}')

if [ -z $irq_p ]; then
	irq_p=0
fi
if [ -z $ioc0_p ]; then
	ioc0_p=0
fi
if [ -z $ioc1_p ]; then
	ioc1_p=0
fi
if [ -z $ioc2_p ]; then
	ioc2_p=0
fi
echo "expected IRQ  : -15, [$irq_p/100]" 
echo "expected IOCTL0: 15, [$ioc0_p/100]"
echo "expected IOCTL1: 10, [$ioc1_p/100]"
echo "expected IOCTL2: 25, [$ioc2_p/100]"

total=$(($irq_p + $ioc0_p + $ioc1_p + $ioc2_p))
echo "[Total Points = [$total/400]]"
