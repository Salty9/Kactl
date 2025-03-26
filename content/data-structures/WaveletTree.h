/**
 * Author: Idk
 * License: CC0
 * Source: https://ideone.com/Tkters
 * Description:  Number of elements in subarray A[L...R] that are less than
 * or equal to y, Number of occurrences of element x in subarray A[L...R].,The
 * kth smallest element in subarray A[L...R]. ALL PROBELMS IN O(logn)
 * Time: ALL operations are $O(\log N)$.
 * Usage: wavelet_tree T(a+1, a+n+1, 1 , MAX)
 * Status: Stress-tested
 */

struct wavelet_tree {
  int lo, hi;
  wavelet_tree *l, *r;
  vector<int> b;
  // nos are in range [ x , y ]
  // array indices are [ from , to )
  wavelet_tree(int* from, int* to, int x, int y) {
    lo = x, hi = y;
    if (lo == hi or from >= to) return;
    int mid = (lo + hi) / 2;
    auto f = [mid](int x) { return x <= mid; };
    b.reserve(to - from + 1);
    b.push_back(0);
    for (auto it = from; it != to; it++) b.pb(b.back() + f(*it));
    auto pivot = stable_partition(from, to, f);
    l = new wavelet_tree(from, pivot, lo, mid);
    r = new wavelet_tree(pivot, to, mid + 1, hi);
  }
  // kth sma l lest element in [ l , r ]
  int kth(int l, int r, int k) {
    if (l > r) return 0;
    if (lo == hi) return lo;
    int inLeft = b[r] - b[l - 1];
    int lb = b[l - 1];  // amt of nos in first (l-1) nos that go in left
    int rb = b[r];      // amt of nos in first (r) nos that go in left
    if (k <= inLeft) return this->l->kth(lb + 1, rb, k);
    return this->r->kth(l - lb, r - rb, k - inLeft);
  }
  // count of nos in [ l , r ] Less than or equal to k
  int LTE(int l, int r, int k) {
    if (l > r or k < lo) return 0;
    if (hi <= k) return r - l + 1;
    int lb = b[l - 1], rb = b[r];
    return this->l->LTE(lb + 1, rb, k) + this->r->LTE(l - lb, r - rb, k);
  }
  // count of nos in [ l , r ] equal to k
  int count(int l, int r, int k) {
    if (l > r or k < lo or k > hi) return 0;
    if (lo == hi) return r - l + 1;
    int lb = b[l - 1], rb = b[r], mid = (lo + hi) / 2;
    if (k <= mid) return this->l->count(lb + 1, rb, k);
    return this->r->count(l - lb, r - rb, k);
  }
  ~wavelet_tree() {
    delete l;
    delete r;
  }
};  // Usage : wave let tree T(a+1, a+n+1, 1 , MAX)