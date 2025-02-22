
# Сборка и настройка Zlib, OpenSSL и OpenSSH для ARM

## Сборка Zlib

1. Скачал версию Zlib 1.2.13.
2. Установил переменную окружения:
   ```sh
   export CC=arm-linux-gnueabihf-gcc
   ```
3. Выполнил конфигурацию:
   ```sh
   CC=arm-linux-gnueabihf-gcc ./configure --prefix=_install
   ```
4. Собрал проект:
   ```sh
   CC=arm-linux-gnueabihf-gcc make
   ```
5. Установил библиотеку:
   ```sh
   make install
   ```

## Сборка OpenSSL

1. Установил переменные окружения:
   ```sh
   export CC=arm-linux-gnueabihf-gcc
   export AR=arm-linux-gnueabihf-ar
   export NM=arm-linux-gnueabihf-nm
   export RANLIB=arm-linux-gnueabihf-ranlib
   export LD=arm-linux-gnueabihf-ld
   ```
2. Выполнил конфигурацию:
   ```sh
   ./Configure linux-armv4 --prefix=$HOME/VSproj/C/Dop_cont/openssl-install no-asm no-shared zlib zlib-dynamic
   ```
3. Собрал проект:
   ```sh
   make
   ```
4. Установил библиотеки:
   ```sh
   make install_sw
   ```

## Сборка OpenSSH

1. Установил переменные окружения:
   ```sh
   export CC=arm-linux-gnueabihf-gcc
   export AR=arm-linux-gnueabihf-ar
   export RANLIB=arm-linux-gnueabihf-ranlib
   export STRIP=arm-linux-gnueabihf-strip
   export CPPFLAGS="-I$HOME/VSproj/C/Dop_cont/openssl-install/include -I$HOME/VSproj/C/Dop_cont/zlib-1.2.13/_install/include"
   export LDFLAGS="-L$HOME/VSproj/C/Dop_cont/openssl-install/lib -L$HOME/VSproj/C/Dop_cont/zlib-1.2.13/_install/lib"
   ```
2. Выполнил конфигурацию:
   ```sh
   ./configure --host=arm-linux-gnueabihf \
       --prefix=$HOME/VSproj/C/Dop_cont/ssh-install \
       --with-ssl-dir=$HOME/VSproj/C/Dop_cont/openssl-install \
       --with-zlib=$HOME/VSproj/C/Dop_cont/zlib-1.2.13/_install \
       --without-privsep-path
   ```
3. Собрал проект:
   ```sh
   make
   ```
4. Установил OpenSSH:
   ```sh
   make install
   ```

## Создание корневой файловой системы

1. Скопировал директорию `_install` из BusyBox:
   ```sh
   cp -r $HOME/VSproj/C/Dop_cont/test_openSSH/dop/_install .
   ```
2. Скопировал бинарники OpenSSH:
   ```sh
   cp $HOME/VSproj/C/Dop_cont/ssh-install/bin/* bin/
   cp $HOME/VSproj/C/Dop_cont/ssh-install/sbin/sshd sbin/
   ```
3. Скопировал вспомогательные программы SSH:
   ```sh
   mkdir -p libexec
   cp -r $HOME/VSproj/C/Dop_cont/ssh-install/libexec/* libexec/
   ```
4. Скопировал конфигурационные файлы SSH:
   ```sh
   mkdir -p etc
   cp $HOME/VSproj/C/Dop_cont/ssh-install/etc/* etc/
   touch etc/moduli
   ```
5. Создал директорию для SSH-ключей:
   ```sh
   mkdir -p var/run/sshd
   chmod 700 var/run/sshd
   ```
6. Скопировал зависимые библиотеки:
   ```sh
   mkdir -p lib
   cp $HOME/VSproj/C/Dop_cont/openssl-install/lib/libcrypto.so* lib/
   cp $HOME/VSproj/C/Dop_cont/openssl-install/lib/libssl.so* lib/
   cp $HOME/VSproj/C/Dop_cont/zlib-1.2.13/_install/lib/libz.so* lib/
   ```
7. Скопировал базовые библиотеки C:
   ```sh
   cp /usr/arm-linux-gnueabihf/lib/ld-linux-armhf.so* lib/
   cp /usr/arm-linux-gnueabihf/lib/libc.so* lib/
   ```

## Запуск QEMU

1. Создал минимальный `init`-файл:
   ```sh
   #!/bin/sh
   exec /bin/ash
   ```
2. Запустил QEMU:
   ```sh
   QEMU_AUDIO_DRV=none qemu-system-arm \
     -M vexpress-a9 \
     -kernel zImage \
     -dtb vexpress-v2p-ca9.dtb \
     -initrd initramfs.cpio.gz \
     -append "console=ttyAMA0 root=/dev/ram rw" \
     -nographic \
     -net nic -net user,hostfwd=tcp::2222-:22 \
     -m 1024
   ```

