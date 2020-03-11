/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   patchy.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkozma <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/10 18:30:03 by alkozma           #+#    #+#             */
/*   Updated: 2020/03/11 09:03:32 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//info is padded by 8 byte null chunks

// checksum - big endian uint sum of bytes, 32 bits of precision

#ifndef PATCHY_H
# define PATCHY_H

# include <stdio.h>
# include <stdlib.h>
# include <sys/mman.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <stdint.h>
# include <string.h>
# include <sys/stat.h>

# define P4_DEBUG 1

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
	uint64_t	empty;
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

	//char		unused[3];
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

#endif
