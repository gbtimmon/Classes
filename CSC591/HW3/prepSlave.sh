#!/bin/bash
_HADOOP=/opt/hadoop/hadoop-1.2.1
_IP=`/sbin/ifconfig \
           | grep -Eo 'inet (addr:)?([0-9]*\.){3}[0-9]*' \
           | grep -Eo '([0-9]*\.){3}[0-9]*' \
           | grep -v '127.0.0.1' \
           | tail -1`
_ME=`whoami`

  if [ $# -ne 2 ]
  then
    echo "Usage: `basename $0` <master_ip> <slave_count>"
    exit 1
  fi

  echo "My IP : ${_IP}"
  echo "Masta's IP : ${1}";
  echo "Number of slaves : ${2}";
 
  sed -i "s/localhost/${1}/g" ${_HADOOP}/conf/core-site.xml
  sed -i "s/localhost/${1}/g" ${_HADOOP}/conf/mapred-site.xml
  sed -i "s/<value>1<\/value>/<value>${2}<\/value>/g" ${_HADOOP}/conf/hdfs-site.xml
  ssh ${_ME}@${1} "ssh-copy-id -i /home/${_ME}/.ssh/id_rsa.pub ${_ME}@${_IP}";

