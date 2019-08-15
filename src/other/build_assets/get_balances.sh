2> echo "get_balances.sh: " $1
ssh -i /Users/jrush/.ssh/id_rsa ssh://jrush@96.245.231.250:36963 'chifra export --balances --wei --fmt txt --noHeader '$1 >/Users/jrush/DropBox/Desktop/files/$1.bals.txt
2> echo "/Users/jrush/DropBox/Desktop/files/$1.bals.txt"
