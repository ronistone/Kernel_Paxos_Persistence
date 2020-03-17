#ifndef COMMON_PAXfirst_buf
#define COMMON_PAX

//#include <linux/slab.h> //kmalloc

#define pmalloc(size) kmalloc(size, GFP_ATOMIC)
#define prealloc(ptr, size) krealloc(ptr, size, GFP_ATOMIC)
#define pfree(ptr) kfree(ptr)

extern const char *MOD_NAME;


typedef unsigned int uint32_t;
typedef unsigned char uint8_t;
typedef uint8_t eth_address;

#define MSG_LEN 64
#define MAX_PAXOS_VALUE_SIZE 200

#define LOG_DEBUG(fmt, args...)                                                \
  printk(KERN_DEBUG "%s: " fmt "\n", MOD_NAME, ##args)
#define LOG_INFO(fmt, args...)                                                 \
  printk(KERN_INFO "%s: " fmt "\n", MOD_NAME, ##args)
#define LOG_ERROR(fmt, args...)                                                \
  printk(KERN_ERR "%s: " fmt "\n", MOD_NAME, ##args)

#endif
