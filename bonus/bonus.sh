#!/bin/bash
while [[ 1 ]]; do

sleep 1m
curl wttr.in > weather.txt

mor_rain=`cat weather.txt | head -n16 | tail -1 | awk -F'|' '{print $2 }' | \
		 awk '{print $1}' | cut -d% -f1`
non_rain=`cat weather.txt | head -n16 | tail -1 | awk -F'|' '{print $2 }' | \
		 awk '{print $1}' | cut -d% -f1`

#temp=`cat weather.txt | head -n16 | tail -1 | awk -F'|' '{print $2 }' | \
#		 awk '{print $1}' | cut -d% -f1`

ambrella=0

if [[ $mor_rain -gt 50 || $non_rain -gt 50 ]]; then
	(( ambrella++ ))
fi

temp=`curl wttr.in/?format=3 | sed 's/[^0-9]//g'`
#echo $temp
if [[ $temp -gt 13 ]] && [[ $temp -lt 22 ]]; then 
	echo "One coat is needed" > output.txt 
elif [[ $temp -gt 22 ]]; then 
	echo "No coat is needed" > output.txt
else 
	echo "It is recommended to take a jacket coat and hat" > output.txt
fi

if [[ $ambrella -eq 1 ]]; then 
	echo "It is recommended to take an umbrella today" >> output.txt
fi

rm Haifa.png
wget wttr.in/Haifa.png

cat output.txt | mail -s "Weather Updates" -A Haifa.png evyatar100nahum@gmail.com

done