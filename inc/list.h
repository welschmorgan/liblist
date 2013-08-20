#ifndef __LIB_LIST__H
#define __LIB_LIST__H

struct			s_list;
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

t_node	*node_init ( t_list * list, t_node * n );
t_node	*node_alloc( t_list * list );
void	node_free( t_node * n );

t_list	*list_init( t_list *l, unsigned int elem_size, node_allocator alloc, node_deallocator dealloc );
t_list	*list_alloc( unsigned int elem_size, node_allocator alloc, node_deallocator dealloc );
t_list	*list_clear( t_list *l );
void	list_free( t_list *l );

int main (int argc, char *argv[]);

#endif
