#!/bin/bash

# create html file - players
wget https://www.premierleague.com/players

# search for all players link, all links will be in urls.txt file
grep -oP '(\/players\/)([0-9])+(\/)([a-zA-Z-])+(\/overview)' players > urls.txt


# update all links found to currect format - into update_urls.txt file
sed -i 's/\/players/https:\/\/www\.premierleague\.com\/players/' urls.txt

# initialize the output file with total count of players fount - 
# player_results.csv
tot=`cat urls.txt | wc -l`
echo "Total players: $tot" > player_results.csv

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
done <$'urls.txt'

rm players
rm urls.txt
