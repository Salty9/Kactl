/**
 * Author: Lucian Bicsi
 * Date: 2017-10-31
 * License: CC0
 * Source: folklore
 * Description: Just a segtree.
 * Can be changed by modifying T, f and unit.
 * Time: O(\log N)
 * Status: stress-tested
 */
template<class T> struct Segtree {
   // initialize ID if need nonzero & define cmb so that cmb(ID,b) = b
    const T ID{};
    T cmb(T a, T b) {
        return a + b;
    }
    int n = 1; vector<T> seg;
    Segtree(int _n){while(n < _n) n *= 2; seg.assign(2*n,ID);}
    void pull(int p) { seg[p] = cmb(seg[2*p],seg[2*p+1]); }

    // CATION: CHOOSE PROPERLY WHETHER UPDATES SHOULD BE SET OR INCREMENTED !!!!!
    void upd(int p, T val) { seg[p += n] = val; for (p /= 2; p; p /= 2) pull(p); }
    T query(int l, int r) {
        T ra = ID, rb = ID;
        for (l += n, r += n+1; l < r; l /= 2, r /= 2) {
            if (l&1) ra = cmb(ra,seg[l++]);
            if (r&1) rb = cmb(seg[--r],rb);
        }
        return cmb(ra,rb);
    }
    // 0 based range and index for upd and query, root at 1, 2*ind, 2*ind+1 children
    template<class F>
    int walk(int p, int l, int r, int x, int y, F &&pred) {
        if (l > y || r < x) return -1;
        if (l >= x && r <= y && !pred(seg[p])) return -1;
        if (r == l) return l;
        int m = (l + r) / 2;
        int res = walk(2 * p, l, m, x, y, pred);//find_last: flip res order(right range 1st)
        if (res == -1) res = walk(2 * p + 1, m+1, r, x, y, pred);
        return res;
    }
    template<class F> int walk(int l, int r, F &&pred) { return walk(1, 0, n-1, l, r, pred); }
    // given 0 based inclusive range [x, y], first time pred(use lambda) is true
};