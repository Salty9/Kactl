#include <bits/stdc++.h>
using namespace std;

template<class A,class B>
ostream& operator<<(ostream& o,const pair<A,B>& p){
    return o<<"{"<<p.first<<", "<<p.second<<"}";
}

template<class T>
requires (!is_same_v<T,string> && !is_convertible_v<T,const char*> &&
          requires(T v){begin(v);end(v);})
ostream& operator<<(ostream& o,const T& v){
    o<<"[";
    for(auto it=begin(v);it!=end(v);++it)
        o<<(it==begin(v)?"":", ")<<*it;
    return o<<"]";
}

inline void dbg_out(){ cerr<<"\n"; }

template<class T,class... Ts>
void dbg_out(const T& t,const Ts&... ts){
    cerr<<t<<(sizeof...(ts)?", ":"");
    dbg_out(ts...);
}

#define dbg(...) cerr<<"\033[91mLine "<<__LINE__<<": "<<#__VA_ARGS__<<" = ", \
                 dbg_out(__VA_ARGS__), cerr<<"\033[0m"
