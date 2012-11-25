#! /usr/bin/env ruby

def gen_record i
  number = Array.new(7){ rand(0..9) }
  while number[0] == 0 do
    number[0] = rand(0..9)
  end
  number = number.join

  length = rand(5..12)
  family_name = []
  family_name << rand('A'.ord .. 'Z'.ord).chr
  (length - 1).times{ family_name << rand('a'.ord .. 'z'.ord).chr }
  family_name = family_name.join

  length = rand(4..8)
  name = []
  name << rand('A'.ord .. 'Z'.ord).chr
  (length - 1).times{ name << rand('a'.ord .. 'z'.ord).chr }
  name = name.join

  length = rand(8..16)
  middle_name = []
  middle_name << rand('A'.ord .. 'Z'.ord).chr
  (length - 1).times{ middle_name << rand('a'.ord .. 'z'.ord).chr }
  middle_name = middle_name.join

  length = rand(8..16)
  street = []
  street << rand('A'.ord .. 'Z'.ord).chr
  (length - 1).times{ street << rand('a'.ord .. 'z'.ord).chr }
  street = street.join

  house = rand(1..80)
  building = rand(1..3)
  flat = rand(1..600)
  
  res = "#{i}, #{number}, #{family_name}, #{name}, #{middle_name}, #{street}, #{house}, #{building}, #{flat}\n"
  res.encode("UTF-16LE")
end

n = ARGV[0].to_i
n.times{ |i| puts gen_record(i) }
