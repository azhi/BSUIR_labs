p1 <- 0.8
p2 <- 0.6
np1 <- (1 - p1)
np2 <- (1 - p2)

a <- c(1, 0, 0, 0, 0, 0, 0, 0, 0,
       -1, 1, -np1, 0, -np1 * np2, 0, 0, 0, 0,
       0, -1, 1, -np1, 0, -np1 * np2, -np2, 0, 0,
       0, 0, -p1, 1, -p1 * np2, 0, 0, -np1 * np2, 0,
       0, 0, 0, -p1, 1, -np1 * p2 - p1 * np2, -p2, 0, -np1 * np2,
       0, 0, 0, 0, -p1 * p2, 1, 0, -np1 * p2 - p1 * np2, 0,
       0, 0, 0, 0, -np1 * p2, 0, 1, 0, 0,
       0, 0, 0, 0, 0, -p1 * p2, 0, 1, -np1 * p2 - p1 * np2 - p1 * p2,
       1, 1, 1, 1, 1, 1, 1, 1, 1)
A <- t( matrix(a, 9, 9) )
print(A)

B <- c(0, 0, 0, 0, 0, 0, 0, 0, 1)
print(B)

r <- solve(A, B)
print(r)
