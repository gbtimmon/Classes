#!/bin/sh

IN=data/in
OUT=data/out
EXEC=GED/GED2.py
A=GED
B=MGED

echo $EXEC $A "$IN/A/*" "$OUT/A_$A"
echo $EXEC $B "$IN/A/*" "$OUT/A_$B"
echo 
echo $EXEC $A "$IN/E/*" "$OUT/E_$A"
echo $EXEC $B "$IN/E/*" "$OUT/E_$B"
echo 
echo $EXEC $A "$IN/V/*" "$OUT/V_$A"
echo $EXEC $B "$IN/V/*" "$OUT/V_$B"
