k=5;
while [[ $k -gt 0  ]]; do

	#echo -e '\n';
i=0;
while [[ $i -lt 4 ]]; do
	#echo $i is up;
echo 1 > /sys/class/leds/beaglebone:green:usr$i/brightness;
((i++));
sleep 0.01;
done;

i=0;
while [[ $i -lt 4 ]]; do
	#echo $i is down;
echo 0 > /sys/class/leds/beaglebone:green:usr$i/brightness;
((i++));
sleep 0.01;
done;

((i--));

while [[ $i -gt -1 ]]; do
	#echo $i is up;
echo 1 > /sys/class/leds/beaglebone:green:usr$i/brightness;
((i--));
sleep 0.01;
done;

i=3;
while [[ $i -gt -1 ]]; do
	#echo $i:is down;
echo 0 > /sys/class/leds/beaglebone:green:usr$i/brightness;
((i--));
sleep 0.01;
done;
	#echo -e '\n';

	((k--));
done;
