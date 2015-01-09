/*
 * This file was created automatically from SUIF
 *   on Fri Apr  4 03:11:18 2014.
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
extern void vector_dotprod(int *, int *, int *, int);
extern void main();
extern int exit();

extern void vector_dotprod(int *C, int *A, int *B, int n)
  {
    int i;
    int j;
    int t1;
    int t2;
    int t3;
    int _node13;
    int *_node14;
    int _node15;
    int *_node19;
    int _node20;
    int *_node24;
    int _node25;
    int *_node29;
    int _node30;
    int *_node35;
    int _node36;
    int _node39;
    int _node41;
    int _node43;
    int *_node17;
    int *_node22;
    int *_node27;
    int *_node32;
    int *_node38;

    if (!(0 < n))
        goto L6;
    i = 0;
  L9:
    _node13 = 0;
    _node14 = A;
    _node15 = 4;
    _node19 = B;
    _node20 = 4;
    _node24 = A;
    _node25 = 4;
    _node29 = B;
    _node30 = 4;
    _node35 = C;
    _node36 = 4;
    _node39 = 1;
    _node41 = 5;
    _node43 = 1;
  L7:
    j = _node13;
  L10:
    _node17 = (int *)((char *)_node14 + i * _node15);
    _node22 = (int *)((char *)_node19 + i * _node20);
    _node27 = (int *)((char *)_node24 + i * _node25);
    _node32 = (int *)((char *)_node29 + i * _node30);
    _node38 = (int *)((char *)_node35 + i * _node36);
  L5:
    t1 = *_node17;
    t2 = *_node22;
    t3 = *_node27 * *_node32;
    *_node38 = t3;
  L3:
    j = j + _node39;
    if (!(_node41 <= j))
        goto L5;
  L4:
  L1:
    i = i + _node43;
    if (!(n <= i))
        goto L7;
  L2:
    goto __done8;
  L6:
    i = 0;
  __done8:
    return;
  }

extern void main()
  {
    int i;
    int A[1000];
    int B[1000];
    int C[1000];
    struct _IO_FILE *f;
    int *_node4;
    int _node5;
    int *_node9;
    int _node10;
    int _node13;
    int _node15;
    int (*_node27)(const char *, ...);
    const char *_node28;
    int *_node29;
    int _node30;
    int _node34;
    int _node36;

    i = 0;
  L7:
    _node4 = A;
    _node5 = 4;
    _node9 = B;
    _node10 = 4;
    _node13 = 1;
    _node15 = 1000;
  L5:
    *(int *)((char *)_node4 + i * _node5) = i + i;
    *(int *)((char *)_node9 + i * _node10) = i;
  L1:
    i = i + _node13;
    if (!(_node15 <= i))
        goto L5;
  L2:
    vector_dotprod(C, A, B, 1000);
    f = fopen("dotprod.out", "w");
    i = 0;
  L8:
    _node27 = printf;
    _node28 = "C[%d] = %d\n";
    _node29 = C;
    _node30 = 4;
    _node34 = 20;
    _node36 = 1000;
  L6:
    _node27(_node28, i, *(int *)((char *)_node29 + i * _node30));
  L3:
    i = i + _node34;
    if (!(_node36 <= i))
        goto L6;
  L4:
    fclose(f);
    exit(0);
    return;
  }
