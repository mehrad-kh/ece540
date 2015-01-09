/*
 * This file was created automatically from SUIF
 *   on Thu Apr 10 19:41:37 2014.
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
extern void matrix_multiply(int (*)[100], int (*)[100], int (*)[100], int);
extern void main();
extern int exit();

extern void matrix_multiply(int (*C)[100], int (*A)[100], int (*B)[100], int n)
  {
    int i;
    int j;
    int k;
    int *suif_tmp1;
    int _node13;
    int _node60;
    int *_node26;
    int _node17;
    int _node18;
    int _node22;
    int _node24;
    int _node56;
    int *_node34;
    int _node36;
    int _node37;
    int *_node42;
    int _node43;
    int _node46;
    int _node52;

    if (!(0 < n))
        goto L12;
    i = 0;
  L15:
    _node13 = 0 < n;
    _node60 = 1;
  L13:
    if (!_node13)
        goto L9;
    j = 0;
  L16:
    _node26 = (int *)C;
    _node17 = i * 400;
    _node18 = 4;
    _node22 = 0;
    _node24 = 0 < n;
    _node56 = 1;
  L10:
    *(int *)((char *)_node26 + _node17 + j * _node18) = _node22;
    if (!_node24)
        goto L7;
    k = 0;
  L17:
    suif_tmp1 = (int *)((char *)_node26 + i * 400 + j * 4);
    _node34 = (int *)A;
    _node36 = i * 400;
    _node37 = 4;
    _node42 = (int *)B;
    _node43 = 400;
    _node46 = j * 4;
    _node52 = 1;
  L8:
    *suif_tmp1 = *suif_tmp1 + *(int *)((char *)_node34 + _node36 + k * _node37) * *(int *)((char *)_node42 + k * _node43 + _node46);
  L5:
    k = k + _node52;
    if (!(n <= k))
        goto L8;
  L6:
    goto __done9;
  L7:
  __done9:
  L3:
    j = j + _node56;
    if (!(n <= j))
        goto L10;
  L4:
    goto __done11;
  L9:
  __done11:
  L1:
    i = i + _node60;
    if (!(n <= i))
        goto L13;
  L2:
    goto __done14;
  L12:
  __done14:
    return;
  }

extern void main()
  {
    int i;
    int j;
    int (A[100])[100];
    int (B[100])[100];
    int (C[100])[100];
    struct _IO_FILE *f;
    int _node5;
    int *_node6;
    int _node7;
    int _node9;
    int _node14;
    int _node32;
    int _node34;
    int _node36;
    int _node38;
    int _node8;
    int (*_node50)(const char *, ...);
    const char *_node51;
    int *_node52;
    int _node53;
    int _node55;
    int _node60;
    int _node62;

    i = 0;
  L12:
    _node5 = 0;
    _node6 = A[0];
    _node7 = 400;
    _node9 = 4;
    _node14 = 0;
    _node32 = 1;
    _node34 = 100;
    _node36 = 1;
    _node38 = 100;
  L10:
    j = _node5;
  L13:
    _node8 = i * _node7;
  L9:
    *(int *)((char *)_node6 + _node8 + j * _node9) = i * j;
    if (!(j != _node14))
        goto L5;
    *(int *)((char *)B[0] + i * 400 + j * 4) = i / j;
    goto __done8;
  L5:
    *(int *)((char *)B[0] + i * 400 + j * 4) = 0;
  __done8:
  L3:
    j = j + _node32;
    if (!(_node34 <= j))
        goto L9;
  L4:
  L1:
    i = i + _node36;
    if (!(_node38 <= i))
        goto L10;
  L2:
    matrix_multiply((int (*)[100])C[0], (int (*)[100])A[0], (int (*)[100])B[0], 100);
    f = fopen("mmult.out", "w");
    i = 0;
  L14:
    _node50 = printf;
    _node51 = "C[%d][%d] = %d\n";
    _node52 = C[0];
    _node53 = 400;
    _node55 = 4;
    _node60 = 1;
    _node62 = 100;
  L11:
    _node50(_node51, i, i, *(int *)((char *)_node52 + i * _node53 + i * _node55));
  L6:
    i = i + _node60;
    if (!(_node62 <= i))
        goto L11;
  L7:
    fclose(f);
    exit(0);
    return;
  }
