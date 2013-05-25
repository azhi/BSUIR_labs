package adaptiveTest;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class MathProblemGenerator {
	public static MathProblem generate(int complexity) {
		long solution = 0;
		StringBuilder problem = new StringBuilder();
		int prev_operand = generateOperand(complexity);
		problem.append(prev_operand).append(' ');
		for (int i = 0; i < complexity + 1; ++i) {
			int operand = generateOperand(complexity);
			int operation = rand.nextInt(4);
			switch (operation) {
				case 0:
				case 1:
				case 2:
					problem.append('+').append(' ').append(operand);
					solution += prev_operand;
					prev_operand = operand;
					if (i == complexity)
						solution += operand;
					break;
				case 3:
					problem.append('*').append(' ').append(operand);
					prev_operand *= operand;
					if (i == complexity)
						solution += prev_operand;
					break;
			}
		}
		return new MathProblem(problem.toString(), solution);
	}
	
	public static List<Long> generateAnswers(long solution) {
		long eff_solution = solution;
		while (eff_solution == 0)
			eff_solution = rand.nextInt(20);
		List<Long> res = new ArrayList<Long>();
		int count = 2 + rand.nextInt(3);
		for (int i = 0; i < count; ++i) {
			long add = rand.nextLong() % eff_solution;
			add = add < 0 ? ~add : add;
			res.add((long) Math.round(eff_solution / 10.0) + add);
		}
		res.add(rand.nextInt(res.size()), solution);
		return res;
	}
	
	public static final int BASE_COMPLEXITY = 3;
	
	private static int generateOperand(int complexity) {
		int power = 0;
		switch (complexity) {
			case 6:
			case 5:
				power = 3;
				break;
			case 4:
			case 3:
				power = 2;
				break;
			case 2:
			case 1:
			case 0:
				power = 1;
				break;
		}
		int maxValue = (int) Math.round(Math.pow(10, power));
		int operand = rand.nextInt(maxValue);
		return operand;
	}
	
	private static Random rand = new Random();
}
