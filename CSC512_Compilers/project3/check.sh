#!/bin/sh 

> ./Tests/output

for f in ./Tests/*.c; do 
   > ${f}_output
   ./parser.exe $f | grep -v "<.*>" | tee $f_output >> ./Tests/output 2>&1;
done



echo "EXPECTED : " 
ls -ls ./Tests/*.c | grep Gregory | wc -l
echo "SAW : "
cat ./Tests/output | grep "===EXIT NORMAL!===" | wc -l
