/*
 * sys/stat.h - file status
 *
 * libcmini's own, self-contained version: on m68k this used to be found
 * via the mint(elf) cross toolchain's bundled sysroot header instead
 * (which happened to work since nothing else in this include tree also
 * defines these names), but the ARM toolchain's bundled newlib headers
 * define a conflicting set of internal __int32_t/__uid_t/etc. types that
 * clash with this library's own <sys/types.h>. Providing this file keeps
 * libcmini self-contained on both, rather than depending on whatever the
 * cross toolchain happens to ship.
 *
 * struct stat here matches the one already used internally by
 * ext.h (same field set, same fstat()/stat() write it) -- fstat.c and
 * stat.c only need the S_I* mode-bit macros from this header, plus the
 * struct/prototypes ext.h would otherwise be the only other source of.
 */

#ifndef _SYS_STAT_H
#define _SYS_STAT_H 1

#ifndef _FEATURES_H
# include <features.h>
#endif

#ifndef _SYS_TYPES_H
# include <sys/types.h>
#endif

__BEGIN_DECLS

struct stat
{
    int    st_dev;
    int    st_ino;
    int    st_mode;
    int    st_nlink;
    int    st_uid;
    int    st_gid;
    int    st_rdev;
    size_t st_size;
    long   st_atime;
    long   st_mtime;
    long   st_ctime;
};

#define S_IFMT   0170000
#define S_IFREG  0100000
#define S_IFDIR  0040000
#define S_IFCHR  0020000
#define S_IFIFO  0010000
#define S_IFLNK  0120000 /* GEMDOS has no real symlinks; never actually set */

#define S_ISREG(m)  (((m) & S_IFMT) == S_IFREG)
#define S_ISDIR(m)  (((m) & S_IFMT) == S_IFDIR)
#define S_ISCHR(m)  (((m) & S_IFMT) == S_IFCHR)
#define S_ISFIFO(m) (((m) & S_IFMT) == S_IFIFO)
#define S_ISLNK(m)  (((m) & S_IFMT) == S_IFLNK)

#define S_IRUSR  0000400
#define S_IWUSR  0000200
#define S_IXUSR  0000100
#define S_IRWXU  (S_IRUSR | S_IWUSR | S_IXUSR)

#define S_IRGRP  (S_IRUSR >> 3)
#define S_IWGRP  (S_IWUSR >> 3)
#define S_IXGRP  (S_IXUSR >> 3)
#define S_IRWXG  (S_IRWXU >> 3)

#define S_IROTH  (S_IRGRP >> 3)
#define S_IWOTH  (S_IWGRP >> 3)
#define S_IXOTH  (S_IXGRP >> 3)
#define S_IRWXO  (S_IRWXG >> 3)

int fstat(int handle, struct stat *buff);
int stat(const char *path, struct stat *buff);

__END_DECLS

#endif /* _SYS_STAT_H */
