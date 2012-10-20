class RegexpBuilder
  class << self
    def b_ops
      %w[+= -= == != /= *= |= &= ^= && || + - * / = | & ^].map{ |ch| Regexp.escape ch }.join(?|)
    end

    def u_ops
      %w[- & ~ !].map{ |ch| Regexp.escape ch }.join(?|)
    end
  end

  DEF = (%q{
    (?<class_def> \g<mod> \s+ class \s+ \g<identifier> \s* \{ ( \g<field_def> | \g<method_def> | \s+ )* \} ){0}
    (?<field_def>
      (?<def_or_init> \g<identifier> ( \s* = \s* \g<expr> )? ){0}
      \g<mod>  \s+ \g<identifier> \s+ \g<def_or_init> ( \s* , \s* \g<def_or_init> )* \s*; ){0}
    (?<method_def> \g<mod> \s+ (?<ex_name> (\s* \g<identifier>)* ) \s* \( (?<params> (\s* \g<identifier> \s*,?)* ) \s* \) \s* (\g<block>) ){0}
    (?<block> \{ \s* ( \g<expr> \s* ; \s* )* \} ){0}
    (?<method_call> \g<identifier> \s* \( ( \s* \g<expr> (\s* , \s* \g<expr> )* )? \s* \) ){0}
    (?<expr> ( \g<var> \s* \g<b_op> \s* \g<expr> | \g<u_op> \s* \g<expr> | \( \s* \g<expr> \s* \) | \g<method_call> | \g<identifier> )){0}
    (?<mod> (public | private | protected | abstract) ){0}
    (?<var> [_\w\d\.]+ ){0}
    (?<type> \g<identifier> ){0}
    (?<identifier> [_\w][_\d\w]* ){0}
  } +
  %Q{
    (?<b_op> (#{b_ops}) ){0}
    (?<u_op> (#{u_ops}) ){0}
  }).strip

  class << self
    def class_def
      Regexp.compile(DEF + ' \g<class_def>', Regexp::EXTENDED | Regexp::MULTILINE)
    end

    def method_def
      Regexp.compile(DEF + ' \g<method_def>', Regexp::EXTENDED | Regexp::MULTILINE)
    end

    def field_def
      Regexp.compile(DEF + ' \g<field_def>', Regexp::EXTENDED | Regexp::MULTILINE)
    end

    def expr
      Regexp.compile(DEF + ' \g<expr>', Regexp::EXTENDED | Regexp::MULTILINE)
    end

    def surrounded_by b, e
      # Note: b, e - one character strings.
      @sur_i ||= 0
      @sur_i += 1
      /(?<ent#@sur_i> #{Regexp.escape b} (?: (?> [^#{b}#{e}]+ ) | \g<ent#@sur_i> )* #{Regexp.escape e} )/x
    end
  end
end
