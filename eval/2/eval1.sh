#!/usr/bin/bash

org_file=$1
eval_file=./tmp/a.elf
tmp=./tmp/tmp

./tool/org2elf $org_file $eval_file
codn_n="$?"
if [[ codn_n -eq 0 ]]
then
  echo 0
  exit 0
fi
$eval_file > $tmp
exit_flag="$?"
rm $eval_file

point=$((${codn_n} * 3))

if [[ point -ge 80 ]]
then
  point=80
fi
if [[ exit_flag -eq 0 ]]
then
  point=100
fi

echo $point
exit $point

for ((i=1; i<=$point; i++))
do
    echo -n "a"
done

echo "*"