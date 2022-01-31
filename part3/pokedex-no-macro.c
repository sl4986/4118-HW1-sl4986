#include <linux/module.h>
#include <linux/printk.h>
#include <linux/slab.h>
/*
 * Undefine commonly used macros -- DO NOT MODIFY
 * Please also do not use other macros and functions defined in <linux/list.h>,
 * such as INIT_LIST_HEAD, list_add, list_add_tail, list_del, list_empty,
 * or any of the internal helper functions beginning with __.
 */
#undef LIST_HEAD
#undef LIST_HEAD_INIT
#undef list_entry
#undef list_for_each_entry
#undef list_for_each_entry_safe
#undef container_of
#undef offsetof
#undef READ_ONCE
#undef WRITE_ONCE

struct pokemon {
	char name[32];
	int dex_no;
	struct list_head list;
};

void print_pokemon(struct pokemon *p)
{
	printk(KERN_INFO "%s: National Dex No. #%d\n", p->name, p->dex_no);
}

/* TODO: declare a single static struct list_head, named pokedex */
struct list_head pokedex= {&pokedex, &pokedex};
void add_pokemon(char *name, int dex_no)
{
	struct list_head *prev=pokedex.prev;
	struct pokemon *f;
        f=kmalloc(sizeof(struct pokemon), GFP_KERNEL);
	strcpy(f->name,name);
	f->dex_no=dex_no;
        /*add the new pokemon into linkedlist*/
	pokedex.prev=&f->list;
	f->list.next=&pokedex;
	f->list.prev=prev;
	prev->next=&f->list;
	/* TODO: write your code here */
}

void print_pokedex(void)
{
	struct pokemon *p;
	const typeof(((struct pokemon *)0)->list) *_mptr= pokedex.next;
	for (p=(struct pokemon *)((char *)_mptr -((size_t)&((struct pokemon *)0)->list)); &p->list!=&pokedex; p=(struct pokemon *)((char *)(p->list.next)-((size_t)&((struct pokemon *)0)->list))){
		print_pokemon(p);}

	/* TODO: write your code here, using print_pokemon() */
}

void delete_pokedex(void)
{	
	struct pokemon *td, *next;
	const typeof(((struct pokemon *)0)->list) *_mptr =pokedex.next;
	for (td=(struct pokemon *)((char *)_mptr- ((size_t)&((struct pokemon *)0)->list)), next=(struct pokemon *)((char *)(td->list.next)-((size_t)&((struct pokemon *)0)->list)); &td->list!=(&pokedex); td=next, next=(struct pokemon *)((char *)(next->list.next)-((size_t)&((struct pokemon *)0)->list))){
	struct pokemon *temp =td;
	td->list.prev->next=td->list.next;
	td->list.next->prev=temp->list.prev;
	kfree(td);
	}
	
	/* TODO: write your code here */
}

int pokedex_nom_init(void)
{
	printk(KERN_INFO "Loading Module\n");

	add_pokemon("Porygon-Z", 474);
	add_pokemon("Exeggutor", 103);
	add_pokemon("Poliwhirl", 61);
	add_pokemon("Infernape", 392);

	print_pokedex();

	return 0;
}

void pokedex_nom_exit(void)
{
	printk(KERN_INFO "Removing Module\n");

	print_pokedex();

	delete_pokedex();
}

module_init( pokedex_nom_init );
module_exit( pokedex_nom_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A handy Pokedex module, without list macros");
MODULE_AUTHOR("SGG");
