Скачал весии busybox 1.37.0 и 1.36.1(Как в уроке)

Почистили все мистером пропером

делайем базовый конфиг 
ARCH=arm make defconfig
все собралось и видим кучу вопросов 

ARCH=arm make menuconfig

Включили 
Cross compiler c префиксом
arm-linux-gnueabihf-
и включили статичную сборку
Получили:
 ARCH=arm make menuconfig 
  HOSTLD  scripts/kconfig/mconf
  HOSTCC  scripts/kconfig/lxdialog/checklist.o
  HOSTCC  scripts/kconfig/lxdialog/inputbox.o
  HOSTCC  scripts/kconfig/lxdialog/lxdialog.o
  HOSTCC  scripts/kconfig/lxdialog/menubox.o
  HOSTCC  scripts/kconfig/lxdialog/msgbox.o
  HOSTCC  scripts/kconfig/lxdialog/textbox.o
  HOSTCC  scripts/kconfig/lxdialog/util.o
  HOSTCC  scripts/kconfig/lxdialog/yesno.o
  HOSTLD  scripts/kconfig/lxdialog/lxdialog
scripts/kconfig/mconf Config.in
#
# using defaults found in .config
#


*** End of configuration.
*** Execute 'make' to build the project or try 'make help'.

запускаем ARCH=arm make -j$(nproc)
и у еня почемуто будто устаревший вариант чтоли, причем на двух версиях

networking/tc.c:236:27: error: ‘TCA_CBQ_MAX’ undeclared (first use in this function); did you mean ‘TCA_CBS_MAX’?
  236 |         struct rtattr *tb[TCA_CBQ_MAX+1];
      |                           ^~~~~~~~~~~
      |                           TCA_CBS_MAX
