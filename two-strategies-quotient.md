# Comparing naive and optimal winning strategies

We have previously described two winning strategies for the first player.
The first one, the 'naive' strategy allows us to gain at least as much points as the second player by chosing the parity of elements we pick throughout the game. We will note out gain under this strategy $G_{par}$

The second one is based on dynamic programming and allows for the greatest gain given that our adversary plays optimally. We will note our gain under this strategy $G_{opt}$.

**Theorem 1**. Let $k\in\Z$ a whole number and let $G = a_{1}, a_{2},.., a_{n}$ be a game on an array of whole numbers. Then $val_{G} = val_{G + k}$, where $G + k$ is a game on the array $a_{1}+k, a_{2}+k.., a_{n}+k$, that is the same number is added to each element of an array. \
**Proof**. Let $\sigma$ be a first-player strategy and $\rho$ a second player strategy. Let $S_{G, \sigma}$ be the gain of the first player on $G$ and $S_{G, \rho}$ be the gain of the second player on $G$. Then one has the value of the game $val_{G}(\sigma, \rho) = S_{G, \sigma} - S_{G, \rho}$.\
Consequently, $S_{{G+k}, \sigma} = S_{G, \sigma}$ $+ k \times \frac{n}{2}$ and
$S_{{G+k}, \rho} = S_{G, \rho}$ $+ k \times \frac{n}{2}$ as both players make the same amount of moves per game. Hence $val_{G+k}(\sigma, \rho) = S_{{G+k}, \sigma} - S_{{G+k}, \rho} = S_{G, \sigma}$ $+ k \times \frac{n}{2} - S_{G, \rho}$ $- k \times \frac{n}{2} = S_{G, \sigma} - S_{G, \rho} = val_{G}(\sigma, \rho).$ $☐$


It is easy to see that the second strategy almost gives at least as much as the first one. One may apply **Theorem 1** to obtain a game on non-negative integers, and it is then clear that
$
\frac{G_{opt}}{G_{par}} \ge 1
$.

The next thing one wants to know is if the quotient 
$\frac{G_{opt}}{G_{par}}$ is bounded or not.

**Theorem 2**. Let $G = a_{1}, a_{2},.., a_{n}$ a game on positive integers and let  $G_{opt}, G_{par} \ge 0$ gains of the first player under optimal and parity strategy respectively, given that the second player plays optimally. Then one has

$$
\frac{G_{opt}}{G_{par}} \le 2
$$
**Proof**. It is clear that $G_{opt} \le \sum{a_{i}}$. Then, under parity strategy, we have that no matter the strategy of the second player, $G_{par} \ge \frac{1}{2} \times \sum{a_{i}}$.In turn, this easily gives $G_{par} \ge \frac{1}{2} \times G_{opt} \iff \frac{G_{opt}}{2 \times G_{par}} \le 1 \iff \frac{G_{opt}}{G_{par}} \le 2$. \
This upper bound is reached in the following example: consider a play on the array
    [1, 0, 0, 1, 0, 0]. Then following the parity strategy give $G_{par} = 1$ and $G_{opt} = 2$, as  one may also use the parity strategy on one of sub-arrays [0, 0, 1, 0] or [0, 1, 0, 0], which are all possible sub-arrays on the second move of the first player the first being taking the $1$ on the left.
$☐$

One knows now that playing the parity strategy against an optimal adversary, one may never gain less than half under the optimal strategy.

# Tracking greedy and optimal adversary

One has previously discussed the gain of Player 1 under different strategies when Player 2 plays optimally. As the objective is to maximize our gain when the second player changes his strategy, possibly every move, following certain model, we might want to consider the algorithm where we track the adversary's moves to be able to gain more than just again the optimal adversary.

One introduces for this objective a notion of **strategy**.\
A **strategy** is a function $f:(i, j) \in \N^{*} \times \N^{*} \rarr \{i, j\}$, $ j - i = 2 \N + 1$.\
We consider two particular strategies:
$$
f_{gr} = \begin{cases}
i, a_{i} \ge a_{j}\\
j, otherwise
\end{cases}
$$

$$
f_{opt} = \begin{cases}
i, a_{i} - V_{opt}(i+1, j) \ge a_{j} - V_{opt}(i, j - 1)\\
j, otherwise
\end{cases}
$$

$$
V_{opt}(i, j) = \begin{cases}
a_{i}, i=j\\
max \begin{cases}
       a_{i} - V_{opt}(i+1, j)\\
       a_{j} - V_{opt}(i, j - 1)
    \end{cases}
,i < j
\end{cases}
$$

One says that a pair $(i, j) \in \N^{*} \times \N^{*}, i \le j$ has a property $P$ if 
$f_{opt}(i, j) = f_{gr}(i, j)$.

It has been seen in a series of experiments that at least half of all of pairs $(i, j) \in \{1, .. n\}^{2}, j - i = 2 \N + 1$, have a property $P$. This is intuitively logical, as there is always at most two values to choose and one may not immediately deduce the strategy followed by one's adversary from one move.
This allows to formulate the following

**Hypothesis 1**. The probability that a move under a greedy strategy coincides with a move under optimal strategy is at least 0.5.\
In other words, we cannot tell what strategy Player 2 is currently following in at least half of the cases.
This negative result makes us to look more attentively at cases where Player 2 obviously follows greedy strategy.
