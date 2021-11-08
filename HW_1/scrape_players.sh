#!/bin/bash

# create html file - players
wget https://www.premierleague.com/players

# search for all players link, all links will be in urls.txt file
while read line; do
	for word in $line; do
		echo $word | grep -oP '(\/players\/)([0-9])+(\/)([a-zA-Z-])+(\/overview)' >> urls.txt
	done
done <$'players'

# update all links found to currect format - into update_urls.txt file
sed  's/\/players/https:\/\/www\.premierleague\.com\/players/' $'urls.txt' > update_urls.txt

# initialize the uouput file with total count of players fount - player_results.csv
tot=`cat $'update_urls.txt' | wc -l`
echo "Total Players: $tot" > player_results.csv

# search in all players links for midfielder keyword.
while read line; do
	wget $line
	counter=0
	while read line1; do
		for word in $line1; do
			if [[ "$word" =~ (midfielder|Midfielder) ]]; then
				(( counter++ ))
 			fi
		done
	done <$'overview'
	# if the keywork found at least one time - save the player info.
	if [[ $counter -gt 0 ]]; then
		echo "$line, midfielder, $counter" >> player_results.csv
	fi
	rm overview
done <$'update_urls.txt'