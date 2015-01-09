int main (int x){
int y= x *x;
int i;
for (i=0;i<10;i++){
if (x<10)x=x*x;
else y=y*y;
}
return 0;
}