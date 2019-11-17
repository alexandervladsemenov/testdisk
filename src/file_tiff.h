/*

    File: file_tiff.h

    Copyright (C) 2009 Christophe GRENIER <grenier@cgsecurity.org>
  
    This software is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
  
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
  
    You should have received a copy of the GNU General Public License along
    with this program; if not, write the Free Software Foundation, Inc., 51
    Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

 */
#ifdef __cplusplus
extern "C" {
#endif

#define TIFF_ERROR 0xffffffffffffffffull

#define TIFF_BIGENDIAN          	0x4d4d
#define TIFF_LITTLEENDIAN       	0x4949
#define TIFFTAG_IMAGEDESCRIPTION        270     /* info about image */
#define TIFFTAG_MAKE                    271     /* scanner manufacturer name */
#define TIFFTAG_MODEL                   272     /* scanner model name/number */
#define	TIFFTAG_STRIPOFFSETS		273	/* offsets to data strips */
#define	TIFFTAG_STRIPBYTECOUNTS		279	/* bytes counts for strips */
#define TIFFTAG_TILEOFFSETS		324
#define TIFFTAG_TILEBYTECOUNTS		325
#define TIFFTAG_SUBIFD                  330
#define	TIFFTAG_JPEGIFOFFSET		513	/* !pointer to SOI marker */
#define	TIFFTAG_JPEGIFBYTECOUNT		514	/* !JFIF stream length */
#define TIFFTAG_KODAKIFD 		33424
#define TIFFTAG_EXIFIFD                 34665
#define EXIFTAG_MAKERNOTE		37500	/* Manufacturer notes */
#define TIFFTAG_SONY_FILEFORMAT		0xb000
#define TIFFTAG_IMAGEOFFSET		0xbcc0
#define TIFFTAG_IMAGEBYTECOUNT		0xbcc1
#define TIFFTAG_ALPHAOFFSET		0xbcc2
#define TIFFTAG_ALPHABYTECOUNT		0xbcc3
#define TIFFTAG_PRINTIM			50341
#define TIFFTAG_DNGVERSION		50706
#define TIFFTAG_DNGPRIVATEDATA		50740	/* &manufacturer's private data */

typedef struct {
        uint16_t  tiff_magic;     /* magic number (defines byte order) */
        uint16_t  tiff_version;   /* TIFF version number */
        uint32_t  tiff_diroff;    /* byte offset to first directory */
} TIFFHeader;

typedef struct {
        uint16_t          tdir_tag;       /* see below */
        uint16_t          tdir_type;      /* data type; see below */
        uint32_t          tdir_count;     /* number of items; length in spec */
        uint32_t          tdir_offset;    /* byte offset to field data */
} TIFFDirEntry;

/* Work around a gcc bug https://gcc.gnu.org/bugzilla/show_bug.cgi?id=52991 */
struct ifd_header {
  uint16_t nbr_fields;
  TIFFDirEntry ifd;
} __attribute__ ((gcc_struct, __packed__));

/*@
  @ requires buffer_size >= sizeof(TIFFHeader);
  @ requires \valid_read(buffer+(0..buffer_size-1));
  @ ensures \valid_read(buffer+(0..buffer_size-1));
  @*/
time_t get_date_from_tiff_header(const unsigned char*buffer, const unsigned int buffer_size);

/*@
  @ requires \valid_read(buffer+(0..buffer_size-1));
  @ requires \separated(potential_error, buffer);
  @ ensures \valid_read(buffer+(0..buffer_size-1));
  @*/
unsigned int find_tag_from_tiff_header(const unsigned char *buffer, const unsigned int buffer_size, const unsigned int tag, const unsigned char **potential_error);

#if !defined(MAIN_tiff_be)
/*@
  @ requires tiff_size >= sizeof(TIFFHeader);
  @ requires tiff_size >= sizeof(struct ifd_header);
  @ requires \valid_read(buffer+(0..tiff_size-1));
  @ requires \valid(potential_error);
  @ requires \separated(potential_error, buffer);
  @ ensures \valid_read(buffer+(0..tiff_size-1));
  @*/
unsigned int find_tag_from_tiff_header_le(const unsigned char *buffer, const unsigned int tiff_size, const unsigned int tag, const unsigned char**potential_error);
#endif

#if !defined(MAIN_tiff_be) && !defined(MAIN_jpg)
/*@
  @ requires \valid(fr);
  @ requires \valid(fr->handle);
  @ requires \valid_read(&fr->extension);
  @ requires valid_read_string(fr->extension);
  @ ensures \valid(fr->handle);
  @ ensures valid_read_string(fr->extension);
  @*/
void file_check_tiff_le(file_recovery_t *fr);

/*@
  @ requires buffer_size >= 15;
  @ requires \valid_read(buffer+(0..buffer_size-1));
  @ requires \valid_read(file_recovery);
  @ requires \valid(file_recovery_new);
  @ requires file_recovery_new->blocksize > 0;
  @ ensures \result == 0 || \result == 1;
  @ ensures (\result == 1) ==> (file_recovery_new->file_check == &file_check_tiff_le);
  @ ensures (\result == 1) ==> (file_recovery_new->extension != \null);
  @ ensures (\result == 1) ==>  valid_read_string(file_recovery_new->extension);
  @*/
int header_check_tiff_le(const unsigned char *buffer, const unsigned int buffer_size, const unsigned int safe_header_only, const file_recovery_t *file_recovery, file_recovery_t *file_recovery_new);
#endif

#if !defined(MAIN_tiff_le)
/*@
  @ requires tiff_size >= sizeof(TIFFHeader);
  @ requires tiff_size >= sizeof(struct ifd_header);
  @ requires \valid_read(buffer+(0..tiff_size-1));
  @ requires \valid(potential_error);
  @ requires \separated(potential_error, buffer);
  @ ensures \valid_read(buffer+(0..tiff_size-1));
  @*/
unsigned int find_tag_from_tiff_header_be(const unsigned char*buffer, const unsigned int tiff_size, const unsigned int tag, const unsigned char**potential_error);
#endif

#if !defined(MAIN_tiff_le) && !defined(MAIN_jpg)
/*@
  @ requires buffer_size >= 15;
  @ requires \valid_read(buffer+(0..buffer_size-1));
  @ requires \valid_read(file_recovery);
  @ requires \valid(file_recovery_new);
  @ requires file_recovery_new->blocksize > 0;
  @ ensures \result == 0 || \result == 1;
  @ ensures (\result == 1) ==> (file_recovery_new->extension != \null);
  @ ensures (\result == 1) ==>  valid_read_string(file_recovery_new->extension);
  @*/
int header_check_tiff_be(const unsigned char *buffer, const unsigned int buffer_size, const unsigned int safe_header_only, const file_recovery_t *file_recovery, file_recovery_t *file_recovery_new);
#endif

unsigned int tiff_type2size(const unsigned int type);
#ifdef DEBUG_TIFF
const char *tag_name(unsigned int tag);
#endif

#ifdef __cplusplus
} /* closing brace for extern "C" */
#endif
