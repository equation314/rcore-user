#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include <rvm.h>

const int GUEST_MEM_SZ = 16 * 1024 * 1024; // 16 MiB
const int GUEST_ENTRY_POINT = 0x8000;

void test_hypercall() {
    for (int i = 0;; i++) {
        asm volatile (
            "vmcall"
            :
            : "a"(i),
              "b"(2),
              "c"(3),
              "d"(3),
              "S"(3)
            : "memory");
    }
}

int main()
{
    int fd = open("/dev/rvm", O_RDWR);
    printf("rvm fd = %d\n", fd);

    int vmid = ioctl(fd, RVM_GUEST_CREATE, GUEST_MEM_SZ);
    printf("vmid = %d\n", vmid);

    void *guest_phys_mem_ptr = (void *)(intptr_t)ioctl(fd, RVM_GUEST_ACCESS_MEMORY, vmid);
    printf("guest_phys_mem_ptr = %p\n", guest_phys_mem_ptr);

    if (guest_phys_mem_ptr == (void *)-1)
        return 0;

    memcpy((char *)guest_phys_mem_ptr + GUEST_ENTRY_POINT, (char *)test_hypercall, 0x100);

    struct rvm_vcpu_create_args args = {vmid, GUEST_ENTRY_POINT};
    int vcpu_id = ioctl(fd, RVM_VCPU_CREATE, &args);

    printf("vcpu_id = %d\n", vcpu_id);

    for (;;) {
        int ret = ioctl(fd, RVM_VCPU_RESUME, vcpu_id);
        printf("RVM_VCPU_RESUME returns %d\n", ret);
        break;
    }

    close(fd);
    return 0;
}
