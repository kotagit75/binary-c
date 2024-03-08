#!/usr/bin/bash

org_file=$1
eval_file=./tmp/a.elf
tmp=./tmp/tmp

./tool/org2elf $org_file $eval_file
codn_n=$?
if [[ $codn_n -eq 0 ]];
then
  exit 200
fi
$eval_file > $tmp
exit_flag="$?"
rm $eval_file

point=$((${codn_n} * 5))
if [[ exit_flag -eq 0 ]]
then
  point=100
fi

for ((i=1; i<=$point; i++))
do
    echo -n "a"
done

echo "*"

exit $point