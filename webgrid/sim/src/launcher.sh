#!/bin/bash
# This is launcher for Stackelberg Game experiments

EXPECTED_ARGS="6"

USAGE="USAGE: ./launcher.sh min/GA  sec/ris util/nonutil exec/nonexec abort/nonabort  MB_STATIC_SMALL/..."

if [ $# -ne $EXPECTED_ARGS ]; then
	echo $USAGE
	exit
fi


###################################################
#       Parameters set through command line
#              DON'T CHANGE HERE
#                (no effect)
###################################################


ENABLE_GAME="0"
SECURITY="0"
QLU="1"
QLE="1"
QLAB="1"

###################################################
#             Additional parameters
#             Free to change here
#             Will be aplied to all modes 
#             (static/dynamic/small/large)
###################################################

SECURITY_EXTRA="20"
ABORT_EXTRA="50"
CALC_PENALTIES="0"
EXEC="Debug"










if [ $1 == "min" ]; then
	ENABLE_GAME="1"
elif [ $1 == "GA" ]; then
	ENABLE_GAME="0"
else
	echo $USAGE
	exit
fi

if [ $2 == "sec" ]; then
	SECURITY="1"
elif [ $2 == "ris" ]; then
	SECURITY="0"
else
	echo $USAGE
	exit
fi

if [ $3 == "util" ]; then
	QLU="1"
elif [ $3 == "nonutil" ]; then
	QLU="0"
else
	echo $USAGE
	exit
fi

if [ $4 == "exec" ]; then
	QLE="1"
elif [ $4 == "nonexec" ]; then
	QLE="0"
else
	echo $USAGE
	exit
fi

if [ $5 == "abort" ]; then
	QLAB="1"
elif [ $5 == "nonabort" ]; then
	QLAB="0"
else
	echo $USAGE
	exit
fi



if [ $6 == "MB_TEST" ]; then
#from dynamic_small


./$EXEC/sim \
--ttasks "512" \
--itasks "384" \
--iatime "e(7812.50)" \
--workload "n(250000000,43750000)" \
--ihosts      "32" \
--mips        "n(1000,175)" \
--addhost     "n(625000,93750)" \
--delhost     "n(750000,112500)" \
--minhosts    "27" \
--maxhosts    "37" \
--strategy    "HGS_scheduler(5,h)" \
--activate    "resource_and_time_interval(c(250000))" \
--hostselect  "all" \
--taskselect  "all" \
--allocpolicy "fcfs" \
--nruns       "2" \
--ga_enable_game $ENABLE_GAME \
--ga_security    $SECURITY \
--ga_calc_penalty  $CALC_PENALTIES \
--ga_enable_qlu $QLU \
--ga_enable_qle $QLE \
--ga_enable_qlab $QLAB \
--ga_security_extra $SECURITY_EXTRA \
--ga_abort_extra $ABORT_EXTRA \
2>$1 | tee raport_$6_$1_$2_$3_$4_$5 \

fi


if [ $6 == "MB_STATIC_SMALL" ]; then

./$EXEC/sim \
--ttasks "512" \
--itasks "512" \
--iatime "c(9999999999)" \
--workload "n(250000000,43750000)" \
--ihosts      "32" \
--mips        "n(1000,175)" \
--addhost     "c(9999999999)" \
--delhost     "c(9999999999)" \
--minhosts    "32" \
--maxhosts    "32" \
--strategy    "SG_scheduler(5,h)" \
--activate    "resource_and_time_interval(c(9999999999))" \
--hostselect  "all" \
--taskselect  "all" \
--allocpolicy "fcfs" \
--nruns       "20" \
--ga_enable_game $ENABLE_GAME \
--ga_security    $SECURITY \
--ga_calc_penalty  $CALC_PENALTIES \
--ga_enable_qlu $QLU \
--ga_enable_qle $QLE \
--ga_enable_qlab $QLAB \
--ga_security_extra $SECURITY_EXTRA \
--ga_abort_extra $ABORT_EXTRA \
2>$1 | tee raport_$6_$1_$2_$3_$4_$5 \

fi


if [ $6 == "MB_STATIC_MEDIUM" ]; then

./$EXEC/sim \
--ttasks "1024" \
--itasks "1024" \
--iatime "c(9999999999)" \
--workload "n(250000000,43750000)" \
--ihosts      "64" \
--mips        "n(1000,175)" \
--addhost     "c(9999999999)" \
--delhost     "c(9999999999)" \
--minhosts    "64" \
--maxhosts    "64" \
--strategy    "SG_scheduler(5,h)" \
--activate    "resource_and_time_interval(c(9999999999))" \
--hostselect  "all" \
--taskselect  "all" \
--allocpolicy "fcfs" \
--nruns       "20" \
--ga_enable_game $ENABLE_GAME \
--ga_security    $SECURITY \
--ga_calc_penalty  $CALC_PENALTIES \
--ga_enable_qlu $QLU \
--ga_enable_qle $QLE \
--ga_enable_qlab $QLAB \
--ga_security_extra $SECURITY_EXTRA \
--ga_abort_extra $ABORT_EXTRA \
2>$1 | tee raport_$6_$1_$2_$3_$4_$5 \

fi


if [ $6 == "MB_STATIC_LARGE" ]; then

./$EXEC/sim \
--ttasks "2048" \
--itasks "2048" \
--iatime "c(9999999999)" \
--workload "n(250000000,43750000)" \
--ihosts      "128" \
--mips        "n(1000,175)" \
--addhost     "c(9999999999)" \
--delhost     "c(9999999999)" \
--minhosts    "128" \
--maxhosts    "128" \
--strategy    "SG_scheduler(5,h)" \
--activate    "resource_and_time_interval(c(9999999999))" \
--hostselect  "all" \
--taskselect  "all" \
--allocpolicy "fcfs" \
--nruns       "10" \
--ga_enable_game $ENABLE_GAME \
--ga_security    $SECURITY \
--ga_calc_penalty  $CALC_PENALTIES \
--ga_enable_qlu $QLU \
--ga_enable_qle $QLE \
--ga_enable_qlab $QLAB \
--ga_security_extra $SECURITY_EXTRA \
--ga_abort_extra $ABORT_EXTRA \
2>$1 | tee raport_$6_$1_$2_$3_$4_$5 \

fi


if [ $6 == "MB_STATIC_VERY_LARGE" ]; then

./$EXEC/sim \
--ttasks "4096" \
--itasks "4096" \
--iatime "c(9999999999)" \
--workload "n(250000000,43750000)" \
--ihosts      "256" \
--mips        "n(1000,175)" \
--addhost     "c(9999999999)" \
--delhost     "c(9999999999)" \
--minhosts    "256" \
--maxhosts    "256" \
--strategy    "SG_scheduler(5,h)" \
--activate    "resource_and_time_interval(c(9999999999))" \
--hostselect  "all" \
--taskselect  "all" \
--allocpolicy "fcfs" \
--nruns       "10" \
--ga_enable_game $ENABLE_GAME \
--ga_security    $SECURITY \
--ga_calc_penalty  $CALC_PENALTIES \
--ga_enable_qlu $QLU \
--ga_enable_qle $QLE \
--ga_enable_qlab $QLAB \
--ga_security_extra $SECURITY_EXTRA \
--ga_abort_extra $ABORT_EXTRA \
2>$1 | tee raport_$6_$1_$2_$3_$4_$5 \

fi


if [ $6 == "MB_DYNAMIC_SMALL" ]; then

./$EXEC/sim \
--ttasks "512" \
--itasks "384" \
--iatime "e(7812.50)" \
--workload "n(250000000,43750000)" \
--ihosts      "32" \
--mips        "n(1000,175)" \
--addhost     "n(625000,93750)" \
--delhost     "n(750000,112500)" \
--minhosts    "27" \
--maxhosts    "37" \
--strategy    "SG_scheduler(5,h)" \
--activate    "resource_and_time_interval(c(250000))" \
--hostselect  "all" \
--taskselect  "all" \
--allocpolicy "fcfs" \
--nruns       "10" \
--ga_enable_game $ENABLE_GAME \
--ga_security    $SECURITY \
--ga_calc_penalty  $CALC_PENALTIES \
--ga_enable_qlu $QLU \
--ga_enable_qle $QLE \
--ga_enable_qlab $QLAB \
--ga_security_extra $SECURITY_EXTRA \
--ga_abort_extra $ABORT_EXTRA \
2>$1 | tee raport_$6_$1_$2_$3_$4_$5 \

fi


if [ $6 == "MB_DYNAMIC_MEDIUM" ]; then

./$EXEC/sim \
--ttasks "1024" \
--itasks "768" \
--iatime "e(3906.25)" \
--workload "n(250000000,43750000)" \
--ihosts      "64" \
--mips        "n(1000,175)" \
--addhost     "n(562500,84375)" \
--delhost     "n(750000,112500)" \
--minhosts    "58" \
--maxhosts    "70" \
--strategy    "SG_scheduler(5,h)" \
--activate    "resource_and_time_interval(c(250000))" \
--hostselect  "all" \
--taskselect  "all" \
--allocpolicy "fcfs" \
--nruns       "8" \
--ga_enable_game $ENABLE_GAME \
--ga_security    $SECURITY \
--ga_calc_penalty  $CALC_PENALTIES \
--ga_enable_qlu $QLU \
--ga_enable_qle $QLE \
--ga_enable_qlab $QLAB \
--ga_security_extra $SECURITY_EXTRA \
--ga_abort_extra $ABORT_EXTRA \
2>$1 | tee raport_$6_$1_$2_$3_$4_$5 \

fi


if [ $6 == "MB_DYNAMIC_LARGE" ]; then

./$EXEC/sim \
--ttasks "2048" \
--itasks "1536" \
--iatime "e(1953.125)" \
--workload "n(250000000,43750000)" \
--ihosts      "128" \
--mips        "n(1000,175)" \
--addhost     "n(500000,75000)" \
--delhost     "n(750000,112500)" \
--minhosts    "121" \
--maxhosts    "135" \
--strategy    "SG_scheduler(5,h)" \
--activate    "resource_and_time_interval(c(250000))" \
--hostselect  "all" \
--taskselect  "all" \
--allocpolicy "fcfs" \
--nruns       "6" \
--ga_enable_game $ENABLE_GAME \
--ga_security    $SECURITY \
--ga_calc_penalty  $CALC_PENALTIES \
--ga_enable_qlu $QLU \
--ga_enable_qle $QLE \
--ga_enable_qlab $QLAB \
--ga_security_extra $SECURITY_EXTRA \
--ga_abort_extra $ABORT_EXTRA \
2>$1 | tee raport_$6_$1_$2_$3_$4_$5 \

fi


if [ $6 == "MB_DYNAMIC_VERY_LARGE" ]; then

./$EXEC/sim \
--ttasks "4096" \
--itasks "3072" \
--iatime "e(976.5625)" \
--workload "n(250000000,43750000)" \
--ihosts      "256" \
--mips        "n(1000,175)" \
--addhost     "n(437500,65625)" \
--delhost     "n(750000,112500)" \
--minhosts    "248" \
--maxhosts    "264" \
--strategy    "SG_scheduler(5,h)" \
--activate    "resource_and_time_interval(c(250000))" \
--hostselect  "all" \
--taskselect  "all" \
--allocpolicy "fcfs" \
--nruns       "6" \
--ga_enable_game $ENABLE_GAME \
--ga_security    $SECURITY \
--ga_calc_penalty  $CALC_PENALTIES \
--ga_enable_qlu $QLU \
--ga_enable_qle $QLE \
--ga_enable_qlab $QLAB \
--ga_security_extra $SECURITY_EXTRA \
--ga_abort_extra $ABORT_EXTRA \
2>$1 | tee raport_$6_$1_$2_$3_$4_$5 \

fi

