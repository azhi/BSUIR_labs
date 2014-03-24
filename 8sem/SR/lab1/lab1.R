f <- function(lambda) function(t) lambda * exp(-lambda*t)
F <- function(lambda) function(t) 1 - exp(-lambda * t)
m <- function(lambda) 1 / lambda
D <- function(lambda) 1 / lambda ^ 2
median <- function(lambda) - log(0.5) / lambda

plot_samples <- function(f, samples, colors, xrange, yrange, title, legendpos) {
  plot(xrange, yrange, type="n", main=title, ylab=title, xlab="t")
  for (i in 1:length(samples))
    plot(f(samples[i]), xrange[1], xrange[2], add=TRUE, col=colors[i])
  legend_labels = sapply(samples, function(sample) paste("lambda=", sample, sep=""))
  legend(legendpos, NULL, legend_labels, fill=colors[1:length(samples)])
}

lambdas <- c(3.0, 1.5, 1.0, 0.5, 0.2)
colors <- c("black", "red", "blue", "green", "yellow")

pdf("out.pdf")
plot_samples(f, lambdas, colors, c(0,5), c(0,3), "f(t)", "topright")
plot_samples(F, lambdas, colors, c(0,5), c(0,1), "F(t)", "bottomright")
plot(m, 0, 3, main="m(lambda)", ylab="m(lambda)", xlab="lambda")
plot(D, 0, 3, main="D(lambda)", ylab="D(lambda)", xlab="lambda")
plot(median, 0, 3, main="median(lambda)", ylab="median(lambda)", xlab="lambda")
plot(c(0,5), type="n", main="mod(lambda)", ylab="mod(lambda)", xlab="lambda")
abline(h=0.0)
plot(c(0,5), type="n", main="assymetric(lambda)", ylab="assymetric(lambda)", xlab="lambda")
abline(h=2.0)
