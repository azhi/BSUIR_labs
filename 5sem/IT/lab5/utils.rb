def str_gen n, base
  ar = []
  n.times{ ar << rand(0...base) }
  ar.join(' ')
end

def stti str, base
  str.split.reverse.map.with_index{ |dig, i| dig.to_i * base ** i }.inject(0){ |sum, el| sum + el }
end

def gen n1, n2, base
  p str1 = str_gen(n1, base)
  p str2 = str_gen(n2, base)
  a1 = stti(str1, base)
  a2 = stti(str2, base)
  File.open('in.txt', 'w') do |file|
    file.puts base
    file.puts str1
    file.puts str2
  end
  return a1, a2
end
