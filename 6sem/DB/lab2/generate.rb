File.write("base1.txt", File.readlines('base.txt').first(800000).sort_by{ |a| a.split[1].to_i }.join)
File.write("base2.txt", File.readlines('base.txt').first(800000).sort_by{ |a| a.split[2] }.join)
File.write("base_tail.txt", File.readlines('base.txt').last(200000).join)
