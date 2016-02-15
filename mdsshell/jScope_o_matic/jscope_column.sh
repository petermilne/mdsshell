#!/usr/local/bin/bash

# plot char as a function of ROW, NUMEXPR
# normally, either ROW==1 or NUMEXPR==1
# no support for multicolumn
# ./jscope_column.sh # default 16 rows, channel per row
# ROWS=1 NUMEXPR=16 ./jscope_column.sh # one plot, 16 channels
# ROWS=16 NUMEXPR=8 ./jscope_column.sh # nice for ACQ196?
CONFIG=${1:-column.jscp}
ROWS=${ROWS:-16}
COLS=${COLS:-1}
NUMEXPR=${NUMEXPR:-1}
SERVER_CLASS=${SERVER_CLASS:-MdsDataProvider}
SERVER=${SERVER:-kilda}
SERVER_PORT=${SERVER_PORT:-8000}
TREE=${TREE:-z2}
SHOT=${SHOT:-0}
FIELDFMT=${FIELDFMT:-"ACQ216_023:CH%02d[1:32768]"}


cat - <<EOF 


Scope.geometry: 750x1168+801+6
Scope.update.disable: false
Scope.update.disable_when_icon: true
Scope.font: java.awt.Font[family=Dialog,name=Times,style=plain,size=14]


Scope.color_0: Black,java.awt.Color[r=0,g=0,b=0]
Scope.color_1: Brown,java.awt.Color[r=120,g=100,b=60]
Scope.color_2: Red,java.awt.Color[r=255,g=0,b=0]
Scope.color_3: Orange,java.awt.Color[r=255,g=200,b=0]
Scope.color_4: Yellow,java.awt.Color[r=255,g=2255,b=0]
Scope.color_5: Green,java.awt.Color[r=0,g=255,b=0]
Scope.color_6: Blue,java.awt.Color[r=0,g=255,b=0]
Scope.color_7: Magenta,java.awt.Color[r=255,g=0,b=255]
Scope.color_8: Orange,java.awt.Color[r=255,g=200,b=0]
Scope.color_9: Gray,java.awt.Color[r=100,g=100,b=100]
Scope.color_10: Black,java.awt.Color[r=0,g=0,b=0]
Scope.color_11: Black,java.awt.Color[r=0,g=0,b=0]
Scope.color_12: Black,java.awt.Color[r=0,g=0,b=0]
Scope.color_13: Black,java.awt.Color[r=0,g=0,b=0]
Scope.color_14: Black,java.awt.Color[r=0,g=0,b=0]
Scope.color_15: Black,java.awt.Color[r=0,g=0,b=0]
Scope.color_15: Black,java.awt.Color[r=0,g=0,b=0]
EOF

if [ "$SERVER_CLASS" = "LocalDataProvider" ]
then
cat - <<EOF
Scope.data_server_name: Local
Scope.data_server_class: LocalDataProvider	
EOF
else
cat - <<EOF
Scope.data_server_name: ${SERVER}
Scope.data_server_class: ${SERVER_CLASS}
Scope.data_server_argument: ${SERVER_PORT}
EOF
fi

cat - <<EOF
Scope.fast_network_access: false
Scope.reversed: false

Scope.global_1_1.horizontal_offset: 0
Scope.global_1_1.vertical_offset: 0

Scope.columns: ${COLS}
EOF

let COL=1
let ch=1

while [ $COL -le $COLS ]
do
  echo Scope.rows_in_column_$COL: $ROWS
  
  let ROW=1
  while [ $ROW -le $ROWS ]
    do
    
    . ./scope_plot.sh

    let exp=1
    while [ $exp -le $NUMEXPR ]
      do
      mds_exp=`printf $FIELDFMT $ch` 
      if [ $exp -eq 1 ]
      then
	  if [ $NUMEXPR -gt 1 ]
	  then
	      dotdot='...'
	  else
	      dotdot=
	  fi
	  echo Scope.plot_${ROW}_${COL}.title: \'$mds_exp $dotdot\'
      fi
      expression $exp $mds_exp
      let exp="$exp+1"
      let ch="$ch+1"
    done

    echo;echo
    let ROW="$ROW+1"
  done
# not sure what this is ...
  echo Scope.vpane_$COL: 500
  let COL="$COL+1"
done



