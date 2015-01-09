/*
 * This file was created automatically from SUIF
 *   on Thu Apr 10 19:41:15 2014.
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

extern double (x[300])[300];

extern int main();
extern int exit();

double (x[300])[300];

extern int main()
  {
    int i;
    int j;
    int k;
    int t;
    double ctr;
    struct _IO_FILE *f;
    int _node9;
    double _node16;
    double *_node20;
    int _node21;
    int _node23;
    int _node27;
    int _node29;
    int _node31;
    int _node33;
    unsigned int _node10;
    int _node22;
    int _node36;
    int _node37;
    int _node38;
    double *_node39;
    int _node40;
    int _node42;
    double *_node46;
    int _node47;
    int _node49;
    double *_node54;
    int _node55;
    int _node57;
    int _node59;
    double *_node65;
    int _node66;
    int _node68;
    int _node70;
    double *_node76;
    int _node77;
    int _node79;
    int _node81;
    double *_node87;
    int _node88;
    int _node90;
    int _node92;
    double _node98;
    int _node102;
    int _node104;
    int _node106;
    int _node108;
    int _node110;
    int _node112;
    int _node114;
    int _node116;
    int _node41;
    int _node48;
    int _node58;
    int _node69;
    int _node78;
    int _node89;
    double *_node45;
    double *_node52;
    double *_node62;
    double *_node73;
    double *_node84;
    double *_node95;
    int _node123;
    int _node124;
    int _node125;
    double *_node126;
    int _node127;
    int _node129;
    double *_node133;
    int _node134;
    int _node136;
    double *_node141;
    int _node142;
    int _node144;
    int _node146;
    double *_node152;
    int _node153;
    int _node155;
    int _node157;
    double *_node163;
    int _node164;
    int _node166;
    int _node168;
    double *_node174;
    int _node175;
    int _node177;
    int _node179;
    double _node185;
    int _node189;
    int _node191;
    int _node193;
    int _node195;
    int _node197;
    int _node199;
    int (*_node201)(const char *, ...);
    const char *_node202;
    double *_node205;
    double *_node209;
    int _node211;
    int _node213;
    int _node128;
    int _node135;
    int _node145;
    int _node156;
    int _node165;
    int _node176;
    double *_node132;
    double *_node139;
    double *_node149;
    double *_node160;
    double *_node171;
    double *_node182;

    ctr = 0.0;
    i = 0;
  L32:
    _node9 = 0;
    _node16 = 200.0;
    _node20 = x[0];
    _node21 = 2400;
    _node23 = 8;
    _node27 = 1;
    _node29 = 300;
    _node31 = 1;
    _node33 = 300;
  L23:
    j = _node9;
  L33:
    _node10 = i;
    _node22 = i * _node21;
  L22:
    ctr = ctr + (double)(int)(_node10 | (unsigned int)j);
    if (!(_node16 < ctr))
        goto L5;
    ctr = ctr - 300.0;
  L5:
    *(double *)((char *)_node20 + _node22 + j * _node23) = ctr;
  L3:
    j = j + _node27;
    if (!(_node29 <= j))
        goto L22;
  L4:
  L1:
    i = i + _node31;
    if (!(_node33 <= i))
        goto L23;
  L2:
    t = 0;
  L34:
    _node36 = 1;
    _node37 = 1;
    _node38 = 1;
    _node39 = x[0];
    _node40 = 2400;
    _node42 = 8;
    _node46 = x[0];
    _node47 = 2400;
    _node49 = 8;
    _node54 = x[0];
    _node55 = 2400;
    _node57 = 2400;
    _node59 = 8;
    _node65 = x[0];
    _node66 = 2400;
    _node68 = 2400;
    _node70 = 8;
    _node76 = x[0];
    _node77 = 2400;
    _node79 = 8;
    _node81 = 8;
    _node87 = x[0];
    _node88 = 2400;
    _node90 = 8;
    _node92 = 8;
    _node98 = 5.0;
    _node102 = 1;
    _node104 = 5;
    _node106 = 1;
    _node108 = 299;
    _node110 = 1;
    _node112 = 299;
    _node114 = 1;
    _node116 = 5;
  L27:
    i = _node36;
  L35:
  L26:
    j = _node37;
  L36:
    _node41 = i * _node40;
    _node48 = i * _node47;
    _node58 = i * _node55 - _node57;
    _node69 = i * _node66 + _node68;
    _node78 = i * _node77;
    _node89 = i * _node88;
  L25:
    k = _node38;
  L37:
    _node45 = (double *)((char *)_node39 + _node41 + j * _node42);
    _node52 = (double *)((char *)_node46 + _node48 + j * _node49);
    _node62 = (double *)((char *)_node54 + _node58 + j * _node59);
    _node73 = (double *)((char *)_node65 + _node69 + j * _node70);
    _node84 = (double *)((char *)_node76 + _node78 + (j * _node79 - _node81));
    _node95 = (double *)((char *)_node87 + _node89 + j * _node90 + _node92);
  L24:
    *_node45 = (*_node52 + *_node62 + *_node73 + *_node84 + *_node95) / _node98 + (double)k;
  L12:
    k = k + _node102;
    if (!(_node104 <= k))
        goto L24;
  L13:
  L10:
    j = j + _node106;
    if (!(_node108 <= j))
        goto L25;
  L11:
  L8:
    i = i + _node110;
    if (!(_node112 <= i))
        goto L26;
  L9:
  L6:
    t = t + _node114;
    if (!(_node116 <= t))
        goto L27;
  L7:
    f = fopen("sor.out", "w");
    t = 0;
  L38:
    _node123 = 1;
    _node124 = 1;
    _node125 = 1;
    _node126 = x[0];
    _node127 = 2400;
    _node129 = 8;
    _node133 = x[0];
    _node134 = 2400;
    _node136 = 8;
    _node141 = x[0];
    _node142 = 2400;
    _node144 = 2400;
    _node146 = 8;
    _node152 = x[0];
    _node153 = 2400;
    _node155 = 2400;
    _node157 = 8;
    _node163 = x[0];
    _node164 = 2400;
    _node166 = 8;
    _node168 = 8;
    _node174 = x[0];
    _node175 = 2400;
    _node177 = 8;
    _node179 = 8;
    _node185 = 5.0;
    _node189 = 1;
    _node191 = 5;
    _node193 = 1;
    _node195 = 299;
    _node197 = 1;
    _node199 = 299;
    _node201 = printf;
    _node202 = "x[20][5] = %.3f, x[149][100] = %.3f\n";
    _node205 = x[0] + 6005;
    _node209 = x[0] + 44800;
    _node211 = 1;
    _node213 = 5;
  L31:
    i = _node123;
  L39:
  L30:
    j = _node124;
  L40:
    _node128 = i * _node127;
    _node135 = i * _node134;
    _node145 = i * _node142 - _node144;
    _node156 = i * _node153 + _node155;
    _node165 = i * _node164;
    _node176 = i * _node175;
  L29:
    k = _node125;
  L41:
    _node132 = (double *)((char *)_node126 + _node128 + j * _node129);
    _node139 = (double *)((char *)_node133 + _node135 + j * _node136);
    _node149 = (double *)((char *)_node141 + _node145 + j * _node146);
    _node160 = (double *)((char *)_node152 + _node156 + j * _node157);
    _node171 = (double *)((char *)_node163 + _node165 + (j * _node166 - _node168));
    _node182 = (double *)((char *)_node174 + _node176 + j * _node177 + _node179);
  L28:
    *_node132 = (*_node139 + *_node149 + *_node160 + *_node171 + *_node182) / _node185 + (double)k;
  L20:
    k = k + _node189;
    if (!(_node191 <= k))
        goto L28;
  L21:
  L18:
    j = j + _node193;
    if (!(_node195 <= j))
        goto L29;
  L19:
  L16:
    i = i + _node197;
    if (!(_node199 <= i))
        goto L30;
  L17:
    _node201(_node202, *_node205, *_node209);
  L14:
    t = t + _node211;
    if (!(_node213 <= t))
        goto L31;
  L15:
    fclose(f);
    exit(0);
    return 0;
  }
