
---

Для начала скачаем исходники ядра командой:
```bash
git clone git://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git
```

Потом обновим пакеты, требующиеся для сборки ядра:
- **build-essential**: Набор инструментов для компиляции программ, включая компилятор GCC.
- **libncurses-dev**: Библиотека для создания текстовых интерфейсов, используется в `menuconfig`.
- **bison и flex**: Инструменты для генерации парсеров, необходимые для сборки ядра.
- **libssl-dev**: Библиотека для работы с SSL, необходима для некоторых функций ядра.
- **libelf-dev**: Библиотека для работы с ELF-файлами, используется в ядре.

Сделаем базовую конфигурацию с помощью:
```bash
make defconfig
```
```
  HOSTCC  scripts/basic/fixdep
  HOSTCC  scripts/kconfig/conf.o
  HOSTCC  scripts/kconfig/confdata.o
  HOSTCC  scripts/kconfig/expr.o
  LEX     scripts/kconfig/lexer.lex.c
  YACC    scripts/kconfig/parser.tab.[ch]
  HOSTCC  scripts/kconfig/lexer.lex.o
  HOSTCC  scripts/kconfig/menu.o
  HOSTCC  scripts/kconfig/parser.tab.o
  HOSTCC  scripts/kconfig/preprocess.o
  HOSTCC  scripts/kconfig/symbol.o
  HOSTCC  scripts/kconfig/util.o
  HOSTLD  scripts/kconfig/conf
*** Default configuration is based on 'x86_64_defconfig'
#
# configuration written to .config
#
```

Ну а теперь залазаем в недры мироздания с помощью:
```bash
make menuconfig
```

Первый опыт получен... неудачный:
```
[Firmware Bug]: TSC doesn't count with P0 frequency!
APIC calibration not consistent with PM-Timer: 102ms instead of 100ms
TSC synchronization [CPU#0 -> CPU#1]:
Measured 107326 cycles TSC warp between CPUs, turning off TSC clock...
amd_pstate: The CPPC feature is supported but currently disabled by the BIOS.
Please enable it if your BIOS has the CPPC option
amd_pstate: the _CPC object is not present in SBTS on ACPI disabled
/dev/sda2: clean, 696366/3276800 files, 5167402/13106432 blocks
SELinux:  could not open policy file <= /etc/selinux/targeted/policy/policy.34: No such file or directory
systemd[1]: Failed to mount tmpfs (type tmpfs) on /sys/fs/cgroup (MS_NOSUID|MS_NODEV|MS_NOEXEC|MS_STRICTATIME 'mode=0755,size=4m,nr_nodes=2k'): No such file or directory
systemd[1]: Failed to mount cgroup (type cgroup) on /sys/fs/cgroup/systemd (MS_NOSUID|MS_NODEV|MS_NOEXEC 'none,name=systemd'): No such file or directory
[!!!!!!] Failed to mount API filesystems.
Systemd[1]: Freezing execution.
```

Второй опыт не был удачнее, теперь у меня просто черный экран и курсор превратился в крест. Подход с тыканьем того, что звучит сомнительно, не оправдал себя. Начнем тогда со сборки базового ядра и проверки его работоспособности. И последовательно будем включать функции.

Сборка и запуск простой версии после `defconfig` успешна. Но очевидно, начались проблемы. Я ничего вроде не выключил, и появились проблемы, часто выходят ошибки, и я не могу запустить `menuconfig` на новой системе.

Думаю, есть все же некоторая особенность того, что я делаю это через VirtualBox. Теперь вернемся к попыткам сделать ядро легче. Попытаемся облегчить файловую систему:

- **Ext4 POSIX Access Control Lists**: Если вы не используете POSIX ACLs, вы можете отключить эту опцию.
- **Ext4 Security Labels**: Если вы не используете метки безопасности (например, SELinux), вы можете отключить эту опцию.
- **Dnotify и Inotify**:
  - **Dnotify support**: Если вы не используете Dnotify, вы можете отключить эту опцию.
  - **Inotify support for userspace**: Если вы не используете Inotify, вы можете отключить эту опцию.
- **Quota**:
  - **Quota support**: Если вы не используете квоты на файловой системе, вы можете отключить эту опцию.
  - **Report quota messages through netlink interface**: Если вы не используете квоты, вы можете отключить эту опцию.
- **Kernel automounter**:
  - **Kernel automounter support (supports v3, v4 and v5)**: Если вы не используете автоматическое монтирование, вы можете отключить эту опцию.

