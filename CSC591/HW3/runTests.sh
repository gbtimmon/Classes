#!/bin/sh
JAR=${JAVA_WORK}/myHadoop.jar
RC=0
CPP_RUN="time -p ${CSC591}/cpp/bin/problem1.o "
CPP2_RUN="time -p ${CSC591}/cpp/bin/problem1_Optimized
CPP_OUT=${CSC591}/cpp_output
ST1_OUT=${CSC591}/hadoop_step_1_output
ST2_OUT=${CSC591}/hadoop_step_2_output
TME_OUT=${CSC591}/times

REDUCE_CNT=" -D mapred.reduce.tasks = 10 ";
MAP_CNT=" -D mapred.map.tasks = 10 ";

for graph in `ls $GRAPHS/*.graph`; do
   graph=`basename $graph`
   echo "##################################"
   echo "# $graph"
   echo "##################################"

   echo "-----------------"
   echo "STEP 1 : cpp test"
   echo "-----------------"
   echo "{"
   echo "    ${CPP_RUN} ${GRAPHS}/${graph} &> ${CPP_OUT}/${graph}_${RC}.output "
   echo "} &> ${TME_OUT}/${graph}_cpp_${RC}.time"

   #{ 
   #    ${CPP_RUN} ${GRAPHS}/${graph} &> ${CPP_OUT}/${graph}_${RC}.output 
   #} &> ${TME_OUT}/${graph}_${RC}.time
 
   echo "-----------------"
   echo "STEP 1 : cpp test"
   echo "-----------------"
   echo "{"
   echo "    ${CPP_RUN} ${GRAPHS}/${graph} &> ${CPP_OUT}/${graph}_${RC}.output "
   echo "} &> ${TME_OUT}/${graph}_cpp_${RC}.time"

   #{
   #    ${CPP_RUN} ${GRAPHS}/${graph} &> ${CPP_OUT}/${graph}_${RC}.output
   #} &> ${TME_OUT}/${graph}_${RC}.time

done


