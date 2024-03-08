#!/usr/bin/bash

cell_file=$1
eval_file=${cell_file%.*}.elf
tmp=tmp

./tool/cell2elf $cell_file $eval_file
./$eval_file > $tmp
exit_flag="$?"
rm $eval_file

codn_n=$(cat $cell_file | jq -r '.codn_n' )

if [[ codn_n -le 0  ]];
then
  exit 200
fi

point=0
if [[ exit_flag -eq 0 ]]
then
  point=$((${point}+30))
else
  exit 10
fi

put=$(<$tmp)
if [[ "$put" =~ "A" ]];
then
  point=90

  if [[ exit_flag -eq 0  ]];
  then
    if [[ codn_n -eq 10  ]];
    then
      echo -n a
      exit 97
    fi
    if [[ codn_n -eq 9  ]];
    then
      echo -n b
      exit 99
    fi
    if [[ codn_n -le 8  ]];
    then
      exit 100
    fi
  fi
else
  exit 200
fi

c_point=$((${codn_n} * -1))
point=$((${point}+${c_point}))


exit $point