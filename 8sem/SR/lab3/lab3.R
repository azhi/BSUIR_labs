N0 <- 10000
lambda <- 1.5

f <- function(t) lambda * exp(-lambda*t)
F <- function(t) 1 - exp(-lambda * t)
inv_F <- function(t) exp(-lambda * t)

reverse_exp_function <- function(x) -1 / lambda * log(x)
sequence <- sort(sapply(runif(N0, 0, 1), reverse_exp_function))
max_t <- max(sequence)

pdf("out.pdf")

N <- sapply(c(0:(N0-1)), function(x) (N0 - x) / N0)
plot(sequence, N, type="l", col="blue", lwd = 10, main="rel(t)", ylab="rel(t)", xlab="t")
plot(inv_F, 0, max_t, col="red", lwd = 4, add=TRUE)
legend("topright", NULL, c("theory rel", "expiremental rel"), fill=c("red", "blue"))

rev_N <- sapply(c(0:(N0-1)), function(x) x / N0)
plot(sequence, rev_N, type="l", col="blue", lwd = 10, main="unrel(t)", ylab="rel(t)", xlab="t")
plot(F, 0, max_t, col="red", lwd = 4, add=TRUE)
legend("bottomright", NULL, c("theory unrel", "expiremental unrel"), fill=c("red", "blue"))

histogram <- hist(sequence, breaks=200, plot=FALSE)
exp_f <- histogram$density
exp_f_xs <- histogram$mids
plot(exp_f_xs, exp_f, type="l", col="blue", lwd = 10, main="f(t)", ylab="f(t)", xlab="t")
plot(f, 0, max(exp_f_xs), col="red", lwd = 4, add=TRUE)
legend("topright", NULL, c("theory f(t)", "expiremental f(t)"), fill=c("red", "blue"))

intence <- function(t) f(t) / inv_F(t)
counts <- histogram$counts
breaks <- histogram$breaks
breaks <- breaks[2:length(breaks)]
print(counts)
print(sapply(1:length(counts), function(i) (N0 - findInterval(breaks[i], sequence))))
exp_intence <- sapply(1:length(counts), function(i) counts[i] / (N0 - findInterval(breaks[i], sequence)) / diff(breaks)[1])
plot(exp_f_xs, exp_intence, type="l", col="blue", lwd = 10, main="lambda(t)", ylab="lambda(t)", xlab="t")
plot(intence, 0, max_t, col="red", lwd = 4, add=TRUE)
legend("topleft", NULL, c("theory lambda(t)", "expiremental lambda(t)"), fill=c("red", "blue"))
