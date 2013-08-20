#include "list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum	e_errcode
{
	ERR_INVALID_PTR,
	ERR_INVALID_VALUE
}				t_errcode;

char	*tr_err( t_errcode c )
{
	static char	ret[80];
	switch (c)
	{
	case ERR_INVALID_PTR:
		strncpy(ret, "INVALID_PTR", 80);
		break;
	case ERR_INVALID_VALUE: 
		strncpy(ret, "INVALID_VALUE", 80);
		break;
	default:
		strncpy(ret, "UNKNOWN", 80);
	}
	return ret;
}

#define error( code, desc )\
	fprintf (stderr, "[-] Error<%i,%s>: %s in %s\n\t(%s:%i)\n", __FUNCTION__, __FILE__, __LINE__);

#ifdef NDEBUG
#	define logmsg(msg)	fprintf (stdout, "[+] %s\n", msg)
#	define logmsgi(msg, int_)	fprintf (stdout, "[+] %s %i\n", msg, int_)
#	define logmsgn(action, name) fprintf (stdout, "[+] %s %s\n", action, name)
#	define logmsgp(action, ptr) fprintf (stdout, "[+] %s %p\n", action, (void*)ptr)
#else
#	define logmsg(msg)
#	define logmsgn(action, name)
#	define logmsgp(action, ptr)
#	define logmsgi(action, int_)
#endif

#define safe_free(p) if (p) {free(p); p=NULL;}
#define safe_new(type, count) ((type*) malloc( sizeof(type) * count ));

typedef void (*list_iterator)(t_node *);
typedef void (*list_data_printer)(void *data);

t_node	*node_init (t_list * list, t_node * n)
{
	if (!n)
		return NULL;
	logmsgp ("Initialising node", (void*)n);
	n->list = list;
	n->prev = n->next = NULL;
	safe_free (n->data);
	if (!n->data)
		n->data = malloc (list->elem_size);
	return n;
}

t_node	*node_alloc( t_list * list )
{
	t_node * n;
	logmsg ("Allocating new node");
	n = safe_new (t_node, 1);
	n->data = NULL;
	node_init (list, n);
	if (list->alloc_func)
		list->alloc_func (n);
	return n;
}

void	node_free( t_node * n )
{
	logmsgp ("Freeing node", n);
	if (n && n->list && n->list->dealloc_func )
		n->list->dealloc_func (n->data);
	safe_free (n->data);
	if (n->next)
		n->next->prev = n->prev;
	if (n->prev)
		n->prev->next = n->next;
	if (n->list && n->list->tail == n)
		n->list->tail = n->list->tail->next;
	if (n->list && n->list->head == n)
		n->list->head = n->list->head->prev;
	safe_free (n);
}

t_list	*list_init (t_list *l, unsigned int elem_size, node_allocator alloc, node_deallocator dealloc )
{
	if (!l)
		return l;
	logmsgp ("Initialising list", l);
	l->tail = l->head = NULL;
	l->size = 0;
	l->elem_size = elem_size;
	l->alloc_func = alloc;
	l->dealloc_func = dealloc;
	return l;
}

t_list	*list_alloc (unsigned int elem_size, node_allocator alloc, node_deallocator dealloc)
{
	t_list * l;
	logmsgi ("Allocating list, elemsize:", elem_size);
	if (elem_size==0)
		return NULL;
	l = safe_new (t_list, 1);
	list_init (l, elem_size, alloc, dealloc);
	return l;
}

void	list_free( t_list *l )
{
	if (!l)
		return ;
	logmsgp ("Freeing list", l);
	list_clear (l);
	safe_free (l);
}

t_list	*list_clear (t_list *l)
{
	t_node *c, *n;
	logmsgp ("Clearing list", l);
	if (!l)
		return (l);
	c = l->tail;
	while (c)
	{
		n = c->next;
		node_free (c);
		c = n;
	}
	l->tail=l->head=NULL;
	l->size=0;
	return (l);
}

t_node * list_push_back(t_list* l, t_node* n)
{
	if (!l || !n)
	{
		return (n);
	}
	logmsgp ("Pushing node back", n);
	if (!l->tail)
	{
		l->tail=l->head=n;
	}
	else
	{
		l->head->next = n;
		n->prev = l->head;
		l->head = n;
	}
	n->list = l;
	return n;
}

t_list	*list_iterate ( t_list* l, list_iterator iter )
{
	t_node *n;
	if (!l)
	{
		return (l);
	}
	if (!iter)
	{
		return (l);
	}
	n = l->tail;
	while (n)
	{
		iter (n);
		n = n->next;
	}
	return (l);
}

void list_print_iter( t_node * n, list_data_printer printer )
{
	printf ("\tNode %p (prev: %p, next: %p)\n",
			(void*)n,
			(void*)(n?n->prev:NULL),
			(void*)(n?n->next:NULL));
	if (n)
	{
		printf ("\tValue: ");
		printer(n->data);
	}
}

void list_print( t_list* l, list_data_printer printer )
{
	t_node *n;
	
	printf ("List %p, elem_size:%u, count:%u, alloc:%s, dealloc:%s\n",
			(void*)l,
			(l?(l->elem_size):0),
			(l?(l->size):0),
			((l&&l->alloc_func)?"Assigned":"NULL"),
			((l&&l->dealloc_func)?"Assigned":"NULL"));
	if (!l)
	{
		return ;
	}
	if (!printer)
	{
		return ;
	}
	n = l->tail;
	while (n)
	{
		list_print_iter(n, printer);
		n = n->next;
	}
}

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
	list_push_back (l, node_alloc (l));
	list_push_back (l, node_alloc (l));
	list_push_back (l, node_alloc (l));
	list_push_back (l, node_alloc (l));
	list_push_back (l, node_alloc (l));
	list_push_back (l, node_alloc (l));
	list_push_back (l, node_alloc (l));
	list_push_back (l, node_alloc (l));
	list_push_back (l, node_alloc (l));
	list_push_back (l, node_alloc (l));
	list_push_back (l, node_alloc (l));
	list_push_back (l, node_alloc (l));
	list_push_back (l, node_alloc (l));
	list_push_back (l, node_alloc (l));
	list_push_back (l, node_alloc (l));
	*((int*)list_push_back (l, node_alloc (l))->data)=90;
	list_print (l, data_print);
	list_free (l);
	return 0;
}
