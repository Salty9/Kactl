/**
 * Author: Simon Lindholm
 * Date: 2016-10-08
 * License: CC0
 * Source: me
 * Description: Segment tree with ability to add or set values of large intervals, and compute max of intervals.
 * Can be changed to other things.
 * Use with a bump allocator for better performance, and SmallPtr or implicit indices to save memory.
 * Time: O(\log N).
 * Usage: Node* tr = new Node(v, 0, sz(v));
 * Status: stress-tested a bit
 */
#pragma once

#include "../various/BumpAllocator.h"

const int inf = 1e9;
struct Node {
  Node * l = 0, * r = 0;
  int lo, hi, mset = inf, madd = 0, val = -inf;
  Node(int lo, int hi): lo(lo), hi(hi) {} // Large interva l of -inf
  Node(vi & v, int lo, int hi): lo(lo), hi(hi) {
    if (lo + 1 < hi) {
      int mid = lo + (hi - lo) / 2;
      l = new Node(v, lo, mid);
      r = new Node(v, mid, hi);
      val = max(l -> val, r -> val);
    } else val = v[lo];
  }
  int query(int L, int R) {
    if (R <= lo || hi <= L) return -inf;
    if (L <= lo && hi <= R) return val;
    push();
    return max(l -> query(L, R), r -> query(L, R));
  }
  void set(int L, int R, int x) {
    if (R <= lo || hi <= L) return;
    if (L <= lo && hi <= R) mset = val = x, madd = 0;
    else {
      push(), l -> set(L, R, x), r -> set(L, R, x);
      val = max(l -> val, r -> val);
    }
  }
  void add(int L, int R, int x) {
    if (R <= lo || hi <= L) return;
    if (L <= lo && hi <= R) {
      if (mset != inf) mset += x;
      else madd += x;
      val += x;
    } else {
      push(), l -> add(L, R, x), r -> add(L, R, x);
      val = max(l -> val, r -> val);
    }
  }
  void push() {
    if (!l) {
      int mid = lo + (hi - lo) / 2;
      l = new Node(lo, mid);
      r = new Node(mid, hi);
    }
    if (mset != inf)
      l -> set(lo, hi, mset), r -> set(lo, hi, mset), mset = inf;
    else if (madd)
      l -> add(lo, hi, madd), r -> add(lo, hi, madd), madd = 0;
  }
};
// Other implementation
template<class Lazy, class Info>
struct LazySeg {
    vector<Lazy> lazy;
    vector<Info> seg;
    int SZ = 1;
    LazySeg(const int _sz, Info Initial = Info()) {
        while (SZ < _sz) SZ *= 2;
        seg.assign(2*SZ, Initial); lazy.resize(2*SZ);
        for(int i = SZ-1; i >= 1; i--) pull(i);
    }
    LazySeg(const vector<Info>& _seg) {
        while (SZ < _seg.size()) SZ *= 2;
        seg.resize(2*SZ); lazy.resize(2*SZ);
        for(int i = 0; i < SZ and i < _seg.size(); i++) seg[SZ+i] = _seg[i];
        for(int i = SZ-1; i >= 1; i--) pull(i);
    }
    void push(int ind) {// modify values for current node (lazy upds floatin until pushed)
        seg[ind] *= lazy[ind];
        if (ind < SZ) rep(i,0, 2) lazy[2*ind+i] *= lazy[ind];
        lazy[ind] = Lazy();
    }
    void pull(int ind) { seg[ind] = seg[2*ind]+seg[2*ind+1]; }

    // CATION: CHOOSE PROPERLY WHETHER UPDATES SHOULD BE SET OR INCREMENTED !!!!!
    void upd(int lo, int hi, Lazy inc, int ind, int L, int R) {
        push(ind); 
        if (hi < L || R < lo) return;
        if (lo <= L && R <= hi) { lazy[ind] = inc; push(ind); return;}
        int M = (L+R)/2; 
        upd(lo,hi,inc,2*ind,L,M);
        upd(lo,hi,inc,2*ind+1,M+1,R);
        pull(ind);
    }
    void upd(int lo, int hi, Lazy inc) { return upd(lo, hi, inc, 1, 0, SZ-1); }

    Info query(int lo, int hi, int ind, int L, int R) {
        push(ind); 
        if (lo > R || L > hi) return Info();
        if (lo <= L && R <= hi) return seg[ind];
        int M = (L+R)/2;
        return query(lo,hi,2*ind,L,M) + query(lo,hi,2*ind+1,M+1,R);
    }
    Info query(int lo, int hi){ return query(lo, hi, 1, 0, SZ-1);}
    
    // 0 based range and index for upd and query, root at 1, 2*ind, 2*ind + 1 children
    template<class F>
    int walk(int p, int l, int r, int x, int y, F &&pred) {
        if (l > y || r < x) return -1;
        push(p);
        if (l >= x && r <= y && !pred(seg[p])) return -1;
        if (r == l) return l;
        int m = (l + r)/ 2;
        int res = walk(2*p, l, m, x, y, pred);//find_last: flip res order(right range 1st)
        if (res == -1) res = walk(2*p + 1, m+1, r, x, y, pred);
        return res;
    }
    template<class F> int walk(int l, int r, F &&pred) { return walk(1, 0, SZ-1, l, r, pred); }
    // given 0 based inclusive range [x, y], first time pred(use lambda) is true    
};
struct Lazy {
    int inc;
    Lazy(int x = 0): inc(x) {}
    Lazy& operator*=(const Lazy& a) {
        inc += a.inc;
        return *this;
    }
};
struct Info { // info you need to store for each interval
    int mx;
    Info(int x = -1e9) {}
    friend Info operator+(const Info& a, const Info& b) {
        return Info(max(a.mx, b.mx));
    }
    Info& operator*=(const Lazy& a) {
        // sum += sz*a.add;
        // ckmax(mx, a.mx);
        return *this;
    }
};