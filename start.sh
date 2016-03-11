#!/bin/bash

WORK_PATH=$(pwd)
HTTPD_CONF=$WORK_PATH/conf/httpd.conf
#HTTP=$WORK_PATH/httpd


firewall_switch=`grep -Ei 'FIREWALL_SWITCH' $HTTPD_CONF | awk -F':' '{print $NF}' | tr -d ' '`
httpd_port=`grep -Ei 'HTTPD_PORT' $HTTPD_CONF | awk -F':' '{print $NF}' | tr -d ' '`
httpd_ip=`grep -Ei 'HTTPD_IP' $HTTPD_CONF | awk -F':' '{print $NF}' | tr -d ' '`

if [ "X$firewall_switch" == "XYES" ];then
	service iptables stop
else
	service iptables start
fi


WHOAMI=`whoami`
PID=`ps -u $WHOAMI | grep myhttpd | awk '{print $1}'`

if (test "$#" = 0); then
	echo "Usage: $0 [stop] [start] [status]"
	exit 0
fi

if (test "$1" = "start"); then
	if (test "$PID" = ""); then
          ./myhttpd  $httpd_ip  $httpd_port
	else
		echo "myhttp is running"
	fi
	exit 0
fi

if (test "$1" = "stop"); then
	if (test "$PID" != ""); then
		kill -s 2 $PID
	fi
	exit 0
fi

if (test "$1" = "status"); then
	if (test "$PID" = ""); then
		echo "myhttp is not run"
	else
		echo "myhttp is running"
	fi
	exit 0
fi

echo "Usage: $0 [stop] [start] [status]"


