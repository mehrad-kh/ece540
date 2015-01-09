/*
 * This file was created automatically from SUIF
 *   on Fri Apr  4 03:04:23 2014.
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
extern float f1(float, float);
extern int f2(int);
extern int f3(int, int);
extern int f4(int, int);
extern int f5(int, int);
extern int f6(int);
extern int f7(int, int);
extern void main();
extern int exit();

extern float f1(float b, float c)
  {
    int i;
    float j;
    float k;

    j = c;
    i = 0;
  L4:
  L3:
    k = b * (float)i;
    j = j + k;
  L1:
    i = i + 1;
    if (!(300 <= i))
        goto L3;
  L2:
    return k;
  }

extern int f2(int b)
  {
    int i;
    int j;
    int k;

    k = b;
    i = 0;
  L7:
  L6:
    j = 0;
  L8:
  L5:
    k = k + j;
  L3:
    j = j + 1;
    if (!(3 <= j))
        goto L5;
  L4:
  L1:
    i = i + 1;
    if (!(300 <= i))
        goto L6;
  L2:
    return k;
  }

extern int f3(int b, int c)
  {
    int i;
    int j;
    int k;

    j = c;
    i = 0;
  L4:
  L3:
    k = b * i;
    j = j + k;
  L1:
    i = i + 1;
    if (!(300 <= i))
        goto L3;
  L2:
    return k;
  }

extern int f4(int b, int c)
  {
    int i;
    int j;
    int k;

    j = 0;
    i = 0;
  L4:
  L3:
    k = b + c;
    j = j + k;
  L1:
    i = i + 1;
    if (!(300 <= i))
        goto L3;
  L2:
    return j;
  }

extern int f5(int b, int c)
  {
    int i;
    int j;
    int k;

    j = 3;
    i = 0;
  L6:
  L5:
    if (!(j == 0))
        goto L3;
    k = i * b;
    goto __done4;
  L3:
    k = i * c;
  __done4:
  L1:
    i = i + 1;
    if (!(300 <= i))
        goto L5;
  L2:
    return k;
  }

extern int f6(int c)
  {
    int a[300];
    int b[300];
    int i;
    int k;

    k = 0;
    i = 0;
  L10:
  L7:
    *(int *)((char *)a + i * 4) = i;
  L1:
    i = i + 1;
    if (!(300 <= i))
        goto L7;
  L2:
    i = 0;
  L11:
  L8:
    *(int *)((char *)b + i * 4) = c + *(int *)((char *)a + i * 4);
  L3:
    i = i + 1;
    if (!(300 <= i))
        goto L8;
  L4:
    i = 0;
  L12:
  L9:
    k = k + *(int *)((char *)b + i * 4);
  L5:
    i = i + 1;
    if (!(300 <= i))
        goto L9;
  L6:
    return k;
  }

extern int f7(int b, int c)
  {
    int i;
    int j;

    j = 3;
    i = 0;
  L6:
  L5:
    if (!(i == 3))
        goto L3;
    j = b;
    goto __done4;
  L3:
    j = c;
  __done4:
  L1:
    i = i + 1;
    if (!(300 <= i))
        goto L5;
  L2:
    return j;
  }

extern void main()
  {
    int t;
    struct _IO_FILE *f;
    float suif_tmp0;
    int suif_tmp1;
    int suif_tmp2;
    int suif_tmp3;
    int suif_tmp4;
    int suif_tmp5;
    int suif_tmp6;

    t = 3;
    suif_tmp0 = f1(4.0F, 5.0F);
    t = (float)t + suif_tmp0;
    suif_tmp1 = f2(3);
    t = t + suif_tmp1;
    suif_tmp2 = f3(4, 5);
    t = t + suif_tmp2;
    suif_tmp3 = f4(4, 5);
    t = t + suif_tmp3;
    suif_tmp4 = f5(4, 5);
    t = t + suif_tmp4;
    suif_tmp5 = f6(3);
    t = t + suif_tmp5;
    suif_tmp6 = f7(3, 4);
    t = t + suif_tmp6;
    f = fopen("testprog.out", "w");
    printf("finished: %d\n", t);
    fclose(f);
    exit(0);
    return;
  }
