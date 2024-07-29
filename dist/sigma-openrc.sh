#!/sbin/openrc-run

description="Sigma Service"

command="/usr/local/bin/sigma"
command_args="--silent"
command_user="root:root"

pidfile="/var/run/sigma.pid"
timeout_start="5"

depend() {
    need localmount
    after bootmisc
}

start_pre() {
    ebegin "Preparing to start Sigma Service"
}

start() {
    ebegin "Starting Sigma Service"
    start-stop-daemon --start --exec ${command} -- ${command_args}
    eend $?
}

stop() {
    ebegin "Stopping Sigma Service"
    start-stop-daemon --stop --exec ${command}
    eend $?
}

restart() {
    ebegin "Restarting Sigma Service"
    svc_stop
    svc_start
    eend $?
}
