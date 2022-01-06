#save all packets 
packets_in=`cat`
output=""

while read rule
do
	#delete spaces,empty lines and comments
	rule=`echo "$rule" |sed 's/ //g' | sed '/^$/d' | grep -o '^[^#]*'`
	if [[ -n $rule ]]; then
		#turn the rule into filters
		readarray -d , -t check <<< "$rule"
		#check packets
		packets_out=`echo "$packets_in" | \
		./firewall.exe "${check[0]}" | \
		./firewall.exe "${check[1]}" | \
		./firewall.exe "${check[2]}" | \
		./firewall.exe "${check[3]}"`
		#create output
		#add the passed packets to output
		output+=`echo "${packets_out}" | sed '/^$/d' | sort -u `
		output+="\n"
	fi	
done < "$1"

echo -e "${output}" | sed '/^$/d' | sed 's/ //g' | sort -u