## Настройка внутри QEMU

1. Создал необходимые устройства:
   ```sh
   mkdir -p /dev
   mknod -m 666 /dev/null c 1 3
   mknod -m 666 /dev/zero c 1 5
   mknod -m 666 /dev/random c 1 8
   mknod -m 666 /dev/urandom c 1 9
   ```
2. Смонтировал файловые системы:
   ```sh
   mkdir -p /proc
   mkdir -p /sys
   mount -t proc none /proc
   mount -t sysfs none /sys
   ```
3. Создал пользователя `root`:
   ```sh
   echo "root:x:0:0:root:/root:/bin/ash" > /etc/passwd
   echo "root:x:0:" > /etc/group
   mkdir -p /root
   chmod 700 /root
   ```
4. Создал хост-ключи SSH:
   ```sh
   ssh-keygen -t rsa -f /etc/ssh/ssh_host_rsa_key -N ''
   ssh-keygen -t ecdsa -f /etc/ssh/ssh_host_ecdsa_key -N ''
   ssh-keygen -t ed25519 -f /etc/ssh/ssh_host_ed25519_key -N ''
   ```
5. Настроил права доступа для ключей:
   ```sh
   chmod 600 /etc/ssh/ssh_host_*_key
   chmod 644 /etc/ssh/ssh_host_*_key.pub
   ```
6. Создал минимальный файл конфигурации SSH:
   ```sh
   echo "Port 22" > /etc/ssh/sshd_config
   echo "PermitRootLogin yes" >> /etc/ssh/sshd_config
   echo "PasswordAuthentication yes" >> /etc/ssh/sshd_config
   echo "HostKey /etc/ssh/ssh_host_rsa_key" >> /etc/ssh/sshd_config
   echo "HostKey /etc/ssh/ssh_host_ecdsa_key" >> /etc/ssh/sshd_config
   echo "HostKey /etc/ssh/ssh_host_ed25519_key" >> /etc/ssh/sshd_config
   ```
7. Создал пользователя `sshd`:
   ```sh
   echo "sshd:x:74:74:Privilege-separated SSH:/var/run/sshd:/sbin/nologin" >> /etc/passwd
   echo "sshd:x:74:" >> /etc/group
   ```
8. Создал необходимые директории:
   ```sh
   mkdir -p /var/run/sshd
   chmod 700 /var/run/sshd
   chown sshd:sshd /var/run/sshd
   mkdir -p /var/empty
   chown root:root /var/empty
   chmod 700 /var/empty
   ```
9. Запустил SSH демон:
   ```sh
   /sbin/sshd -ddd

   /sbin/sshd -ddd
debug1: sshd version OpenSSH_9.9, OpenSSL 3.3.1 4 Jun 2024
debug3: Running on Linux 6.14.0-rc2-00034-gfebbc555cf0f #1 SMP Thu Feb 13 15:43:47 +07 2025 armv7l
debug3: Started with: /sbin/sshd -ddd
debug2: load_server_config: filename //etc/sshd_config
debug2: load_server_config: done config len = 3294
debug2: parse_server_config_depth: config //etc/sshd_config len 3294
debug3: //etc/sshd_config:41 setting AuthorizedKeysFile .ssh/authorized_keys
debug3: //etc/sshd_config:111 setting Subsystem sftp	//libexec/sftp-server
debug1: private host key #0: ssh-rsa SHA256:ZZqTSemYzasz5Ha6J+kC+PUq276wBzpmQUrRf1K3OO4

debug1: private host key #1: ecdsa-sha2-nistp256 SHA256:M2dv7LgpelEOsBp3tHxKasKe8+iJKvSxh3AhMxb7ObE
debug1: private host key #2: ssh-ed25519 SHA256:7upc8Kb+qDLl5u4ISUGFVJ8+yonGtTnRLb1VTXVeolo
debug1: rexec_argv[1]='-ddd'
debug3: using //libexec/sshd-session for re-exec
debug3: oom_adjust_setup
debug1: Set /proc/self/oom_score_adj from 0 to -1000
debug2: fd 7 setting O_NONBLOCK
debug3: sock_set_v6only: set socket 7 IPV6_V6ONLY
debug1: Bind to port 22 on ::.
Server listening on :: port 22.
debug2: fd 8 setting O_NONBLOCK
debug1: Bind to port 22 on 0.0.0.0.
Server listening on 0.0.0.0 port 22.

   ```
Я не особо понимаю почему подключение не работает, думаю я брендмауэр потрогать надо или еще что.
## Проверка работы SSH

```sh
ssh -p 2222 root@localhost


```
