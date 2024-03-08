#!/usr/bin/bash

cell_file=$1
eval_file=${cell_file%.*}.elf

codn_n=$(cat $cell_file | jq -r '.codn_n' )
if [[ codn_n -eq 0 ]]
then
  exit 0
fi

./cell2elf $cell_file $eval_file
sudo ./$eval_file
exit_flag="$?"
rm $eval_file

point=0
if [[ exit_flag -eq 0 ]]
then
  point=100
else
  point=17
fi

exit $point