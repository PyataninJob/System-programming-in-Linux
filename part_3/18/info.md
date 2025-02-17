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