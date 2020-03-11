/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fileparser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkozma <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/10 18:30:03 by alkozma           #+#    #+#             */
/*   Updated: 2020/03/10 20:03:28 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//info is padded by 8 byte null chunks

// checksum - big endian uint sum of bytes, 32 bits of precision

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>

#define P4_DEBUG 1

enum e_err
{
	NONE,
	USAGE,
	PATH,
	MEMFAIL,
	BADFILE
};

struct	p4_header
{
	char				magic[4];
	uint32_t			unk0;		// sizeof header?
	uint32_t			empty0;

	// 4 bits empty, advance pointer by 4 here
	uint32_t			version_count;
	uint32_t			unk1;
	uint32_t			rounded_size;

	//all files in all versions
	uint32_t			total_file_count;
	uint32_t			total_file_size;

	uint32_t			unk2;

	//checksum of "UpdateFileList.bin" without the header itself
	uint32_t			checksum;

	uint64_t			empty;
};

// properties below pertain to "catalog_env.info"
struct	p4_catalog_env
{
	uint32_t	size;
	uint32_t	checksum;
	uint32_t	empty;
};

struct	p4_version
{
	//zero-based index
	uint32_t	index;

	// offset position
	uint32_t	offset;

	//total size and count for all files in this version
	uint32_t	ver_file_size;
	uint32_t	ver_file_count;

	char		unused[3];
	//version dir name
	char		path[32];
};

struct	p4_file
{
	uint32_t	size;
	uint32_t	checksum;
	uint64_t	empty0;
	char		path[128];
};

int		patchy_error(int code, char *str)
{
	switch (code)
	{
		case 1:
			return (printf("usage: ./patchy [path_to_bin] [path_to_pac]\n"));
		case 2:
			return (printf("patchy: invalid path: %s\n", str));
		case 3:
			return (printf("patchy: mmap failed\n"));
		case 4:
			return (printf("patchy: bad file\n"));
		default:
			break ;
	}
	return (0);
}

uint32_t	swapend(uint32_t num)
{
	return (((num >> 24) & 0xff) | ((num << 8) & 0xff0000)
		| ((num >> 8) & 0xff00) | ((num << 24) & 0xff000000));
}

void	parse_header(struct p4_header *h)
{
	struct p4_catalog_env	*ce;
	struct p4_version		*v;
	struct p4_file			*f;

	ce = ((void*)h) + sizeof(struct p4_header);
	v = (void*)(ce + 1);
	f = (void*)(v + 1);
	if (strcmp(h->magic, "BBDL"))
		patchy_error(BADFILE, NULL);
	printf("GAME VERSION: %d\n", swapend(h->version_count));
	printf("SIZE OF FILE: %d\n", swapend(h->rounded_size));
	printf("TOTAL FILE COUNT: %d\n", swapend(h->total_file_count));
	printf("TOTAL SIZE OF FILES: %d\n", swapend(h->total_file_size));
	printf("%d\n", swapend(ce->size));
	printf("%zu\n", sizeof(struct p4_catalog_env));
	printf("%s\n", v->path);
	printf("%s\n", f->path);
}

int		main(int argc, char **argv)
{
	void		*start;
	struct stat	s;
	int			fd;

	printf("PERSONA 4 BIN PATCHER\n=============\n");
	if (argc != 3)
		return (patchy_error(USAGE, NULL));
	if ((fd = open(argv[1], O_RDONLY)) < 0 || fstat(fd, &s) < 0)
		return (patchy_error(PATH, argv[1]));
	start = mmap(0, s.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	printf("INPUT FILE: %s\n", argv[1]);
	if (start == MAP_FAILED)
		return (patchy_error(MEMFAIL, NULL));
	parse_header(start);
}
