package adaptiveTest;

public class MathProblem {
	public MathProblem(String problem, long solution) {
		this.problem = problem;
		this.solution = solution;
	}

	public String getProblem() {
		return problem;
	}
	
	public long getSolution() {
		return solution;
	}
	
	private String problem;
	private long solution;
}
