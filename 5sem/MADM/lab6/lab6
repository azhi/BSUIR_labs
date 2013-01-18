#!/usr/bin/env ruby

require 'graphviz'

def recalculate_distances h, dist_matr
  dist_matr[h.keys.max] = {}
  (0...h.size).each do |ind|
    unless h[ind][:grouped] || ind == h.keys.max
      dist = [dist_matr[h[h.keys.max][:data].first][ind], dist_matr[h[h.keys.max][:data].last][ind]].min
      dist_matr[ind][h.keys.max] = dist
      dist_matr[h.keys.max][ind] = dist
    end
  end

  p dist_matr
end

oper_sym = (gets.chomp == 'min') ? :min : :max

n = gets.chomp.to_i
dist_matr = (0...n).map{ gets.chomp.split.map(&:to_f) }
p dist_matr = Hash[ *dist_matr.map.with_index{ |ar, i| [i, Hash[ *ar.map.with_index{ |el, j| i != j ? [j, el] : nil }.flatten.compact ]] }.flatten ]
h = {}
n.times{ |i| h[i] = {grouped: false, data: [i]}}
p h
puts

if oper_sym == :max
  puts "Inversing distances in order to use min criterion instead of max"
  dist_matr.keys.each{ |k| dist_matr[k].keys.each{ |sk| dist_matr[k][sk] = 1 / dist_matr[k][sk] } }
  p dist_matr
end

(n - 1).times do
  min = dist_matr.max.last.max.last + 1
  min_i = 0
  min_j = 0
  dist_matr.keys.each do |i|
    dist_matr[i].keys.each do |j|
      if ( dist_matr[i][j] < min && dist_matr[i][j] != 0 &&
           !h[i][:grouped] && !h[j][:grouped] )
        min = dist_matr[i][j]
        min_i = i
        min_j = j
      end
    end
  end

  h[h.keys.max + 1] = {grouped: false, data: [min_i, min_j]}
  h[min_i][:grouped] = true
  h[min_j][:grouped] = true
  recalculate_distances h, dist_matr
  p h
  puts
end

if oper_sym == :max
  puts "Inversing distances once more to return to initial values"
  dist_matr.keys.each{ |k| dist_matr[k].keys.each{ |sk| dist_matr[k][sk] = 1 / dist_matr[k][sk] } }
  p dist_matr
end

g = GraphViz.new( :G, :type => :digraph )
g.node[:color]    = "#ddaa66"
g.node[:style]    = "filled"
g.node[:shape]    = "box"
g.node[:penwidth] = "1"
g.node[:fontname] = "Trebuchet MS"
g.node[:fontsize] = "8"
g.node[:fillcolor]= "#ffeecc"
g.node[:fontcolor]= "#775500"
g.node[:margin]   = "0.0"

g.edge[:color]    = "#999999"
g.edge[:weight]   = "1"
g.edge[:fontsize] = "6"
g.edge[:fontcolor]= "#444444"
g.edge[:fontname] = "Verdana"
g.edge[:dir]      = "forward"
g.edge[:arrowsize]= "0.5"

nodes = {}
h.each do |k,v|
  nodes[k] = g.add_nodes(k.to_s)
  nodes[k].label = v[:data].size > 1 ? "#{k < n ? k+1 : ('a'.ord + (k-n).ord).chr} - " + 
  "#{dist_matr[v[:data].first][v[:data].last].round(2)}" : (k+1).to_s
end

h.find_all{ |k, v| v[:data].size > 1 }.each do |k, v|
  g.add_edges(nodes[k], nodes[v[:data].first])
  g.add_edges(nodes[k], nodes[v[:data].last])
end

g.output( :x11 => nil )
