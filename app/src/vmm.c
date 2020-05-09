#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include <rvm.h>

const int GUEST_MEM_SZ = 16 * 1024 * 1024; // 16 MiB

int main()
{
    int fd = open("/dev/rvm", O_RDWR);
    printf("rvm fd = %d\n", fd);

    int vmid = ioctl(fd, RVM_GUEST_CREATE, GUEST_MEM_SZ);
    printf("vmid = %d\n", vmid);

    void *guest_phys_mem_ptr = ioctl(fd, RVM_GUEST_ACCESS_MEMORY, vmid);
    printf("guest_phys_mem_ptr = %p\n", guest_phys_mem_ptr);

    {
        int* mem_test = (int*)guest_phys_mem_ptr;
        for (int i = 0; i < 100; i ++) {
            mem_test[i] = i;
        }
        int error = 0;
        for (int i = 0; i < 100; i ++) {
            if (mem_test[i] != i) {
                printf("mem test failed\n");
                error = 1;
                break;
            }
        }
        if (!error) {
            printf("mem test success\n");
        }
    }

    struct rvm_vcpu_create_args args = {vmid, 0x8000};
    int vcpu_id = ioctl(fd, RVM_VCPU_CREATE, &args);

    printf("vcpu_id = %d\n", vcpu_id);

    // void *guest_phys_mem_ptr = mmap(0, GUEST_MEM_SZ, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    // printf("guest_phys_mem_ptr = %p\n", guest_phys_mem_ptr);
    // if (guest_phys_mem_ptr != (void*)-1)
    //     munmap(guest_phys_mem_ptr, GUEST_MEM_SZ);
    close(fd);
    return 0;
}
