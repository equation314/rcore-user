#ifndef RVM_H
#define RVM_H

#include <stdint.h>

#define RVM_IO 0xAE00
#define RVM_GUEST_CREATE (RVM_IO + 0x01)
#define RVM_GUEST_ADD_MEMORY_REGION (RVM_IO + 0x02)
#define RVM_GUEST_SET_TRAP (RVM_IO + 0x03)
#define RVM_VCPU_CREATE (RVM_IO + 0x11)
#define RVM_VCPU_RESUME (RVM_IO + 0x12)

struct rvm_vcpu_create_args {
    uint16_t vmid;
    uint64_t entry;
};

struct rvm_guest_add_memory_region_args {
    uint16_t vmid;
    uint64_t guest_start_paddr;
    uint64_t memory_size;
};

enum rvm_trap_kind {
    RVM_TRAP_KIND_MEM = 1,
    RVM_TRAP_KIND_IO = 2,
};

struct rvm_guest_set_trap_args {
    uint16_t vmid;
    uint32_t kind;
    uint64_t addr;
    uint64_t size;
    uint64_t key;
};

#endif // RVM_H
