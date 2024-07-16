
struct list_head {
    struct list_head *next, *prev;
};


struct car{
    struct list_head	list;
    unsigned int	registered_number;
    unsigned int	price_in_dollars;
};

struct list_head my_car_list;

#define list_entry(ptr, type, member) \
    container_of(ptr, type, member)

#define container_of(ptr, type, member) ({ \
const typeof( ((type *)0)->member ) *__mptr = (ptr); \
(type *)( (char *)__mptr - offsetof(type,member) );})

#define offsetof(TYPE, MEMBER) ((size_t)&((TYPE *)0)->MEMBER)


int main(){

    struct car a;
    struct car b;
    struct car c;
    struct list_head* curr;

    my_car_list.next = &a.list;
    a.list.next = &b.list;
    b.list.next = &c.list;

    curr = &my_car_list;
    curr = curr -> next;





}
