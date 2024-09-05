/* sub-function giving the order m for the single index i */
int i2m(int i)
{
  int m;
  m = (i+1) / 2;
  if ((i & 1) == 0)
    m = -m;
  return m;
}  /*i2m*/

int imin(int x,int y){ if (x < y) return x; else return y; }
int imax(int x,int y){ if (x > y) return x; else return y; }

int indx(int n,int m)
{
  int i;

  if (m == 0) {
    i = (n - 1) * (n + 1) + 1;
    return i-1;
  }
  if (m >= 0)
    i = (n - 1) * (n + 1) + m * 2;
  else
    i = (n - 1) * (n + 1) - m * 2 + 1;
  return i-1;
}  /*indx*/

