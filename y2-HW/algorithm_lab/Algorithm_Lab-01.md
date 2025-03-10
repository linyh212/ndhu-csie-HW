---
title: Algorithm_Lab-01
tags: [Algorithm_Lab]

---

[![hackmd-github-sync-badge](https://hackmd.io/pYF2HcfhTqKwnzsKt-iN_w/badge)](https://hackmd.io/pYF2HcfhTqKwnzsKt-iN_w)
#### Question 1:  
```c=
int Classify(int a) {
    switch (a) {
        case 0:
            return 1;
        case 1:
            return 2;
        default:
            return 3;
    }
}
```
#### Question 2:  
>a=1: 1 comparison (matches case 1 directly)  
>a=2: 2 comparisons (checks case 0, then case 1, then goes to default)  
>a=3: 2 comparisons (same as a = 2)
>Average Time:  
>>Assuming a is uniformly distributed over {1, 2, 3}: (1×1+1×2+1×2)/3 = 3/5 ≈1.67  
#### Question 3:  
**Proof:**
>1. **Given:**  
>$f(x)=O(g(x))⇒∃M_1, x_1>0$ such that $∣f(x)∣ ≤ M_1∣g(x)∣, ∀x≥x_1$  
>$g(x)=O(h(x))⇒∃M_2, x_2>0$ such that $∣g(x)∣ ≤ M_2∣h(x)∣, ∀x≥x_2$  
>2. **Let $𝑥_0=max⁡(𝑥_1, 𝑥_2)$, then for all $𝑥≥𝑥_0$:** $∣f(x)∣ ≤ M_1∣g(x)∣ ≤ M_1×M_2∣h(x)∣$  
**Let $𝑀=𝑀_1×𝑀_2$, so:** $∣f(x)∣ ≤ M∣h(x)∣, ∀x≥x_0$  
**Thus:** f(x)=O(h(x))
>3. **Proof completed.**
#### Question 4:  
**Proof:**
>1. Using the logarithm base change formula: $log_an=\dfrac{log_bn}{log_ba}$  
>2. Since $log⁡_𝑏𝑎$ is a constant (as 𝑎,𝑏 are fixed and greater than 1), we get: $log_an=O(log_bn)$  
>3. Proof completed.  