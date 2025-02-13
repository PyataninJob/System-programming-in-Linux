
# Выполняем шаги по лекции

## Устанавливаем QEMU

```bash
sudo apt install qemu qemu-kvm libvirt-daemon-system libvirt-clients bridge-utils virt-manager
```

И под ARM тоже:

```bash
sudo apt install qemu-system-arm
```

## Устанавливаем компилятор

Установим именно `gcc-linux-gnueabihf-base` и сам компилятор:

```bash
sudo apt install gcc-arm-linux-gnueabihf
```

## Создаем базовый конфиг и запускаем сборку

```bash
ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- make zImage
```

## Результат

В общем, все по видео абсолютно, и получили такую же ошибку:

```
[    5.027647]
[    5.028223] Kernel panic - not syncing: VFS: Unable to mount root fs on unknown-block(0,0)
[    5.030548] CPU: 0 UID: 0 PID: 1 Comm: swapper/0 Not tainted 6.14.0-rc2-00034-gfebbc555cf0f #1
[    5.032246] Hardware name: ARM-Versatile Express
[    5.034047] Call trace:
[    5.035872]  unwind_backtrace from show_stack+0x10/0x14
[    5.039117]  show_stack from dump_stack_lvl+0x54/0x68
[    5.039310]  dump_stack_lvl from panic+0x10c/0x370
[    5.039788]  panic from mount_root_generic+0x1f8/0x2a8
[    5.040660]  mount_root_generic from prepare_namespace+0x1fc/0x254
[    5.041442]  prepare_namespace from kernel_init+0x1c/0x12c
[    5.041812]  kernel_init from ret_from_fork+0x14/0x28
[    5.042239] Exception stack(0xc8825fb0 to 0xc8825ff8)
[    5.042973] 5fa0:                                     00000000 00000000 00000000 00000000
[    5.044122] 5fc0: 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
[    5.048049] 5fe0: 00000000 00000000 00000000 00000000 00000013 00000000
[    5.050584] ---[ end Kernel panic - not syncing: VFS: Unable to mount root fs on unknown-block(0,0) ]---
QEMU: Terminated
```
