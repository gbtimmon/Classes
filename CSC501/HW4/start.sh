MT=~/mnt
RT=~/rt
rm -fR ${MT}/*
rmdir  ${MT}
mkdir  ${MT}

rm -fR ${RT}/*
rmdir  ${RT} 
mkdir  ${RT}

./greg_fs ${MT} ${RT}