networking/tc.c:236:27: note: each undeclared identifier is reported only once for each function it appears in
networking/tc.c:249:16: error: ‘TCA_CBQ_RATE’ undeclared (first use in this function); did you mean ‘TCA_TBF_RATE64’?
  249 |         if (tb[TCA_CBQ_RATE]) {
      |                ^~~~~~~~~~~~
      |                TCA_TBF_RATE64
miscutils/watchdog.c: In function ‘watchdog_main’:
miscutils/watchdog.c:161:17: warning: ignoring return value of ‘write’ declared with attribute ‘warn_unused_result’ [-Wunused-result]
  161 |                 write(3, "", 1); /* write zero byte */
      |                 ^~~~~~~~~~~~~~~
networking/tc.c:255:16: error: ‘TCA_CBQ_LSSOPT’ undeclared (first use in this function)
  255 |         if (tb[TCA_CBQ_LSSOPT]) {
      |                ^~~~~~~~~~~~~~
miscutils/watchdog.c: In function ‘shutdown_watchdog’:
miscutils/watchdog.c:71:9: warning: ignoring return value of ‘write’ declared with attribute ‘warn_unused_result’ [-Wunused-result]
   71 |         write(3, &V, 1);  /* Magic, see watchdog-api.txt in kernel */
      |         ^~~~~~~~~~~~~~~
networking/tc.c:256:61: error: invalid application of ‘sizeof’ to incomplete type ‘struct tc_cbq_lssopt’
  256 |                 if (RTA_PAYLOAD(tb[TCA_CBQ_LSSOPT]) < sizeof(*lss))
      |                                                             ^
  CC      networking/udhcp/d6_dhcpc.o
networking/tc.c:261:16: error: ‘TCA_CBQ_WRROPT’ undeclared (first use in this function)
  261 |         if (tb[TCA_CBQ_WRROPT]) {
      |                ^~~~~~~~~~~~~~
networking/tc.c:262:61: error: invalid application of ‘sizeof’ to incomplete type ‘struct tc_cbq_wrropt’
  262 |                 if (RTA_PAYLOAD(tb[TCA_CBQ_WRROPT]) < sizeof(*wrr))
      |                                                             ^
networking/tc.c:267:16: error: ‘TCA_CBQ_FOPT’ undeclared (first use in this function)
  267 |         if (tb[TCA_CBQ_FOPT]) {
      |                ^~~~~~~~~~~~
networking/tc.c:268:59: error: invalid application of ‘sizeof’ to incomplete type ‘struct tc_cbq_fopt’
  268 |                 if (RTA_PAYLOAD(tb[TCA_CBQ_FOPT]) < sizeof(*fopt))
      |                                                           ^
networking/tc.c:273:16: error: ‘TCA_CBQ_OVL_STRATEGY’ undeclared (first use in this function)
  273 |         if (tb[TCA_CBQ_OVL_STRATEGY]) {
      |                ^~~~~~~~~~~~~~~~~~~~
networking/tc.c:274:67: error: invalid application of ‘sizeof’ to incomplete type ‘struct tc_cbq_ovl’
  274 |                 if (RTA_PAYLOAD(tb[TCA_CBQ_OVL_STRATEGY]) < sizeof(*ovl))
      |                                                                   ^
networking/tc.c:277:50: error: invalid application of ‘sizeof’ to incomplete type ‘struct tc_cbq_ovl’
  277 |                                 (unsigned) sizeof(*ovl));
      |                                                  ^
networking/tc.c:293:23: error: invalid use of undefined type ‘struct tc_cbq_lssopt’
  293 |         if (lss && lss->flags) {
      |                       ^~
networking/tc.c:296:24: error: invalid use of undefined type ‘struct tc_cbq_lssopt’
  296 |                 if (lss->flags&TCF_CBQ_LSS_BOUNDED) {
      |                        ^~
  CC      libbb/pidfile.o
  AR      miscutils/lib.a
networking/tc.c:296:32: error: ‘TCF_CBQ_LSS_BOUNDED’ undeclared (first use in this function)
  296 |                 if (lss->flags&TCF_CBQ_LSS_BOUNDED) {
      |                                ^~~~~~~~~~~~~~~~~~~
networking/tc.c:300:24: error: invalid use of undefined type ‘struct tc_cbq_lssopt’
  300 |                 if (lss->flags&TCF_CBQ_LSS_ISOLATED) {
      |                        ^~
networking/tc.c:300:32: error: ‘TCF_CBQ_LSS_ISOLATED’ undeclared (first use in this function)
  300 |                 if (lss->flags&TCF_CBQ_LSS_ISOLATED) {
      |                                ^~~~~~~~~~~~~~~~~~~~
networking/tc.c:308:24: error: invalid use of undefined type ‘struct tc_cbq_wrropt’
  308 |                 if (wrr->priority != TC_CBQ_MAXPRIO)
      |                        ^~
networking/tc.c:308:38: error: ‘TC_CBQ_MAXPRIO’ undeclared (first use in this function)
  308 |                 if (wrr->priority != TC_CBQ_MAXPRIO)
      |                                      ^~~~~~~~~~~~~~
networking/tc.c:309:46: error: invalid use of undefined type ‘struct tc_cbq_wrropt’
  309 |                         printf("prio %u", wrr->priority);
      |                                              ^~
networking/tc.c:313:43: error: invalid use of undefined type ‘struct tc_cbq_wrropt’
  313 |                         printf("/%u ", wrr->cpriority);
      |                                           ^~
networking/tc.c:314:32: error: invalid use of undefined type ‘struct tc_cbq_wrropt’
  314 |                         if (wrr->weight != 1) {
      |                                ^~
networking/tc.c:315:65: error: invalid use of undefined type ‘struct tc_cbq_wrropt’
  315 |                                 print_rate(buf, sizeof(buf), wrr->weight);
      |                                                                 ^~
networking/tc.c:318:32: error: invalid use of undefined type ‘struct tc_cbq_wrropt’
  318 |                         if (wrr->allot)
      |                                ^~
networking/tc.c:319:57: error: invalid use of undefined type ‘struct tc_cbq_wrropt’
  319 |                                 printf("allot %ub ", wrr->allot);
      |                                                         ^~
networking/tc.c:236:24: warning: unused variable ‘tb’ [-Wunused-variable]
  236 |         struct rtattr *tb[TCA_CBQ_MAX+1];
      |                        ^~

  
я нашел тред
https://github.com/gramineproject/gramine/issues/1909#issuecomment-2172408534
Так что сейчас я спускаюсь с своей 6.11 версии на 5.4

Результат:
Да такой же

Я пока отправлю так. надеюсь вы мне подскажете как лучше сделать, но я предположу что вы дадите мне сборку в ручную. 

В общем я начал попытку поставить патч для своей версии линукса https://patchwork.yoctoproject.org/project/oe-core/patch/20240714165904.736928-1-raj.khem@gmail.com/

Создаем busybox-1.36.1-no-cbq.patch внутри папки бусибокса
и вставляем:
diff --git a/meta/recipes-core/busybox/busybox/busybox-1.36.1-no-cbq.patch b/meta/recipes-core/busybox/busybox/busybox-1.36.1-no-cbq.patch
new file mode 100644
index 00000000000..80cbc73fc41
--- /dev/null
+++ b/meta/recipes-core/busybox/busybox/busybox-1.36.1-no-cbq.patch
@@ -0,0 +1,61 @@ 
+Remove CBQ functionality from tc
+
+6.8+ kernel has dropped CBQ support [1], Now that OE uses 6.9 for
+kernel-headers means we are hitting the undefined symbol
+TCA_CBQ_MAX [2]  
+
+[1] https://github.com/torvalds/linux/commit/33241dca486264193ed68167c8eeae1fb197f3df
+[2] https://bugs.busybox.net/show_bug.cgi?id=15934
+
+Upstream-Status: Submitted [https://bugs.busybox.net/show_bug.cgi?id=15931]
+Signed-off-by: Khem Raj <raj.khem@gmail.com>
+
+diff -up busybox-1.36.1/networking/tc.c.no-cbq busybox-1.36.1/networking/tc.c
+--- busybox-1.36.1/networking/tc.c.no-cbq	2024-01-29 10:24:09.135082923 -0500
++++ busybox-1.36.1/networking/tc.c	2024-01-29 10:28:12.009502552 -0500
+@@ -31,7 +31,7 @@
+ //usage:	"qdisc [handle QHANDLE] [root|"IF_FEATURE_TC_INGRESS("ingress|")"parent CLASSID]\n"
+ /* //usage: "[estimator INTERVAL TIME_CONSTANT]\n" */
+ //usage:	"	[[QDISC_KIND] [help|OPTIONS]]\n"
+-//usage:	"	QDISC_KIND := [p|b]fifo|tbf|prio|cbq|red|etc.\n"
++//usage:	"	QDISC_KIND := [p|b]fifo|tbf|prio|red|etc.\n"
+ //usage:	"qdisc show [dev STRING]"IF_FEATURE_TC_INGRESS(" [ingress]")"\n"
+ //usage:	"class [classid CLASSID] [root|parent CLASSID]\n"
+ //usage:	"	[[QDISC_KIND] [help|OPTIONS] ]\n"
+@@ -230,7 +230,7 @@ static int cbq_parse_opt(int argc, char
+ {
+ 	return 0;
+ }
+-#endif
++
+ static int cbq_print_opt(struct rtattr *opt)
+ {
+ 	struct rtattr *tb[TCA_CBQ_MAX+1];
+@@ -322,6 +322,7 @@ static int cbq_print_opt(struct rtattr *
+  done:
+ 	return 0;
+ }
++#endif
+ 
+ static FAST_FUNC int print_qdisc(
+ 		const struct sockaddr_nl *who UNUSED_PARAM,
+@@ -373,7 +374,8 @@ static FAST_FUNC int print_qdisc(
+ 		if (qqq == 0) { /* pfifo_fast aka prio */
+ 			prio_print_opt(tb[TCA_OPTIONS]);
+ 		} else if (qqq == 1) { /* class based queuing */
+-			cbq_print_opt(tb[TCA_OPTIONS]);
++			/* cbq_print_opt(tb[TCA_OPTIONS]); */
++			printf("cbq not supported");
+ 		} else {
+ 			/* don't know how to print options for this qdisc */
+ 			printf("(options for %s)", name);
+@@ -444,7 +446,8 @@ static FAST_FUNC int print_class(
+ 			/* nothing. */ /*prio_print_opt(tb[TCA_OPTIONS]);*/
+ 		} else if (qqq == 1) { /* class based queuing */
+ 			/* cbq_print_copt() is identical to cbq_print_opt(). */
+-			cbq_print_opt(tb[TCA_OPTIONS]);
++			/* cbq_print_opt(tb[TCA_OPTIONS]); */
++			printf("cbq not supported");
+ 		} else {
+ 			/* don't know how to print options for this class */
+ 			printf("(options for %s)", name);
diff --git a/meta/recipes-core/busybox/busybox_1.36.1.bb b/meta/recipes-core/busybox/busybox_1.36.1.bb
index 7ce57bb0d00..67bc5c0cadd 100644
--- a/meta/recipes-core/busybox/busybox_1.36.1.bb
+++ b/meta/recipes-core/busybox/busybox_1.36.1.bb
@@ -52,6 +52,7 @@ SRC_URI = "https://busybox.net/downloads/busybox-${PV}.tar.bz2;name=tarball \
           file://CVE-2021-42380.patch \
           file://0001-awk-fix-segfault-when-compiled-by-clang.patch \
           file://CVE-2023-42363.patch \
+           file://busybox-1.36.1-no-cbq.patch \
           "
SRC_URI:append:libc-musl = " file://musl.cfg "
# TODO http://lists.busybox.net/pipermail/busybox/2023-January/090078.html

Применяем патч

patch -p0 < busybox-1.36.1-no-cbq.patch
patching file b/meta/recipes-core/busybox/busybox/busybox-1.36.1-no-cbq.patch
can't find file to patch at input line 72
Perhaps you used the wrong -p or --strip option?
The text leading up to this was:
--------------------------
|diff --git a/meta/recipes-core/busybox/busybox_1.36.1.bb b/meta/recipes-core/busybox/busybox_1.36.1.bb
|index 7ce57bb0d00..67bc5c0cadd 100644
|--- a/meta/recipes-core/busybox/busybox_1.36.1.bb
|+++ b/meta/recipes-core/busybox/busybox_1.36.1.bb
--------------------------
File to patch: meta/recipes-core/busybox/busybox_1.36.1.bb
meta/recipes-core/busybox/busybox_1.36.1.bb: No such file or directory
Skip this patch? [y] y


После подкупа и мольбы принято рещение сделать это вручную и убрать CBQ

Теперь все собирается и все хорошо
busybox-1.36.1/_install/bin$ file ./busybox 
./busybox: ELF 32-bit LSB executable, ARM, EABI5 version 1 (GNU/Linux), statically linked, BuildID[sha1]=add78816abddf36f2022cb8fafbc896ad6c7b2f8, for GNU/Linux 3.2.0, stripped


И все запускается
[    4.342659] Freeing unused kernel image (initmem) memory: 2048K
[    4.349082] Run /bin/ash as init process
/bin/ash: can't access tty; job control turned off
~ # [    4.567303] input: ImExPS/2 Generic Explorer Mouse as /devices/platform/bus@40000000/bus@40000000:motherboard-bus@40000000/bus@40000000:motherboard-bus@40000000:iofpga@7,00000000/10007000.kmi/serio1/input/input2

~ # 
~ # [   14.870412] amba 1000f000.wdt: deferred probe pending: (reason unknown)
[   14.871682] amba 100e0000.memory-controller: deferred probe pending: (reason unknown)
[   14.872531] amba 100e1000.memory-controller: deferred probe pending: (reason unknown)
[   14.872887] amba 100e5000.watchdog: deferred probe pending: (reason unknown)
QEMU: Terminated
(base) pyatanin@pyatanin-VirtualBox:~/VSproj/test_arm$ 
