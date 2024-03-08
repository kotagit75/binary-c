#!/usr/bin/bash

cell_file=$1
eval_file=${cell_file%.*}.elf

./tool/cell2elf $cell_file $eval_file
./$eval_file
exit_flag="$?"
rm $eval_file

codn_n=$(cat $cell_file | jq -r '.codn_n' )

point=0
if [[ exit_flag -eq 0 ]]
then
  point=$((${point}+90))
  if [[ codn_n -eq 2 ]];
  then
    exit 100
  fi
else
  point=$((${point}+70))
fi

c_point=$((${codn_n} * -5))
if [[ c_point -gt 40 ]];
then
  c_point=40
fi
point=$((${point}+${c_point}))

exit $point