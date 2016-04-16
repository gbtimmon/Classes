echo BEFORE
ps -ef | grep greg_fs | grep -v grep 
ps -ef | grep greg_fs | grep -v grep | awk '{print $2}' | xargs kill -9
echo AFTER
ps -ef | grep greg_fs | grep -v grep 
