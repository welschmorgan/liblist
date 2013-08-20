#include "list.h"
#include "stdio.h"

void data_print (void *d)
{
	printf("%i\n", *((int*)d));
}
int main (int argc, char *argv [])
{
	t_list *l;
	(void) argc;
	(void) argv;
	puts("Lib list");
	l = list_alloc (sizeof(int), NULL, NULL);
	list_push_back (l, node_alloc (l));
	list_push_back (l, node_alloc (l));
	list_push_back (l, node_alloc (l));
	list_print (l, data_print);
	list_clear(l);
	list_print (l, data_print);
	*((int*)list_push_back (l, node_alloc (l))->data)=90;
	list_print (l, data_print);
	list_free (l);
	return 0;
}
