/*
 * This file was created automatically from SUIF
 *   on Fri Apr  4 03:56:45 2014.
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
    int j;
    int k;
    int (A[50])[25];
    int (B[25])[50];
    int (C[50])[50];
    struct _IO_FILE *f;
    int _node6;
    int _node37;
    int _node39;
    int *_node7;
    int _node9;
    int _node10;
    int _node15;
    int _node33;
    int _node35;
    int _node42;
    int _node77;
    int _node79;
    int _node43;
    int _node73;
    int _node75;
    int *_node50;
    int *_node51;
    int _node53;
    int _node54;
    int *_node60;
    int _node61;
    int _node64;
    int _node69;
    int _node71;
    int (*_node86)(const char *, ...);
    const char *_node87;
    int *_node88;
    int _node89;
    int _node91;
    int _node96;
    int _node98;

    i = 0;
  L21:
    _node6 = 0;
    _node37 = 1;
    _node39 = 50;
  L16:
    j = _node6;
  L22:
    _node7 = A[0];
    _node9 = i * 100;
    _node10 = 4;
    _node15 = 0;
    _node33 = 1;
    _node35 = 25;
  L15:
    *(int *)((char *)_node7 + _node9 + j * _node10) = i * j;
    if (!(j != _node15))
        goto L5;
    *(int *)((char *)B[0] + j * 200 + i * 4) = i / j;
    goto __done14;
  L5:
    *(int *)((char *)B[0] + j * 200 + i * 4) = 0;
  __done14:
  L3:
    j = j + _node33;
    if (!(_node35 <= j))
        goto L15;
  L4:
  L1:
    i = i + _node37;
    if (!(_node39 <= i))
        goto L16;
  L2:
    i = 0;
  L23:
    _node42 = 0;
    _node77 = 1;
    _node79 = 50;
  L19:
    j = _node42;
  L24:
    _node43 = 0;
    _node73 = 1;
    _node75 = 50;
  L18:
    k = _node43;
  L25:
    _node50 = (int *)((char *)C[0] + i * 200 + j * 4);
    _node51 = A[0];
    _node53 = i * 100;
    _node54 = 4;
    _node60 = B[0];
    _node61 = 200;
    _node64 = j * 4;
    _node69 = 1;
    _node71 = 25;
  L17:
    *_node50 = i * *(int *)((char *)_node51 + _node53 + k * _node54) - *(int *)((char *)_node60 + k * _node61 + _node64);
  L10:
    k = k + _node69;
    if (!(_node71 <= k))
        goto L17;
  L11:
  L8:
    j = j + _node73;
    if (!(_node75 <= j))
        goto L18;
  L9:
  L6:
    i = i + _node77;
    if (!(_node79 <= i))
        goto L19;
  L7:
    f = fopen("arrays.out", "w");
    i = 0;
  L26:
    _node86 = printf;
    _node87 = "C[%d][%d] = %d\n";
    _node88 = C[0];
    _node89 = 200;
    _node91 = 4;
    _node96 = 1;
    _node98 = 50;
  L20:
    _node86(_node87, i, i, *(int *)((char *)_node88 + i * _node89 + i * _node91));
  L12:
    i = i + _node96;
    if (!(_node98 <= i))
        goto L20;
  L13:
    fclose(f);
    exit(0);
    return;
  }
