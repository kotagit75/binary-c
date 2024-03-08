#!/usr/bin/bash

cell_file=$1
eval_file=${cell_file%.*}.elf
tmp=./tmp/tmp

./tool/cell2elf $cell_file $eval_file
sudo ./$eval_file > $tmp
exit_flag="$?"
rm $eval_file

point=0
if [[ exit_flag -eq 0 ]]
then
  point=$((${point}+50))
else
  point=$((${point}+30))
fi
codn_n=$(cat $cell_file | jq -r '.codn_n' )
c_point=$((${codn_n} * 6))
if [[ c_point -gt 40 ]];
then
c_point=20
fi
point=$((${point}+${c_point}))


put=$(<$tmp)
if [[ "$put" =~ "A" ]];
then
  echo put
  point=95
  if [[ exit_flag -eq 0  ]];
  then
    point=100
  fi
else if [[ "$put" != "$null" ]];
then
  echo puto
  point=99
fi
fi

echo -n "ev:$point"

exit $point