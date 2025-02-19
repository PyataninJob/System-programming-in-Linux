

# Сборка Busybox для ARM

## Шаги сборки

1. **Скачал версии Busybox 1.37.0 и 1.36.1** (как в уроке).
2. **Почистили все мистером пропером**.
3. **Создали базовый конфиг**:
   ```bash
   ARCH=arm make defconfig
   ```
   Всё собралось, но появилось много вопросов.

4. **Запустили конфигурацию через меню**:
   ```bash
   ARCH=arm make menuconfig
   ```
   Включили:
   - Cross compiler с префиксом `arm-linux-gnueabihf-`.
   - Статичную сборку.

5. **Результат конфигурации**:
   ```bash
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
   ```

6. **Запустили сборку**:
   ```bash
   ARCH=arm make -j$(nproc)
   ```
   Но возникли ошибки, связанные с устаревшим кодом CBQ в ядре:

   ```bash
   networking/tc.c:236:27: error: ‘TCA_CBQ_MAX’ undeclared (first use in this function); did you mean ‘TCA_CBS_MAX’?
     236 |         struct rtattr *tb[TCA_CBQ_MAX+1];
         |                           ^~~~~~~~~~~
         |                           TCA_CBS_MAX
   networking/tc.c:236:27: note: each undeclared identifier is reported only once for each function it appears in
   networking/tc.c:249:16: error: ‘TCA_CBQ_RATE’ undeclared (first use in this function); did you mean ‘TCA_TBF_RATE64’?
     249 |         if (tb[TCA_CBQ_RATE]) {
         |                ^~~~~~~~~~~~
         |                TCA_TBF_RATE64
   ...
   ```

## Решение проблемы

1. **Нашел тред на GitHub**: [https://github.com/gramineproject/gramine/issues/1909#issuecomment-2172408534](https://github.com/gramineproject/gramine/issues/1909#issuecomment-2172408534).
2. **Попытка понизить версию ядра с 6.11 до 5.4**, но результат остался таким же.

3. **Применение патча**:
   - Создал файл `busybox-1.36.1-no-cbq.patch` внутри папки Busybox.
   - Вставил следующий патч:

     ```diff
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
     --- busybox-1.36.1/networking/tc.c.no-cbq	2024-01-29 10:24:09.135082923 -0500
     +++ busybox-1.36.1/networking/tc.c	2024-01-29 10:28:12.009502552 -0500
     @@ -31,7 +31,7 @@
     //usage:	"qdisc [handle QHANDLE] [root|"IF_FEATURE_TC_INGRESS("ingress|")"parent CLASSID]\n"
     /* //usage: "[estimator INTERVAL TIME_CONSTANT]\n" */
     //usage:	"	[[QDISC_KIND] [help|OPTIONS]]\n"
     -//usage:	"	QDISC_KIND := [p|b]fifo|tbf|prio|cbq|red|etc.\n"
     +//usage:	"	QDISC_KIND := [p|b]fifo|tbf|prio|red|etc.\n"
     //usage:	"qdisc show [dev STRING]"IF_FEATURE_TC_INGRESS(" [ingress]")"\n"
     //usage:	"class [classid CLASSID] [root|parent CLASSID]\n"
     //usage:	"	[[QDISC_KIND] [help|OPTIONS] ]\n"
     @@ -230,7 +230,7 @@ static int cbq_parse_opt(int argc, char
     {
     return 0;
     }
     -#endif
     +
     static int cbq_print_opt(struct rtattr *opt)
     {
     struct rtattr *tb[TCA_CBQ_MAX+1];
     @@ -322,6 +322,7 @@ static int cbq_print_opt(struct rtattr *
     done:
     return 0;
     }
     +#endif
     +
     static FAST_FUNC int print_qdisc(
     const struct sockaddr_nl *who UNUSED_PARAM,
     @@ -373,7 +374,8 @@ static FAST_FUNC int print_qdisc(
     if (qqq == 0) { /* pfifo_fast aka prio */
     prio_print_opt(tb[TCA_OPTIONS]);
     } else if (qqq == 1) { /* class based queuing */
     -			cbq_print_opt(tb[TCA_OPTIONS]);
     +			/* cbq_print_opt(tb[TCA_OPTIONS]); */
     +			printf("cbq not supported");
     } else {
     /* don't know how to print options for this qdisc */
     printf("(options for %s)", name);
     @@ -444,7 +446,8 @@ static FAST_FUNC int print_class(
     /* nothing. */ /*prio_print_opt(tb[TCA_OPTIONS]);*/
     } else if (qqq == 1) { /* class based queuing */
     /* cbq_print_copt() is identical to cbq_print_opt(). */
     -			cbq_print_opt(tb[TCA_OPTIONS]);
     +			/* cbq_print_opt(tb[TCA_OPTIONS]); */
     +			printf("cbq not supported");
     } else {
     /* don't know how to print options for this class */
     printf("(options for %s)", name);
     ```

4. **Применение патча**:
   ```bash
   patch -p0 < busybox-1.36.1-no-cbq.patch
   ```
   Возникла ошибка, что ожидаемо. Я же не подумал что у меня нет проекта здесь, еще и :
   ```bash
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
   ```

5. **Ручное исправление**:
   После неудачной попытки применить патч, было решено вручную убрать CBQ из кода.

## Результат

1. **Сборка завершена успешно**:
   ```bash
   busybox-1.36.1/_install/bin$ file ./busybox 
   ./busybox: ELF 32-bit LSB executable, ARM, EABI5 version 1 (GNU/Linux), statically linked, BuildID[sha1]=add78816abddf36f2022cb8fafbc896ad6c7b2f8, for GNU/Linux 3.2.0, stripped
   ```

2. **Запуск Busybox**:
   ```bash
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
   ```

Теперь всё работает корректно.
