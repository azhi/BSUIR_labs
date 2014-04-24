require Record

defmodule Lab5 do
  defmodule Grammar do
    Record.defrecord :grammar, [dict: [], term_dict: nil, rules: nil,  base: [], i: 1]
  end

  defmodule GrammarGen1 do
    require Lab5.Grammar

    def add1(gram, :one, [a]) do
      base = Grammar.grammar(gram, :base)
      term_dict = Set.put(Grammar.grammar(gram, :term_dict), a)
      Grammar.grammar(gram, base: [[{:lit, a}] | base],
                            term_dict: term_dict)
    end

    def add1(gram, :two, [a, b]) do
      base = Grammar.grammar(gram, :base)
      term_dict = Set.put(Grammar.grammar(gram, :term_dict), a, b)
      Grammar.grammar(gram, base: [[{:lit, a}, {:lit, b}] | base],
                            term_dict: term_dict)
    end

    def add1(gram, len, [a | tail]) do
      base = Grammar.grammar(gram, :base)
      rules = Grammar.grammar(gram, :rules) || HashDict.new()
      term_dict = Grammar.grammar(gram, :term_dict) || HashSet.new |> Set.put(a)
      m = Enum.find(base, fn(l) -> List.first(l) == {:lit, a} end)
      case m do
        nil ->
          i = Grammar.grammar(gram, :i)
          Grammar.grammar(gram, base: [[{:lit, a}, {:var, i}] | base],
                          rules: rules, term_dict: term_dict, i: i+1)
          |> add(i, len, tail)

        [lit: _, var: j] ->
          Grammar.grammar(gram, term_dict: term_dict, rules: rules)
          |> add(j, len, tail)
      end
    end

    def add(gram, s, :one, [a]) do
      term_dict = Set.put(Grammar.grammar(gram, :term_dict), a)
      rules = Grammar.grammar(gram, :rules)
      rule = Dict.get(rules, s, HashSet.new()) |> Set.put([lit: a])
      Grammar.grammar(gram, term_dict: term_dict, rules: Dict.put(rules, s, rule))
    end

    def add(gram, s, :two, [a, b]) do
      term_dict = Grammar.grammar(gram, :term_dict) |> Set.put(a) |> Set.put(b)
      rules = Grammar.grammar(gram, :rules)
      rule = Dict.get(rules, s, HashSet.new()) |> Set.put([{:lit, a}, {:lit, b}])
      Grammar.grammar(gram, term_dict: term_dict,
                            rules: Dict.put(rules, s, rule))
    end

    def add(gram, s, len, [a | tail]) do
      term_dict = Grammar.grammar(gram, :term_dict) |> Set.put(a)
      rules = Grammar.grammar(gram, :rules)
      ns = HashSet.new()
      rule = Dict.get(rules, s, ns)
      m = Enum.find(rule, fn(l) -> (List.first(l) == {:lit, a}) and List.keymember?(l, :var, 0) end)
      case m do
        nil -> (
                i = Grammar.grammar(gram, :i)
                new_rule = rule |> Set.put([{:lit, a}, {:var, i}])
                Grammar.grammar(gram, rules: Dict.put(rules, s, new_rule),
                                      term_dict: term_dict, i: i+1)
                |> add(i, len, tail)
        )
        [lit: ^a, var: j] ->
          Grammar.grammar(gram, term_dict: term_dict)
          |> add(j, len, tail)
      end
    end
  end

  defmodule GrammarGen2 do
    require Lab5.Grammar

    def recursive(gram) do
      rules = Grammar.grammar(gram, :rules)
      new_rules = do_recursive(rules)
      Grammar.grammar(gram, rules: new_rules)
    end

    def do_recursive(rules) do
      case Enum.find(rules, &do_find_long_term/1) do
        {i, _} when is_integer(i) ->
          case find_rule_match(rules, i) do
            j when is_integer(j) and i != j ->
              update_rules(rules, i, j) |> do_recursive
            nil ->
              rules # TODO: handle this
          end

        nil -> rules
      end
    end

    def do_find_long_term({_, rs}) do
      case Enum.find(rs, &long_term?/1) do
        [lit: _, lit: _] -> true
        nil -> nil
      end
    end

    def update_rules(rules, i, j) do
      # :io.fwrite("~p -> ~p~n", [i, j])
      Enum.reduce(rules, rules, fn({s, rs}, acc) ->
                                    new_rule = Enum.reduce(rs, rs, fn(r, accc) ->
                                                                       accc
                                                                       |> Set.delete(r)
                                                                       |> Set.put(update_rule(r, i, j))
                                                                   end)
                                    Dict.put(acc, s, new_rule)
                                end)
      |> Dict.delete(i)
    end

    def update_rule([{:var, i} | rest], i, j), do: [{:var, j} | update_rule(rest, i, j)]
    def update_rule([x | rest], i, j), do: [x | update_rule(rest, i, j)]
    def update_rule([], _, _), do: []

    def find_rule_match(rules, i) do
      rs = Dict.get(rules, i)
      case Enum.find(rules, &rule_subset?(&1, rs, rules)) do
        {i, _} -> i
        _ -> nil
      end
    end

    def rule_subset?({_, n_rs}, rs, rules) do
      Enum.all?(rs, &match_rule_set?(&1, n_rs, rules))
    end

    def match_rule_set?([lit: a], n_rs, _), do: Set.member?(n_rs, [lit: a])

    def match_rule_set?([lit: a, lit: b], n_rs, rules) do
      Enum.any?(n_rs, fn(rule) ->
                          case rule do
                            [lit: ^a, var: i] -> match_rule_set?([lit: b], rules[i], rules)
                            _ -> nil
                          end
                      end)
    end

    def long_term?([lit: _, lit: _]), do: true

    def long_term?(_), do: nil
  end

  defmodule GrammarGen3 do
    require Lab5.Grammar

    def minimize(gram) do
      rules = Grammar.grammar(gram, :rules)
      base = Grammar.grammar(gram, :base)
      {new_rules, new_base} = do_minimize(rules, base)
      Grammar.grammar(gram, base: new_base, rules: new_rules)
    end

    def do_minimize(rules, base) do
      Enum.reduce(rules, {rules, base}, fn({i, _}, {rr, bb}) ->
                                            rs = rr[i]
                                            z = Enum.find(rr, fn({j, rs2}) ->
                                                                  (i != j) and Set.subset?(rs2, rs)
                                                              end)
                                            case z do
                                              {j, _} ->
                                                new_rr = rr |> Dict.delete(i) |> GrammarGen2.update_rules(i, j)
                                                new_bb = Enum.map(bb, &GrammarGen2.update_rule(&1, i, j))
                                                {new_rr, new_bb}
                                              _ ->
                                                # :io.fwrite("~p~n", [[i, z]])
                                                {rr, bb}
                                            end
                                        end)
    end
  end


  def generate(gram) do
    require Grammar
    base = Grammar.grammar(gram, :base)
    rules = Grammar.grammar(gram, :rules)
    r = :random.uniform(Enum.count(base)) - 1
    [{:lit, x}, {:var, y}] = Enum.at(base, r)
    do_generate([x], y, rules)
  end

  def do_generate(list, s, rules) do
    rs = rules[s]
    r = :random.uniform(Enum.count(rs)) - 1
    case Enum.at(rs, r) do
      [lit: x, var: y] -> do_generate([x | list], y, rules)
      [lit: x] -> [x | list]
    end
  end

  def func do
    :random.seed(:os.timestamp)
    require Grammar
    g = Grammar.grammar
    |> GrammarGen1.add1(:two, String.split("caaab", "", trim: true))
    |> GrammarGen1.add1(:two, String.split("bbaab", "", trim: true))
    |> GrammarGen1.add1(:one, String.split("caab", "", trim: true))
    |> GrammarGen1.add1(:one, String.split("bbab", "", trim: true))
    |> GrammarGen1.add1(:one, String.split("cab", "", trim: true))
    |> GrammarGen1.add1(:one, String.split("bbb", "", trim: true))
    |> GrammarGen1.add1(:one, String.split("cb", "", trim: true))
    |> GrammarGen2.recursive()
    |> GrammarGen3.minimize()
    # Grammar.grammar(g, :rules) |> Enum.each(fn {k, v} -> :io.fwrite('~p: ~p~n', [k, v]) end)

    Enum.each(1..20, fn(_) -> :io.fwrite('~s~n', [Enum.join(generate(g))]) end)
  end
end
