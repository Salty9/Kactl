/**
 * Author: Håkan Terelius
 * Date: 2009-08-26
 * License: CC0
 * Source: http://en.wikipedia.org/wiki/Sieve_of_Eratosthenes
 * Description: Prime sieve for generating all primes up to a certain limit. isprime$[i]$ is true iff $i$ is a prime.
 * Time: lim=100'000'000 $\approx$ 0.8 s. Runs 30\% faster if only odd indices are stored.
 * Status: Tested
 */
#pragma once


vector<int> primes, spf;
void Spf(int lim) {  // lim=1e8 -> 0.8 sec .
  spf.resize(lim);   // smallest prime that divides
  rep(i, 2, lim) {
    if (spf[i] == 0) spf[i] = i, primes.push_back(i);
    for (int p : primes) {
      if (p > spf[i] || i * p >= lim) break;
      spf[i * p] = p;
    }
  }
};
