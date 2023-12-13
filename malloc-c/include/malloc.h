#ifndef MALLOC_H
# define MALLOC_H

# include <sys/mman.h>
# include <stdint.h>
# include <unistd.h>
# include "malloc_error.h"

// 1432005180
# define MAGIC_N			0x555AAA3C

# define PAGE_TINY_SIZE		(16 * getpagesize())
# define BLOCK_TINY_LIMIT	(size_t)(PAGE_TINY_SIZE / 128)
# define PAGE_SMALL_SIZE	(32 * getpagesize())
# define BLOCK_SMALL_LIMIT	(size_t)(PAGE_SMALL_SIZE / 128)

# define STRUCT_PAGE_SIZE	(size_t)(sizeof(t_page))
# define STRUCT_BLOCK_SIZE	(size_t)(sizeof(t_block))

typedef void				*t_v;
typedef char				*t_ch;

# define PAGE_JUMP(p, s)	((t_ch)p + STRUCT_PAGE_SIZE + s)
# define BLOCK_JUMP(b, s)	((t_ch)b + STRUCT_BLOCK_SIZE + s)
# define BLOCK_META(b)		((t_block *)((t_ch)b - STRUCT_BLOCK_SIZE))

# define AVAILABLE			1
# define UNAVAILABLE		0

typedef struct s_main_page
{
	struct s_page	*tiny_head;
	struct s_page	*tiny_last;
	struct s_page	*small_head;
	struct s_page	*small_last;
	struct s_page	*large_head;
	struct s_page	*large_last;
}	t_main_page;

typedef enum e_page_type
{
	P_TINY,
	P_SMALL,
	P_LARGE
}	t_page_type;

typedef struct s_page
{
	size_t			size;
	size_t			block_unvail_qt;
	t_page_type		type;
	struct s_page	*prev;
	struct s_page	*next;
	struct s_block	*block_head;
}	t_page;

typedef struct s_block
{
	size_t			magic_num;
	size_t			size;
	int				avail;
	struct s_block	*prev;
	struct s_block	*next;
}	t_block;

void		*malloc(size_t area_size);
void		*calloc(size_t nmemb, size_t memb_size);
void		*realloc(void *ptr, size_t area_size);
void		free(void *ptr);

void		*alloc_memory(size_t page_size);

t_page		*page_validation(void *ptr);
t_block		*block_validation(t_page *page, void *ptr);

t_main_page	*main_page_get(void);
void		main_page_update(t_page *page);
t_page		*page_get_available(size_t area_size);
t_page		*page_create(size_t block_size);

t_block		*block_get_available(t_page *page, size_t area_size);
void		block_reserve(t_block *block, size_t area_size, t_page_type page_type);
t_block		*block_place(void *page, size_t area_size);

void		show_alloc_mem(void);
void		printl(int fd, char *s);
void		print_address_hex(int fd, void *p);
void		print_num(int fd, long n);
void		print_error_malloc(void *ptr, char *msg);
void		*ft_memset(void *s, int c, size_t len);
void		*ft_memcpy(void *dest, const void *source, size_t size);

#endif
