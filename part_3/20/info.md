# Простой пример

## Создание тестового файла `test.c`

```c
#include <linux/module.h>
#include <linux/kernel.h>

int init_module(void)
{
    pr_info("My test module loaded!!!!!\n");
    return 0;
}

void cleanup_module(void)
{
    pr_info("My test module unloaded!!!!!\n");
}

MODULE_LICENSE("GPL");  // лицензия
```

## Создание Makefile

```makefile
obj-m += test.o

PWD := $(CURDIR)

all:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
```

В статье сказали, что так будет лучше из-за проблем с переменными окружения.

## Просмотр логов до загрузки модуля

```bash
sudo dmesg | tail -5
```

Пример вывода:

```
[12135.235311] audit: type=1400 audit(1740905008.527:75329): apparmor="DENIED" operation="open" class="file" profile="snap.snapd-desktop-integration.snapd-desktop-integration" name="/usr/share/icons/" pid=635768 comm="desktop-launch" requested_mask="r" denied_mask="r" fsuid=1000 ouid=0
[12135.236783] audit: type=1400 audit(1740905008.527:75330): apparmor="DENIED" operation="open" class="file" profile="snap.snapd-desktop-integration.snapd-desktop-integration" name="/var/lib/snapd/desktop/icons/" pid=635768 comm="desktop-launch" requested_mask="r" denied_mask="r" fsuid=1000 ouid=0
[12135.402507] audit: type=1400 audit(1740905008.600:75331): apparmor="DENIED" operation="open" class="file" profile="snap.snapd-desktop-integration.snapd-desktop-integration" name="/usr/share/glib-2.0/schemas/" pid=635940 comm="desktop-launch" requested_mask="r" denied_mask="r" fsuid=1000 ouid=0
[12135.409055] audit: type=1400 audit(1740905008.600:75332): apparmor="DENIED" operation="open" class="file" profile="snap.snapd-desktop-integration.snapd-desktop-integration" name="/usr/share/glib-2.0/schemas/" pid=635942 comm="desktop-launch" requested_mask="r" denied_mask="r" fsuid=1000 ouid=0
[12135.505141] audit: type=1400 audit(1740905008.784:75333): apparmor="DENIED" operation="connect" class="file" profile="snap.snapd-desktop-integration.snapd-desktop-integration" name="/run/user/1000/wayland-0" pid=635950 comm="snapd-desktop-i" requested_mask="wr" denied_mask="wr" fsuid=1000 ouid=1000
```

У меня какой-то высокий уровень журналирования, поэтому посмотрим так:

```bash
sudo dmesg -l info
```

Пример вывода:

```
[   77.044939] My test module loaded!!!!!
[  258.268759] My test module unloaded!!!!!
[  401.991510] My test module loaded!!!!!
```

## После удаления модуля:

```bash
sudo rmmod test
sudo dmesg -l info
```

Пример вывода:

```
[   77.044939] My test module loaded!!!!!
[  258.268759] My test module unloaded!!!!!
[  401.991510] My test module loaded!!!!!
[  548.743773] My test module unloaded!!!!!
```

## Модуль ввода/вывода

Makefile такой же, код взят из лекции.

```bash
sudo dmesg -l info
sudo insmod test.ko
sudo dmesg -l info
```

Пример вывода:

```
[ 1918.751649] Test module is loaded.
[ 1918.751661] Major number is 240.
```

Мой номер 240, поэтому:

```bash
sudo mknod test c 240 0
ls -l | grep test
```

Пример вывода:

```
crw-r--r--  1 root     root    240,   0 мар  2 16:30 test
```

## Проблема с чтением/записью

```bash
cat /dev/test
cat: /dev/test: Неправильный адрес
echo "NewData" > /dev/test
cat /dev/test
cat: /dev/test: Неправильный адрес
```

Подумал сначала на компилятор, поэтому добавил:

```makefile
KBUILD_CFLAGS += -no-canonical-prefixes
KBUILD_HOSTCC := x86_64-linux-gnu-gcc-13
KBUILD_CFLAGS += -Wall -Werror
```

## Модификация программы для отладки

```bash
[ 9775.206336] Test module is unloaded.
[ 9962.554184] Test module is loading...
[ 9962.554199] Test module loaded successfully. Major number: 240
[10011.550767] test_write called with size=9, off=0
[10011.550777] Wrote 9 bytes to buffer. New offset: 9. Buffer content: 'New data
               '
[10015.166644] test_read called with size=131072, off=0
```

Последняя запись тут — это вызов `cat`. Почему-то она имеет некое желание прочитать вообще все, что существует в этом мире.

А вот это вызов `sudo dd if=/dev/test bs=1 count=15`. Ну то есть я по одному блоку прочитал, и все хорошо читается.

```bash
[10084.782388] Read 1 bytes from buffer. New offset: 1
[10084.782670] test_read called with size=1, off=1
[10084.782675] Read 1 bytes from buffer. New offset: 2
[10084.782680] test_read called with size=1, off=2
[10084.782682] Read 1 bytes from buffer. New offset: 3
[10084.782686] test_read called with size=1, off=3
[10084.782688] Read 1 bytes from buffer. New offset: 4
[10084.782691] test_read called with size=1, off=4
[10084.782693] Read 1 bytes from buffer. New offset: 5
[10084.782697] test_read called with size=1, off=5
[10084.782699] Read 1 bytes from buffer. New offset: 6
[10084.782702] test_read called with size=1, off=6
[10084.782704] Read 1 bytes from buffer. New offset: 7
[10084.782708] test_read called with size=1, off=7
[10084.782710] Read 1 bytes from buffer. New offset: 8
[10084.782713] test_read called with size=1, off=8
[10084.782715] Read 1 bytes from buffer. New offset: 9
[10084.782719] test_read called with size=1, off=9
[10084.782721] Read 1 bytes from buffer. New offset: 10
[10084.782731] test_read called with size=1, off=10
[10084.782733] Read 1 bytes from buffer. New offset: 11
[10084.782790] test_read called with size=1, off=11
[10084.782795] Read 1 bytes from buffer. New offset: 12
[10084.782799] test_read called with size=1, off=12
[10084.782801] Read 1 bytes from buffer. New offset: 13
[10084.782805] test_read called with size=1, off=13
[10084.782807] Read 1 bytes from buffer. New offset: 14
[10084.782811] test_read called with size=1, off=14
[10084.782813] Read 1 bytes from buffer. New offset: 15
```

## Проверка странной записи

```bash
echo "New dataфывайцкйукпуйкпйкпйупй3рй3кфвацф4й34фвпйу" > /dev/test
```

Вывод:

```
bash: echo: ошибка записи: Недопустимый аргумент
[10436.042652] test_write called with size=86, off=0
```