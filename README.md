# Linux Kernel KeyLogger

This is a Linux kernel keylogger written in C, it's purpose is for educating myself more about the Linux kernel and system as a whole.

The way it logs the keys is by creating a keyboard event handler, that manages our callbacks from the keyboard. From there it appends each key press to the `msg` string. 
After each keypress it runs file operations on the `msg` and writes it to a device file, which created by dynamically allocating a MAJOR number for the device and registering it's class and the device driver. And that it, after those operations succeed and there are no errors at all, the device file will be created and each keystroke would be added to it automatically. To try it yourself run the following:

```

$ git clone https://github.com/ItayNaf/Linux-Kernel-KeyLogger.git
$ cd Linux-Kernel-KeyLogger
$ make

$ sudo insmod kkeylogger.ko
$ cat /dev/kkeylogger

```

**Note** it was only tested on my Virtual Machine: `Linux 5.19.0-40-generic #41~22.04.1-Ubuntu SMP PREEMPT_DYNAMIC 2 x86_64 x86_64 x86_64 GNU/Linux` so use it at your own risk.
