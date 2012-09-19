require 'mathn'

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
    coeffs_array = @coeffs_edits.map{|ar| ar.map{|el| el.text.to_r}}.map{|ar| Vector[*ar]}
    
    # straight Gauss
    (0...coeffs_array.size).each{ |i|
      if coeffs_array[i][i] == 0
        swap_indexes = Hash[* (i+1...coeffs_array.size).map{ |j| [j, coeffs_array[j][i]] }.flatten ].find_all{ |key, val| val != 0 }
        break if swap_indexes.empty?
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
      @out2.replace coeffs_array.each_with_index.map{ |vector, i| 
        "X#{i+1} = #{vector[-1]}"
      }.join(", ")
    else
      null_indexes = Hash[* (0...coeffs_array.size).map{ |i| [i, coeffs_array[i][i]] }.flatten ].find_all{ |key, val| val != 1 }
      if null_indexes.map{ |key, val| coeffs_array[key][-1]}.find{ |el| el != 0 }.nil?
        @out.replace "Infinite number of solutions"
        @out2.replace ""
      else
        @out.replace "No solutions"
        @out2.replace ""
      end
    end
     
  end


  background "#FFF".."#000", :angle => 60

  flow :margin => 10 do
    @eq_count = edit_line :width => 50
    button "Generate fields" do
      generate_fields @eq_count.text.to_i
    end
  end
  @fields = stack :margin => 5
  flow :margin => 10 do
    inscription "Solve by "
    button "Gauss" do
      solveByGauss
    end
    button "Other guy"
    button "And other guy"
  end
  stack :margin => 5 do
    # background "#CCC"
    @out = para
    @out2 = para
  end
  # button "Show" do
  #   Shoes.debug @coeffs_edits
  #   Shoes.debug @coeffs_edits.map{|ar| ar.map{|el| el.text.to_i}}
  # end
end
