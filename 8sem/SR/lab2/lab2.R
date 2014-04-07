N <- 100000
lambda <- 1.5

reverse_exp_function <- function(x) -1 / lambda * log(x)
F <- function(t) 1 - exp(-lambda * t)

sequence <- sapply(runif(N, 0, 1), reverse_exp_function)

distribution_histogramm <- hist(sequence, breaks=20, plot=FALSE)
freqs <- sapply(distribution_histogramm$counts, function(c) c / N)
breaks <- distribution_histogramm$breaks
break_diffs <- diff(breaks)
th_freqs <- sapply(1:(length(breaks)-1), function(i) F(breaks[i+1]) - F(breaks[i]))

pdf("out.pdf")
break_labels <- breaks[2:length(breaks)]
th_break_xs <- sapply(1:(length(breaks) - 1), function(i) breaks[i] + break_diffs[i] / 2 )
barplot(freqs, names.arg=break_labels, width=break_diffs[1], space=0, main="Distribution histogram", xlab="t", ylab="freq")
lines(th_break_xs, th_freqs, col="red")

m <- mean(sequence)
D <- var(sequence)
th_m <- 1 / lambda
th_D <- 1 / lambda ^ 2

plot(0:100, 0:100, type="n", axes=FALSE, xlab="", ylab="")
text(20, 90, paste("m = ", m))
text(70, 90, paste("th_m = ", th_m))
text(40, 80, paste("dm = ", abs(th_m - m)))
text(20, 50, paste("D = ", D))
text(70, 50, paste("th_D = ", th_D))
text(40, 40, paste("dD = ", abs(th_D - D)))
