#define _GNU_SOURCE
#include <string.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <syscall.h>

static char child_stack[1048576];

static int child_fn(void* arg) {
    system("mount -t proc proc /proc --make-private");
    system("ifconfig veth1 10.1.1.2 up");
    system("dd if=/dev/zero of=fsimage.fs count=1024 bs=1048675");
    system("losetup -d /dev/loop5");
    system("losetup /dev/loop5 fsimage.fs");
    system("mkfs fsimage.fs");
    system("mount -t ext2 /dev/loop5 /home --make-private");
    system("dd if=/dev/zero of=/home/test.txt count=1024 bs=1048675");
    system("echo Hello world!");
    system("ip link");
    system("ls");
    system("pstree");
    system("/bin/bash");
    system("umount /home");
    system("umount /proc");
    return 0;
}

int main() {
    pid_t child_pid = clone(
            child_fn,
            child_stack+1048576,
            CLONE_NEWPID| CLONE_NEWNET | CLONE_NEWNS | SIGCHLD, NULL);
    waitpid(child_pid, NULL, 0);
    _exit(EXIT_SUCCESS);
}