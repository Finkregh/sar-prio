#! /bin/sh

source /usr/local/etc/tc-routing.sh

clear_qdiscs()
{
    # clear up- and downlink qdiscs
    for device in $DEVLIST ; do
        $TC qdisc del dev $device ingress 2>&1 > /dev/null
        $TC qdisc del dev $device root 2>&1 > /dev/null
    done
}

create_qdiscs()
{
    $TCNG /usr/local/etc/tc/eth1.tcc | sed s,^tc,$TC, | $BASH
    $TCNG /usr/local/etc/tc/eth2.tcc | sed s,^tc,$TC, | $BASH
}


case "$1" in
    status)
        for device in $EDEVLIST ; do
            $TC -s qdisc ls dev $device
            $TC -s class ls dev $device
        done
    ;;
    start)
        clear_qdiscs;
        create_qdiscs;
    ;;
    stop)
        clear_qdiscs;
    ;;
    restart)
        clear_qdiscs;
        create_qdiscs;
    ;;
    *)
        echo "Usage: $0 {start|stop|restart|status}"
        exit 1
    ;;
esac

exit 0

# vim:tabstop=2:expandtab:shiftwidth=2
