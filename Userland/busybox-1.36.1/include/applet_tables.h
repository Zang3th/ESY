/* This is a generated file, don't edit */

#define NUM_APPLETS 16
#define KNOWN_APPNAME_OFFSETS 0

const char applet_names[] ALIGN1 = ""
"ash" "\0"
"bash" "\0"
"cat" "\0"
"echo" "\0"
"httpd" "\0"
"ifconfig" "\0"
"kill" "\0"
"loadkmap" "\0"
"ls" "\0"
"mkdir" "\0"
"mount" "\0"
"ps" "\0"
"pwd" "\0"
"sh" "\0"
"uname" "\0"
"vi" "\0"
;

#define APPLET_NO_ash 0
#define APPLET_NO_bash 1
#define APPLET_NO_cat 2
#define APPLET_NO_echo 3
#define APPLET_NO_httpd 4
#define APPLET_NO_ifconfig 5
#define APPLET_NO_kill 6
#define APPLET_NO_loadkmap 7
#define APPLET_NO_ls 8
#define APPLET_NO_mkdir 9
#define APPLET_NO_mount 10
#define APPLET_NO_ps 11
#define APPLET_NO_pwd 12
#define APPLET_NO_sh 13
#define APPLET_NO_uname 14
#define APPLET_NO_vi 15

#ifndef SKIP_applet_main
int (*const applet_main[])(int argc, char **argv) = {
ash_main,
ash_main,
cat_main,
echo_main,
httpd_main,
ifconfig_main,
kill_main,
loadkmap_main,
ls_main,
mkdir_main,
mount_main,
ps_main,
pwd_main,
ash_main,
uname_main,
vi_main,
};
#endif

