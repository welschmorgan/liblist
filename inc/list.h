#ifndef __LIB_LIST__H
#define __LIB_LIST__H

struct				s_list;
typedef struct  	s_node
{
	struct s_list *	list;
	struct s_node *	prev,
				  *	next;
	void		  * data;
}					t_node;

typedef void (*node_allocator)   (void *data);
typedef void (*node_deallocator) (void *data);

typedef struct 			s_list
{
	struct s_node *		tail,
				  *		head;
	unsigned int		size;
	unsigned int		elem_size;
	node_allocator 		alloc_func;
	node_deallocator  	dealloc_func;
}						t_list;

typedef void (*list_iterator)(t_node *);
typedef void (*list_data_printer)(void *data);

t_node	*node_init ( t_list * list, t_node * n );
t_node	*node_alloc( t_list * list );
void	node_free( t_node * n );

t_list	*list_init( t_list *l, unsigned int elem_size, node_allocator alloc, node_deallocator dealloc );
t_list	*list_alloc( unsigned int elem_size, node_allocator alloc, node_deallocator dealloc );
t_list	*list_clear( t_list *l );
void	list_free( t_list *l );

t_node * list_push_back(t_list* l, t_node* n);
t_node * list_push_front(t_list* l, t_node* n);
t_list * list_pop_back(t_list* l);
t_list * list_pop_front(t_list* l);
t_node * list_insert_back(t_list * l, t_node * after, t_node * n );
t_node * list_insert_front(t_list * l, t_node * before, t_node * n );

t_list	*list_iterate ( t_list* l, list_iterator iter );
void list_print_iter( t_node * n, list_data_printer printer );
void list_print( t_list* l, list_data_printer printer );

#endif
