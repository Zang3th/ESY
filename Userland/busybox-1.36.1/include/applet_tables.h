/* This is a generated file, don't edit */

#define NUM_APPLETS 21
#define KNOWN_APPNAME_OFFSETS 0

const char applet_names[] ALIGN1 = ""
"adduser" "\0"
"ash" "\0"
"bash" "\0"
"cat" "\0"
"clear" "\0"
"deluser" "\0"
"echo" "\0"
"httpd" "\0"
"ifconfig" "\0"
"kill" "\0"
"loadkmap" "\0"
"login" "\0"
"ls" "\0"
"mkdir" "\0"
"mount" "\0"
"passwd" "\0"
"ps" "\0"
"pwd" "\0"
"sh" "\0"
"uname" "\0"
"vi" "\0"
;

#define APPLET_NO_adduser 0
#define APPLET_NO_ash 1
#define APPLET_NO_bash 2
#define APPLET_NO_cat 3
#define APPLET_NO_clear 4
#define APPLET_NO_deluser 5
#define APPLET_NO_echo 6
#define APPLET_NO_httpd 7
#define APPLET_NO_ifconfig 8
#define APPLET_NO_kill 9
#define APPLET_NO_loadkmap 10
#define APPLET_NO_login 11
#define APPLET_NO_ls 12
#define APPLET_NO_mkdir 13
#define APPLET_NO_mount 14
#define APPLET_NO_passwd 15
#define APPLET_NO_ps 16
#define APPLET_NO_pwd 17
#define APPLET_NO_sh 18
#define APPLET_NO_uname 19
#define APPLET_NO_vi 20

#ifndef SKIP_applet_main
int (*const applet_main[])(int argc, char **argv) = {
adduser_main,
ash_main,
ash_main,
cat_main,
clear_main,
deluser_main,
echo_main,
httpd_main,
ifconfig_main,
kill_main,
loadkmap_main,
login_main,
ls_main,
mkdir_main,
mount_main,
passwd_main,
ps_main,
pwd_main,
ash_main,
uname_main,
vi_main,
};
#endif