Возникают ошибки:
```
[Firmware Bug]: TSC doesn't count with P0 frequency!
Speculative Return Stack Overflow: IBPB-extending microcode not applied!
Speculative Return Stack Overflow: WARNING: See https://kernel.org/doc/html/latest/st/admin-guide/hw-vuln/srso.html for mitigation options.
APIC calibration not consistent with PM-timer: 120ms instead of 100ms
TSC synchronization [CPU#0 -> CPU#1]:
Measured 166614 cycles TSC warp between CPUs, turning off TSC clock.
amd_pstate: The CPPC feature is supported but currently disabled by the BIOS.
Please enable it if your BIOS has the CPPC option.
amd_pstate: the _CPC object is not present in SBIOs or ACPI disabled
ntp timer: interrupt took 5823553 ns
/dev/sda2: clean, 658865/3276800 files, 9127294/13106432 blocks
Failed to send exit request: Connection refused
SELinux:  Could not open policy file <= /etc/selinux/targeted/policy/policy.34: No such file or directory
systemd[1]: Failed to allocate manager object: Function not implemented
systemd[1]: Freezing execution.
[********] Failed to allocate manager object.
```

Предположу, что это из-за монтирования.

Вообще, нужна метрика для определения такого понятия, как "легкое ядро". Вроде как это то, что работает с наименьшим количеством включенных функций? Но это ведь так просто не проверишь, и нужно сидеть, смотреть и сравнивать все функции, так что попробуем по весу.

Вот мои ядра:
```bash
ls -lh /boot/vmlinuz-*
```
```
-rw------- 1 root root 15M янв 21 03:51 /boot/vmlinuz-6.11.0-17-generic ~ 15kK
-rw-r--r-- 1 root root 13M фев 12 17:26 /boot/vmlinuz-6.14.0-rc2DefConf-00034-gfebbc555cf0f 13274K
-rw-r--r-- 1 root root 13M фев 12 19:00 /boot/vmlinuz-6.14.0-rc2V0.1-00034-gfebbc555cf0f 13205K (Не запускается)
-rw------- 1 root root 15M янв 11 05:24 /boot/vmlinuz-6.8.0-52-generic
```

Видно, что те, которые я сделал, легче, но пока работает только `defconfig`.

Теперь попробуем `make localmodconfig` или `make localyesconfig`. Вроде это должно помочь.

Разница между `make localmodconfig` и `make localyesconfig`:
- **make localmodconfig**: Эта команда создает конфигурацию ядра, включающую только те модули, которые в данный момент загружены в систему. Это позволяет исключить ненужные модули, но оставляет возможность загружать дополнительные модули по мере необходимости. Это более гибкий подход, так как модули могут быть загружены динамически.
- **make localyesconfig**: Эта команда создает конфигурацию ядра, включающую только те модули, которые в данный момент загружены в систему, и компилирует их непосредственно в ядро. Это делает ядро более монолитным и может уменьшить его размер, но лишает вас гибкости динамической загрузки модулей.

---

Напишем простенький shell-скрипт для того, чтобы чуть больше времени освободить, пока все собирается:

```bash
make -j$(nproc)
make modules
sudo make modules_install
sudo make install
sudo update-grub
```

Версия после `localmodconfig` весит 13265K, запускается, но происходят некоторые ошибки, которые в основном связаны с VirtualBox.

Попробуем `make localyesconfig`.

Версия после `localyesconfig` весит 13265K, как и было, запускается, но ошибок не наблюдается.

Теперь опять будем лезть в недра, только теперь после `localyesconfig`.

**General setup**
Отключите отладочные функции и устаревшие компоненты.

**Processor type and features**
Отключите поддержку процессоров, которые вы не используете.

**Power management and ACPI options**
Отключите ненужные функции управления питанием и ACPI.

**Bus options (PCI etc.)**
Отключите поддержку устаревших шин, таких как ISA и MCA.

**Virtualization**
Отключите поддержку виртуализации, если вы не используете виртуальные машины.

**Networking support**
Отключите ненужные сетевые протоколы и функции.

**Device Drivers**
Отключите драйверы устройств, которые вы не используете.

**File systems**
Отключите поддержку файловых систем, которые вы не используете.

**Security options**
Отключите ненужные функции безопасности.

**Kernel hacking**
Отключите отладочные функции ядра.

---

Начну с Kernel hacking. Я узнал, что некоторые символы в названии версии ядра недопустимы...
так. Имеем 12745K. Думаю пока можно остановиться на этом и познать оставшиеся лекции.

---
