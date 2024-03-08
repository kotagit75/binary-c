comp="comp"
if [ "$1" = "$comp" ];
then
	watch -d=permanent ls ./compound
else
	watch -d=permanent ls ./cell
fi
