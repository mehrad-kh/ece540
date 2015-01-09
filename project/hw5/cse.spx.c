/*
 * This file was created automatically from SUIF
 *   on Thu Apr 10 19:39:38 2014.
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

extern double x[10000];

extern int main();
extern int exit();

double x[10000];

extern int main()
  {
    int i;
    int a;
    int b;
    int c;
    int d;
    int e;
    int f;
    int g;
    double ctr;
    struct _IO_FILE *file;
    double _node15;
    double *_node19;
    int _node20;
    int _node23;
    int _node25;
    double *_node28;
    int _node29;
    int _node31;
    double *_node36;
    int _node37;
    int _node39;
    int _node59;
    double *_node61;
    int _node62;
    int _node66;
    int _node68;
    int _node54;
    int _node55;
    int _node56;
    int _node57;
    int (*_node75)(const char *, ...);
    const char *_node76;
    double *_node77;
    int _node78;
    int _node82;
    int _node84;

    ctr = 0.0;
    i = 0;
  L11:
    _node15 = 200.0;
    _node19 = x;
    _node20 = 8;
    _node23 = 1;
    _node25 = 10000;
  L14:
  L8:
    ctr = ctr + (double)i;
    if (!(_node15 < ctr))
        goto L3;
    ctr = ctr - 300.0;
  L3:
    *(double *)((char *)_node19 + i * _node20) = ctr;
  L1:
    i = i + _node23;
    if (!(_node25 <= i))
        goto L8;
  L2:
    i = 1;
  L12:
    _node28 = x;
    _node29 = 8;
    _node31 = 8;
    _node36 = x;
    _node37 = 8;
    _node39 = 8;
    _node59 = 200;
    _node61 = x;
    _node62 = 8;
    _node66 = 1;
    _node68 = 9999;
  L15:
  L9:
    a = *(double *)((char *)_node28 + i * _node29 + _node31);
    b = *(double *)((char *)_node36 + (i * _node37 - _node39));
    _node54 = a + b;
    _node55 = _node54 + _node54;
    _node56 = _node55 + _node55;
    _node57 = _node56 + _node56;
    g = (_node57 + _node57) % _node59;
    *(double *)((char *)_node61 + i * _node62) = g;
  L4:
    i = i + _node66;
    if (!(_node68 <= i))
        goto L9;
  L5:
    file = fopen("cse.out", "w");
    i = 10;
  L13:
    _node75 = printf;
    _node76 = "x[%d] = %.3f\n";
    _node77 = x;
    _node78 = 8;
    _node82 = 1;
    _node84 = 20;
  L16:
  L10:
    _node75(_node76, i, *(double *)((char *)_node77 + i * _node78));
  L6:
    i = i + _node82;
    if (!(_node84 <= i))
        goto L10;
  L7:
    fclose(file);
    exit(0);
    return 0;
  }
