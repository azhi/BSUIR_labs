require 'mathn'
class Vector
  def []=(i,val)
    @elements[i] = val
  end
end

Shoes.app do
  def generate_fields count
    @coeffs_edits = Array.new(count){ |index| Array.new(count+1){} }
    @free_vector = Array.new(count)
    @fields.clear
    @fields.append do
      count.to_i.times do |x|
        flow :margin => 5 do
          count.times do |y|
            @coeffs_edits[x][y] = edit_line :width => 50
            if y == count - 1
              inscription "X#{y+1} = "
              @free_vector[x] = edit_line :width => 50
              @coeffs_edits[x][y+1] = @free_vector[x]
            else
              inscription "X#{y+1} + "
            end
          end
        end
      end
    end
  end

  def solveByGauss
    @out.replace ""
    coeffs_array = @coeffs_edits.map{|ar| ar.map{|el| el.text.to_r}}.map{|ar| Vector[*ar]}
    
    # straight Gauss
    (0...coeffs_array.size).each{ |i|
      if coeffs_array[i][i] == 0
        swap_indexes = Hash[* (i+1...coeffs_array.size).map{ |j| [j, coeffs_array[j][i]] }.flatten ].find_all{ |key, val| val != 0 }
        next if swap_indexes.empty?
        coeffs_array[i], coeffs_array[ swap_indexes[0].first ] = coeffs_array[ swap_indexes[0].first ], coeffs_array[i]
        coeffs_array[i] /= coeffs_array[i][i]
      else
        coeffs_array[i] /= coeffs_array[i][i]
      end
      (i+1...coeffs_array.size).each{ |j| coeffs_array[j] -= coeffs_array[i] * coeffs_array[j][i] }
    }

    # reverse Gauss
    (1...coeffs_array.size).to_a.reverse.each{ |i|
      (0...i).each{ |j| coeffs_array[j] -= coeffs_array[i] * coeffs_array[j][i] }
    }

    Shoes.debug coeffs_array
    if (0...coeffs_array.size).map{ |i| coeffs_array[i][i] }.find{ |el| el != 1 }.nil?
      # got solution
      @out.replace coeffs_array.each_with_index.map{ |vector, i| 
        "X#{i+1} = #{vector[-1].to_f}"
      }.join(", ")
    else
      null_indexes = Hash[* (0...coeffs_array.size).map{ |i| [i, coeffs_array[i][i]] }.flatten ].find_all{ |key, val| val != 1 }
      if null_indexes.map{ |key, val| coeffs_array[key][-1]}.find{ |el| el != 0 }.nil?
        @out.replace "Infinite number of solutions"
      else
        @out.replace "No solution"
      end
    end
     
  end

  def checkSeidalSolvable coeffs_array
    unless coeffs_array.each_with_index.map do |el, i|
      sum = el.inject(0){ |res, n| res + n };
      sum -= el[i]
      el[i] > sum
    end.find{ |el| el == false }.nil?
      return false
    end

    coeffs_array.transpose
    unless coeffs_array.each_with_index.map do |el, i|
      sum = el.inject(0){ |res, n| res + n };
      sum -= el[i]
      el[i] > sum
    end.find{ |el| el == false }.nil?
      return false
    end
    coeffs_array.transpose

    coeffs_array.map{ |el| Vector[*el].norm }
    return true
  end

  def solveBySeidal eps
    @out.replace ""
    coeffs_array = @coeffs_edits.map{|ar| ar.map{|el| el.text.to_r}[0...-1]}
    free = Vector[* @free_vector.map{ |el| el.text.to_r }]
    Shoes.debug free
    Shoes.debug coeffs_array

    unless checkSeidalSolvable coeffs_array
      @out.replace "One of the main diagonal elements less than row/column sum\nCan't solve this system with Seidal"
      return;
    end

    @X = Vector[* free ];
    @Xprev = Vector[* Array.new(@X.size){0} ]
    Shoes.debug @X.size
    while (0...@X.size).map{ |i| (@X[i] - @Xprev[i]).abs }.max > eps do
      @Xprev = Vector[*@X]
      @X = Vector[* (0...@X.size).map do |i|
        sum = -(0...i).inject(0){ |res, j| res + coeffs_array[i][j] / coeffs_array[i][i] * @X[j] }
        sum -= (i+1...@X.size).inject(0){ |res, j| res + coeffs_array[i][j] / coeffs_array[i][i] * @Xprev[j] }
        sum += free[i] / coeffs_array[i][i]
        sum
      end ]
    end

    @out.replace @X.each_with_index.map{ |el, i| "X#{i+1} = #{el.to_f}" }.join(", ")
  end

  def solveByCholetsky
    @out.replace ""
    coeffs_array = @coeffs_edits.map{|ar| ar.map{|el| el.text.to_r}[0...-1]}
    free = Vector[* @free_vector.map{ |el| el.text.to_r }]

    unless Matrix[* coeffs_array].symmetric?
      @out.replace "Coefficients matrix is not symmetric one\nCan't solve this system with Choletsky"
      return
    end

    @L = Matrix.build(coeffs_array.size, coeffs_array.size){0}.row_vectors
    (0...@L.size).each do |j|
      (j...@L.size).each do |i|
        if i==j
          @sum = 0
          (0...i).each{ |k| @sum += @L[i][k] ** 2 }
          @L[i][j] = (coeffs_array[i][i] - @sum) ** 0.5
        elsif j<i
          @sum = 0
          (0...j).each{ |k| @sum += @L[i][k] * @L[j][k] }
          @L[i][j] = (coeffs_array[i][j] - @sum ) / @L[j][j]
        else
          break
        end
      end
    end

    @MEq1 = Matrix[* @L].row_vectors
    @MEq1.each_with_index.each{ |vector, i| vector[ vector.size ] = free[i] }
    (0...@MEq1.size).each{ |i|
      @MEq1[i] /= @MEq1[i][i]
      (i+1...@MEq1.size).each{ |j| @MEq1[j] -= @MEq1[i] * @MEq1[j][i] }
    }

    unless (0...@MEq1.size).map{ |i| @MEq1[i][i] }.find{ |el| el != 1 }.nil?
      null_indexes = Hash[* (0...@MEq1.size).map{ |i| [i, @MEq1[i][i]] }.flatten ].find_all{ |key, val| val != 1 }
      if null_indexes.map{ |key, val| @MEq1[key][-1]}.find{ |el| el != 0 }.nil?
        @out.replace "Unable to solve"
        return
      else
        @out.replace "No solution"
        return
      end
    end

    @MEq2 = Matrix[* @L.map{ |vec| vec.to_a }.transpose].row_vectors
    @MEq2.each_with_index.each{ |vector, i| vector[ vector.size ] = @MEq1[i][-1] }
    (0...@MEq2.size).each{ |i|
      @MEq2[i] /= @MEq2[i][i]
      (0...i).each{ |j| @MEq2[j] -= @MEq2[i] * @MEq2[j][i] }
    }

    Shoes.debug @MEq2
    @MEq2.each{ |vector| if vector[-1].is_a?(Complex) && vector[-1].imag < 0.0000001 then vector[-1] = vector[-1].real end }
    Shoes.debug @MEq2
    if (0...@MEq2.size).map{ |i| @MEq2[i][i] }.find{ |el| el != 1 }.nil?
      # got solution
      @out.replace @MEq2.each_with_index.map{ |vector, i| 
        "X#{i+1} = #{vector[-1].to_f}"
      }.join(", ")
    else
      null_indexes = Hash[* (0...@MEq2.size).map{ |i| [i, @MEq2[i][i]] }.flatten ].find_all{ |key, val| val != 1 }
      if null_indexes.map{ |key, val| @MEq2[key][-1]}.find{ |el| el != 0 }.nil?
        @out.replace "Infinite number of solutions"
      else
        @out.replace "No solution"
      end
    end
  end

  background "#FFF".."#000", :angle => 60

  flow :margin => 10 do
    @eq_count = edit_line "3", :width => 50
    button "Generate fields" do
      generate_fields @eq_count.text.to_i
    end

    inscription "Precision: " 
    @precision = edit_line "0.01", :width => 50
  end

  @fields = stack :margin => 5
  flow :margin => 10 do
    inscription "Solve by "
    button "Gauss" do
      solveByGauss
    end
    button "Choletsky" do
      solveByCholetsky
    end
    button "Seidal" do
      solveBySeidal @precision.text.to_r
    end

  end
  stack :margin => 5 do
    # background "#CCC"
    @out = para
  end
  generate_fields 3
end
