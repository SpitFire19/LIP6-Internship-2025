# Estimating gain and optimal strategy

Let A[i, j, e] be the expected gain under the optimal first-player strategy on the contigious sub-array from coin i to coin j (exclusive) when the adversary
is in the state $e \in \{G, O\}$. The adversary is in state $G$ when he follows greedy strategy, and in the state $O$ if he plays optimally.

The adversary (or alternatively, the second player) is modelized by a Markov chain.

The following dynamic program computes the array A for plays of even length, $ (j-i) = 2\N$:

$$
\begin{cases}
A[i. i, e] = 0 & i = j \\
A[i, j, e] = max \begin{cases}
    X_i + A[i + 1,j,e]\\
    X_{j-1} + A[i, j - 1, e]
\end{cases} & i \lt j
\end{cases}
$$

When $j-i=2\N+1$, then the value of a game is calculated the following way:

$$
\begin{cases}
A[i, j, G] = p \times \begin{cases}
    -X_{j-1} + A[i][j- 1][G] &greedy[i][j] = R\\
    -X_{i}   + A[i + 1][j][G] &greedy[i][j] = L\\
\end{cases}\\
+(1-p) \times \begin{cases}
-X_{j-1} + A[i][j- 1][O] &opt[i][j] = R\\
    -X_{i}   + A[i + 1][j][O] &opt[i][j] = L\\
\end{cases}\\
\\A[i, j, O] = q \times \begin{cases}
    -X_{j-1} + A[i][j- 1][O] &opt[i][j] = R\\
    -X_{i}   + A[i + 1][j][O] &opt[i][j] = L
\end{cases}\\
+(1-q)\times \begin{cases}
-X_{j-1} + A[i][j- 1][G] &opt[i][j] = R\\
    -X_{i}   + A[i + 1][j][G] &opt[i][j] = L
\end{cases}

\end{cases}
$$
