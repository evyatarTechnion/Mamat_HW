#!/bin/bash

wget https://www.premierleague.com/players

FILE="/home/mamat/Mamat_HW/HW_1/players"
U_FILE="/home/mamat/Mamat_HW/HW_1/urls.txt"
UP_FILE="/home/mamat/Mamat_HW/HW_1/update_urls.txt"
OVERVIEW="/home/mamat/Mamat_HW/HW_1/overview"

while read line; do
	for word in $line; do
		echo $word | grep -oP '(\/players\/)([0-9])+(\/)([a-zA-Z-])+(\/overview)' >> urls.txt
	done
done <"$FILE"

sed  's/\/players/https:\/\/www\.premierleague\.com\/players/' $U_FILE > update_urls.txt

tot=`cat $UP_FILE | wc -l`
echo "Total Players: $tot" > result.txt

while read line; do
	wget $line
	counter=0
	while read line1; do
		for word in $line1; do
			if [[ "$word" =~ (midfielder|Midfielder) ]]; then
				(( counter++ ))
 			fi
		done
	done <"$OVERVIEW"
	if [[ $counter -gt 0 ]]; then
		echo "$line, midfielder, $counter" >> result.txt
	fi
	rm overview
done <"$UP_FILE"