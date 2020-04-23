#ifndef RVM_H
#define RVM_H

#define RVM_IO 0xAE00
#define RVM_GUEST_CREATE (RVM_IO + 0x01)
#define RVM_VCPU_CREATE (RVM_IO + 0x11)
#define RVM_VCPU_RESUME (RVM_IO + 0x12)

struct rvm_vcpu_create_args {
    unsigned short vmid;
    unsigned long long entry;
};

#endif // RVM_H
