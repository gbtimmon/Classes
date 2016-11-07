#!/bin/sh 

> ./Tests/output

for f in ./Tests/*.c; do 
   ./parser.exe $f | grep -v "<.*>" >> ./Tests/output 2>&1;
done



echo "EXPECTED : " 
ls -ls ./Tests/*.c | grep Gregory | wc -l
echo "SAW : "
cat ./Tests/output | grep "===EXIT NORMAL!===" | wc -l
