/**
 * Author: usaco
 * Description: Computes mobius values in sieve fashion
 */
const int VALMAX = 1e7;
int mobius[VALMAX];
mobius[1] = -1;
for (int i = 1; i < VALMAX; i++) {
	if (mobius[i]) {
		mobius[i] = -mobius[i];
		for (int j = 2 * i; j < VALMAX; j += i) { mobius[j] += mobius[i]; }
	}
}