/*
 * This file was created automatically from SUIF
 *   on Wed Apr  9 00:52:42 2014.
 *
 * Created by:
 * s2c 5.138 (plus local changes) compiled Thu Oct 9 05:14:25 EDT 2008 by kingyen on ug153
 *     Based on SUIF distribution 1.1.2
 *     Linked with:
 *   libsuif 5.228 (plus local changes) compiled Thu Oct 9 05:12:27 EDT 2008 by kingyen on ug153
 *   libuseful 1.243 (plus local changes) compiled Thu Oct 9 05:12:38 EDT 2008 by kingyen on ug153
 */


extern int printf(const char *, ...);
extern int main();

static char __tmp_string_3[6] = "****\n";
static char __tmp_string_4[7] = "*****\n";

extern int main()
  {
    int a;
    int b;
    int c;
    int d;
    int e;
    int f;
    int suif_tmp0;
    int suif_tmp1;
    int _node49;
    int _node46;
    int _node43;
    int _node40;
    int _node37;
    int _node30;
    int _node24;
    int _node21;
    int _node18;

    a = 2;
    b = 4;
    _node49 = a + b;
    _node46 = _node49;
    _node43 = _node46;
    _node40 = _node43;
    _node37 = _node40;
    _node30 = _node37;
    _node24 = _node30;
    _node21 = _node24;
    _node18 = _node21;
    c = _node18;
    suif_tmp0 = a;
    if (suif_tmp0 < 1 || 4 < suif_tmp0)
        goto L7;
    switch (suif_tmp0)
      {
      case 1:
        goto L2;
      case 2:
        goto L3;
      case 3:
        goto L4;
      case 4:
        goto L5;
      default:
        goto L6;
      }
  L7:
    goto L6;
  L2:
    printf("*\n");
    c = _node18;
  L3:
    printf("**\n");
    d = _node21;
  L4:
    printf("***\n");
    b = _node24;
  L5:
    printf(__tmp_string_3);
    _node49 = a + b;
    _node46 = _node49;
    _node43 = _node46;
    _node40 = _node43;
    _node37 = _node40;
    _node30 = _node37;
    e = _node30;
  L6:
    printf(__tmp_string_4);
    f = _node30;
  L1:
    suif_tmp1 = a;
    if (suif_tmp1 < 1 || 4 < suif_tmp1)
        goto L14;
    switch (suif_tmp1)
      {
      case 1:
        goto L9;
      case 2:
        goto L10;
      case 3:
        goto L11;
      case 4:
        goto L12;
      default:
        goto L13;
      }
  L14:
    goto L13;
  L9:
    printf("*\n");
    b = _node37;
    goto L8;
  L10:
    printf("**\n");
    f = _node40;
    goto L8;
  L11:
    printf("***\n");
    b = _node43;
    goto L8;
  L12:
    printf(__tmp_string_3);
    e = _node46;
    goto L8;
  L13:
    printf(__tmp_string_4);
    c = _node49;
    goto L8;
  L8:
    return 0;
  }
