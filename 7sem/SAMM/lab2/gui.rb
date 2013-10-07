require 'green_shoes'
require 'pry'
require File.join(File.dirname(__FILE__), 'distributions.rb')

INPUTS = {
  uniform: ['a', 'b'],
  gauss: ['mx', 'dx'],
  exp: ['lambda'],
  gamma: ['n', 'lambda'],
  triangle: ['a', 'b'],
  simpson: ['a', 'b']
}


Shoes.app :title => "Distributions", :width => 700 do
  def gen_interface(type)
    @type = type
    @interface && @interface.clear
    @interface ||= stack :margin => 5
    @interface.append do
      stack do
        tagline 'INPUTS'
        @inputs = INPUTS[type].dup
        @inputs.map!{ |name| inscription name ; edit_line width: 200 }
        button 'Generate' do
          gen_distribution
        end
      end
    end
    output_fields
  end

  def output_fields
    @out = {}
    @interface.append do
      stack do
        tagline 'OUTPUTS'
        @out[:mx] = inscription 'mx: '
        @out[:D] = inscription 'D: '
        @out[:dx] = inscription 'dx: '
        button 'Plot diagramm' do
          @distribution && @distribution.diagramm
        end
      end
    end
  end

  def gen_distribution
    @distribution = Distribution.by_type(@type).new(*@inputs.map{ |input| input.text.to_f })
    @out[:mx].text = "mx: #{@distribution.mx.to_f}"
    @out[:D].text = "D: #{@distribution.D.to_f}"
    @out[:dx].text = "dx: #{@distribution.dx.to_f}"
  end

  background "#FFF".."#333", :angle => 60

  flow do
    flow :width => '33%' do
      radio :action, :width => 30 do
        gen_interface(:uniform)
      end
      para "Uniform", width: 100
    end
    flow :width => '33%' do
      radio :action, :width => 30 do
        gen_interface(:gauss)
      end
      para "Gauss", width: 100
    end
    flow :width => '33%' do
      radio :action, :width => 30 do
        gen_interface(:exp)
      end
      para "Exponential", width: 100
    end
  end

  flow do
    flow :width => '33%' do
      radio :action, :width => 30 do
        gen_interface(:gamma)
      end
      para "Gamma", width: 100
    end
    flow :width => '33%' do
      radio :action, :width => 30 do
        gen_interface(:triangle)
      end
      para "Triangle", width: 100
    end
    flow :width => '33%' do
      radio :action, :width => 30 do
        gen_interface(:simpson)
      end
      para "Simpson", width: 100
    end
  end

  @interface = stack :margin => 5 do
  end
  gen_interface(:uniform)
end
