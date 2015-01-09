/*
 * This file was created automatically from SUIF
 *   on Thu Apr 10 19:41:49 2014.
 *
 * Created by:
 * s2c 5.138 (plus local changes) compiled Thu Oct 9 05:14:25 EDT 2008 by kingyen on ug153
 *     Based on SUIF distribution 1.1.2
 *     Linked with:
 *   libsuif 5.228 (plus local changes) compiled Thu Oct 9 05:12:27 EDT 2008 by kingyen on ug153
 *   libuseful 1.243 (plus local changes) compiled Thu Oct 9 05:12:38 EDT 2008 by kingyen on ug153
 */


struct _IO_marker;

struct __tmp_struct1 { int __val[2]; };
struct _IO_FILE { int _flags;
                  char *_IO_read_ptr;
                  char *_IO_read_end;
                  char *_IO_read_base;
                  char *_IO_write_base;
                  char *_IO_write_ptr;
                  char *_IO_write_end;
                  char *_IO_buf_base;
                  char *_IO_buf_end;
                  char *_IO_save_base;
                  char *_IO_backup_base;
                  char *_IO_save_end;
                  struct _IO_marker *_markers;
                  struct _IO_FILE *_chain;
                  int _fileno;
                  int _flags2;
                  int _old_offset;
                  unsigned short _cur_column;
                  char _vtable_offset;
                  char _shortbuf[1];
                  void *_lock;
                  struct __tmp_struct1 _offset;
                  void *__pad1;
                  void *__pad2;
                  void *__pad3;
                  void *__pad4;
                  unsigned int __pad5;
                  int _mode;
                  char _unused2[40]; };
struct _IO_marker { struct _IO_marker *_next;
                    struct _IO_FILE *_sbuf;
                    int _pos; };

extern int fclose(struct _IO_FILE *);
extern struct _IO_FILE *fopen(const char *, const char *);
extern int printf(const char *, ...);
extern void main();
extern int exit();

extern void main()
  {
    int i;
    int c1;
    int c2;
    int x[2000];
    struct _IO_FILE *f;
    int _node8;
    int _node15;
    int _node27;
    int _node29;
    int (*_node36)(const char *, ...);
    const char *_node37;
    int *_node38;
    int _node39;
    int _node43;
    int _node45;

    i = 0;
  L9:
    c1 = 15;
    _node8 = c1 < 17;
    c2 = c1 + 5;
    _node15 = c1 < c2;
    _node27 = 1;
    _node29 = 2000;
  L7:
    if (!_node8)
        goto L3;
    *(int *)((char *)x + i * 4) = c1;
  L3:
    if (!_node15)
        goto L4;
    *(int *)((char *)x + i * 4) = c2;
  L4:
  L1:
    i = i + _node27;
    if (!(_node29 <= i))
        goto L7;
  L2:
    f = fopen("cond.out", "w");
    i = 0;
  L10:
    _node36 = printf;
    _node37 = "%d\n";
    _node38 = x;
    _node39 = 4;
    _node43 = 1;
    _node45 = 20;
  L8:
    _node36(_node37, *(int *)((char *)_node38 + i * _node39));
  L5:
    i = i + _node43;
    if (!(_node45 <= i))
        goto L8;
  L6:
    fclose(f);
    exit(0);
    return;
  }
