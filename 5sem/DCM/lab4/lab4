#! /usr/bin/env ruby

Shoes.app :title => "Gray code", :width => 700 do
  def generate_gray_code
    n = @inN.text.to_i
    s = [ "0", "1" ]
    (n-1).times do |c|
      s = s + s.reverse
      s = s.map.with_index do |str, i|
        if i < s.size / 2
          "0" + str
        else
          "1" + str
        end
      end
    end
    s
  end

  def convgb input
    out = []
    input = input.to_s(2)
    input.size.times do |i|
      Shoes.debug input.scan(/./).reverse[i..-1].inspect
      out[i] = input.scan(/./).reverse[i..-1].inject(0){ |res, el| res ^ el.to_i }
    end
    out.join.reverse.to_i(2)
  end

  def convbg input
    input ^ (input >> 1)
  end

  def gen_interface
    @interface.clear unless @interface.nil?
    @interface = stack :margin => 5 if @interface.nil?
    @interface.append do
      flow :margin => 3 do
        inscription "n:"
        @inN = edit_line :width => 50
        button "Generate" do
          4.times{ |i| @out[i].replace "" }
          s = generate_gray_code
          s = s.map.with_index{ |el, i| {ind: i, el: el} }
          if s.size > 16
            if (s.size > 64)
              rand_pos = rand(0...(s.size - 64))
              Shoes.debug rand_pos
              s = s[rand_pos...rand_pos + 63] << "....."
            end
            each_count = s.size / 4
            4.times do |i|
              str = s[(i * each_count)..((i + 1) * each_count - 1)].map do |hash|
                hash.is_a?(Hash) ? "#{hash[:ind]} - #{hash[:el]}" : hash.to_s
              end.join("\n")
              @out[i].replace str
            end
          else
            @out[0].replace s.map{ |hash| "#{hash[:ind]} - #{hash[:el]}" }.join("\n")
          end
        end
      end

      flow do
        @out = []
        4.times do
          stack :width => '25%' do
            @out << para
          end
        end
      end
    end
  end

  def conv_interface type
    @interface.clear unless @interface.nil?
    @interface = stack :margin => 5 if @interface.nil?
    @interface.append do
      flow do
        inscription "input base:"
        @inB = edit_line "2", :width => 50
      end

      flow do
        inscription "input:"
        @input = edit_line :width => 150
      end

      flow do
        inscription "in base 2:"
        @in2 = para
      end if type == :bg

      inscription "out:"
      flow do
        inscription "in base 2:" if type == :gb
        inscription "in Gray code:" if type == :bg
        @out2 = para
      end
      flow do
        inscription "in base 10:"
        @out10 = para
      end if type == :gb
      button "Convert" do
        if type == :gb
          res = convgb @input.text.to_i(@inB.text.to_i)
        elsif type == :bg
          strbase2 = @input.text.to_i(@inB.text.to_i).to_s(2)
          strbase2 = '0' * (@input.text.size - strbase2.size) + strbase2 if (@input.text.size - strbase2.size) > 0
          @in2.replace strbase2
          res = convbg @input.text.to_i(@inB.text.to_i)
        end
        outstr2 = res.to_s(2)
        outstr2 = '0' * (@input.text.size - outstr2.size) + outstr2 if (@input.text.size - outstr2.size) > 0
        @out2.replace outstr2
        @out10.replace res
      end

    end
  end

  def convgb_interface
    conv_interface :gb
  end

  def convbg_interface
    conv_interface :bg
  end

  background "#FFF".."#333", :angle => 60

  flow do
    flow :width => '33%' do
      radio :action, :width => 30 do
        gen_interface
      end
      para "Generate Gray code"
    end
    flow :width => '33%' do
      radio :action, :width => 30 do
        convgb_interface
      end
      para "Convert Gray code to binary"
    end
    flow :width => '33%' do
      radio :action, :width => 30 do
        convbg_interface
      end
      para "Convert binary to Gray code"
    end
  end
  gen_interface
end
